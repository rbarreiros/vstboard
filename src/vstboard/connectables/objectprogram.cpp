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
    while(i!=listIn.constEnd() && i.key()<listParametersIn.size()) {
        i.value()->GetValues( listParametersIn[i.key()] );
        ++i;
    }

    hashListParamPin::const_iterator j = listOut.constBegin();
    while(j!=listOut.constEnd() && j.key()<listParametersOut.size()) {
        j.value()->GetValues( listParametersOut[j.key()] );
        ++j;
    }
}


QDataStream & ObjectProgram::toStream(QDataStream& out) const
{
    const quint16 file_version = 1;
    out << file_version;

    out << (qint16)progId;

    out << (qint16)listParametersIn.size();
    foreach(ObjectParameter param, listParametersIn) {
        out << param;
    }
    out << (qint16)listParametersOut.size();
    foreach(ObjectParameter param, listParametersOut) {
        out << param;
    }

    return out;
}

QDataStream & ObjectProgram::fromStream(QDataStream& in)
{
    quint16 file_version;
    in >> file_version;

    in >> (qint16&)progId;

    qint16 nbParam;
    in >> nbParam;
    for(int i=0; i<nbParam; i++) {
        ObjectParameter param;
        in >> param;
        listParametersIn << param;
    }
    in >> nbParam;
    for(int i=0; i<nbParam; i++) {
        ObjectParameter param;
        in >> param;
        listParametersOut << param;
    }

    return in;
}


QDataStream & operator<< (QDataStream& out, const Connectables::ObjectProgram& prog)
{
    return prog.toStream(out);
}

QDataStream & operator>> (QDataStream& in, Connectables::ObjectProgram& prog)
{
    return prog.fromStream(in);
}
