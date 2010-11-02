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

#include "program.h"
#include "../globals.h"
#include "../mainhost.h"

using namespace Project;

Program::Program(int index, QObject *parent, int group, int placeInGroup) :
    QObject(parent),
    progIndex(index),
    progGroup(group),
    progPlaceInGroup(placeInGroup),
    dirty(true),
    savedId(0)
{
}

Program::~Program()
{
//    listCables.clear();
}

//void Program::CopyFrom(const Program& c)
//{
//    progGroup = c.progGroup;
//    progIndex = c.progIndex;
//    savedId = c.savedId;
//    listCables = c.listCables;
//}

//void Program::Load()
//{
//    hashCables::iterator i = listCables.begin();
//    while (i != listCables.end()) {
//        emit CableAdded(i.key(),i.value());
//        ++i;
//    }

//    dirty=false;
//}

//void Program::Unload()
//{
//    hashCables::iterator i = listCables.begin();
//    while (i != listCables.end()) {
//        emit CableRemoved(i.key(),i.value());
//        ++i;
//    }
//}

//QDataStream & Program::toStream(QDataStream& stream) const
//{
//    stream << (qint16)listCables.size();
//    hashCables::const_iterator i = listCables.constBegin();
//    while (i != listCables.constEnd()) {
//        stream << i.key();
//        stream << i.value();
//        ++i;
//    }
//    return stream;
//}

//QDataStream & Program::fromStream(QDataStream& stream)
//{
//    listCables.clear();

//    qint16 nbCables;
//    stream >> nbCables;
//    for(int i=0;i<nbCables;i++) {
//        ConnectionInfo infoOut;
//        stream >> infoOut;
//        ConnectionInfo infoIn;
//        stream >> infoIn;
//        listCables.insert(infoOut,infoIn);
//    }

//    return stream;
//}

//QDataStream & operator<< (QDataStream& stream, const Project::Program& prog)
//{
//   return prog.toStream(stream);
//}

//QDataStream & operator>> (QDataStream& stream, Project::Program& prog)
//{
//    return prog.fromStream(stream);
//}
