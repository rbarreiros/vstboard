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

#include "objectprogram.h"
#include "parameterpin.h"

using namespace Connectables;

ObjectProgram::ObjectProgram(int progId,const hashListParamPin& listIn, const hashListParamPin& listOut) :
    progId(progId)
{
    ObjectParameter param;
    hashListParamPin::const_iterator i = listIn.constBegin();
    while(i!=listIn.constEnd()) {
        i.value()->GetDefault(param);
        listParametersIn << param;
        ++i;
    }

    hashListParamPin::const_iterator j = listOut.constBegin();
    while(j!=listOut.constEnd()) {
        j.value()->GetDefault(param);
        listParametersOut << param;
        ++j;
    }
}

void ObjectProgram::Load(hashListParamPin& listIn, hashListParamPin& listOut)
{
    hashListParamPin::iterator i = listIn.begin();
    while(i!=listIn.end()) {
        if(i.key()<listParametersIn.size())
            i.value()->Load(&listParametersIn.at(i.key()));
        ++i;
    }

    hashListParamPin::iterator j = listOut.begin();
    while(j!=listOut.end()) {
        if(j.key()<listParametersOut.size())
        j.value()->Load(&listParametersOut.at(j.key()));
        ++j;
    }
}

void ObjectProgram::Save(const hashListParamPin& listIn, const hashListParamPin& listOut)
{
    hashListParamPin::const_iterator i = listIn.constBegin();
    while(i!=listIn.constEnd()) {
        i.value()->GetValues( listParametersIn[i.key()] );
        ++i;
    }

    hashListParamPin::const_iterator j = listOut.constBegin();
    while(j!=listOut.constEnd()) {
        j.value()->GetValues( listParametersOut[j.key()] );
        ++j;
    }
}


QDataStream & ObjectProgram::toStream(QDataStream& stream) const
{
    stream << (qint16)progId;

    stream << (qint16)listParametersIn.size();
    foreach(ObjectParameter param, listParametersIn) {
        stream << param;
    }
    stream << (qint16)listParametersOut.size();
    foreach(ObjectParameter param, listParametersOut) {
        stream << param;
    }

    return stream;
}

QDataStream & ObjectProgram::fromStream(QDataStream& stream)
{
    stream >> (qint16&)progId;

    qint16 nbParam;
    stream >> nbParam;
    for(int i=0; i<nbParam; i++) {
        ObjectParameter param;
        stream >> param;
        listParametersIn << param;
    }
    stream >> nbParam;
    for(int i=0; i<nbParam; i++) {
        ObjectParameter param;
        stream >> param;
        listParametersOut << param;
    }

    return stream;
}


QDataStream & operator<< (QDataStream& stream, const Connectables::ObjectProgram& prog)
{
    return prog.toStream(stream);
}

QDataStream & operator>> (QDataStream& stream, Connectables::ObjectProgram& prog)
{
    return prog.fromStream(stream);
}
