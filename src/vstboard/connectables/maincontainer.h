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

#ifndef MAINCONTAINER_H
#define MAINCONTAINER_H

#include "container.h"

namespace Connectables {

    class MainContainer : public Container
    {
    public:
        MainContainer(MainHost *myHost,int index, const ObjectInfo &info);
        virtual ~MainContainer();

        bool Close();
        void AddObject(QSharedPointer<Object> objPtr);
        void ParkObject(QSharedPointer<Object> objPtr);

        QSharedPointer<Object> bridgeSend;
        QSharedPointer<Object> bridgeReturn;

    protected:

    };
}
#endif // MAINCONTAINER_H
