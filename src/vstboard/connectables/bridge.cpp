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

#include "bridge.h"
#include "objectfactory.h"
#include "../audiobuffer.h"
#include "../mainhost.h"
#include "bridgepinin.h"
#include "bridgepinout.h"

using namespace Connectables;

Bridge::Bridge(MainHost *myHost,int index, const ObjectInfo & info) :
        Object(myHost,index, info)
{

}

bool Bridge::Open()
{
    closed=false;

    if(objInfo.objType == ObjType::BridgeIn || objInfo.objType == ObjType::BridgeReturn) {
        for(int i=0; i<8; i++) {
            listBridgePinIn << new BridgePinIn(this, i, true );
            listBridgePinOut << new BridgePinOut(this,i );
        }
    }

    if(objInfo.objType == ObjType::BridgeOut || objInfo.objType == ObjType::BridgeSend) {
        for(int i=0; i<8; i++) {
            listBridgePinIn << new BridgePinIn(this, i );
            listBridgePinOut << new BridgePinOut(this, i, true );
        }
    }

    Object::Open();
    return true;
}

