/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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
**************************************************************************/

#ifndef OBJECTPROGRAM_H
#define OBJECTPROGRAM_H

#include "../precomp.h"
#include "objectparameter.h"
#include "pinslist.h"

namespace Connectables {

    class ParameterPin;
    class ObjectProgram
    {

    public:
        ObjectProgram(int progId) : progId(progId),isDirty(false) {}
        ObjectProgram(int progId,PinsList *in, PinsList *out);

        ObjectProgram(const ObjectProgram &c) {
            *this = c;
            isDirty=false;
        }

        void Load(PinsList *in, PinsList *out);
        void Save(PinsList *in,PinsList *out);

        int progId;

        QMap<ushort,ObjectParameter> listParametersIn;
        QMap<ushort,ObjectParameter> listParametersOut;

        QMap<int, QVariant> listOtherValues;

        QDataStream & toStream(QDataStream& out) const;
        QDataStream & fromStream(QDataStream& in);

        bool isDirty;
    };

}

QDataStream & operator<< (QDataStream& out, const Connectables::ObjectProgram& prog);
QDataStream & operator>> (QDataStream& in, Connectables::ObjectProgram& prog);

#endif // OBJECTPROGRAM_H