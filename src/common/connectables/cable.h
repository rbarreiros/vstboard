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

#ifndef CABLE_H
#define CABLE_H

#include "connectioninfo.h"

class MainHost;
namespace Connectables {

    class Cable
    {
    public:
        Cable(MainHost *myHost,const ConnectionInfo &pinOut, const ConnectionInfo &pinIn);
        Cable(const Cable & c);
        void AddToParentNode(const QModelIndex &parentIndex);
        void RemoveFromParentNode(const QModelIndex &parentIndex);

        /*!
          Get the output pin info
          \return a ConnectionInfo
          */
        inline const ConnectionInfo & GetInfoOut() const {return pinOut;}

        /*!
          Get the input pin info
          \return a ConnectionInfo
          */
        inline const ConnectionInfo & GetInfoIn() const {return pinIn;}

    protected:
        /// the output pin (the one sending messages)
        const ConnectionInfo pinOut;

        /// the input pin (the receiver)
        const ConnectionInfo pinIn;

        /// the cable index in the model
        QPersistentModelIndex modelIndex;

        /// pointer to the MainHost
        MainHost *myHost;
    };
}

#endif // CABLE_H
