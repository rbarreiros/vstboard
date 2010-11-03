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

//    solverScene = 0;

    mainHost = MainHost::Get(this);

    ui->setupUi(this);

    ui->treeAudioInterfaces->setModel(mainHost->GetAudioDevicesModel());
    ui->treeAudioInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeAudioInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->resizeSection(1,30);
    ui->treeAudioInterfaces->header()->resizeSection(2,30);

    BuildListMidiInterfaces();
    BuildListTools();

    //ProjectFile
    project = new Project::ProjectFile(this);
    ui->treePrograms->setModel(&project->programModel);

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
    timerVu = new QTimer(this);
    timerVu->start(40);

    //start timer (used by cpuload)
//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateCpuLoad()));
//    timer->start(200);

    ui->treeHostModel->setModel(mainHost->GetModel());
    ui->treeParking->setModel(mainHost->GetParkingModel());

//    ui->sceneView->setModel(mainHost->GetModel());
//    connect(ui->actionZoom_in,SIGNAL(triggered()),
//            ui->sceneView->viewProgram,SLOT(zoomIn()));
//    connect(ui->actionZoom_out,SIGNAL(triggered()),
//            ui->sceneView->viewProgram,SLOT(zoomOut()));
//    connect(ui->actionZoom_100,SIGNAL(triggered()),
//            ui->sceneView->viewProgram,SLOT(zoomReset()));

    mySceneView = new View::SceneView(ui->hostView, ui->projectView, ui->programView);
    mySceneView->setModel(mainHost->GetModel());

    ui->solverView->setModel(&mainHost->solver.model);

//    connect(ui->actionHost_panel,SIGNAL(triggered(bool)),
//            ui->sceneView,SLOT(ToggleHostView(bool)));
//    connect(ui->sceneView,SIGNAL(hostShown(bool)),
//            ui->actionHost_panel,SLOT(setChecked(bool)));
//    ui->sceneView->ToggleHostView(false);

//    connect(ui->actionProject_panel,SIGNAL(triggered(bool)),
//            ui->sceneView,SLOT(ToggleProjectView(bool)));
//    connect(ui->sceneView,SIGNAL(projectShown(bool)),
//            ui->actionProject_panel,SLOT(setChecked(bool)));
//    ui->sceneView->ToggleProjectView(false);

//    connect(ui->actionProgram_panel,SIGNAL(triggered(bool)),
//            ui->sceneView,SLOT(ToggleProgramView(bool)));
//    connect(ui->sceneView,SIGNAL(programShown(bool)),
//            ui->actionProgram_panel,SLOT(setChecked(bool)));
//    ui->sceneView->ToggleProgramView(true);

    connect(mainHost, SIGNAL(ProgramChanged(int)),
            this,SLOT(OnProgramChange(int)));

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

    //    connect(mainHost,SIGNAL(finished()),
    //            this,SLOT(close()));
    //    connect(mainHost,SIGNAL(terminated()),
    //            this,SLOT(close()));
    //    mainHost->start(QThread::TimeCriticalPriority);



}

MainWindow::~MainWindow()
{
    writeSettings();
    delete mySceneView;
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

void MainWindow::OnProgramChange(int prog)
{
    //    //we only show the program layer's program
    //    if(layer!=DefaultLayers::program)
    //        return;

    //    Project::Program *prg = project->GetProgram(prog);
    //    QStandardItem *item = project->programModel.invisibleRootItem()->child(prg->progGroup)->child(prg->progPlaceInGroup);
    //    ui->treePrograms->setCurrentIndex(project->programModel.indexFromItem(item));

    QStandardItem *item = project->ItemFromProgId(prog);
    if(!item)
        return;
    ui->treePrograms->expand(item->parent()->index());
    ui->treePrograms->selectionModel()->select(item->index(), QItemSelectionModel::ClearAndSelect );
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

void MainWindow::BuildListMidiInterfaces()
{
    QStringList headerLabels;
    headerLabels << "Name";
    headerLabels << "In";
    headerLabels << "Out";

    listMidiInterfacesModel.setHorizontalHeaderLabels(  headerLabels );
    QStandardItem *parentItem = listMidiInterfacesModel.invisibleRootItem();

    QString lastName;
    int cptDuplicateNames=0;

    for(int i=0;i<Pm_CountDevices();i++) {
        QList<QStandardItem *>  items;
        const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);

        QString devName= QString::fromStdString(devInfo->name);
        if(lastName == devName) {
            cptDuplicateNames++;
        } else {
            cptDuplicateNames=0;
        }
        lastName = devName;

        ObjectInfo obj;
        obj.nodeType = NodeType::object;
        obj.objType = ObjType::MidiInterface;
        obj.id = i;
        obj.name = devName;
        obj.api = QString::fromStdString( devInfo->interf );
        obj.duplicateNamesCounter = cptDuplicateNames;
        obj.inputs = devInfo->input;
        obj.outputs = devInfo->output;

        items << new QStandardItem(devName);
        items << new QStandardItem(QString::number(devInfo->input));
        items << new QStandardItem(QString::number(devInfo->output));

        //        items[0]->setData(cptDuplicateNames,Qt::UserRole+1);
        //        items[0]->setData(devInfo->input,Qt::UserRole+2);
        //        items[0]->setData(devInfo->output,Qt::UserRole+3);
        items[0]->setData(QVariant::fromValue(obj), UserRoles::objInfo);

        parentItem->appendRow(items);
    }

    ui->treeMidiInterfaces->setModel(&listMidiInterfacesModel);

    ui->treeMidiInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeMidiInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);

    ui->treeMidiInterfaces->header()->resizeSection(1,30);
    ui->treeMidiInterfaces->header()->resizeSection(2,30);
}



//void MainWindow::NewSolver(orderedNodes *renderLines)
//{
//    ui->solverView->setScene(0);

//    if(solverScene)
//        delete solverScene;

//    solverScene = new View::SolverScene(this);
//    ui->solverView->setScene(solverScene);
//    solverScene->NewSolver(renderLines);
//  //  contProject->SetModelItem(mainHost->model->invisibleRootItem()->child(0)->child(2));


//}

void MainWindow::UpdateCpuLoad()
{
    ui->progressCpuLoad->setValue(mainHost->GetCpuLoad());
}

void MainWindow::on_actionLoad_triggered()
{
    //    MainHost::Get()->ClearHost();
    //QTimer::singleShot(0,project,SLOT(LoadFromFile()));
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


void MainWindow::OnFileLoaded()
{
    //    contProject->SetModelItem(mainHost->model->invisibleRootItem()->child(0)->child(2));
}

void MainWindow::on_treePrograms_activated(QModelIndex index)
{
    if(!index.isValid())
        return;

    bool ok;
    int prog = index.data(UserRoles::programNumber).toInt(&ok);
    if(!ok)
        return;

    Project::Program *prg = project->GetProgram(prog);
    if(!prg) {
        debug("MainWindow on_treePrograms_activated : prog not found")
                return;
    }

    mainHost->SetProgram(prg->progIndex);
}

void MainWindow::on_treePrograms_clicked(QModelIndex index)
{
    if(!index.isValid())
        return;

    bool ok;
    int prog = index.data(UserRoles::programNumber).toInt(&ok);
    if(!ok)
        return;

    Project::Program *prg = project->GetProgram(prog);
    if(!prg) {
        debug("MainWindow on_treePrograms_clicked : prog not found")
                return;
    }

    mainHost->SetProgram(prg->progIndex);
}

void MainWindow::on_actionShortcuts_toggled(bool onOff)
{
    MainConfig::Get()->shortcutConfig = onOff;
    ui->dockMidiDevices->setMouseTracking(onOff);
}

void MainWindow::on_treeHostModel_doubleClicked(QModelIndex index)
{
    on_actionDelete_triggered();
}

void MainWindow::on_actionDelete_triggered()
{
    QModelIndexList indexes = ui->treeHostModel->selectionModel()->selectedRows();
    for(int i=indexes.size()-1; i>=0; i--) {
        //        mainHost->modelProxy->Remove(indexes.at(i).row(),indexes.at(i).parent());
        //        mainHost->model->removeRow(indexes.at(i).row(),indexes.at(i).parent());
    }
}

//void MainWindow::on_treeParking_doubleClicked(QModelIndex index)
//{
//    QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->GetObj(index);
//    if(objPtr.isNull())
//        return;
//}

//#ifndef QT_NO_DEBUG
//void MainWindow::on_actionTest_triggered()
//{
//    tst.Start();
//    QTimer::singleShot(2000,&tst,SLOT(Stop()));
//}

//#endif

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
        restoreDefaultDocking();
    }

    settings.endGroup();
}

void MainWindow::restoreDefaultDocking()
{
    tabifyDockWidget(ui->dockTools,ui->dockMidiDevices);
    tabifyDockWidget(ui->dockMidiDevices,ui->dockAudioDevices);
    tabifyDockWidget(ui->dockAudioDevices,ui->dockVstBrowser);

    tabifyDockWidget(ui->dockParking,ui->dockSolver);
    tabifyDockWidget(ui->dockSolver,ui->dockHostModel);

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

void MainWindow::on_actionProject_panel_triggered(bool checked)
{

}
