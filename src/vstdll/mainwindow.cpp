/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "views/configdialog.h"
#include "projectfile/setupfile.h"
#include "projectfile/projectfile.h"
#include "views/aboutdialog.h"
#include "globals.h"

MainWindow::MainWindow(MainHost * myHost, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myHost(myHost)
{
    myHost->mainWindow=this;
    connect(myHost,SIGNAL(programParkingModelChanged(QStandardItemModel*)),
            this,SLOT(programParkingModelChanges(QStandardItemModel*)));
    connect(myHost,SIGNAL(groupParkingModelChanged(QStandardItemModel*)),
            this,SLOT(groupParkingModelChanges(QStandardItemModel*)));
    ui->setupUi(this);
    setWindowTitle(APP_NAME);

    connect(ui->mainToolBar, SIGNAL(visibilityChanged(bool)),
            ui->actionTool_bar, SLOT(setChecked(bool)));

    BuildListTools();

    //programs
    ui->Programs->SetModel(myHost->programList->GetModel());
    connect(myHost->programList, SIGNAL(ProgChanged(QModelIndex)),
            ui->Programs,SLOT(OnProgChange(QModelIndex)));
    connect(ui->Programs,SIGNAL(ChangeProg(QModelIndex)),
            myHost->programList,SLOT(ChangeProg(QModelIndex)));

    //vst plugins browser
#ifndef __GNUC__
    //sse2 crash with gcc

    listVstPluginsModel = new QFileSystemModel(this);
    listVstPluginsModel->setReadOnly(true);
    listVstPluginsModel->setResolveSymlinks(true);
    QStringList fileFilter;
    fileFilter << "*.dll";
    listVstPluginsModel->setNameFilters(fileFilter);
    listVstPluginsModel->setNameFilterDisables(false);
    listVstPluginsModel->setRootPath(ConfigDialog::defaultVstPath());
    ui->VstBrowser->setModel(listVstPluginsModel);

#endif

    mySceneView = new View::SceneView(myHost, myHost->objFactory, ui->hostView, ui->projectView, ui->programView, ui->groupView, this);
    mySceneView->setModel(myHost->GetModel());

    ui->solverView->setModel(&myHost->solver->model);

//    myHost->SetSampleRate( ConfigDialog::defaultSampleRate() );
    myHost->Open();

    ui->treeHostModel->setModel(myHost->GetModel());

    readSettings();

//    //load default setup file
//    currentSetupFile = ConfigDialog::defaultSetupFile();
//    if(!currentSetupFile.isEmpty()) {
//        if(!SetupFile::LoadFromFile(currentSetupFile))
//            currentSetupFile = "";
//    }

//    //load default project file
//    currentProjectFile = ConfigDialog::defaultProjectFile();
//    if(!currentProjectFile.isEmpty()) {
//        if(!ProjectFile::LoadFromFile(currentProjectFile))
//            currentProjectFile = "";
//    }
}

MainWindow::~MainWindow()
{
    debug2(<< "delete MainWindow" << hex << (long)this)
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (userReallyWantsToQuit()) {
//        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::hideEvent ( QHideEvent * event )
{

}

bool MainWindow::userReallyWantsToQuit()
{
    return true;
}


void MainWindow::BuildListTools()
{
    QStringList headerLabels;
    headerLabels << "Name";

//audio devices (vst in/out)
//================================
    listAudioDevModel = new ListToolsModel();
    listAudioDevModel->setHorizontalHeaderLabels(headerLabels);
    QStandardItem *parentItem = listAudioDevModel->invisibleRootItem();

    //vst audio in
    QStandardItem *item = new QStandardItem(tr("Vst audio input"));
    ObjectInfo infoai;
    infoai.nodeType = NodeType::object;
    infoai.objType = ObjType::AudioInterfaceIn;
    infoai.name = "Vst audio In";
    item->setData(QVariant::fromValue(infoai), UserRoles::objInfo);
    parentItem->appendRow(item);

    //vst audio out
    item = new QStandardItem(tr("Vst audio output"));
    ObjectInfo infoao;
    infoao.nodeType = NodeType::object;
    infoao.objType = ObjType::AudioInterfaceOut;
    infoao.name = "Vst audio Out";
    item->setData(QVariant::fromValue(infoao), UserRoles::objInfo);
    parentItem->appendRow(item);

    ui->treeAudioInterfaces->setModel(listAudioDevModel);
    ui->treeAudioInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);

//midi devices (vst in/out)
//================================
    listMidiDevModel = new ListToolsModel();
    listMidiDevModel->setHorizontalHeaderLabels(headerLabels);
    parentItem = listMidiDevModel->invisibleRootItem();

    //vst midi in
    item = new QStandardItem(tr("Vst midi input"));
    ObjectInfo infomi;
    infomi.nodeType = NodeType::object;
    infomi.objType = ObjType::MidiInterface;
    infomi.inputs = 1;
    infomi.name = "Vst midi In";
    item->setData(QVariant::fromValue(infomi), UserRoles::objInfo);
    parentItem->appendRow(item);

    //vst midi out
    item = new QStandardItem(tr("Vst midi output"));
    ObjectInfo infomo;
    infomo.nodeType = NodeType::object;
    infomo.objType = ObjType::MidiInterface;
    infomo.outputs = 1;
    infomo.name = "Vst midi Out";
    item->setData(QVariant::fromValue(infomo), UserRoles::objInfo);
    parentItem->appendRow(item);

    ui->treeMidiInterfaces->setModel(listMidiDevModel);
    ui->treeMidiInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);


//tools
//================================
    listToolsModel = new ListToolsModel();
    listToolsModel->setHorizontalHeaderLabels(headerLabels);
    parentItem = listToolsModel->invisibleRootItem();

    //vst automation
    item = new QStandardItem(tr("Vst Automation"));
    ObjectInfo info;
    info.nodeType = NodeType::object;
    info.objType = ObjType::VstAutomation;
    item->setData(QVariant::fromValue(info), UserRoles::objInfo);
    parentItem->appendRow(item);

    //midi parameters
    item = new QStandardItem(tr("Midi to parameter"));
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

    ui->treeTools->setModel(listToolsModel);
    ui->treeTools->header()->setResizeMode(0,QHeaderView::Stretch);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("statusBar", ui->statusBar->isVisible());
    settings.endGroup();
    settings.setValue("lastVstPath", ui->VstBrowser->path());
    ui->Programs->writeSettings();
    settings.sync();
}

void MainWindow::readSettings()
{
    QSettings settings;

//    if( !settings.value("splashHide",false).toBool() ) {
//        Splash spl;
//        spl.exec();
//    }

    //add dock visibility to the menu
    QList<QDockWidget*>listDocks;
    listDocks << ui->dockTools;
    listDocks << ui->dockMidiDevices;
    listDocks << ui->dockAudioDevices;
    listDocks << ui->dockVstBrowser;
    listDocks << ui->dockPrograms;

    foreach(QDockWidget *dock, listDocks) {
        ui->menuView->addAction(dock->toggleViewAction());
        ui->mainToolBar->addAction(dock->toggleViewAction());
    }

    QList<QDockWidget*>listDocksNoToolbar;
    listDocksNoToolbar << ui->dockSolver;
    listDocksNoToolbar << ui->dockHostModel;
    foreach(QDockWidget *dock, listDocksNoToolbar) {
        ui->menuView->addAction(dock->toggleViewAction());
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

    if(settings.contains("geometry") && settings.contains("state")) {
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("state").toByteArray());
        bool statusb = settings.value("statusBar",false).toBool();
        ui->actionStatus_bar->setChecked( statusb );
        ui->statusBar->setVisible(statusb);
    } else {
        resetSettings();
    }

    settings.endGroup();

    ui->Programs->readSettings();
}

void MainWindow::resetSettings()
{
    QList<QDockWidget*>listDocksVisible;
    listDocksVisible << ui->dockTools;
    listDocksVisible << ui->dockMidiDevices;
    listDocksVisible << ui->dockAudioDevices;
    listDocksVisible << ui->dockVstBrowser;

    listDocksVisible << ui->dockPrograms;

    foreach(QDockWidget *dock, listDocksVisible) {
        dock->setFloating(false);
        dock->setVisible(true);
    }

    QList<QDockWidget*>listDocksHidden;
    listDocksHidden << ui->dockSolver;
    listDocksHidden << ui->dockHostModel;
    foreach(QDockWidget *dock, listDocksHidden) {
        dock->setFloating(false);
        dock->setVisible(false);
    }

    ui->Programs->resetSettings();

    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockTools);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockMidiDevices);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockAudioDevices);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockVstBrowser);

    addDockWidget(Qt::RightDockWidgetArea,  ui->dockPrograms);
    addDockWidget(Qt::RightDockWidgetArea,  ui->dockSolver);
    addDockWidget(Qt::RightDockWidgetArea,  ui->dockHostModel);

    tabifyDockWidget(ui->dockTools,ui->dockMidiDevices);
    tabifyDockWidget(ui->dockMidiDevices,ui->dockAudioDevices);
    tabifyDockWidget(ui->dockAudioDevices,ui->dockVstBrowser);

    tabifyDockWidget(ui->dockHostModel,ui->dockSolver);

    ui->actionHost_panel->setChecked(true);
    ui->actionProject_panel->setChecked(true);
    ui->actionProgram_panel->setChecked(true);
    ui->actionGroup_panel->setChecked(true);

    ui->actionTool_bar->setChecked(true);
    ui->actionStatus_bar->setChecked(false);
    ui->statusBar->setVisible(false);

    int h = ui->splitterPanels->height()/4;
    QList<int>heights;
    heights << h << h << h << h;
    ui->splitterPanels->setSizes(heights);

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

     if(SetupFile::LoadFromFile(myHost,fileName)) {
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

     if(ProjectFile::LoadFromFile(myHost,fileName)) {
         ConfigDialog::AddRecentProjectFile(fileName);
         currentProjectFile=fileName;
         updateRecentFileActions();
     }
 }

void MainWindow::programParkingModelChanges(QStandardItemModel *model)
{
    ui->programParkList->setModel(model);
}

void MainWindow::groupParkingModelChanges(QStandardItemModel *model)
{
    ui->groupParkList->setModel(model);
}

void MainWindow::on_actionRestore_default_layout_triggered()
{
    resetSettings();
}
void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog conf(myHost);
    conf.exec();
}
void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project or Setup"), "", tr("Supported Files (*.%1 *.%2);;Project Files (*.%1);;Setup Files (*.%2)").arg(PROJECT_FILE_EXTENSION).arg(SETUP_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(fileName.endsWith(PROJECT_FILE_EXTENSION, Qt::CaseInsensitive)) {
        if(ProjectFile::LoadFromFile(myHost,fileName)) {
            ConfigDialog::AddRecentProjectFile(fileName);
            currentProjectFile = fileName;
            updateRecentFileActions();
        }
        return;
    }

    if(fileName.endsWith(SETUP_FILE_EXTENSION, Qt::CaseInsensitive)) {
        if(SetupFile::LoadFromFile(myHost,fileName)) {
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
    ProjectFile::Clear(myHost);
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
    ProjectFile::SaveToFile(myHost,currentProjectFile);
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

    if(ProjectFile::SaveToFile(myHost,fileName)) {
        settings.setValue("lastProjectDir",QFileInfo(fileName).absolutePath());
        ConfigDialog::AddRecentProjectFile(fileName);
        currentProjectFile = fileName;
        updateRecentFileActions();
    }
}

void MainWindow::on_actionNew_Setup_triggered()
{
    SetupFile::Clear(myHost);
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

    SetupFile::SaveToFile(myHost,currentSetupFile);
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


    if(SetupFile::SaveToFile(myHost,fileName)) {
        settings.setValue("lastSetupDir",QFileInfo(fileName).absolutePath());
        ConfigDialog::AddRecentSetupFile(fileName);
        currentSetupFile = fileName;
        updateRecentFileActions();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog about;
    about.exec();
}
