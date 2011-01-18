/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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

#include "maincontainer.h"

using namespace Connectables;

MainContainer::MainContainer(MainHost *myHost,int index, const ObjectInfo &info) :
        Container(myHost,index, info),
        bridgeSend(0),
        bridgeReturn(0)
{
}

MainContainer::~MainContainer()
{
    Close();
}

bool MainContainer::Close()
{
    bridgeSend.clear();
    bridgeReturn.clear();
    return true;
}

void MainContainer::AddObject(QSharedPointer<Object> objPtr)
{
    objPtr->SetContainerId(index);

    //bridges are not stored in program
    if(objPtr->info().nodeType == NodeType::bridge) {
        if(objPtr->info().objType==ObjType::BridgeIn) {
            bridgeIn=objPtr;
        }
        if(objPtr->info().objType==ObjType::BridgeOut) {
            bridgeOut=objPtr;
        }

        if(objPtr->info().objType==ObjType::BridgeSend) {
            bridgeSend=objPtr;
        }
        if(objPtr->info().objType==ObjType::BridgeReturn) {
            bridgeReturn=objPtr;
        }

        objPtr->listenProgramChanges=false;
        listStaticObjects << objPtr;
        AddChildObject(objPtr);
        objPtr->LoadProgram(currentProgId);
        return;
    }

    Container::AddObject(objPtr);

}

void MainContainer::ParkObject(QSharedPointer<Object> objPtr)
{
    Container::ParkObject(objPtr);

    if(objPtr==bridgeSend)
        bridgeSend.clear();
    if(objPtr==bridgeReturn)
        bridgeReturn.clear();
}
