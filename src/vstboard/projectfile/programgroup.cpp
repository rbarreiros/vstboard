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

#include "programgroup.h"

//using namespace Project;
//
//int ProgramGroup::groupCounter = 0;
//QMap<int,ProgramGroup*>ProgramGroup::listProgramsGroup;
//
//ProgramGroup::ProgramGroup(QObject *parent) :
//    QObject(parent)
//{
//    groupIndex = groupCounter++;
//    listProgramsGroup.insert(groupIndex,this);
//
//    for(int i=0; i<5; i++) {
//        Program *prog = new Program(groupIndex,this);
//        listPrograms << prog;
//    }
//    savedId=0;
//}
//
//QDataStream & ProgramGroup::toStream(QDataStream& stream) const
//{
//    stream << (qint32)groupIndex;
//    stream << (qint32)listPrograms.size();
//    foreach(Program *prog,listPrograms) {
//        stream << *prog;
//    }
//    return stream;
//}
//
//QDataStream & ProgramGroup::fromStream(QDataStream& stream)
//{
//    foreach(Program *prog, listPrograms) {
//        delete prog;
//    }
//    listPrograms.clear();
//
//    stream >> (qint32&)savedId;
//
//    qint32 nbProg;
//    stream >> nbProg;
//    for(int i=0; i<nbProg; i++) {
//        Program *prog = new Program(groupIndex,this);
//        listPrograms << prog;
//        stream >> *prog;
//    }
//    return stream;
//}
//
//QDataStream & operator<< (QDataStream& stream, const Project::ProgramGroup& grp)
//{
//    return grp.toStream(stream);
//}
//
//QDataStream & operator>> (QDataStream& stream, Project::ProgramGroup& grp)
//{
//    return grp.fromStream(stream);
//}
