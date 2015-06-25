/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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
#include "mainhost.h"
#include "mainwindow.h"
#include "models/programsmodel.h"
#include "connectables/container.h"

#ifdef VSTSDK
    #include "connectables/vstplugin.h"
    int MainHost::vstUsersCounter=0;
#endif

#include "projectfile/fileversion.h"
#include "projectfile/projectfile.h"
#include "views/configdialog.h"

quint32 MainHost::currentFileVersion=PROJECT_AND_SETUP_FILE_VERSION;

MainHost::MainHost(QObject *parent, QString settingsGroup) :
    QObject(parent),
    objFactory(0),
    mainWindow(0),
    solverNeedAnUpdate(false),
    solverUpdateEnabled(true),
//    mutexListCables(new QMutex(QMutex::Recursive)),
    settingsGroup(settingsGroup),
    undoProgramChangesEnabled(false),
    solver(new PathSolver(this))
{
    doublePrecision=GetSetting("doublePrecision",false).toBool();

    setObjectName("MainHost");

#ifdef SCRIPTENGINE
    scriptEngine = new QScriptEngine(this);
    QScriptValue scriptObj = scriptEngine->newQObject(this);
    scriptEngine->globalObject().setProperty("MainHost", scriptObj);
#endif

#ifdef VSTSDK
    if(!vst::CVSTHost::Get())
        vstHost = new vst::CVSTHost();
    else
        vstHost = vst::CVSTHost::Get();

    vstUsersCounter++;
#endif

    model = new HostModel(this);
    model->setObjectName("MainModel");
    model->setColumnCount(1);

    sampleRate = 44100.0;
    bufferSize = 100;

    currentTempo=120;
    currentTimeSig1=4;
    currentTimeSig2=4;

    renderer = new Renderer(this);

    programsModel = new ProgramsModel(this);

    //timer
    timeFromStart.start();

    updateViewTimer = new QTimer(this);
    updateViewTimer->start(40);

    connect(this,SIGNAL(SolverToUpdate()),
            this,SLOT(UpdateSolver()),
            Qt::QueuedConnection);


}

MainHost::~MainHost()
{
    EnableSolverUpdate(false);

    updateViewTimer->stop();
    delete updateViewTimer;
    updateViewTimer=0;

//    mutexListCables->lock();
//    workingListOfCables.clear();
//    mutexListCables->unlock();

    hashCables lstCables;
    solver->Resolve(lstCables, renderer);
//    solver->Resolve(workingListOfCables, renderer);
    delete renderer;

    mainContainer.clear();
    hostContainer.clear();
    projectContainer.clear();
    groupContainer.clear();
    programContainer.clear();

    delete objFactory;

#ifdef VSTSDK
    vstUsersCounter--;
    if(vstUsersCounter==0)
        delete vstHost;
#endif

//    delete mutexListCables;
}

void MainHost::Open()
{
    EnableSolverUpdate(false);

    SetupMainContainer();
    SetupHostContainer();
    SetupProjectContainer();
    SetupProgramContainer();
    SetupGroupContainer();

    EnableSolverUpdate(true);
    programsModel->BuildDefaultModel();
}

void MainHost::SetupMainContainer()
{
    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::Container;
    info.name = "mainContainer";
    info.forcedObjId = FixedObjId::mainContainer;

    mainContainer = objFactory->NewObject(info).staticCast< Connectables::Container >();
    if(mainContainer.isNull())
        return;

    mainContainer->SetLoadingMode(true);

    mainContainer->LoadProgram(0);
    QStandardItem *item = mainContainer->GetFullItem();
    model->invisibleRootItem()->appendRow(item);
    mainContainer->modelIndex=item->index();
    mainContainer->parked=false;
    mainContainer->listenProgramChanges=false;

    mainContainer->SetLoadingMode(false);
    mainContainer->UpdateModificationTime();
    SetSolverUpdateNeeded();
}

void MainHost::SetupHostContainer()
{
    if(!hostContainer.isNull()) {
        mainContainer->ParkObject( hostContainer );
        hostContainer.clear();
        UpdateSolver(true);
        if(mainWindow)
            mainWindow->mySceneView->viewHost->ClearViewPrograms();
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::Container;
    info.name = "hostContainer";
    info.forcedObjId = FixedObjId::hostContainer;

    hostContainer = objFactory->NewObject(info).staticCast<Connectables::Container>();
    if(hostContainer.isNull())
        return;

    hostContainer->SetLoadingMode(true);

    hostContainer->LoadProgram(0);
    mainContainer->AddObject(hostContainer);

    QSharedPointer<Connectables::Object> bridge;


    //bridge in
    ObjectInfo in;
    in.name="in";
    in.nodeType = NodeType::bridge;
    in.objType = ObjType::BridgeIn;
    in.forcedObjId = FixedObjId::hostContainerIn;

    bridge = objFactory->NewObject(in);
    hostContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    hostContainer->bridgeIn = bridge;

    //bridge out
    ObjectInfo out;
    out.name="out";
    out.nodeType = NodeType::bridge;
    out.objType = ObjType::BridgeOut;
    out.forcedObjId = FixedObjId::hostContainerOut;

    bridge = objFactory->NewObject(out);
    hostContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    hostContainer->bridgeOut = bridge;

    //connect with groupContainer
    if(!groupContainer.isNull()) {
        mainContainer->ConnectObjects(groupContainer->bridgeSend, hostContainer->bridgeIn,true);
        mainContainer->ConnectObjects(hostContainer->bridgeOut, groupContainer->bridgeReturn,true);
    }

    //send bridge
    ObjectInfo send;
    send.name = "send";
    send.nodeType = NodeType::bridge;
    send.objType = ObjType::BridgeSend;
    send.forcedObjId = FixedObjId::hostContainerSend;

    bridge = objFactory->NewObject(send);
    hostContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    hostContainer->bridgeSend = bridge;

    //return bridge
    ObjectInfo retrn;
    retrn.name = "return";
    retrn.nodeType = NodeType::bridge;
    retrn.objType = ObjType::BridgeReturn;
    retrn.forcedObjId = FixedObjId::hostContainerReturn;

    bridge = objFactory->NewObject(retrn);
    hostContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    hostContainer->bridgeReturn = bridge;

    //connect with projectContainer
    if(!projectContainer.isNull()) {
        mainContainer->ConnectObjects(hostContainer->bridgeSend, projectContainer->bridgeIn,true);
        mainContainer->ConnectObjects(projectContainer->bridgeOut, hostContainer->bridgeReturn,true);
    }
    hostContainer->listenProgramChanges=false;

    if(projectContainer)
        hostContainer->childContainer=projectContainer;

    hostContainer->SetLoadingMode(false);
    hostContainer->UpdateModificationTime();
    SetSolverUpdateNeeded();
}

void MainHost::SetupProjectContainer()
{
    if(!projectContainer.isNull()) {
        mainContainer->ParkObject( projectContainer );
        projectContainer.clear();
        UpdateSolver(true);
        if(mainWindow)
            mainWindow->mySceneView->viewProject->ClearViewPrograms();
    }

    timeFromStart.restart();

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::Container;
    info.name = "projectContainer";
    info.forcedObjId = FixedObjId::projectContainer;

    projectContainer = objFactory->NewObject(info).staticCast<Connectables::Container>();
    if(projectContainer.isNull())
        return;

    projectContainer->SetLoadingMode(true);

    projectContainer->LoadProgram(0);
    mainContainer->AddObject(projectContainer);

    QSharedPointer<Connectables::Object> bridge;

    //bridge in
    ObjectInfo in;
    in.name="in";
    in.nodeType = NodeType::bridge;
    in.objType = ObjType::BridgeIn;
    in.forcedObjId = FixedObjId::projectContainerIn;

    bridge = objFactory->NewObject(in);
    projectContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    projectContainer->bridgeIn = bridge;

    //bridge out
    ObjectInfo out;
    out.name="out";
    out.nodeType = NodeType::bridge;
    out.objType = ObjType::BridgeOut;
    out.forcedObjId = FixedObjId::projectContainerOut;

    bridge = objFactory->NewObject(out);
    projectContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    projectContainer->bridgeOut = bridge;

    //connect with hostContainer
    if(!hostContainer.isNull()) {
        mainContainer->ConnectObjects(hostContainer->bridgeSend, projectContainer->bridgeIn,true);
        mainContainer->ConnectObjects(projectContainer->bridgeOut, hostContainer->bridgeReturn,true);
    }


    //bridge send
    ObjectInfo send;
    send.name="send";
    send.nodeType = NodeType::bridge;
    send.objType = ObjType::BridgeSend;
    send.forcedObjId = FixedObjId::projectContainerSend;

    bridge = objFactory->NewObject(send);
    projectContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    projectContainer->bridgeSend = bridge;

    //bridge return
    ObjectInfo retrn;
    retrn.name="return";
    retrn.nodeType = NodeType::bridge;
    retrn.objType = ObjType::BridgeReturn;
    retrn.forcedObjId = FixedObjId::projectContainerReturn;

    bridge = objFactory->NewObject(retrn);
    projectContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    projectContainer->bridgeReturn = bridge;

    //connect with programContainer
    if(!programContainer.isNull()) {
        mainContainer->ConnectObjects(projectContainer->bridgeSend, programContainer->bridgeIn,true);
        mainContainer->ConnectObjects(programContainer->bridgeOut, projectContainer->bridgeReturn,true);
    }

    //connect with itself (pass-though cables)
    projectContainer->ConnectObjects(projectContainer->bridgeIn, projectContainer->bridgeSend,false);
    projectContainer->ConnectObjects(projectContainer->bridgeReturn, projectContainer->bridgeOut,false);

    projectContainer->listenProgramChanges=false;

    if(hostContainer)
        hostContainer->childContainer=projectContainer;
    if(groupContainer)
        projectContainer->childContainer=groupContainer;

    projectContainer->SetLoadingMode(false);
    projectContainer->UpdateModificationTime();
    SetSolverUpdateNeeded();
}

void MainHost::SetupProgramContainer()
{
    if(!programContainer.isNull()) {
        mainContainer->ParkObject( programContainer );
        programContainer.clear();
        UpdateSolver(true);
        if(mainWindow)
            mainWindow->mySceneView->viewProgram->ClearViewPrograms();
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::Container;
    info.name = "programContainer";
    info.forcedObjId = FixedObjId::programContainer;

    programContainer = objFactory->NewObject(info).staticCast<Connectables::Container>();
    if(programContainer.isNull())
        return;

    programContainer->SetLoadingMode(true);

    programContainer->SetOptimizerFlag(true);
    programContainer->LoadProgram(0);
    mainContainer->AddObject(programContainer);

    QSharedPointer<Connectables::Object> bridge;

    //bridge in
    ObjectInfo in;
    in.name="in";
    in.nodeType = NodeType::bridge;
    in.objType = ObjType::BridgeIn;
    in.forcedObjId = FixedObjId::programContainerIn;

    bridge = objFactory->NewObject(in);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    programContainer->bridgeIn = bridge;

    //bridge out
    ObjectInfo out;
    out.name="out";
    out.nodeType = NodeType::bridge;
    out.objType = ObjType::BridgeOut;
    out.forcedObjId = FixedObjId::programContainerOut;

    bridge = objFactory->NewObject(out);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    programContainer->bridgeOut = bridge;

    //connect with projectContainer
    if(!projectContainer.isNull()) {
        mainContainer->ConnectObjects(projectContainer->bridgeSend, programContainer->bridgeIn,true);
        mainContainer->ConnectObjects(programContainer->bridgeOut, projectContainer->bridgeReturn,true);
    }


    //bridge send
    ObjectInfo send;
    send.name="send";
    send.nodeType = NodeType::bridge;
    send.objType = ObjType::BridgeSend;
    send.forcedObjId = FixedObjId::programContainerSend;

    bridge = objFactory->NewObject(send);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    programContainer->bridgeSend = bridge;

    //bridge return
    ObjectInfo retrn;
    retrn.name="return";
    retrn.nodeType = NodeType::bridge;
    retrn.objType = ObjType::BridgeReturn;
    retrn.forcedObjId = FixedObjId::programContainerReturn;

    bridge = objFactory->NewObject(retrn);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    programContainer->bridgeReturn = bridge;

    //connect with groupContainer
    if(!groupContainer.isNull()) {
        mainContainer->ConnectObjects(programContainer->bridgeSend, groupContainer->bridgeIn,true);
        mainContainer->ConnectObjects(groupContainer->bridgeOut, programContainer->bridgeReturn,true);
    }

    connect(programsModel, SIGNAL(ProgChanged(QModelIndex)),
            programContainer.data(), SLOT(SetProgram(QModelIndex)));
    connect(programsModel, SIGNAL(ProgDelete(QModelIndex)),
            programContainer.data(), SLOT(RemoveProgram(QModelIndex)));
    connect(this,SIGNAL(Rendered()),
            programContainer.data(), SLOT(PostRender()));

    emit programParkingModelChanged(&programContainer->parkModel);

    if(groupContainer) {
        groupContainer->childContainer=programContainer;
        programContainer->parentContainer=groupContainer;
    }

    programContainer->SetLoadingMode(false);
    programContainer->UpdateModificationTime();
    SetSolverUpdateNeeded();
}

void MainHost::SetupGroupContainer()
{
    if(!groupContainer.isNull()) {
        mainContainer->ParkObject( groupContainer );
        groupContainer.clear();
        UpdateSolver(true);
        if(mainWindow)
            mainWindow->mySceneView->viewGroup->ClearViewPrograms();
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::Container;
    info.name = "groupContainer";
    info.forcedObjId = FixedObjId::groupContainer;

    groupContainer = objFactory->NewObject(info).staticCast<Connectables::Container>();
    if(groupContainer.isNull())
        return;

    groupContainer->SetLoadingMode(true);

    groupContainer->LoadProgram(0);
    mainContainer->AddObject(groupContainer);

    QSharedPointer<Connectables::Object> bridge;

    //bridge in
    ObjectInfo in;
    in.name="in";
    in.nodeType = NodeType::bridge;
    in.objType = ObjType::BridgeIn;
    in.forcedObjId = FixedObjId::groupContainerIn;

    bridge = objFactory->NewObject(in);
    groupContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    groupContainer->bridgeIn = bridge;

    //bridge out
    ObjectInfo out;
    out.name="out";
    out.nodeType = NodeType::bridge;
    out.objType = ObjType::BridgeOut;
    out.forcedObjId = FixedObjId::groupContainerOut;

    bridge = objFactory->NewObject(out);
    groupContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    groupContainer->bridgeOut = bridge;

    //connect with programContainer
    if(!programContainer.isNull()) {
        mainContainer->ConnectObjects(programContainer->bridgeSend, groupContainer->bridgeIn,true);
        mainContainer->ConnectObjects(groupContainer->bridgeOut, programContainer->bridgeReturn,true);
    }

    //bridge send
    ObjectInfo send;
    send.name="send";
    send.nodeType = NodeType::bridge;
    send.objType = ObjType::BridgeSend;
    send.forcedObjId = FixedObjId::groupContainerSend;

    bridge = objFactory->NewObject(send);
    groupContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    groupContainer->bridgeSend = bridge;

    //bridge return
    ObjectInfo retrn;
    retrn.name="return";
    retrn.nodeType = NodeType::bridge;
    retrn.objType = ObjType::BridgeReturn;
    retrn.forcedObjId = FixedObjId::groupContainerReturn;

    bridge = objFactory->NewObject(retrn);
    groupContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    groupContainer->bridgeReturn = bridge;

    //connect with hostContainer
    if(!hostContainer.isNull()) {
        mainContainer->ConnectObjects(groupContainer->bridgeSend, hostContainer->bridgeIn,true);
        mainContainer->ConnectObjects(hostContainer->bridgeOut, groupContainer->bridgeReturn,true);
    }

    connect(programsModel, SIGNAL(GroupChanged(QModelIndex)),
            groupContainer.data(), SLOT(SetProgram(QModelIndex)));
    connect(programsModel, SIGNAL(GroupDelete(QModelIndex)),
            groupContainer.data(), SLOT(RemoveProgram(QModelIndex)));
    connect(this,SIGNAL(Rendered()),
            groupContainer.data(), SLOT(PostRender()));

    emit groupParkingModelChanged(&groupContainer->parkModel);

    if(projectContainer)
        projectContainer->childContainer=groupContainer;
    if(programContainer) {
        groupContainer->childContainer=programContainer;
        programContainer->parentContainer=groupContainer;
    }

    groupContainer->SetLoadingMode(false);
    groupContainer->UpdateModificationTime();
    SetSolverUpdateNeeded();
}

bool MainHost::EnableSolverUpdate(bool enable)
{
    solverMutex.lock();
    bool ret = solverUpdateEnabled;
    solverUpdateEnabled = enable;
    solverMutex.unlock();
    return ret;
}

void MainHost::ResetDelays()
{
    if(mainContainer && mainContainer->GetCurrentProgram())
        mainContainer->GetCurrentProgram()->ResetDelays();
    if(hostContainer && hostContainer->GetCurrentProgram())
        hostContainer->GetCurrentProgram()->ResetDelays();
    if(projectContainer && projectContainer->GetCurrentProgram())
        projectContainer->GetCurrentProgram()->ResetDelays();
    if(groupContainer && groupContainer->GetCurrentProgram())
        groupContainer->GetCurrentProgram()->ResetDelays();
    if(programContainer && programContainer->GetCurrentProgram())
        programContainer->GetCurrentProgram()->ResetDelays();
}

void MainHost::UpdateSolver(bool forceUpdate)
{
    solverMutex.lock();

        //update not forced, not needed or disabled : return
        if( (!solverUpdateEnabled || !solverNeedAnUpdate) && !forceUpdate) {
            solverMutex.unlock();
            return;
        }

        //disable other solver updates
        bool solverWasEnabled=solverUpdateEnabled;
        solverUpdateEnabled = false;

        //allow others to ask for a new update while we're updating
        solverNeedAnUpdate = false;

    solverMutex.unlock();

    //if forced : lock rendering
    if(forceUpdate) {
        mutexRender.lock();
    } else {
        //not forced : do it later if we can't do it now
        if(!mutexRender.tryLock()) {
            //can't lock, ask for a ne update
            SetSolverUpdateNeeded();
            EnableSolverUpdate(solverWasEnabled);
            return;
        }
    }

    //update the solver
    hashCables lstCables;
    if(mainContainer && mainContainer->GetCurrentProgram())
        mainContainer->GetCurrentProgram()->AddToCableList(&lstCables);
    if(hostContainer && hostContainer->GetCurrentProgram())
        hostContainer->GetCurrentProgram()->AddToCableList(&lstCables);
    if(projectContainer && projectContainer->GetCurrentProgram())
        projectContainer->GetCurrentProgram()->AddToCableList(&lstCables);
    if(groupContainer && groupContainer->GetCurrentProgram())
        groupContainer->GetCurrentProgram()->AddToCableList(&lstCables);
    if(programContainer && programContainer->GetCurrentProgram())
        programContainer->GetCurrentProgram()->AddToCableList(&lstCables);
    solver->Resolve(lstCables, renderer);
//    mutexListCables->lock();
//        solver->Resolve(workingListOfCables, renderer);
//    mutexListCables->unlock();

    mutexRender.unlock();
    EnableSolverUpdate(solverWasEnabled);
}

void MainHost::ChangeNbThreads(int nbThreads)
{
    if(!renderer)
            return;

        if(nbThreads<=0) {
    #ifdef _WIN32
            SYSTEM_INFO info;
            GetSystemInfo(&info);
            nbThreads = info.dwNumberOfProcessors;
    #else
            nbThreads = 1;
    #endif
        }

        mutexRender.lock();
        renderer->SetNbThreads(nbThreads);
        mutexRender.unlock();
        SetSolverUpdateNeeded();

}

//void MainHost::SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data)
//{
//    QMutexLocker lock(mutexListCables);


//    hashCables::const_iterator i = workingListOfCables.constFind(senderPin);
//    while (i != workingListOfCables.constEnd()  && i.key() == senderPin) {
////        const ConnectionInfo &destPin = i.value();
////        Connectables::Pin *pin = objFactory->GetPin(destPin);
////        if(!pin) {
////            LOG("unknown pin"<<destPin.objId<<"from"<<senderPin.objId);
////            return;
////        }
////        pin->ReceiveMsg(msgType,data);
//        i.value()->Render(msgType,data);
//        ++i;
//    }
//}

void MainHost::SetBufferSizeMs(unsigned int ms)
{
    ulong size = ms*sampleRate/1000;
    SetBufferSize(size);
}

void MainHost::SetBufferSize(unsigned long size)
{
    qDebug()<<"MainHost::SetBufferSize"<<size;
    bufferSize = size;
    emit BufferSizeChanged(bufferSize);
}

void MainHost::SetSampleRate(float rate)
{
    if(sampleRate == rate)
        return;

    sampleRate = rate;
    emit SampleRateChanged(sampleRate);
}

//void MainHost::OnNewRenderingOrder(orderedNodes * renderLines)
//{
//    emit NewSolver(renderLines);
//}

void MainHost::Render()
{

#ifdef VSTSDK
    CheckTempo();
#endif

    mutexRender.lock();

    if(mainContainer)
        mainContainer->NewRenderLoop();
    if(hostContainer)
        hostContainer->NewRenderLoop();
    if(projectContainer)
        projectContainer->NewRenderLoop();
    if(groupContainer)
        groupContainer->NewRenderLoop();
    if(programContainer)
        programContainer->NewRenderLoop();

    renderer->StartRender();
    mutexRender.unlock();

    if(solverNeedAnUpdate && solverUpdateEnabled)
        emit SolverToUpdate();

    emit Rendered();
}

#ifdef VSTSDK
void MainHost::SetTimeInfo(const VstTimeInfo *info)
{
    vstHost->SetTimeInfo(info);
//    CheckTempo();
}
#endif

void MainHost::SetTempo(int tempo, int sign1, int sign2)
{
#ifdef VSTSDK
    vstHost->SetTempo(tempo,sign1,sign2);
//    CheckTempo();
#else
    Q_UNUSED(tempo);
    Q_UNUSED(sign1);
    Q_UNUSED(sign2);
#endif
}

void MainHost::CheckTempo()
{
#ifdef VSTSDK
    int tempo=0;
    int sign1=0;
    int sign2=0;

    vstHost->GetTempo(tempo,sign1,sign2);
    if(tempo!=currentTempo
       || sign1!=currentTimeSig1
       || sign2!=currentTimeSig2) {

        currentTempo=tempo;
        currentTimeSig1=sign1;
        currentTimeSig2=sign2;
        emit TempoChanged(currentTempo,currentTimeSig1,currentTimeSig2);
    }
#endif
}

void MainHost::GetTempo(int &tempo, int &sign1, int &sign2)
{
#ifdef VSTSDK
    vstHost->GetTempo(tempo,sign1,sign2);
#else
    tempo=120;
    sign1=4;
    sign2=4;
#endif
}

void MainHost::SetSetting(QString name, QVariant value)
{
    settings.setValue(settingsGroup + name,value);
}

QVariant MainHost::GetSetting(QString name, QVariant defaultVal)
{
    return settings.value(settingsGroup + name,defaultVal);
}

bool MainHost::SettingDefined(QString name)
{
    return settings.contains(settingsGroup + name);
}

void MainHost::LoadFile(const QString &filename)
{
    QFileInfo info(filename);
    if ( info.suffix()==SETUP_FILE_EXTENSION ) {
        LoadSetupFile(filename);
    }
    if ( info.suffix()==PROJECT_FILE_EXTENSION ) {
        LoadProjectFile(filename);
    }
}

void MainHost::LoadSetupFile(const QString &filename)
{
    if(!programsModel->userWantsToUnloadSetup())
        return;

    QString name = filename;

    if(name.isEmpty()) {
        QString lastDir = GetSetting("lastSetupDir").toString();
        name = QFileDialog::getOpenFileName(mainWindow, tr("Open a Setup file"), lastDir, tr("Setup Files (*.%1)").arg(SETUP_FILE_EXTENSION));
    }

    if(name.isEmpty())
        return;

    undoStack.clear();

    if(ProjectFile::LoadFromFile(this,name)) {
        ConfigDialog::AddRecentSetupFile(name,this);
        currentSetupFile = name;
    } else {
        ConfigDialog::RemoveRecentSetupFile(name,this);
        ClearSetup();
    }
    emit currentFileChanged();
}

void MainHost::LoadProjectFile(const QString &filename)
{
    if(!programsModel->userWantsToUnloadProject())
        return;

    QString name = filename;

    if(name.isEmpty()) {
        QString lastDir = GetSetting("lastProjectDir").toString();
        name = QFileDialog::getOpenFileName(mainWindow, tr("Open a Project file"), lastDir, tr("Project Files (*.%1)").arg(PROJECT_FILE_EXTENSION));
    }

    if(name.isEmpty())
        return;

    undoStack.clear();

    if(ProjectFile::LoadFromFile(this,name)) {
        ConfigDialog::AddRecentProjectFile(name,this);
        currentProjectFile = name;
    } else {
        ConfigDialog::RemoveRecentProjectFile(name,this);
        ClearProject();
    }
    emit currentFileChanged();
}

void MainHost::ReloadProject()
{
    if(currentProjectFile.isEmpty())
        return;

    undoStack.clear();

    ProjectFile::LoadFromFile(this,currentProjectFile);
}

void MainHost::ReloadSetup()
{
    if(currentSetupFile.isEmpty())
        return;

    undoStack.clear();

    ConfigDialog::AddRecentSetupFile(currentSetupFile,this);
}

void MainHost::ClearSetup()
{
    if(!programsModel->userWantsToUnloadSetup())
        return;

    undoStack.clear();

    EnableSolverUpdate(false);
    SetupHostContainer();
    EnableSolverUpdate(true);
    if(mainWindow)
        mainWindow->viewConfig->LoadFromRegistry();

    ConfigDialog::AddRecentSetupFile("",this);
    currentSetupFile = "";
    emit currentFileChanged();
}

void MainHost::ClearProject()
{
    if(!programsModel->userWantsToUnloadProject())
        return;

    undoStack.clear();

    EnableSolverUpdate(false);
    SetupProjectContainer();
    SetupProgramContainer();
    SetupGroupContainer();
    EnableSolverUpdate(true);

    programsModel->BuildDefaultModel();

    ConfigDialog::AddRecentProjectFile("",this);
    currentProjectFile = "";
    emit currentFileChanged();
}

void MainHost::SaveSetupFile(bool saveAs)
{
    QString filename;

    if(currentSetupFile.isEmpty() || saveAs) {
        QString lastDir = GetSetting("lastSetupDir").toString();
        filename = QFileDialog::getSaveFileName(mainWindow, tr("Save Setup"), lastDir, tr("Setup Files (*.%1)").arg(SETUP_FILE_EXTENSION));

        if(filename.isEmpty())
            return;

        if(!filename.endsWith(SETUP_FILE_EXTENSION, Qt::CaseInsensitive)) {
            filename += ".";
            filename += SETUP_FILE_EXTENSION;
        }
    } else {
        filename = currentSetupFile;
    }

    if(ProjectFile::SaveToSetupFile(this,filename)) {
        SetSetting("lastSetupDir",QFileInfo(filename).absolutePath());
        ConfigDialog::AddRecentSetupFile(filename,this);
        currentSetupFile = filename;
        emit currentFileChanged();
    }
}

void MainHost::SaveProjectFile(bool saveAs)
{
    QString filename;

    if(currentProjectFile.isEmpty() || saveAs) {
        QString lastDir = GetSetting("lastProjectDir").toString();
        filename = QFileDialog::getSaveFileName(mainWindow, tr("Save Project"), lastDir, tr("Project Files (*.%1)").arg(PROJECT_FILE_EXTENSION));

        if(filename.isEmpty())
            return;

        if(!filename.endsWith(PROJECT_FILE_EXTENSION, Qt::CaseInsensitive)) {
            filename += ".";
            filename += PROJECT_FILE_EXTENSION;
        }
    } else {
        filename = currentProjectFile;
    }

    if(ProjectFile::SaveToProjectFile(this,filename)) {
        SetSetting("lastProjectDir",QFileInfo(filename).absolutePath());
        ConfigDialog::AddRecentProjectFile(filename,this);
        currentProjectFile = filename;
        emit currentFileChanged();
    }
}
