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

#ifndef CONTAINERPROGRAM_H
#define CONTAINERPROGRAM_H

#include "precomp.h"
#include "cable.h"

namespace Connectables {

    struct objAttirbs {
        QPointF position;
        QSizeF size;
        bool editor;
    };

    class Object;
    class Container;
    class ParkingContainer;
    class ContainerProgram
    {
    public:
        ContainerProgram(Container *container);
        ContainerProgram(const ContainerProgram & c);
        ~ContainerProgram();
        void Load(int progId);
        void Unload();
        void Save();
        ContainerProgram * Copy(int fromId, int toId);
        void Remove(int prgId);

        void AddObject(QSharedPointer<Object> objPtr);
        void RemoveObject(QSharedPointer<Object> objPtr);

        void AddCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin, bool hidden);
        void RemoveCable(const QModelIndex & index);
        void RemoveCable(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);
        void RemoveCableFromPin(const ConnectionInfo &pin);
        void RemoveCableFromObj(int objId);

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

    protected:
        Container *container;
        bool CableExists(const ConnectionInfo &outputPin, const ConnectionInfo &inputPin);

        QList< QSharedPointer<Object> >listObjects;
        QList<Cable*>listCables;

        QMap<int,objAttirbs>mapObjAttribs;

        friend class Container;
        friend class ParkingContainer;
    };
}

QDataStream & operator<< (QDataStream & out, const Connectables::ContainerProgram& value);
QDataStream & operator>> (QDataStream & in, Connectables::ContainerProgram& value);

#endif // CONTAINERPROGRAM_H
