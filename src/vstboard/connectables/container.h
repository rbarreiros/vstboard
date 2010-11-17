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

#ifndef CONTAINER_H
#define CONTAINER_H

#include "object.h"
#include "bridge.h"
#include "containerprogram.h"

namespace Connectables {

    class Container : public Object
    {
    Q_OBJECT
    public:
        Container(int index, const ObjectInfo &info);
        virtual ~Container();

        virtual bool Close();
        void Hide();

        void ConnectBridges(QSharedPointer<Object> bridgeA, QSharedPointer<Object> bridgeB, bool hidden=true);

        virtual void AddObject(QSharedPointer<Object> objPtr);
        virtual void RemoveObject(QSharedPointer<Object> objPtr);
        void RemoveCable(QModelIndex & index);

        QSharedPointer<Object> bridgeIn;
        QSharedPointer<Object> bridgeOut;

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        void SetContainerId(quint16 id);
        void SetParentModeIndex(const QModelIndex &parentIndex);
        void CopyProgram(int ori, int dest);
        void RemoveProgram(int prg);

    protected:
        QList< QSharedPointer< Object > >listStaticObjects;

        void AddChildObject(QSharedPointer<Object> objPtr);
        void RemoveChildObject(QSharedPointer<Object> objPtr);

        void LoadFromProgram(ContainerProgram *prog);
        void SaveToProgram(ContainerProgram *prog);
        QHash<int,ContainerProgram*>listContainerPrograms;

        ContainerProgram* currentProgram;
//        QStandardItem *cablesNode;
        QPersistentModelIndex cablesNode;

    signals:

    public slots:
        void AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden=false);
        void RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
        void RemoveCableFromPin(const ConnectionInfo &pin);
        void RemoveCableFromObj(int objId);

        void SaveProgram();
        void UnloadProgram();
        void LoadProgram(int prog);


        void SetSleep(bool sleeping);

        friend class ContainerProgram;
    };


}

#endif // CONTAINER_H
