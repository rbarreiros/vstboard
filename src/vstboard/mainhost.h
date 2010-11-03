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

#ifndef MAINHOST_H
#define MAINHOST_H

#include "precomp.h"
#include "connectables/objectfactory.h"
#include "connectables/object.h"
#include "connectables/maincontainer.h"
#include "connectables/parkingcontainer.h"
#include "projectfile/program.h"
#include "renderer.h"
#include "globals.h"
#include "models/hostmodel.h"
#include "models/hostmodelproxy.h"
#include "audiodevices.h"

#include "projectfile/setupfile.h"

#ifdef VSTSDK
    #include "vst/cvsthost.h"
#endif


class MainHost : public QObject
{
Q_OBJECT
public:
    static MainHost* Get(QObject *parent = 0);
    ~MainHost();

    void Open();

    void SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data);
    void Render(unsigned long samples);
    void SetBufferSize(long size);
    void SetSampleRate(float rate=44100.0);
    int GetBufferSize() {return (int)bufferSize;}
    float GetSampleRate() {return sampleRate;}
    int GetCpuLoad();

    void OnObjectAdded(QSharedPointer<Connectables::Object> objPtr);
    void OnObjectRemoved(QSharedPointer<Connectables::Object> objPtr, Connectables::Object *container=0);

    void EnableSolverUpdate(bool enable);
    bool IsSolverUpdateEnabled();

    QDataStream & toStream(QDataStream& stream) const;
    QDataStream & fromStream(QDataStream& stream);

    QSharedPointer<Connectables::MainContainer> mainContainer;
    QSharedPointer<Connectables::MainContainer> hostContainer;
    QSharedPointer<Connectables::MainContainer> projectContainer;
    QSharedPointer<Connectables::MainContainer> programContainer;
    QSharedPointer<Connectables::ParkingContainer> parkingContainer;

    ListAudioInterfacesModel * GetAudioDevicesModel() {return listAudioDevices->GetModel();}

    PathSolver solver;

    QTimer *updateViewTimer;

    static HostModelProxy *modelProxy;
    static HostModel * GetModel() {return model;}
    static HostModel * GetParkingModel() {return modelParking;}

    int filePass;
private:
    bool solverNeedAnUpdate;
    bool solverUpdateEnabled;

    void SetupMainContainer();
    void SetupHostContainer();
    void SetupProjectContainer();
    void SetupProgramContainer();
    void SetupParking();

    AudioDevices *listAudioDevices;

    QMap<int,Connectables::Object*>listContainers;
    QMap<ConnectionInfo,Connectables::Pin*>listPins;

    QList< QSharedPointer<Connectables::Object> >listMidiDevices;
    static void MidiReceive_poll(PtTimestamp timestamp, void *userData);
    QMutex mutexListMidi;

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
    float cpuLoad;

    QList<Connectables::Object*>listObjToRemove;


    int progToChange;

    MainHost(QObject *parent = 0);
    static MainHost *theHost;
    static QMutex singletonMutex;

    friend QDataStream & operator<< (QDataStream&, const MainHost&);
    friend QDataStream & operator>> (QDataStream&, MainHost&);

    static HostModel *model;
    static HostModel *modelParking;

signals:
    void SampleRateChanged(float rate);
    void BufferSizeChanged(long size);
    void NewSolver(orderedNodes *renderLines);
    void ProgramChanged(int progId);
    void ObjectRemoved(int contrainerId, int obj);
    void SolverToUpdate();

public slots:
    void OnCableAdded(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
    void OnCableRemoved(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);

    void SetProgram(int prog);
    void UpdateSolver(bool forceUpdate=false);
    void ClearHost();

    void UpdateCpuLoad(float load);
    void SetTempo(int tempo=120, int sign1=4, int sign2=4);
    void OnNewRenderingOrder(orderedNodes *renderLines);

private slots:

    friend class SetupFile;
};

QDataStream & operator<< (QDataStream& out, MainHost& value);
QDataStream & operator>> (QDataStream& in, MainHost& value);

#endif // MAINHOST_H

