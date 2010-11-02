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

/*
#ifdef WIN32
#ifndef QT_NO_DEBUG

//#include <string.h>
//#include "crtdbg.h"
#include "debugmemreport.h"

QMap<long,memAlloc>memAlloc::mapAllocc;
int memAlloc::count = 0;

void fdumpLeaks()
{
    QMap<int,QString>listStr;

    foreach(memAlloc a, memAlloc::mapAllocc) {
        listStr.insert(a.counter,QString("%1 %2:%3 %4 %5").arg(a.counter).arg(a.fileName).arg(a.lineNumber).arg(a.ptr,0,16).arg(a.size));
    }

    foreach(QString str, listStr) {
        debug(str.toAscii())
    }
}

#endif
#endif
*/
