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
#ifndef DEBUGMEMREPORT_H
#define DEBUGMEMREPORT_H

#ifdef WIN32
#ifndef QT_NO_DEBUG

#include "precomp.h"

class memAlloc {
public:
    memAlloc(long ptr,long size,const char *fileName, int lineNumber) : ptr(ptr), size(size), fileName(fileName), lineNumber(lineNumber) {
        counter = count++;
//        debug(QString("alloc %1").arg((long)ptr,0,16).toAscii())
    }

    long ptr;
    long size;
    const char *fileName;
    int lineNumber;
    int counter;

    static int count;
    static QMap<long,memAlloc>mapAllocc;
};

void fdumpLeaks();

inline void * __cdecl operator new(size_t size, const char *fileName, int lineNumber) throw(std::bad_alloc)
{
    void *ptr = (void *)std::malloc(size);
    if (!ptr)
    throw std::bad_alloc() ;
    memAlloc::mapAllocc.insert((long)ptr,memAlloc((long)ptr,size,fileName,lineNumber));
//    debug("new 0x%x",(long)ptr)
    return(ptr);
}
inline void __cdecl operator delete(void *p) throw ()
{
//    debug("delete 0x%x",(long)p)
    if(p) {
        std::free(p);
        memAlloc::mapAllocc.remove((long)p);
    }
}

inline void * __cdecl operator new[](size_t size, const char *fileName, int lineNumber)
{
    return ::operator new(size,fileName,lineNumber);
}
inline void __cdecl operator delete[](void *p) throw ()
{
    ::operator delete(p);
}

inline void __cdecl operator delete(void *p, const char * fileName, int lineNumber)
{
    ::operator delete(p);
}

#define new new(__FILE__, __LINE__)
//#define new DEBUG_NEW

#endif
#endif

#endif // DEBUGMEMREPORT_H
*/
