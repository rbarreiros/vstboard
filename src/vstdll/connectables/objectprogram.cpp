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
        listParametersIn.insert(i.key(), param);
        ++i;
    }

    hashListParamPin::const_iterator j = listOut.constBegin();
    while(j!=listOut.constEnd()) {
        j.value()->GetDefault(param);
        listParametersOut.insert(j.key(), param);
        ++j;
    }
}

void ObjectProgram::Load(hashListParamPin& listIn, hashListParamPin& listOut)
{
    hashListParamPin::iterator i = listIn.begin();
    while(i!=listIn.end()) {
        if(listParametersIn.contains(i.key())) {
            i.value()->Load(listParametersIn.value(i.key()));
        } else {
            debug2(<< "ObjectProgram::Load paramIn not found" << i.key())
        }
        ++i;
    }

    hashListParamPin::iterator j = listOut.begin();
    while(j!=listOut.end()) {
        if(listParametersOut.contains(j.key())) {
            j.value()->Load(listParametersOut.value(j.key()));
        } else {
            debug2(<< "ObjectProgram::Load paramOut not found" << j.key())
        }
        ++j;
    }
}

void ObjectProgram::Save(const hashListParamPin& listIn, const hashListParamPin& listOut)
{
    hashListParamPin::ConstIterator i = listIn.constBegin();
    while(i!=listIn.constEnd()) {
        i.value()->GetValues( listParametersIn[i.key()] );
        ++i;
    }

    hashListParamPin::ConstIterator j = listOut.constBegin();
    while(j!=listOut.constEnd()) {
        j.value()->GetValues( listParametersOut[j.key()] );
        ++j;
    }
}


QDataStream & ObjectProgram::toStream(QDataStream& out) const
{
    const quint16 file_version = 1;
    out << file_version;

    out << (quint16)progId;

    out << (quint16)listParametersIn.size();
    QMap<ushort,ObjectParameter>::ConstIterator i = listParametersIn.constBegin();
    while(i!=listParametersIn.constEnd()) {
        out << (quint16)i.key();
        out << i.value();
        ++i;
    }

    out << (quint16)listParametersOut.size();
    QMap<ushort,ObjectParameter>::ConstIterator j = listParametersOut.constBegin();
    while(j!=listParametersOut.constEnd()) {
        out << (quint16)j.key();
        out << j.value();
        ++j;
    }

    out << (quint16)listOtherValues.size();
    QMap<int, QVariant>::ConstIterator k = listOtherValues.constBegin();
    while(k!=listOtherValues.constEnd()) {
        out << (quint16)k.key();
        out << k.value();
        ++k;
    }

    return out;
}

QDataStream & ObjectProgram::fromStream(QDataStream& in)
{
    quint16 file_version;
    in >> file_version;

    in >> (quint16&)progId;

    quint16 nbParam;
    in >> nbParam;
    for(int i=0; i<nbParam; i++) {
        ObjectParameter param;
        quint16 id;
        in >> id;
        in >> param;
        listParametersIn.insert(id, param);
    }
    in >> nbParam;
    for(int i=0; i<nbParam; i++) {
        ObjectParameter param;
        quint16 id;
        in >> id;
        in >> param;
        listParametersOut.insert(id, param);
    }

    in >> nbParam;
    for(int i=0; i<nbParam; i++) {
        quint16 id;
        in >> id;
        QVariant value;
        in >> value;

        listOtherValues.insert(id,value);
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
