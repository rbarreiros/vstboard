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

#ifndef CONTAINER_H
#define CONTAINER_H

#include "object.h"
#include "bridge.h"
#include "containerprogram.h"
#include "models/hostmodel.h"

#define LOADSAVE_STAGES 3

namespace Connectables {

    class Container : public Object
    {
    Q_OBJECT
    public:
        Container(MainHost *myHost,int index, const ObjectInfo &info);
        virtual ~Container();

        void Hide();
        void ConnectBridges(QSharedPointer<Object> bridgeA, QSharedPointer<Object> bridgeB, bool hidden=true);
        void RemoveCable(QModelIndex & index);
        void SetContainerId(quint16 id);
        const QModelIndex &GetCablesIndex();

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        void OnChildDeleted(Object *obj);

        virtual bool Close();
        virtual void AddObject(QSharedPointer<Object> objPtr);
        virtual void AddParkedObject(QSharedPointer<Object> objPtr);
        virtual void ParkObject(QSharedPointer<Object> objPtr);
        virtual bool IsDirty();

        /// shared pointer to the bridge in object
        QSharedPointer<Object> bridgeIn;

        /// shared pointer to the bridge out object
        QSharedPointer<Object> bridgeOut;

        /// model for the parking storage
        HostModel parkModel;

    protected:
        void AddChildObject(QSharedPointer<Object> objPtr);
        void ParkChildObject(QSharedPointer<Object> objPtr);

        /// list of programs
        QHash<int,ContainerProgram*>listContainerPrograms;

        /// pointer to the current program
        ContainerProgram* currentProgram;

        /// list of static objects (bridges are static)
        QList< QSharedPointer< Object > >listStaticObjects;

        /// a child model index for the cables
        QPersistentModelIndex cablesNode;

        /// list of all loaded Objects
        QList<Object*>listLoadedObjects;

        /// store the objects while loading, preventing them from being deleted. (the objects are loaded before the programs using them)
        QList< QSharedPointer< Object > >listLoadingObjects;

        /// id of the progam to change on the next rendering loop
        int progToSet;

    public slots:
        void AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden=false);
        void RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
        void RemoveCableFromPin(const ConnectionInfo &pin);
        void RemoveCableFromObj(int objId);

        void SaveProgram();
        void UnloadProgram();
        void LoadProgram(int prog);

        void SetSleep(bool sleeping);

        void SetProgram(const QModelIndex &prg);
        void CopyProgram(int ori, int dest);
        void RemoveProgram(int prg);
        void Render();

        void SetBufferSize(unsigned long size);
        void SetSampleRate(float rate=44100.0);

        friend class ContainerProgram;
    };


}

#endif // CONTAINER_H
