/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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
#include "connectables/container.h"

#ifdef VSTBOARD
    #include "connectables/mididevice.h"
    #include "connectables/audiodevice.h"
    #include "audiodevices.h"
#endif

#ifdef VST_PLUGIN
    #include "vst.h"
#endif

#ifdef VSTSDK
    #include "connectables/vstplugin.h"
#endif

#include "projectfile/fileversion.h"


MainHost::MainHost(Vst *myVstPlugin, QObject *parent, QString settingsGroup) :
    QObject(parent),
    solver(new PathSolver(this)),
    filePass(0),
    objFactory(0),
    mainWindow(0),
    myVstPlugin(myVstPlugin),
    solverNeedAnUpdate(false),
    solverUpdateEnabled(true),
    mutexListCables(new QMutex(QMutex::Recursive)),
    currentFileVersion(PROJECT_AND_SETUP_FILE_VERSION),
    settingsGroup(settingsGroup)
{
    doublePrecision=GetSetting("doublePrecision",false).toBool();

    setObjectName("MainHost");

//    scriptObj = scriptEngine.newQObject(this);
//    scriptEngine.globalObject().setProperty("MainHost", scriptObj);

    objFactory = new Connectables::ObjectFactory(this);

    if(!vst::CVSTHost::Get())
        vstHost = new vst::CVSTHost();
    else
        vstHost = vst::CVSTHost::Get();

    model = new HostModel(this);
    model->setObjectName("MainModel");
    model->setColumnCount(1);

    sampleRate = 44100.0;
    bufferSize = 512;

    currentTempo=120;
    currentTimeSig1=4;
    currentTimeSig2=4;

    programList = new Programs(this);

#ifdef VSTBOARD
    midiDevices = new MidiDevices(this);

    audioDevices = new AudioDevices(this);
    connect(this,SIGNAL(OnAudioDeviceToggleInUse(ObjectInfo,bool)),
            audioDevices,SLOT(OnToggleDeviceInUse(ObjectInfo,bool)));
    connect(&audioDevices->fakeRenderTimer,SIGNAL(timeout()),
            this, SLOT(Render()));
#endif

    //timer
    timeFromStart.start();

    updateViewTimer = new QTimer(this);
    updateViewTimer->start(40);

    connect(solver,SIGNAL(NewRenderingOrder(orderedNodes*)),
            &renderer, SLOT(OnNewRenderingOrder(orderedNodes*)));

    connect(solver,SIGNAL(NewRenderingOrder(orderedNodes*)),
            this, SLOT(OnNewRenderingOrder(orderedNodes*)));

    connect(this,SIGNAL(SolverToUpdate()),
            this,SLOT(UpdateSolver()),
            Qt::QueuedConnection);
}

MainHost::~MainHost()
{
    EnableSolverUpdate(false);

    updateViewTimer->stop();
    updateViewTimer->deleteLater();

    mutexListCables->lock();
    workingListOfCables.clear();
    mutexListCables->unlock();

    mainContainer.clear();
    hostContainer.clear();
    projectContainer.clear();
    groupContainer.clear();
    programContainer.clear();

    solver->Resolve(workingListOfCables);
    renderer.Clear();

#ifdef VSTBOARD
    delete audioDevices;
    delete midiDevices;
#endif

    delete objFactory;
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
    programList->BuildModel();
}

void MainHost::SetupMainContainer()
{
    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "mainContainer";
    info.forcedObjId = FixedObjId::mainContainer;

    mainContainer = objFactory->NewObject(info).staticCast< Connectables::MainContainer >();
    if(mainContainer.isNull())
        return;

    mainContainer->LoadProgram(0);
    mainContainer->SetParentModeIndex( model->invisibleRootItem()->index() );
    mainContainer->listenProgramChanges=false;
}

void MainHost::SetupHostContainer()
{
    if(!hostContainer.isNull()) {
        mainContainer->ParkObject( hostContainer );
        hostContainer.clear();
        UpdateSolver(true);
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "hostContainer";
    info.forcedObjId = FixedObjId::hostContainer;

    hostContainer = objFactory->NewObject(info).staticCast<Connectables::MainContainer>();
    if(hostContainer.isNull())
        return;

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
    if(!programContainer.isNull()) {
        mainContainer->ConnectBridges(groupContainer->bridgeSend, hostContainer->bridgeIn);
        mainContainer->ConnectBridges(hostContainer->bridgeOut, groupContainer->bridgeReturn);
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

    //connect with programContainer
    if(!projectContainer.isNull()) {
        mainContainer->ConnectBridges(hostContainer->bridgeSend, projectContainer->bridgeIn);
        mainContainer->ConnectBridges(projectContainer->bridgeOut, hostContainer->bridgeReturn);
    }
    hostContainer->listenProgramChanges=false;
}

void MainHost::SetupProjectContainer()
{
    if(!projectContainer.isNull()) {
        mainContainer->ParkObject( projectContainer );
        projectContainer.clear();
        UpdateSolver(true);
    }

    timeFromStart.restart();

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "projectContainer";
    info.forcedObjId = FixedObjId::projectContainer;

    projectContainer = objFactory->NewObject(info).staticCast<Connectables::MainContainer>();
    if(projectContainer.isNull())
        return;

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
        mainContainer->ConnectBridges(hostContainer->bridgeSend, projectContainer->bridgeIn);
        mainContainer->ConnectBridges(projectContainer->bridgeOut, hostContainer->bridgeReturn);
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
        mainContainer->ConnectBridges(projectContainer->bridgeSend, programContainer->bridgeIn);
        mainContainer->ConnectBridges(programContainer->bridgeOut, projectContainer->bridgeReturn);
    }

    //connect with itself (pass-though cables)
    projectContainer->ConnectBridges(projectContainer->bridgeIn, projectContainer->bridgeSend,false);
    projectContainer->ConnectBridges(projectContainer->bridgeReturn, projectContainer->bridgeOut,false);

    projectContainer->listenProgramChanges=false;
}

void MainHost::SetupProgramContainer()
{
    if(!programContainer.isNull()) {
        mainContainer->ParkObject( programContainer );
        programContainer.clear();
        UpdateSolver(true);
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "programContainer";
    info.forcedObjId = FixedObjId::programContainer;

    programContainer = objFactory->NewObject(info).staticCast<Connectables::MainContainer>();
    if(programContainer.isNull())
        return;

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
        mainContainer->ConnectBridges(projectContainer->bridgeSend, programContainer->bridgeIn);
        mainContainer->ConnectBridges(programContainer->bridgeOut, projectContainer->bridgeReturn);
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
        mainContainer->ConnectBridges(programContainer->bridgeSend, groupContainer->bridgeIn);
        mainContainer->ConnectBridges(groupContainer->bridgeOut, programContainer->bridgeReturn);
    }

    connect(programList, SIGNAL(ProgChanged(QModelIndex)),
            programContainer.data(), SLOT(SetProgram(QModelIndex)));
    connect(programList, SIGNAL(ProgCopy(int,int)),
            programContainer.data(), SLOT(CopyProgram(int,int)));
    connect(programList, SIGNAL(ProgDelete(int)),
            programContainer.data(), SLOT(RemoveProgram(int)));
    connect(this,SIGNAL(Rendered()),
            programContainer.data(), SLOT(Render()));

    emit programParkingModelChanged(&programContainer->parkModel);
}

void MainHost::SetupGroupContainer()
{
    if(!groupContainer.isNull()) {
        mainContainer->ParkObject( groupContainer );
        groupContainer.clear();
        UpdateSolver(true);
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "groupContainer";
    info.forcedObjId = FixedObjId::groupContainer;

    groupContainer = objFactory->NewObject(info).staticCast<Connectables::MainContainer>();
    if(groupContainer.isNull())
        return;

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
        mainContainer->ConnectBridges(programContainer->bridgeSend, groupContainer->bridgeIn);
        mainContainer->ConnectBridges(groupContainer->bridgeOut, programContainer->bridgeReturn);
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
        mainContainer->ConnectBridges(groupContainer->bridgeSend, hostContainer->bridgeIn);
        mainContainer->ConnectBridges(hostContainer->bridgeOut, groupContainer->bridgeReturn);
    }

    connect(programList, SIGNAL(GroupChanged(QModelIndex)),
            groupContainer.data(), SLOT(SetProgram(QModelIndex)));
    connect(programList, SIGNAL(GroupCopy(int,int)),
            groupContainer.data(), SLOT(CopyProgram(int,int)));
    connect(programList, SIGNAL(GroupDelete(int)),
            groupContainer.data(), SLOT(RemoveProgram(int)));
    connect(this,SIGNAL(Rendered()),
            groupContainer.data(), SLOT(Render()));

    emit groupParkingModelChanged(&groupContainer->parkModel);
}

void MainHost::EnableSolverUpdate(bool enable)
{
    solverMutex.lock();
        solverUpdateEnabled = enable;
    solverMutex.unlock();
}

//bool MainHost::IsSolverUpdateEnabled()
//{
//    QMutexLocker l(&solverMutex);
//    return solverUpdateEnabled;
//}

void MainHost::SetSolverUpdateNeeded(bool need)
{
    solverMutex.lock();
    solverNeedAnUpdate = need;
    solverMutex.unlock();
}

void MainHost::UpdateSolver(bool forceUpdate)
{
    solverMutex.lock();

        //solver needs an update
        solverNeedAnUpdate = true;

        bool solverWasEnabled=solverUpdateEnabled;

        //return if solver update was disabled
        if(!solverUpdateEnabled && !forceUpdate) {
            solverMutex.unlock();
            return;
        }

        //disable other solver updates
        solverUpdateEnabled = false;

    solverMutex.unlock();

    //if forced : wait the end of rendering
    if(forceUpdate)
        mutexRender.lock();
    else {
        //not forced : do it later if we can't do it now
        if(!mutexRender.tryLock()) {
            EnableSolverUpdate(solverWasEnabled);
            return;
        }
    }

    mutexListCables->lock();
        solver->Resolve(workingListOfCables);
    mutexListCables->unlock();

    SetSolverUpdateNeeded(false);

    //allow rendering
    mutexRender.unlock();

    EnableSolverUpdate(solverWasEnabled);
}

void MainHost::SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data)
{
    QMutexLocker lock(mutexListCables);


    hashCables::const_iterator i = workingListOfCables.constFind(senderPin);
    while (i != workingListOfCables.constEnd()  && i.key() == senderPin) {
        Connectables::Pin *pin = objFactory->GetPin(i.value());
        if(!pin) {
            debug("MainHost::SendMsg : unknown pin")
            return;
        }

        pin->ReceiveMsg(msgType,data);
        ++i;
    }
}

void MainHost::SetBufferSize(unsigned long size)
{
    debug("MainHost::SetBufferSize %ld",size)
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

void MainHost::OnNewRenderingOrder(orderedNodes * renderLines)
{
    emit NewSolver(renderLines);
}

void MainHost::Render(unsigned long samples)
{
    if(samples==0)
        samples=bufferSize;

#ifdef VSTSDK

    CheckTempo();

    #ifndef VST_PLUGIN
        //as a host : update internal clock
        vstHost->UpdateTimeInfo(timeFromStart.elapsed(), samples, sampleRate);
    #endif
#endif

    mutexRender.lock();
    renderer.StartRender();
    mutexRender.unlock();

    if(solverNeedAnUpdate && solverUpdateEnabled)
        emit SolverToUpdate();

    emit Rendered();
}

void MainHost::OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    mutexListCables->lock();
    workingListOfCables.insert(outputPin,inputPin);
    mutexListCables->unlock();

    SetSolverUpdateNeeded();
}

void MainHost::OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    mutexListCables->lock();
    workingListOfCables.remove(outputPin,inputPin);
    mutexListCables->unlock();

    SetSolverUpdateNeeded();
}

void MainHost::SetTimeInfo(const VstTimeInfo *info)
{
#ifdef VSTSDK
    vstHost->SetTimeInfo(info);
//    CheckTempo();
#endif
}

void MainHost::SetTempo(int tempo, int sign1, int sign2)
{
#ifdef VSTSDK
    vstHost->SetTempo(tempo,sign1,sign2);
//    CheckTempo();
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
