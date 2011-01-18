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

#include "connectioninfo.h"
#include "object.h"
#include "objectfactory.h"
#include "mainhost.h"

ConnectionInfo::ConnectionInfo()
    : container(0), objId(0), type(PinType::Audio), direction(PinDirection::Output), pinNumber(0), bridge(false), myHost(0)
{
}

ConnectionInfo::ConnectionInfo(MainHost *myHost,quint16 objId, PinType::Enum type, PinDirection::Enum direction, quint16 pinNumber, bool bridge) :
    container(-1),
    objId(objId),
    type(type),
    direction(direction),
    pinNumber(pinNumber),
    bridge(bridge),
    myHost(myHost)
{
}

ConnectionInfo::ConnectionInfo(const ConnectionInfo &c) {
    *this = c;
}

bool ConnectionInfo::CanConnectTo(const ConnectionInfo &c) {

    //don't connect object to itself
    if(objId == c.objId)
        return false;

    //must be the same type (audio/midi/automation) or a bridge pin
    if(type!=PinType::Bridge && c.type!=PinType::Bridge && type != c.type)
        return false;


    //must be opposite directions
    if(direction == c.direction)
        return false;

    int cntA = container;
    int cntB = c.container;

    //if it's a bridge : get the container's container id
    if(bridge)
        cntA = myHost->objFactory->GetObjectFromId( container )->GetContainerId();

    if(c.bridge)
        cntB = myHost->objFactory->GetObjectFromId( c.container )->GetContainerId();

    //must be in the same container
    if(cntA == cntB)
        return true;

    return false;
}

QDataStream & ConnectionInfo::toStream(QDataStream& out) const
{
    out << bridge;
    out << container;
    out << objId;
    out << (quint8)type;
    out << (quint8)direction;
    out << pinNumber;
    return out;
}

QDataStream & ConnectionInfo::fromStream(QDataStream& in)
{
    in >> bridge;
    quint16 savedContainerId;
    in >> savedContainerId;
    int id = myHost->objFactory->IdFromSavedId(savedContainerId);
    if(id==-1) {
        debug("ConnectionInfo::fromStream : container not found")
        return in;
    }
    container = id;

    quint16 savedId;
    in >> savedId;
    id = myHost->objFactory->IdFromSavedId(savedId);
    if(id==-1) {
        debug("ConnectionInfo::fromStream : obj not found")
        return in;
    }
    objId = id;

    in >> (quint8&)type;
    in >> (quint8&)direction;
    in >> pinNumber;

    return in;
}

QDataStream & operator<< (QDataStream& out, const ConnectionInfo& connInfo)
{
   return connInfo.toStream(out);
}

QDataStream & operator>> (QDataStream& in, ConnectionInfo& connInfo)
{
    return connInfo.fromStream(in);
}

