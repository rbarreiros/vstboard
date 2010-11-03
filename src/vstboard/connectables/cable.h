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

#ifndef CABLE_H
#define CABLE_H

#include "connectioninfo.h"

namespace Connectables {

    class Cable
    {
    public:
        Cable(const ConnectionInfo &pinOut, const ConnectionInfo &pinIn);
        Cable(const Cable & c);
        void AddToParentNode(QStandardItem *item);
        void RemoveFromParentNode(QStandardItem *item);
        inline const ConnectionInfo & GetInfoOut() const {return pinOut;}
        inline const ConnectionInfo & GetInfoIn() const {return pinIn;}

    protected:
        const ConnectionInfo pinOut;
        const ConnectionInfo pinIn;
        QStandardItem *modelItem;
    };
}

#endif // CABLE_H
