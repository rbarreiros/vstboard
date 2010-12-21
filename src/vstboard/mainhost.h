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
#include "pathsolver.h"
#include "renderer.h"
#include "globals.h"
#include "models/hostmodel.h"
#ifdef VST_PLUGIN
    #include "connectables/vstaudiodevicein.h"
    #include "connectables/vstaudiodeviceout.h"
#else
    #include "audiodevices.h"
    #include "mididevices.h"
#endif

#include "programs.h"
//#include "projectfile/setupfile.h"

#ifdef VSTSDK
    #include "vst/cvsthost.h"
#endif

class Vst;
class MainWindow;
//class AudioDevices;
//class MainConfig;
class MainHost : public QObject
{
Q_OBJECT
public:
//    inline static MainHost* Get() {return theHost;}
//    static MainHost* Create(QObject *parent = 0);
    MainHost(Vst *myVstPlugin = 0, QObject *parent = 0);
    ~MainHost();

    void Open();

    void SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data);

    void SetBufferSize(unsigned long size);
    void SetSampleRate(float rate=44100.0);
    unsigned long GetBufferSize() {return bufferSize;}
    float GetSampleRate() {return sampleRate;}

    void OnObjectAdded(QSharedPointer<Connectables::Object> objPtr);
    void OnObjectRemoved(QSharedPointer<Connectables::Object> objPtr, Connectables::Object *container=0);

    void EnableSolverUpdate(bool enable);
    bool IsSolverUpdateEnabled();


    QSharedPointer<Connectables::MainContainer> mainContainer;
    QSharedPointer<Connectables::MainContainer> hostContainer;
    QSharedPointer<Connectables::MainContainer> projectContainer;
    QSharedPointer<Connectables::MainContainer> programContainer;
    QSharedPointer<Connectables::MainContainer> groupContainer;
    QSharedPointer<Connectables::ParkingContainer> parkingContainer;

    PathSolver *solver;

    QTimer *updateViewTimer;

    HostModel * GetModel() {return model;}
    HostModel * GetParkingModel() {return modelParking;}

    int filePass;

    Programs *programList;

#ifndef VST_PLUGIN
    AudioDevices *audioDevices;
    MidiDevices *midiDevices;
#endif

    Connectables::ObjectFactory *objFactory;
    MainWindow *mainWindow;

#ifdef VSTSDK
    vst::CVSTHost *vstHost;
#endif

Vst *myVstPlugin;

private:
    void SetupMainContainer();
    void SetupHostContainer();
    void SetupProjectContainer();
    void SetupProgramContainer();
    void SetupGroupContainer();
    void SetupParking();

    bool solverNeedAnUpdate;
    bool solverUpdateEnabled;

    QMap<int,Connectables::Object*>listContainers;
    QMap<ConnectionInfo,Connectables::Pin*>listPins;


    hashCables workingListOfCables;
    QMutex *mutexListCables;
    Renderer renderer;
    QMutex mutexRender;
    QMutex solverMutex;

    float sampleRate;
    unsigned long bufferSize;
    QTime timeFromStart;

    QList<Connectables::Object*>listObjToRemove;

    HostModel *model;
    HostModel *modelParking;

signals:
    void SampleRateChanged(float rate);
    void BufferSizeChanged(unsigned long size);
    void NewSolver(orderedNodes *renderLines);
    void ObjectRemoved(int contrainerId, int obj);
    void SolverToUpdate();
    void OnAudioDeviceToggleInUse(const ObjectInfo &objInfo, bool inUse);
    void Rendered();

public slots:
    void OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
    void OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);

    void UpdateSolver(bool forceUpdate=false);

    void SetTempo(int tempo=120, int sign1=4, int sign2=4);
    void OnNewRenderingOrder(orderedNodes *renderLines);
    void Render(unsigned long samples=0);


private slots:

    friend class SetupFile;
    friend class ProjectFile;
};

#endif // MAINHOST_H

