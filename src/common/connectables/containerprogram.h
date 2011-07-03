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

#ifndef CONTAINERPROGRAM_H
#define CONTAINERPROGRAM_H

//#include "precomp.h"
#include "cable.h"
#include "objectinfo.h"

class MainHost;

class RendererNode;
namespace Connectables {

    class Object;
    class Container;
    class ContainerProgram
    {
    public:
        ContainerProgram(MainHost *myHost, Container *container);
        ContainerProgram(const ContainerProgram & c);
        ~ContainerProgram();
        void Load(int progId);
        void Unload();
        void Save(bool saveChildPrograms=true);
        void ParkAllObj();
        ContainerProgram * Copy(int fromId, int toId);
        ContainerProgram * CopyTo(int toId);
        void Remove(int prgId);

        void AddObject(QSharedPointer<Object> objPtr);
        void RemoveObject(QSharedPointer<Object> objPtr);
        void ReplaceObject(QSharedPointer<Object> newObjPtr, QSharedPointer<Object> replacedObjPtr);

        bool AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden=false);
        void RemoveCable(Cable *cab);
        void RemoveCable(const QModelIndex & index);
        void RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
        void RemoveCableFromPin(const ConnectionInfo &pin);
        void RemoveCableFromObj(int objId);
        void CreateBridgeOverObj(int objId);
        void CopyCablesFromObj(int newObjId, int oldObjId);
        void MoveOutputCablesFromObj(int newObjId, int oldObjId);
        void MoveInputCablesFromObj(int newObjId, int oldObjId);

        bool IsDirty();

        void SaveRendererState();
        void LoadRendererState();

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        QList<RendererNode*>listOfRendererNodes;
        QTime timeSavedRendererNodes;
        static QTime unsavedTime;

    protected:
        bool CableExists(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
        bool PinExistAndVisible(const ConnectionInfo &info);

        Container *container;
        bool dirty;
        MainHost *myHost;

        QList< QSharedPointer<Object> >listObjects;
        QList<Cable*>listCables;

        QMap<int,ObjectContainerAttribs>mapObjAttribs;


        friend class Container;
        friend class ParkingContainer;
    };
}

QDataStream & operator<< (QDataStream & out, const Connectables::ContainerProgram& value);
QDataStream & operator>> (QDataStream & in, Connectables::ContainerProgram& value);




#endif // CONTAINERPROGRAM_H
