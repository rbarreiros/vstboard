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

#include "mainhost.h"
#include "connectables/mididevice.h"
#include "mainwindow.h"
#include "connectables/container.h"
#include "connectables/audiodevice.h"

#ifdef VSTSDK
#include "connectables/vstplugin.h"
#endif

MainHost *MainHost::theHost = 0;
HostModel *MainHost::model = 0;
HostModel *MainHost::modelParking = 0;
HostModelProxy *MainHost::modelProxy = 0;

MainHost* MainHost::Create(QObject *parent)
{
    if(!theHost)
        theHost = new MainHost(parent);

    return theHost;
}

MainHost::MainHost(QObject *parent) :
    QObject(parent),
    mainContainer(0),
    hostContainer(0),
    programContainer(0),
    insertContainer(0),
    filePass(0),
    solverNeedAnUpdate(false),
    solverUpdateEnabled(true),
    mutexListCables(QMutex::Recursive),
    progToChange(-1)
{
    model = new HostModel(this);
    model->setColumnCount(1);
    modelParking = new HostModel(this);
    modelParking->setColumnCount(1);
    modelProxy = new HostModelProxy(model);

    Connectables::ObjectFactory::Create(this);

    sampleRate = 44100.0;
    bufferSize = 512;

    programList = new Programs(this);
    connect(programList, SIGNAL(ProgChanged(QModelIndex)),
            this, SLOT(SetProgram(QModelIndex)));

    listMidiDevices = new MidiDevices();
    AudioDevices * devices = AudioDevices::Create(this);
    connect(this,SIGNAL(OnAudioDeviceToggleInUse(ObjectInfo,bool)),
            devices,SLOT(OnToggleDeviceInUse(ObjectInfo,bool)));
    connect(&devices->fakeRenderTimer,SIGNAL(timeout()),
            this, SLOT(Render()));

    //timer
    timeFromStart.start();

    updateViewTimer = new QTimer(this);
    updateViewTimer->start(40);

    connect(&solver,SIGNAL(NewRenderingOrder(orderedNodes*)),
            &renderer, SLOT(OnNewRenderingOrder(orderedNodes*)));

    connect(&solver,SIGNAL(NewRenderingOrder(orderedNodes*)),
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

    mutexListCables.lock();
    workingListOfCables.clear();
    mutexListCables.unlock();

    solver.Resolve(workingListOfCables);
    renderer.Clear();

    mainContainer.clear();
    hostContainer.clear();
    insertContainer.clear();
    programContainer.clear();
    parkingContainer.clear();

    delete listMidiDevices;
    delete Connectables::ObjectFactory::Get();
    theHost = 0;
}

void MainHost::Open()
{
    EnableSolverUpdate(false);

    SetupMainContainer();
    SetupHostContainer();
    SetupInsertContainer();
    SetupProgramContainer();
    SetupParking();

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

    mainContainer = Connectables::ObjectFactory::Get()->NewObject(info).staticCast< Connectables::MainContainer >();
    mainContainer->LoadProgram(0);
    OnObjectAdded(mainContainer);
    mainContainer->SetParentModeIndex( model->invisibleRootItem()->index() );

    mainContainer->listenProgramChanges=false;
}

void MainHost::SetupHostContainer()
{
    if(!hostContainer.isNull()) {
        mainContainer->RemoveObject( hostContainer );
        hostContainer.clear();
        UpdateSolver(true);
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "hostContainer";
    info.forcedObjId = FixedObjId::hostContainer;

    hostContainer = Connectables::ObjectFactory::Get()->NewObject(info).staticCast<Connectables::MainContainer>();
    hostContainer->LoadProgram(0);
    mainContainer->AddObject(hostContainer);

    QSharedPointer<Connectables::Object> bridge;


    //send bridge
    ObjectInfo send;
    send.name = "send";
    send.nodeType = NodeType::bridge;
    send.objType = ObjType::BridgeSend;
    send.forcedObjId = FixedObjId::hostContainerSend;

    bridge = Connectables::ObjectFactory::Get()->NewObject(send);
    hostContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    hostContainer->bridgeSend = bridge;

    //return bridge
    ObjectInfo retrn;
    retrn.name = "return";
    retrn.nodeType = NodeType::bridge;
    retrn.objType = ObjType::BridgeReturn;
    retrn.forcedObjId = FixedObjId::hostContainerReturn;

    bridge = Connectables::ObjectFactory::Get()->NewObject(retrn);
    hostContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    hostContainer->bridgeReturn = bridge;

    //connect with programContainer
    if(!programContainer.isNull()) {
        mainContainer->ConnectBridges(hostContainer->bridgeSend, programContainer->bridgeIn);
        mainContainer->ConnectBridges(programContainer->bridgeOut, hostContainer->bridgeReturn);
    }
    hostContainer->listenProgramChanges=false;
}


void MainHost::SetupProgramContainer()
{
    if(!programContainer.isNull()) {
        mainContainer->RemoveObject( programContainer );
        programContainer.clear();
        UpdateSolver(true);
    }

    timeFromStart.restart();

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "programContainer";
    info.forcedObjId = FixedObjId::programContainer;

    programContainer = Connectables::ObjectFactory::Get()->NewObject(info).staticCast<Connectables::MainContainer>();
    programContainer->LoadProgram(0);
    mainContainer->AddObject(programContainer);

    QSharedPointer<Connectables::Object> bridge;

    //bridge in
    ObjectInfo in;
    in.name="in";
    in.nodeType = NodeType::bridge;
    in.objType = ObjType::BridgeIn;
    in.forcedObjId = FixedObjId::programContainerIn;

    bridge = Connectables::ObjectFactory::Get()->NewObject(in);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    programContainer->bridgeIn = bridge;

    //bridge out
    ObjectInfo out;
    out.name="out";
    out.nodeType = NodeType::bridge;
    out.objType = ObjType::BridgeOut;
    out.forcedObjId = FixedObjId::programContainerOut;

    bridge = Connectables::ObjectFactory::Get()->NewObject(out);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    programContainer->bridgeOut = bridge;

    //connect with hostContainer
    if(!hostContainer.isNull()) {
        mainContainer->ConnectBridges(hostContainer->bridgeSend, programContainer->bridgeIn);
        mainContainer->ConnectBridges(programContainer->bridgeOut, hostContainer->bridgeReturn);
    }


    //bridge send
    ObjectInfo send;
    send.name="send";
    send.nodeType = NodeType::bridge;
    send.objType = ObjType::BridgeSend;
    send.forcedObjId = FixedObjId::programContainerSend;

    bridge = Connectables::ObjectFactory::Get()->NewObject(send);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    programContainer->bridgeSend = bridge;

    //bridge return
    ObjectInfo retrn;
    retrn.name="return";
    retrn.nodeType = NodeType::bridge;
    retrn.objType = ObjType::BridgeReturn;
    retrn.forcedObjId = FixedObjId::programContainerReturn;

    bridge = Connectables::ObjectFactory::Get()->NewObject(retrn);
    programContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    programContainer->bridgeReturn = bridge;

    //connect with insertContainer
    if(!insertContainer.isNull()) {
        mainContainer->ConnectBridges(programContainer->bridgeSend, insertContainer->bridgeIn);
        mainContainer->ConnectBridges(insertContainer->bridgeOut, programContainer->bridgeReturn);
    }
}

void MainHost::SetupInsertContainer()
{
    if(!insertContainer.isNull()) {
        mainContainer->RemoveObject( insertContainer );
        insertContainer.clear();
        UpdateSolver(true);
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::MainContainer;
    info.name = "insertContainer";
    info.forcedObjId = FixedObjId::insertContainer;

    insertContainer = Connectables::ObjectFactory::Get()->NewObject(info).staticCast<Connectables::MainContainer>();
    insertContainer->LoadProgram(0);
    mainContainer->AddObject(insertContainer);

    QSharedPointer<Connectables::Object> bridge;

    //bridge in
    ObjectInfo in;
    in.name="in";
    in.nodeType = NodeType::bridge;
    in.objType = ObjType::BridgeIn;
    in.forcedObjId = FixedObjId::insertContainerIn;

    bridge = Connectables::ObjectFactory::Get()->NewObject(in);
    insertContainer->AddObject( bridge );
    bridge->SetBridgePinsInVisible(false);
    insertContainer->bridgeIn = bridge;

    //bridge out
    ObjectInfo out;
    out.name="out";
    out.nodeType = NodeType::bridge;
    out.objType = ObjType::BridgeOut;
    out.forcedObjId = FixedObjId::insertContainerOut;

    bridge = Connectables::ObjectFactory::Get()->NewObject(out);
    insertContainer->AddObject( bridge );
    bridge->SetBridgePinsOutVisible(false);
    insertContainer->bridgeOut = bridge;

    //connect with programContainer
    if(!programContainer.isNull()) {
        mainContainer->ConnectBridges(programContainer->bridgeSend, insertContainer->bridgeIn);
        mainContainer->ConnectBridges(insertContainer->bridgeOut, programContainer->bridgeReturn);
    }

    insertContainer->listenProgramChanges=false;
}


void MainHost::SetupParking()
{
    if(parkingContainer) {
        parkingContainer->Clear();
        return;
    }

    ObjectInfo info;
    info.nodeType = NodeType::container;
    info.objType = ObjType::ParkingContainer;
    info.name = "parkingContainer";
    info.forcedObjId = FixedObjId::parkingContainer;

    parkingContainer = Connectables::ObjectFactory::Get()->NewObject(info).staticCast< Connectables::ParkingContainer >();
    parkingContainer->SetParentModeIndex( modelParking->invisibleRootItem()->index() );
}

void MainHost::ClearHost()
{
    EnableSolverUpdate(false);

    SetupInsertContainer();
    SetupProgramContainer();
    SetupParking();

    EnableSolverUpdate(true);
    programList->BuildModel();
}

void MainHost::EnableSolverUpdate(bool enable)
{
    mutexProgChange.lock();
        solverUpdateEnabled = enable;
    mutexProgChange.unlock();
}

bool MainHost::IsSolverUpdateEnabled()
{
    QMutexLocker l(&mutexProgChange);
    return solverUpdateEnabled;
}

void MainHost::UpdateSolver(bool forceUpdate)
{
    mutexProgChange.lock();

        //solver needs an update
        solverNeedAnUpdate = true;

        bool solverWasEnabled=solverUpdateEnabled;

        //return if solver update was disabled
        if(!solverUpdateEnabled && !forceUpdate) {
            mutexProgChange.unlock();
            return;
        }

        //disable other solver updates
        solverUpdateEnabled = false;

    mutexProgChange.unlock();

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

    mutexProgChange.lock();
        //do we need to load a program ?
        int prg = progToChange;
        //mark as loaded
        progToChange=-1;
    mutexProgChange.unlock();

    //load prog if asked
    if(prg!=-1) {
        programContainer->SaveProgram();
        programContainer->UnloadProgram();
        programContainer->LoadProgram(prg);
//        emit ProgramChanged(prg);
//        programList->ChangeProg(prg);
    }

    mutexListCables.lock();
        solver.Resolve(workingListOfCables);
    mutexListCables.unlock();

    mutexProgChange.lock();
        //solver is up to date
        solverNeedAnUpdate = false;
    mutexProgChange.unlock();

    //allow rendering
    mutexRender.unlock();

    EnableSolverUpdate(solverWasEnabled);
}

void MainHost::SetProgram(const QModelIndex &prgIndex)
{
    mutexProgChange.lock();
    progToChange=prgIndex.data(UserRoles::value).toInt();
    mutexProgChange.unlock();
    solverNeedAnUpdate=true;
//    emit SolverToUpdate();

}

void MainHost::SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data)
{
    QMutexLocker lock(&mutexListCables);


    hashCables::const_iterator i = workingListOfCables.constFind(senderPin);
    while (i != workingListOfCables.constEnd()  && i.key() == senderPin) {
        Connectables::Pin *pin = Connectables::ObjectFactory::Get()->GetPin(i.value());
        if(!pin) {
            debug("MainHost::SendMsg : unknown pin")
            return;
        }

        pin->ReceiveMsg(msgType,data);
        ++i;
    }
}

void MainHost::SetBufferSize(long size)
{
    debug("MainHost::SetBufferSize %ld",size)
    bufferSize = size;
    emit BufferSizeChanged(bufferSize);
}

void MainHost::SetSampleRate(float rate)
{
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
    vstHost.UpdateTimeInfo(timeFromStart.elapsed(), samples, sampleRate);
#endif

    mutexRender.lock();
    renderer.StartRender();
    mutexRender.unlock();

//    mutexProgChange.lock();
//    bool b = solverNeedAnUpdate;
//    mutexProgChange.unlock();
//    if(b)
//        emit SolverToUpdate();

    if(solverNeedAnUpdate && solverUpdateEnabled)
        emit SolverToUpdate();
}

void MainHost::OnObjectAdded(QSharedPointer<Connectables::Object> objPtr)
{
    if(objPtr.isNull())
        return;

    if(parkingContainer)
        parkingContainer->RemoveObject(objPtr);

    if(objPtr->info().objType == ObjType::MidiInterface)
        listMidiDevices->OpenDevice(objPtr);

    connect(this,SIGNAL(SampleRateChanged(float)),
            objPtr.data(),SLOT(SetSampleRate(float)));
    connect(this,SIGNAL(BufferSizeChanged(long)),
            objPtr.data(),SLOT(SetBufferSize(long)));

    objPtr->SetSampleRate(sampleRate);
    objPtr->SetBufferSize(bufferSize);
    objPtr->SetSleep(false);

    solverNeedAnUpdate = true;
//    emit SolverToUpdate();
}

void MainHost::OnObjectRemoved(QSharedPointer<Connectables::Object> objPtr, Connectables::Object *container)
{
    if(objPtr.isNull())
        return;

    disconnect(this,SIGNAL(SampleRateChanged(float)),
            objPtr.data(),SLOT(SetSampleRate(float)));
    disconnect(this,SIGNAL(BufferSizeChanged(long)),
            objPtr.data(),SLOT(SetBufferSize(long)));

    listMidiDevices->CloseDevice(objPtr);

    //if the object comes from a programmable container : don't delete it, store it in the parking container
    if(container && container->listenProgramChanges && parkingContainer
       && objPtr->info().nodeType!=NodeType::bridge )
        parkingContainer->AddObject(objPtr);

    solverNeedAnUpdate = true;
//    emit SolverToUpdate();
}

void MainHost::OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    mutexListCables.lock();
    workingListOfCables.insert(outputPin,inputPin);
    mutexListCables.unlock();

    solverNeedAnUpdate = true;
//    emit SolverToUpdate();
}

void MainHost::OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin)
{
    mutexListCables.lock();
    workingListOfCables.remove(outputPin,inputPin);
    mutexListCables.unlock();

    solverNeedAnUpdate = true;
//    emit SolverToUpdate();
}

//float MainHost::GetCpuLoad()
//{
//    float c=cpuLoad;
//    cpuLoad=.0f;
//    return c;
//}

//void MainHost::UpdateCpuLoad(float load)
//{
//    cpuLoad=std::max(cpuLoad, load);
//}

void MainHost::SetTempo(int tempo, int sign1, int sign2)
{
#ifdef VSTSDK
    vstHost.vstTimeInfo.tempo = tempo;
    vstHost.vstTimeInfo.timeSigNumerator = sign1;
    vstHost.vstTimeInfo.timeSigDenominator = sign2;
#endif
}

QDataStream & operator<< (QDataStream& out, MainHost& value)
{
    value.EnableSolverUpdate(false);

    value.insertContainer->SaveProgram();
    value.programContainer->SaveProgram();
    value.parkingContainer->SaveProgram();

    for(MainHost::Get()->filePass=0; MainHost::Get()->filePass<LOADSAVE_STAGES ; MainHost::Get()->filePass++) {
        out << *value.parkingContainer;
        out << *value.insertContainer;
        out << *value.programContainer;
    }

    out << *value.programList;

    value.EnableSolverUpdate(true);

    return out;
}

QDataStream & operator>> (QDataStream& in, MainHost& value)
{
    value.EnableSolverUpdate(false);

    value.SetupInsertContainer();
    value.SetupProgramContainer();
    value.SetupParking();

    for(MainHost::Get()->filePass=0; MainHost::Get()->filePass<LOADSAVE_STAGES ; MainHost::Get()->filePass++) {
        in >> *value.parkingContainer;
        in >> *value.insertContainer;
        in >> *value.programContainer;
    }

    Connectables::ObjectFactory::Get()->ResetSavedId();
    in >> *value.programList;
    value.EnableSolverUpdate(true);
    return in;
}
