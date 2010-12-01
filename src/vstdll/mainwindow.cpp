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


MainWindow *MainWindow::theMainWindow=0;



MainWindow *MainWindow::Create(QWidget *parent)
{
    if(!theMainWindow)
        theMainWindow = new MainWindow(parent);

    return theMainWindow;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    MainHost *mainHost = MainHost::Get();

    connect(ui->mainToolBar, SIGNAL(visibilityChanged(bool)),
            ui->actionTool_bar, SLOT(setChecked(bool)));

    BuildListTools();

    //programs
    ui->Programs->SetModel(mainHost->programList->GetModel());
    connect(mainHost->programList, SIGNAL(ProgChanged(QModelIndex)),
            ui->Programs,SLOT(OnProgChange(QModelIndex)));
    connect(ui->Programs,SIGNAL(ChangeProg(QModelIndex)),
            mainHost->programList,SLOT(ChangeProg(QModelIndex)));

    //vst plugins browser
    listVstPluginsModel = new QFileSystemModel(this);
    listVstPluginsModel->setReadOnly(true);
    listVstPluginsModel->setResolveSymlinks(true);
    QStringList fileFilter;
    fileFilter << "*.dll";
    listVstPluginsModel->setNameFilters(fileFilter);
    listVstPluginsModel->setNameFilterDisables(false);
    listVstPluginsModel->setRootPath(ConfigDialog::defaultVstPath());
    ui->VstBrowser->setModel(listVstPluginsModel);

    mySceneView = new View::SceneView(ui->hostView, ui->projectView, ui->programView, ui->insertView, this);
    mySceneView->setModel(mainHost->GetModel());

    ui->solverView->setModel(&mainHost->solver.model);

//    mainHost->SetSampleRate( ConfigDialog::defaultSampleRate() );
    mainHost->Open();

    ui->treeHostModel->setModel(mainHost->GetModel());
    ui->listParking->setModel(mainHost->GetParkingModel());
    ui->listParking->setRootIndex(mainHost->GetParkingModel()->invisibleRootItem()->child(0)->index());

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
    theMainWindow=0;
    delete ui;
}

void MainWindow::BuildListTools()
{
    QStringList headerLabels;
    headerLabels << "Name";

    listToolsModel.setHorizontalHeaderLabels(headerLabels);
    QStandardItem *parentItem = listToolsModel.invisibleRootItem();

    //vst audio in
    QStandardItem *item = new QStandardItem(tr("Vst audio input"));
    ObjectInfo info;
    info.nodeType = NodeType::object;
    info.objType = ObjType::AudioInterfaceIn;
    info.name = "Vst audio In";
    item->setData(QVariant::fromValue(info), UserRoles::objInfo);
    parentItem->appendRow(item);

    //vst audio out
    item = new QStandardItem(tr("Vst audio output"));
    info.nodeType = NodeType::object;
    info.objType = ObjType::AudioInterfaceOut;
    info.name = "Vst audio Out";
    item->setData(QVariant::fromValue(info), UserRoles::objInfo);
    parentItem->appendRow(item);

    //midi parameters
    item = new QStandardItem(tr("MidiCC to parameter"));
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

    ui->treeTools->setModel(&listToolsModel);
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

    ui->Programs->resetSettings();

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
    ui->actionProgram_panel->setChecked(true);
    ui->actionInsert_panel->setChecked(true);

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

     if(SetupFile::LoadFromFile(fileName)) {
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

     if(ProjectFile::LoadFromFile(fileName)) {
         ConfigDialog::AddRecentProjectFile(fileName);
         currentProjectFile=fileName;
         updateRecentFileActions();
     }
 }

void MainWindow::on_actionRestore_default_layout_triggered()
{
    resetSettings();
}
void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog conf(this);
    conf.exec();
}
void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project or Setup"), "", tr("Supported Files (*.%1 *.%2);;Project Files (*.%1);;Setup Files (*.%2)").arg(PROJECT_FILE_EXTENSION).arg(SETUP_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(fileName.endsWith(PROJECT_FILE_EXTENSION, Qt::CaseInsensitive)) {
        if(ProjectFile::LoadFromFile(fileName)) {
            ConfigDialog::AddRecentProjectFile(fileName);
            currentProjectFile = fileName;
            updateRecentFileActions();
        }
        return;
    }

    if(fileName.endsWith(SETUP_FILE_EXTENSION, Qt::CaseInsensitive)) {
        if(SetupFile::LoadFromFile(fileName)) {
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
    ProjectFile::Clear();
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
    ProjectFile::SaveToFile(currentProjectFile);
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

    if(ProjectFile::SaveToFile(fileName)) {
        settings.setValue("lastProjectDir",QFileInfo(fileName).absolutePath());
        ConfigDialog::AddRecentProjectFile(fileName);
        currentProjectFile = fileName;
        updateRecentFileActions();
    }
}

void MainWindow::on_actionNew_Setup_triggered()
{
    SetupFile::Clear();
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

    SetupFile::SaveToFile(currentSetupFile);
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


    if(SetupFile::SaveToFile(fileName)) {
        settings.setValue("lastSetupDir",QFileInfo(fileName).absolutePath());
        ConfigDialog::AddRecentSetupFile(fileName);
        currentSetupFile = fileName;
        updateRecentFileActions();
    }
}
