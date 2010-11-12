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

#ifndef CONNECTABLEOBJECT_H
#define CONNECTABLEOBJECT_H

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4100 )
#endif

#include "../precomp.h"

#include "audiopinin.h"
#include "audiopinout.h"
#include "midipinin.h"
#include "midipinout.h"
#include "parameterpinin.h"
#include "parameterpinout.h"
#include "bridgepinin.h"
#include "bridgepinout.h"
#include "objectprogram.h"
#include "objectinfo.h"

class SolverNode;

namespace Connectables {

    typedef QHash<int,ObjectProgram*> hashPrograms;
    typedef QHash<ushort,ParameterPin*> hashListParamPin;

    class Object : public QObject
    {
    Q_OBJECT
    public:

        Object(int index, const ObjectInfo &info);
        virtual ~Object();

        virtual bool Open();
        virtual bool Close();
        virtual void Hide();
        virtual void Render() {}
        virtual int GetProcessingTime() {return 0;}

        void setObjectName(const QString &name);

        virtual Pin * GetPin(const ConnectionInfo &pinInfo);
        inline int GetSavedIndex() {return savedIndex;}
        inline void ResetSavedIndex(int id=-2) {savedIndex=id;}

        const ObjectInfo & info() const {return objInfo;}
        SolverNode *GetSolverNode() const {return solverNode;}
        void SetSolverNode(SolverNode *node) {solverNode=node;}

        QList<BridgePinIn*> GetListBridgePinIn() {return listBridgePinIn;}
        QList<BridgePinOut*> GetListBridgePinOut() {return listBridgePinOut;}

        void SetIndex(int i) {index=i;}
        const int GetIndex() const {return index;}

        const int GetContainerId() const {return containerId;}

        //don't include in rendering map if sleeping
        virtual void SetSleep(bool sleeping);
        bool GetSleep();

        virtual float SetParameter(int paramId, float value) {return value;}
        virtual void MidiMsgFromInput(long msg) {}

        inline short GetLearningMode() {return parameterLearning;}
        virtual QString GetParameterName(ConnectionInfo pinInfo) {return "";}

        void NewRenderLoop();

        void SetParkingNode(QStandardItem* parent);
        QStandardItem *GetModelNode() {return modelNode;}

        virtual bool OpenEditor() {return false;}

        virtual QDataStream & toStream (QDataStream &) const;
        virtual QDataStream & fromStream (QDataStream &);

        inline void Lock() { objMutex.lock();}
        inline void Unlock() { objMutex.unlock();}

        void SetPosition(QPointF pos);
        QPointF GetPosition() {return position;}
        void SetSize(QSizeF s);
        virtual QSizeF GetSize() {return size;}
        virtual bool GetEditorVisible() {return false;}
        QPointF position;
        QSizeF size;

        virtual void SetParentModelNode(QStandardItem* parent);
        virtual void UpdateModelNode();

        int GetCurrentProgId() {return currentProgId;}

        bool listenProgramChanges;

        virtual void SetBridgePinsInVisible(bool visible);
        virtual void SetBridgePinsOutVisible(bool visible);

        virtual void CopyProgram(int ori, int dest);
        virtual void RemoveProgram(int prg);

    protected:
        QList<AudioPinIn*>listAudioPinIn;
        QList<AudioPinOut*>listAudioPinOut;
        QList<MidiPinIn*>listMidiPinIn;
        QList<MidiPinOut*>listMidiPinOut;
        hashListParamPin listParameterPinIn;
        hashListParamPin listParameterPinOut;
        QList<BridgePinIn*>listBridgePinIn;
        QList<BridgePinOut*>listBridgePinOut;

        QMutex objMutex;

        //used by pathsolver
        SolverNode *solverNode;

        hashPrograms listPrograms;

        int index;
        int savedIndex;

        bool sleep;
        short parameterLearning;
        ObjectProgram *currentProgram;
        bool progIsDirty;

        bool closed;
        quint16 containerId;

        QStandardItem *modelNode;
        QStandardItem *modelAudioIn;
        QStandardItem *modelAudioOut;
        QStandardItem *modelMidiIn;
        QStandardItem *modelMidiOut;
        QStandardItem *modelParamIn;
        QStandardItem *modelParamOut;
        QStandardItem *modelBridgeIn;
        QStandardItem *modelBridgeOut;
        QStandardItem *modelEditor;

        int currentProgId;

        ObjectInfo objInfo;

    signals:
        void LearningModeChanged(bool learn);
        void UnLearningModeChanged(bool unlearn);
        void CpuLoad(float load);
        void Loaded();
        void CloseEditorWindow();

    public slots:
        virtual void SaveProgram();
        virtual void UnloadProgram();
        virtual void LoadProgram(int prog);
        void SetLearningMode(bool learning);
        void SetUnLearningMode(bool unlearning);
        void OnProgramDirty() {progIsDirty=true;}

        virtual void SetBufferSize(long size) {}
        virtual void SetSampleRate(float rate=44100.0) {}
        virtual void OnParameterChanged(ConnectionInfo pinInfo, float value) {}

        virtual void CloseEditor() {}

        void CloseSlot() {Close();}

    };


}

QDataStream & operator<< (QDataStream & out, const Connectables::Object& value);
QDataStream & operator>> (QDataStream & in, Connectables::Object& value);

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // CONNECTABLEOBJECT_H
