/******************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#ifndef MIDITOAUTOMATION_H
#define MIDITOAUTOMATION_H

#include "../precomp.h"
#include "object.h"

namespace Connectables {

    class MidiToAutomation : public Object
    {
    Q_OBJECT
    public:
        explicit MidiToAutomation(MainHost *myHost,int index);
        void Render();
        void MidiMsgFromInput(long msg);

    protected:
        void ChangeValue(int ctrl, int value);
        QList<QVariant> listValues;
        QHash<quint16,quint8>listChanged;

    };
}

#endif // MIDITOAUTOMATION_H
