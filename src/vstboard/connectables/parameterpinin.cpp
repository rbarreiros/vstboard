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

#include "parameterpinin.h"
#include "object.h"
#include "../globals.h"

using namespace Connectables;

//parameter is a float
ParameterPinIn::ParameterPinIn(Object *parent, int number, float defaultValue, bool defaultVisible, QString name, bool nameCanChange) :
        ParameterPin(parent, PinDirection::Input, number, defaultValue, defaultVisible, name, nameCanChange)
{
}

//parameter is a int, index in a list of values
ParameterPinIn::ParameterPinIn(Object *parent, int number, QVariant defaultValue, QList<QVariant> *listValues, bool defaultVisible, QString name, bool nameCanChange) :
        ParameterPin(parent, PinDirection::Input, number, defaultValue, listValues, defaultVisible, name, nameCanChange)
{
}

void ParameterPinIn::ReceiveMsg(const int msgType,void *data)
{
    if(msgType == PinMessage::ParameterValue) {
        ChangeValue(*(float*)data);
    }
}

