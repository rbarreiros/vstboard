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

#ifndef MIDIPININ_H
#define MIDIPININ_H

#include "pin.h"

namespace Connectables {

    class MidiPinIn : public Pin
    {
    public:
        MidiPinIn(Object *parent, int number=0, bool bridge=false);
        void ReceiveMsg(const int msgType=0,void *data=0);
        virtual float GetVu();

        void SetActivity(int channel);
        int GetActivity();


    protected:
        int activity;
        QList<PmMessage*>msgBuffer;
    };

}
#endif // MIDIPININ_H
