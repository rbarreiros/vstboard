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

#ifndef PARKINGCONTAINER_H
#define PARKINGCONTAINER_H

#include "../precomp.h"
#include "object.h"

namespace Connectables {
    class ParkingContainer
    {
    public:
        ParkingContainer();
        ~ParkingContainer();

        void SetParentModelNode(QStandardItem* parent);

        void AddObject(QSharedPointer<Object> &objPtr);
        void RemoveObject(QSharedPointer<Object> &objPtr);

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        int filePass;

    protected:
        QList<QSharedPointer<Object> >listObj;
        QStandardItem *modelNode;
        bool closing;
    };
}

QDataStream & operator<< (QDataStream & out, const Connectables::ParkingContainer& value);
QDataStream & operator>> (QDataStream & in, Connectables::ParkingContainer& value);

#endif // PARKINGCONTAINER_H
