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

#ifndef PARKINGCONTAINER_H
#define PARKINGCONTAINER_H

//#include "../precomp.h"
//#include "object.h"

#include "container.h"

namespace Connectables {
    class ParkingContainer : public Container
    {
    public:
        ParkingContainer(int index, const ObjectInfo &info);
        ~ParkingContainer();

        void SetParentModeIndex(const QModelIndex &parentIndex);

        void AddObject(QSharedPointer<Object> objPtr);
        void RemoveObject(QSharedPointer<Object> objPtr);

        void Clear();

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);
    protected:
        QList< QWeakPointer< Object > >listStaticObjects;

    };
}

QDataStream & operator<< (QDataStream & out, const Connectables::ParkingContainer& value);
QDataStream & operator>> (QDataStream & in, Connectables::ParkingContainer& value);

#endif // PARKINGCONTAINER_H
