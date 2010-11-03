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

#ifndef PROGRAM_H
#define PROGRAM_H

#include "../precomp.h"
#include "../globals.h"
#include "../pathsolver.h"
#include "../connectables/pin.h"
#include "../connectables/connectioninfo.h"

namespace Project {
    class Program;
}

class ProgChangeEvent : public QEvent
{
public:
    ProgChangeEvent(Project::Program *prog) : QEvent(Event::ProgChange),prog(prog) {}
    Project::Program *prog;
};

namespace Project {

    class Program : public QObject
    {
    Q_OBJECT
    public:
        explicit Program(int index, QObject *parent = 0, int group = 0, int placeInGroup = 0);
        ~Program();

        void CopyFrom(const Program& c);
        void Unload();
        void Load();

        int progIndex;
        int progGroup;
        int progPlaceInGroup;
        bool dirty;
        int savedId;

    };
}

#endif // PROGRAM_H
