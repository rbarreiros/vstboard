/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include "projectfile/setupfile.h"
#include "projectfile/projectfile.h"
#include "views/configdialog.h"
#include "views/aboutdialog.h"
#include "connectables/objectinfo.h"
#ifndef VST_PLUGIN
    #include "views/splash.h"
#endif
//#include "imagecollection.h"


MainWindow::MainWindow(MainHost * myHost,QWidget *parent) :
        QMainWindow(parent),
        openedPrompt(false),
        listToolsModel(0),
#ifdef VST_PLUGIN
        listAudioDevModel(0),
        listMidiDevModel(0),
#endif
        listVstPluginsModel(0),
        solverScene(0),
        mySceneView(0),
        ui(new Ui::MainWindow),
        myHost(myHost)
{
    myHost->mainWindow=this;
    connect(myHost,SIGNAL(programParkingModelChanged(QStandardItemModel*)),
            this,SLOT(programParkingModelChanges(QStandardItemModel*)));
    connect(myHost,SIGNAL(groupParkingModelChanged(QStandardItemModel*)),
            this,SLOT(groupParkingModelChanges(QStandardItemModel*)));

    ui->setupUi(this);

#ifdef VST_PLUGIN
    ui->actionRefresh_Audio_devices->setDisabled(true);
    ui->actionRefresh_Midi_devices->setDisabled(true);
#endif

    setWindowTitle(APP_NAME);

//    ImageCollection::Create(this);

    connect(ui->mainToolBar, SIGNAL(visibilityChanged(bool)),
            ui->actionTool_bar, SLOT(setChecked(bool)));

#ifndef VST_PLUGIN
    //audio devices
    ui->treeAudioInterfaces->setModel(myHost->audioDevices->GetModel());
    ui->treeAudioInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeAudioInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setResizeMode(3,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->resizeSection(1,30);
    ui->treeAudioInterfaces->header()->resizeSection(2,30);
    ui->treeAudioInterfaces->header()->resizeSection(3,40);
    ui->treeAudioInterfaces->expand( myHost->audioDevices->AsioIndex );

    //midi devices
    ui->treeMidiInterfaces->setModel(myHost->midiDevices->GetModel());
    ui->treeMidiInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeMidiInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->resizeSection(1,30);
    ui->treeMidiInterfaces->header()->resizeSection(2,30);
#endif

    BuildListTools();

    //programs
    myHost->programList->SetMainWindow(this);
    ui->Programs->SetModel(myHost->programList->GetModel());
    connect(myHost->programList, SIGNAL(ProgChanged(QModelIndex)),
            ui->Programs,SLOT(OnProgChange(QModelIndex)));
    connect(ui->Programs,SIGNAL(ChangeProg(QModelIndex)),
            myHost->programList,SLOT(ChangeProg(QModelIndex)));

    connect(ui->Programs,SIGNAL(ProgAutoSave(Autosave::Enum)),
            myHost->programList, SLOT(SetProgAutosave(Autosave::Enum)));
    connect(myHost->programList,SIGNAL(ProgAutosaveChanged(Autosave::Enum)),
            ui->Programs,SLOT(OnProgAutoSaveChanged(Autosave::Enum)));

    connect(ui->Programs,SIGNAL(GroupAutoSave(Autosave::Enum)),
            myHost->programList, SLOT(SetGroupAutosave(Autosave::Enum)));
    connect(myHost->programList,SIGNAL(GroupAutosaveChanged(Autosave::Enum)),
            ui->Programs,SLOT(OnGroupAutoSaveChanged(Autosave::Enum)));

    //vst plugins browser
#if !defined(__GNUC__) || !defined(VST_PLUGIN)
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
    mySceneView->SetParkings(ui->programParkList, ui->groupParkList);
    mySceneView->setModel(myHost->GetModel());

    ui->solverView->setModel(&myHost->solver->model);

#ifndef VST_PLUGIN
    myHost->SetSampleRate( ConfigDialog::defaultSampleRate() );
#endif
    myHost->Open();

    ui->treeHostModel->setModel(myHost->GetModel());

    //load default setup file
    currentSetupFile = ConfigDialog::defaultSetupFile();
    if(!currentSetupFile.isEmpty()) {
        if(!SetupFile::LoadFromFile(myHost,currentSetupFile))
            currentSetupFile = "";
    }

    //load default project file
    currentProjectFile = ConfigDialog::defaultProjectFile();
    if(!currentProjectFile.isEmpty()) {
        if(!ProjectFile::LoadFromFile(myHost,currentProjectFile))
            currentProjectFile = "";
    }

}

MainWindow::~MainWindow()
{
    debug2(<< "delete MainWindow" << hex << (long)this)
    if(ui)
        delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!userWantsToUnloadSetup()) {
        event->ignore();
        return;
    }

    if(!userWantsToUnloadProject()) {
        event->ignore();
        return;
    }

    writeSettings();
    event->accept();
}

bool MainWindow::userWantsToUnloadSetup()
{
    if(openedPrompt)
        return false;

    QSettings settings;
    Autosave::Enum onUnsaved = (Autosave::Enum)settings.value("onUnsavedSetup",Autosave::prompt).toInt();

    if(onUnsaved == Autosave::discard)
        return true;

    if(!myHost->hostContainer->IsDirty())
        return true;

    if(onUnsaved == Autosave::save) {
        MainWindow::on_actionSave_Setup_triggered();
        return true;
    }

    //prompt
    QMessageBox msgBox;
    openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The setup has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res=msgBox.exec();
    openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel :
            return false;
        case QMessageBox::Save :
            MainWindow::on_actionSave_Setup_triggered();
            break;
        default :
            break;
    }

    return true;
}

bool MainWindow::userWantsToUnloadProject()
{
    if(openedPrompt)
        return false;

    QSettings settings;
    Autosave::Enum onUnsaved = (Autosave::Enum)settings.value("onUnsavedProject",Autosave::prompt).toInt();

    if(onUnsaved == Autosave::discard)
        return true;

    if(!myHost->programList->userWantsToUnloadProgram())
        return false;

    if(!myHost->programList->userWantsToUnloadGroup())
        return false;

    if(!myHost->programList->isDirty())
        return true;

    if(onUnsaved == Autosave::save) {
        MainWindow::on_actionSave_triggered();
        return true;
    }

    //prompt
    QMessageBox msgBox;
    openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The project has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res=msgBox.exec();
    openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel :
            return false;
        case QMessageBox::Save :
            MainWindow::on_actionSave_triggered();
            return true;
    }

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

    QStandardItem *parentItem=0;
    QStandardItem *item=0;

#ifdef VST_PLUGIN
//audio devices (vst in/out)
//================================
    listAudioDevModel = new ListToolsModel();
    listAudioDevModel->setHorizontalHeaderLabels(headerLabels);
    parentItem = listAudioDevModel->invisibleRootItem();

    //vst audio in
    item = new QStandardItem(tr("Vst audio input"));
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
#endif

//tools
//================================
    listToolsModel = new ListToolsModel();
    listToolsModel->setHorizontalHeaderLabels(headerLabels);
    parentItem = listToolsModel->invisibleRootItem();
    ObjectInfo info;

#ifdef VST_PLUGIN
    //vst automation
    item = new QStandardItem(tr("Vst Automation"));
    info.nodeType = NodeType::object;
    info.objType = ObjType::VstAutomation;
    item->setData(QVariant::fromValue(info), UserRoles::objInfo);
    parentItem->appendRow(item);
#endif

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

    //host controller
    item = new QStandardItem(tr("Host Controller"));
    info.nodeType = NodeType::object;
    info.objType = ObjType::HostController;
    item->setData(QVariant::fromValue(info),UserRoles::objInfo);
    parentItem->appendRow(item);

    ui->treeTools->setModel(listToolsModel);
    ui->treeTools->header()->setResizeMode(0,QHeaderView::Stretch);
}

void MainWindow::on_actionLoad_triggered()
{
    if(!userWantsToUnloadProject())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a Project file"), "", tr("Project Files (*.%1)").arg(PROJECT_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(ProjectFile::LoadFromFile(myHost,fileName)) {
        ConfigDialog::AddRecentProjectFile(fileName);
        currentProjectFile = fileName;
        updateRecentFileActions();
    }
}

void MainWindow::on_actionNew_triggered()
{
    if(!userWantsToUnloadProject())
        return;

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

void MainWindow::on_actionLoad_Setup_triggered()
{
    if(!userWantsToUnloadSetup())
        return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a Setup file"), "", tr("Setup Files (*.%1)").arg(SETUP_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(SetupFile::LoadFromFile(myHost,fileName)) {
        ConfigDialog::AddRecentSetupFile(fileName);
        currentSetupFile = fileName;
        updateRecentFileActions();
    }
}

void MainWindow::on_actionNew_Setup_triggered()
{
    if(!userWantsToUnloadSetup())
        return;

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

void MainWindow::on_actionShortcuts_toggled(bool onOff)
{
//    myHost->mainConfig->shortcutConfig = onOff;
    ui->dockMidiDevices->setMouseTracking(onOff);
}

void MainWindow::on_actionConfig_triggered()
{
    ConfigDialog conf(myHost,this);
    conf.exec();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("statusBar", ui->statusBar->isVisible());
    settings.setValue("splitPan", ui->splitterPanels->saveState());
    settings.setValue("splitProg", ui->splitterProg->saveState());
    settings.setValue("splitGroup", ui->splitterGroup->saveState());

    settings.setValue("planelHost", ui->actionHost_panel->isChecked());
    settings.setValue("planelProject", ui->actionProject_panel->isChecked());
    settings.setValue("planelProgram", ui->actionProgram_panel->isChecked());
    settings.setValue("planelGroup", ui->actionGroup_panel->isChecked());

    settings.endGroup();
    settings.setValue("lastVstPath", ui->VstBrowser->path());
    ui->Programs->writeSettings();
    settings.sync();
}

void MainWindow::readSettings()
{
    QSettings settings;

#ifndef VST_PLUGIN
    if( !settings.value("splashHide",false).toBool() ) {
        Splash spl;
        spl.exec();
    }
#endif

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

    ui->splitterProg->setStretchFactor(0,100);
    ui->splitterGroup->setStretchFactor(0,100);

    if(settings.contains("splitPan"))
        ui->splitterPanels->restoreState(settings.value("splitPan").toByteArray());
    if(settings.contains("splitProg"))
        ui->splitterProg->restoreState(settings.value("splitProg").toByteArray());
    if(settings.contains("splitGroup"))
        ui->splitterGroup->restoreState(settings.value("splitGroup").toByteArray());

    ui->actionHost_panel->setChecked( settings.value("planelHost",true).toBool() );
    ui->actionProject_panel->setChecked( settings.value("planelProject",false).toBool() );
    ui->actionProgram_panel->setChecked( settings.value("planelProgram",true).toBool() );
    ui->actionGroup_panel->setChecked( settings.value("planelGroup",true).toBool() );

    settings.endGroup();

    ui->Programs->readSettings();

    updateRecentFileActions();
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
//    tabifyDockWidget(ui->dockAudioDevices,ui->dockVstBrowser);

    tabifyDockWidget(ui->dockHostModel,ui->dockSolver);

    ui->actionHost_panel->setChecked(true);
    ui->actionProject_panel->setChecked(false);
    ui->actionProgram_panel->setChecked(true);
    ui->actionGroup_panel->setChecked(true);

    ui->actionTool_bar->setChecked(true);
    ui->actionStatus_bar->setChecked(false);
    ui->statusBar->setVisible(false);

    int h = ui->splitterPanels->height()/4;
    QList<int>heights;
    heights << h << h << h << h;
    ui->splitterPanels->setSizes(heights);

    QList<int> szProg;
    szProg << 1000 << 100;
    ui->splitterProg->setSizes(szProg);

    QList<int> szGrp;
    szGrp << 1000 << 100;
    ui->splitterGroup->setSizes(szGrp);
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
    if(!userWantsToUnloadSetup())
        return;

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
    if(!userWantsToUnloadProject())
        return;

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

#ifndef VST_PLUGIN
void MainWindow::on_actionRefresh_Audio_devices_triggered()
{
    ui->treeAudioInterfaces->setModel(myHost->audioDevices->GetModel());
    ui->treeAudioInterfaces->expand( myHost->audioDevices->AsioIndex );
}

void MainWindow::on_actionRefresh_Midi_devices_triggered()
{
    ui->treeMidiInterfaces->setModel(myHost->midiDevices->GetModel());
}
#endif



void MainWindow::on_actionRestore_default_layout_triggered()
{
    resetSettings();
}


