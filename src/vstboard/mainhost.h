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

#ifndef MAINHOST_H
#define MAINHOST_H

#include "precomp.h"
#include "connectables/objectfactory.h"
#include "connectables/object.h"
#include "connectables/maincontainer.h"
#include "connectables/parkingcontainer.h"
//#include "projectfile/program.h"
#include "pathsolver.h"
#include "renderer.h"
#include "globals.h"
#include "models/hostmodel.h"
#include "models/hostmodelproxy.h"
#include "audiodevices.h"
#include "mididevices.h"
#include "programs.h"
#include "projectfile/setupfile.h"

#ifdef VSTSDK
    #include "vst/cvsthost.h"
#endif


class MainHost : public QObject
{
Q_OBJECT
public:
    inline static MainHost* Get() {return theHost;}
    static MainHost* Create(QObject *parent = 0);
    ~MainHost();

    void Open();

    void SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data);

    void SetBufferSize(long size);
    void SetSampleRate(float rate=44100.0);
    int GetBufferSize() {return (int)bufferSize;}
    float GetSampleRate() {return sampleRate;}
//    float GetCpuLoad();

    void OnObjectAdded(QSharedPointer<Connectables::Object> objPtr);
    void OnObjectRemoved(QSharedPointer<Connectables::Object> objPtr, Connectables::Object *container=0);

    void EnableSolverUpdate(bool enable);
    bool IsSolverUpdateEnabled();

    QDataStream & toStream(QDataStream& stream) const;
    QDataStream & fromStream(QDataStream& stream);

    QSharedPointer<Connectables::MainContainer> mainContainer;
    QSharedPointer<Connectables::MainContainer> hostContainer;
    QSharedPointer<Connectables::MainContainer> programContainer;
    QSharedPointer<Connectables::MainContainer> insertContainer;
    QSharedPointer<Connectables::ParkingContainer> parkingContainer;

//    ListAudioInterfacesModel * GetAudioDevicesModel() {return listAudioDevices->GetModel();}
    ListMidiInterfacesModel * GetMidiDeviceModel() {return listMidiDevices->GetModel();}

    PathSolver solver;

    QTimer *updateViewTimer;

    static HostModelProxy *modelProxy;
    static HostModel * GetModel() {return model;}
    static HostModel * GetParkingModel() {return modelParking;}

    int filePass;

    Programs *programList;

private:
    bool solverNeedAnUpdate;
    bool solverUpdateEnabled;

    void SetupMainContainer();
    void SetupHostContainer();
    void SetupProgramContainer();
    void SetupInsertContainer();
    void SetupParking();

    MidiDevices *listMidiDevices;
//    AudioDevices *listAudioDevices;

    QMap<int,Connectables::Object*>listContainers;
    QMap<ConnectionInfo,Connectables::Pin*>listPins;

#ifdef VSTSDK
    vst::CVSTHost vstHost;
#endif

    hashCables workingListOfCables;
    QMutex mutexListCables;
    Renderer renderer;
    QMutex mutexRender;
    QMutex mutexProgChange;

    float sampleRate;
    long bufferSize;
    QTime timeFromStart;
//    float cpuLoad;

    QList<Connectables::Object*>listObjToRemove;


    int progToChange;

    MainHost(QObject *parent = 0);
    static MainHost *theHost;

    friend QDataStream & operator<< (QDataStream&, MainHost&);
    friend QDataStream & operator>> (QDataStream&, MainHost&);

    static HostModel *model;
    static HostModel *modelParking;

signals:
    void SampleRateChanged(float rate);
    void BufferSizeChanged(long size);
    void NewSolver(orderedNodes *renderLines);
//    void ProgramChanged(const QModelIndex &progIndex);
    void ObjectRemoved(int contrainerId, int obj);
    void SolverToUpdate();
    void OnAudioDeviceToggleInUse(const ObjectInfo &objInfo, bool inUse);

public slots:
    void OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
    void OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);

    void SetProgram(const QModelIndex &prgIndex);
    void UpdateSolver(bool forceUpdate=false);
    void ClearHost();

//    void UpdateCpuLoad(float load);
    void SetTempo(int tempo=120, int sign1=4, int sign2=4);
    void OnNewRenderingOrder(orderedNodes *renderLines);
    void Render(unsigned long samples=0);


private slots:

    friend class SetupFile;
};

QDataStream & operator<< (QDataStream& out, MainHost& value);
QDataStream & operator>> (QDataStream& in, MainHost& value);

#endif // MAINHOST_H

