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

#include "tests.h"
#include "mainhost.h"

tests::tests(QObject *parent) :
    QObject(parent)
{
}

void tests::Start()
{
    tstMutex.lock();
    debug("test start ==============")
    for(int i=0;i<100;i++) {
        QSharedPointer<Connectables::Object> objPtr = Connectables::ObjectFactory::Get()->NewObject( ObjectInfo(NodeType::container, ObjType::Container) );
        MainHost::Get()->projectContainer->AddObject( objPtr );
        listObj << objPtr;
    }
    debug("test start finished ==============")
    tstMutex.unlock();

}

void tests::Stop()
{
    tstMutex.lock();
    debug("test stop ==============")
    foreach(QSharedPointer<Connectables::Object> obj, listObj) {
        MainHost::Get()->projectContainer->RemoveObject(obj);
    }
    listObj.clear();
    debug("test stop finished ==============")
    tstMutex.unlock();
}
