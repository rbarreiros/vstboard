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

ConnectionInfo::ConnectionInfo()
    : container(0), objId(0), type(PinType::Audio), direction(PinDirection::Output), pinNumber(0), bridge(false)
{
}

ConnectionInfo::ConnectionInfo(quint16 objId, PinType::Enum type, PinDirection::Enum direction, quint16 pinNumber, bool bridge) :
    container(-1),
    objId(objId),
    type(type),
    direction(direction),
    pinNumber(pinNumber),
    bridge(bridge)
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
        cntA = Connectables::ObjectFactory::Get()->GetObjectFromId( container )->GetContainerId();

    if(c.bridge)
        cntB = Connectables::ObjectFactory::Get()->GetObjectFromId( c.container )->GetContainerId();

    //must be in the same container
    if(cntA == cntB)
        return true;

    return false;
}

QDataStream & ConnectionInfo::toStream(QDataStream& stream) const
{
    stream << bridge;
    stream << container;
    stream << objId;
    stream << type;
    stream << direction;
    stream << pinNumber;
    return stream;
}

QDataStream & ConnectionInfo::fromStream(QDataStream& stream)
{
    stream >> bridge;
    quint16 savedContainerId;
    stream >> savedContainerId;
    int id = Connectables::ObjectFactory::Get()->IdFromSavedId(savedContainerId);
    if(id==-1) {
        debug("ConnectionInfo::fromStream : container not found")
        return stream;
    }
    container = id;

    quint16 savedId;
    stream >> savedId;
    id = Connectables::ObjectFactory::Get()->IdFromSavedId(savedId);
    if(id==-1) {
        debug("ConnectionInfo::fromStream : obj not found")
        return stream;
    }
    objId = id;

    stream >> type;
    stream >> direction;
    stream >> pinNumber;

    return stream;
}

QDataStream & operator<< (QDataStream& stream, const ConnectionInfo& connInfo)
{
   return connInfo.toStream(stream);
}

QDataStream & operator>> (QDataStream& stream, ConnectionInfo& connInfo)
{
    return connInfo.fromStream(stream);
}

