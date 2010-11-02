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

#include "loader.h"
#include "object.h"

using namespace Connectables;

Loader::Loader(QSharedPointer<Object> objToLoad, QObject *parent) :
    QThread(parent),
    objToLoad(objToLoad)
{
    start();
    objToLoad->moveToThread(this);
}

Loader::~Loader() {
    exit();
    objToLoad->moveToThread(QApplication::instance()->thread());
    wait();
}

void Loader::run() {
    if(objToLoad.isNull())
        return;

    objToLoad->Open();

    exec();
}
