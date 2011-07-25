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

#ifndef MAINHOST_H
#define MAINHOST_H

//#include "precomp.h"
#include <QUndoStack>
#include "connectables/objectfactory.h"
#include "connectables/object.h"
#include "connectables/container.h"
#include "renderer/pathsolver.h"
#include "renderer/renderer.h"
#include "globals.h"
#include "models/hostmodel.h"

#ifdef VSTSDK
    #include "vst/cvsthost.h"
#endif

class MainWindow;
class ProgramsModel;
class MainHost : public QObject
{
Q_OBJECT
public:
    MainHost( QObject *parent = 0, QString settingsGroup="");
    virtual ~MainHost();

    void Open();

    void SendMsg(const ConnectionInfo &senderPin,const PinMessage::Enum msgType,void *data);

    void SetBufferSize(unsigned long size);
    void SetSampleRate(float rate=44100.0);
    unsigned long GetBufferSize() {return bufferSize;}
    float GetSampleRate() {return sampleRate;}

    bool EnableSolverUpdate(bool enable);
//    bool IsSolverUpdateEnabled();

    void GetTempo(int &tempo, int &sign1, int &sign2);
    void SetTimeInfo(const VstTimeInfo *info);

    void ChangeNbThreads(int nbThreads);

    QStandardItemModel *GetRendererModel() { return renderer->GetModel(); }

    void OptimizeRenderer() { if(renderer) renderer->Optimize(); }
    Renderer * GetRenderer() { return renderer; }

    void OnCableAdded(Connectables::Cable *cab);
    void OnCableRemoved(Connectables::Cable *cab);

    void SetSetupDirtyFlag() { if(hostContainer) hostContainer->SetDirty(); }

    inline void UpdateSolverNow() {
        //if we need to update everything now, we have to ask for an update and force a render loop
        SetSolverUpdateNeeded();
        Render();
    }

    inline void SetSolverUpdateNeeded() {
        solverMutex.lock();
        solverNeedAnUpdate = true;
        solverMutex.unlock();
    }

    inline bool undoProgramChanges() {return undoProgramChangesEnabled;}

    QSharedPointer<Connectables::Container> mainContainer;
    QSharedPointer<Connectables::Container> hostContainer;
    QSharedPointer<Connectables::Container> projectContainer;
    QSharedPointer<Connectables::Container> programContainer;
    QSharedPointer<Connectables::Container> groupContainer;

    QTimer *updateViewTimer;

    HostModel * GetModel() {return model;}
    ProgramsModel *programsModel;
    Connectables::ObjectFactory *objFactory;
    MainWindow *mainWindow;

#ifdef VSTSDK
    vst::CVSTHost *vstHost;
    static int vstUsersCounter;
#endif

    static quint32 currentFileVersion;
    bool doublePrecision;

    void SetSetting(QString name, QVariant value);
    QVariant GetSetting(QString name, QVariant defaultVal=0);
    bool SettingDefined(QString name);

#ifdef SCRIPTENGINE
    QScriptEngine *scriptEngine;
#endif

    QString currentProjectFile;
    QString currentSetupFile;

    QUndoStack undoStack;

protected:
    QTime timeFromStart;
    float sampleRate;
    unsigned long bufferSize;

private:
    void SetupMainContainer();
    void SetupHostContainer();
    void SetupProjectContainer();
    void SetupProgramContainer();
    void SetupGroupContainer();

    void CheckTempo();

    bool solverNeedAnUpdate;
    bool solverUpdateEnabled;

    QMap<int,Connectables::Object*>listContainers;
    QMap<ConnectionInfo,Connectables::Pin*>listPins;


    hashCables workingListOfCables;
    QMutex *mutexListCables;
    Renderer *renderer;
    QMutex mutexRender;
    QMutex solverMutex;

    HostModel *model;

    int currentTempo;
    int currentTimeSig1;
    int currentTimeSig2;

    QString settingsGroup;
    QSettings settings;

    bool undoProgramChangesEnabled;

    PathSolver *solver;

signals:
    void SampleRateChanged(float rate);
    void BufferSizeChanged(unsigned long size);
    void ObjectRemoved(int contrainerId, int obj);
    void SolverToUpdate();
    void Rendered();
    void programParkingModelChanged(QStandardItemModel *model);
    void groupParkingModelChanged(QStandardItemModel *model);
    void TempoChanged(int tempo=120, int sign1=4, int sign2=4);
    void currentFileChanged();

public slots:
    void SetTempo(int tempo=120, int sign1=4, int sign2=4);
    virtual void Render(unsigned long samples=0);
    void LoadFile(const QString &filename);
    void LoadSetupFile(const QString &filename = QString());
    void LoadProjectFile(const QString &filename = QString());
    void ReloadProject();
    void ReloadSetup();
    void ClearSetup();
    void ClearProject();
    void SaveSetupFile(bool saveAs=false);
    void SaveProjectFile(bool saveAs=false);

private slots:
    void UpdateSolver(bool forceUpdate=false);

    friend class SetupFile;
    friend class ProjectFile;
};

#endif // MAINHOST_H

