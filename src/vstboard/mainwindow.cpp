/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include "mainconfig.h"
#include "projectfile/setupfile.h"
#include "views/configdialog.h"
#include "views/aboutdialog.h"
#include "connectables/objectinfo.h"
#include "views/splash.h"
//#include "imagecollection.h"

MainWindow *MainWindow::theMainWindow=0;

MainWindow *MainWindow::Get(QWidget *parent)
{
    if(!theMainWindow)
        theMainWindow = new MainWindow(parent);

    return theMainWindow;
}

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    MainWindow::theMainWindow=this;
    ui->setupUi(this);

    mainHost = MainHost::Create(this);

//    ImageCollection::Create(this);

    //audio devices
    ui->treeAudioInterfaces->setModel(mainHost->GetAudioDevicesModel());
    ui->treeAudioInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeAudioInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->resizeSection(1,30);
    ui->treeAudioInterfaces->header()->resizeSection(2,30);

    //midi devices
    ui->treeMidiInterfaces->setModel(mainHost->GetMidiDeviceModel());
    ui->treeMidiInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeMidiInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->resizeSection(1,30);
    ui->treeMidiInterfaces->header()->resizeSection(2,30);

    BuildListTools();

    //programs
    ui->Programs->SetModel(mainHost->programList->GetModel());
    connect(mainHost->programList, SIGNAL(ProgChanged(QModelIndex)),
            ui->Programs,SLOT(OnProgChange(QModelIndex)));
    connect(ui->Programs,SIGNAL(ChangeProg(QModelIndex)),
            mainHost->programList,SLOT(ChangeProg(QModelIndex)));

    //vst plugins browser
    listVstPluginsModel.setReadOnly(true);
    listVstPluginsModel.setResolveSymlinks(true);

    QStringList fileFilter;
    fileFilter << "*.dll";
    listVstPluginsModel.setNameFilters(fileFilter);
    listVstPluginsModel.setNameFilterDisables(false);
    listVstPluginsModel.setRootPath(ConfigDialog::defaultVstPath());
    ui->VstBrowser->setModel(&listVstPluginsModel);

    //timer
//    QTimer *timerCpuLoad = new QTimer(this);
//    timerCpuLoad->start(200);
//    connect(timerCpuLoad, SIGNAL(timeout()),
//            this, SLOT(UpdateCpuLoad()));

    mySceneView = new View::SceneView(ui->hostView, ui->projectView, ui->programView, this);
    mySceneView->setModel(mainHost->GetModel());

    ui->solverView->setModel(&mainHost->solver.model);

    mainHost->SetSampleRate( ConfigDialog::defaultSampleRate() );
    mainHost->Open();

    //load default setup file
    currentSetupFile = ConfigDialog::defaultSetupFile();
    if(!currentSetupFile.isEmpty()) {
        SetupFile file(this);
        file.LoadFromFile(currentSetupFile);
    }

    //load default project file
    currentProjectFile = ConfigDialog::defaultProjectFile();
    if(!currentProjectFile.isEmpty()) {
        project->LoadFromFile(currentProjectFile);
    }

    readSettings();

    ui->treeHostModel->setModel(mainHost->GetModel());
    ui->listParking->setModel(mainHost->GetParkingModel());
    ui->listParking->setRootIndex(mainHost->GetParkingModel()->invisibleRootItem()->child(0)->index());

}

MainWindow::~MainWindow()
{
    if(ui)
        delete ui;
    theMainWindow = 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (userReallyWantsToQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::userReallyWantsToQuit()
{
    return true;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::BuildListTools()
{
    QStringList headerLabels;
    headerLabels << "Name";

    listToolsModel.setHorizontalHeaderLabels(headerLabels);
    QStandardItem *parentItem = listToolsModel.invisibleRootItem();

    //midi parameters
    QStandardItem *item = new QStandardItem(tr("MidiCC to parameter"));
    ObjectInfo info;
    info.nodeType = NodeType::object;
    info.objType = ObjType::MidiToAutomation;
    item->setData(QVariant::fromValue(info), UserRoles::objInfo);
    parentItem->appendRow(item);

    //midi sender
    item = new QStandardItem(tr("Midi sender"));
    info.nodeType = NodeType::object;
    info.objType = ObjType::MidiSender;
    item->setData(QVariant::fromValue(info),UserRoles::objInfo);
    parentItem->appendRow(item);

    //host controler
    item = new QStandardItem(tr("Host Controller"));
    info.nodeType = NodeType::object;
    info.objType = ObjType::HostController;
    item->setData(QVariant::fromValue(info),UserRoles::objInfo);
    parentItem->appendRow(item);

    //container
//    item = new QStandardItem(tr("Container"));
//    info.nodeType = NodeType::container;
//    info.objType = ObjType::Container;
//    item->setData(QVariant::fromValue(info),UserRoles::objInfo);
//    parentItem->appendRow(item);

    ui->treeTools->setModel(&listToolsModel);
    ui->treeTools->header()->setResizeMode(0,QHeaderView::Stretch);
}


//void MainWindow::UpdateCpuLoad()
//{
//    ui->progressCpuLoad->setValue(mainHost->GetCpuLoad()*100);
//}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project or Setup"), "", tr("Supported Files (*.%1 *.%2);;Project Files (*.%1);;Setup Files (*.%2)").arg(PROJECT_FILE_EXTENSION).arg(SETUP_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(fileName.endsWith(PROJECT_FILE_EXTENSION, Qt::CaseInsensitive)) {
        if(project->LoadFromFile(fileName)) {
            ConfigDialog::AddRecentProjectFile(fileName);
            currentProjectFile = fileName;
            updateRecentFileActions();
        }
        return;
    }

    if(fileName.endsWith(SETUP_FILE_EXTENSION, Qt::CaseInsensitive)) {
        SetupFile file(this);
        if(file.LoadFromFile(fileName)) {
            ConfigDialog::AddRecentSetupFile(fileName);
            currentSetupFile = fileName;
            updateRecentFileActions();
        }
        return;
    }

    QMessageBox msgBox;
    msgBox.setText(tr("Unknown file extension."));
    msgBox.exec();

}

void MainWindow::on_actionNew_triggered()
{
    MainHost::Get()->ClearHost();
    ConfigDialog::AddRecentProjectFile("");
    currentProjectFile = "";
    updateRecentFileActions();
}

void MainWindow::on_actionSave_triggered()
{
    if(currentProjectFile.isEmpty()) {
        on_actionSave_Project_As_triggered();
        return;
    }

    project->SaveToFile(currentProjectFile);
}

void MainWindow::on_actionSave_Project_As_triggered()
{
    QSettings settings;
    QString lastDir = settings.value("lastProjectDir").toString();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), lastDir, tr("Project Files (*.%1)").arg(PROJECT_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(PROJECT_FILE_EXTENSION, Qt::CaseInsensitive)) {
        fileName += ".";
        fileName += PROJECT_FILE_EXTENSION;
    }

    project->SaveToFile(fileName);
    settings.setValue("lastProjectDir",QFileInfo(fileName).absolutePath());
    ConfigDialog::AddRecentProjectFile(fileName);
    currentProjectFile = fileName;
    updateRecentFileActions();
}

void MainWindow::on_actionNew_Setup_triggered()
{
    SetupFile file;
    file.Clear();
    ConfigDialog::AddRecentSetupFile("");
    currentSetupFile = "";
    updateRecentFileActions();
}

void MainWindow::on_actionSave_Setup_triggered()
{
    if(currentSetupFile.isEmpty()) {
        on_actionSave_Setup_As_triggered();
        return;
    }

    SetupFile file;
    file.SaveToFile(currentSetupFile);
}

void MainWindow::on_actionSave_Setup_As_triggered()
{
    QSettings settings;
    QString lastDir = settings.value("lastSetupDir").toString();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Setup"), lastDir, tr("Setup Files (*.%1)").arg(SETUP_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(SETUP_FILE_EXTENSION, Qt::CaseInsensitive)) {
        fileName += ".";
        fileName += SETUP_FILE_EXTENSION;
    }

    SetupFile file;
    file.SaveToFile(fileName);
    settings.setValue("lastSetupDir",QFileInfo(fileName).absolutePath());
    ConfigDialog::AddRecentSetupFile(fileName);
    currentSetupFile = fileName;
    updateRecentFileActions();
}

void MainWindow::on_actionShortcuts_toggled(bool onOff)
{
    MainConfig::Get()->shortcutConfig = onOff;
    ui->dockMidiDevices->setMouseTracking(onOff);
}

void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog conf(this);
    conf.exec();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();
    settings.setValue("lastVstPath", ui->VstBrowser->path());
    ui->Programs->writeSettings();
    settings.sync();
}

void MainWindow::readSettings()
{
    QSettings settings;

    if( !settings.value("splashHide",false).toBool() ) {
        Splash spl;
        spl.exec();
    }

    //add dock visibility to the menu
    QList<QDockWidget*>listDocks;
    listDocks << ui->dockTools;
    listDocks << ui->dockMidiDevices;
    listDocks << ui->dockAudioDevices;
    listDocks << ui->dockVstBrowser;

    listDocks << ui->dockPrograms;
    listDocks << ui->dockSolver;
    listDocks << ui->dockHostModel;
    listDocks << ui->dockParking;


    foreach(QDockWidget *dock, listDocks) {
        ui->menuView->addAction(dock->toggleViewAction());
        ui->mainToolBar->addAction(dock->toggleViewAction());
    }

    //recent setups
    for(int i=0; i<NB_RECENT_FILES; i++) {
        QAction *act = new QAction(this);
        act->setVisible(false);
        connect(act, SIGNAL(triggered()),
             this, SLOT(openRecentSetup()));

        ui->menuRecent_Setups->addAction(act);

        listRecentSetups << act;
    }

    //recent projects
    for(int i=0; i<NB_RECENT_FILES; i++) {
        QAction *act = new QAction(this);
        act->setVisible(false);
        connect(act, SIGNAL(triggered()),
             this, SLOT(openRecentProject()));

        ui->menuRecent_Projects->addAction(act);

        listRecentProjects << act;
    }

    updateRecentFileActions();

    //window state
    settings.beginGroup("MainWindow");

    if(settings.contains("geometry")) {
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("state").toByteArray());
    } else {
        resetSettings();
    }

    settings.endGroup();

    ui->Programs->readSettings();
}

void MainWindow::resetSettings()
{
    QList<QDockWidget*>listDocks;
    listDocks << ui->dockTools;
    listDocks << ui->dockMidiDevices;
    listDocks << ui->dockAudioDevices;
    listDocks << ui->dockVstBrowser;

    listDocks << ui->dockPrograms;
    listDocks << ui->dockSolver;
    listDocks << ui->dockHostModel;
    listDocks << ui->dockParking;


    foreach(QDockWidget *dock, listDocks) {
        dock->setFloating(false);
        dock->setVisible(true);
    }

    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockTools);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockMidiDevices);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockAudioDevices);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockVstBrowser);

    addDockWidget(Qt::RightDockWidgetArea,  ui->dockPrograms);
    addDockWidget(Qt::RightDockWidgetArea,  ui->dockSolver);
    addDockWidget(Qt::RightDockWidgetArea,  ui->dockHostModel);
    addDockWidget(Qt::RightDockWidgetArea,  ui->dockParking);



    tabifyDockWidget(ui->dockTools,ui->dockMidiDevices);
    tabifyDockWidget(ui->dockMidiDevices,ui->dockAudioDevices);
    tabifyDockWidget(ui->dockAudioDevices,ui->dockVstBrowser);

    tabifyDockWidget(ui->dockHostModel,ui->dockSolver);
    tabifyDockWidget(ui->dockSolver,ui->dockParking);

    ui->actionHost_panel->setChecked(true);
    ui->actionProject_panel->setChecked(true);
    ui->actionProgram_panel->setCheckable(true);
    int h = ui->splitterPanels->height();
    QList<int>heights;
    heights << h << h << h;
    ui->splitterPanels->setSizes(heights);

    ui->Programs->resetSettings();

}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog about;
    about.exec();
}

void MainWindow::updateRecentFileActions()
 {
     QSettings settings;
     {
         QStringList files = settings.value("recentSetupFiles").toStringList();

         int numRecentFiles = qMin(files.size(), (int)NB_RECENT_FILES);

         for (int i = 0; i < numRecentFiles; ++i) {
             QString text = tr("&%1 %2").arg(i + 1).arg( QFileInfo(files[i]).fileName() );
             listRecentSetups[i]->setText(text);
             listRecentSetups[i]->setData(files[i]);
             listRecentSetups[i]->setVisible(true);
         }
         for (int j = numRecentFiles; j < NB_RECENT_FILES; ++j)
             listRecentSetups[j]->setVisible(false);
    }

     {
         QStringList files = settings.value("recentProjectFiles").toStringList();

          int numRecentFiles = qMin(files.size(), (int)NB_RECENT_FILES);

          for (int i = 0; i < numRecentFiles; ++i) {
              QString text = tr("&%1 %2").arg(i + 1).arg( QFileInfo(files[i]).fileName() );
              listRecentProjects[i]->setText(text);
              listRecentProjects[i]->setData(files[i]);
              listRecentProjects[i]->setVisible(true);
          }
          for (int j = numRecentFiles; j < NB_RECENT_FILES; ++j)
              listRecentProjects[j]->setVisible(false);
     }

     ui->actionSave_Setup_As->setEnabled(!currentSetupFile.isEmpty());
     ui->actionSave_Project_As->setEnabled(!currentProjectFile.isEmpty());

 }

void MainWindow::openRecentSetup()
 {
     QAction *action = qobject_cast<QAction *>(sender());
     if(!action)
         return;

     QString fileName = action->data().toString();

     SetupFile file(this);
     if(file.LoadFromFile(fileName)) {
         ConfigDialog::AddRecentSetupFile(fileName);
         currentSetupFile=fileName;
         updateRecentFileActions();
     }
}

void MainWindow::openRecentProject()
 {
     QAction *action = qobject_cast<QAction *>(sender());
     if(!action)
         return;

     QString fileName = action->data().toString();

     if(project->LoadFromFile(fileName)) {
         ConfigDialog::AddRecentProjectFile(fileName);
         currentProjectFile=fileName;
         updateRecentFileActions();
     }
 }

void MainWindow::on_actionRefresh_Audio_devices_triggered()
{
    ui->treeAudioInterfaces->setModel(mainHost->GetAudioDevicesModel());
}

void MainWindow::on_actionRefresh_Midi_devices_triggered()
{
    ui->treeMidiInterfaces->setModel(mainHost->GetMidiDeviceModel());
}




void MainWindow::on_actionRestore_default_layout_triggered()
{
    resetSettings();
}
