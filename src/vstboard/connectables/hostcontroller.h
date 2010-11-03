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

#ifndef HOSTCONTROLLER_H
#define HOSTCONTROLLER_H

#include "../precomp.h"
#include "object.h"

namespace Connectables {

    class HostController : public Object
    {
        enum Param {
            Param_Tempo,
            Param_Sign1,
            Param_Sign2,
        };

    Q_OBJECT
    public:
        explicit HostController(int index);
        void Render();
        void MidiMsgFromInput(long msg);

    protected:
        QList<QVariant> listTempo;
        QList<QVariant> listSign1;
        QList<QVariant> listSign2;

        bool tempoChanged;
        bool progChanged;
        int prog;

    signals:
        void progChange(int prog);
        void tempoChange(int tempo, int sign1, int sign2);

    public slots:
        virtual void OnParameterChanged(ConnectionInfo pinInfo, float value);
    };

}


#endif // HOSTCONTROLLER_H
