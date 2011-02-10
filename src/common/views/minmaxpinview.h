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

#ifndef MINMAXPINVIEW_H
#define MINMAXPINVIEW_H

#include "connectablepinview.h"
#include "cursorview.h"

namespace View {
    class MinMaxPinView : public ConnectablePinView
    {

    public:
        MinMaxPinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, const ConnectionInfo &pinInfo);
        void SetLimitModelIndex(ObjType::Enum type, QPersistentModelIndex index);
        void UpdateLimitModelIndex(const QModelIndex &index);
        void UpdateModelIndex(const QModelIndex &index);

    protected:
        void CreateCursors();
        void UpdateScaleView();
        CursorView *inMin;
        CursorView *inMax;
        CursorView *outMin;
        CursorView *outMax;
        QGraphicsPolygonItem *scaledView;
        bool cursorCreated;

    };
}
#endif // MINMAXPINVIEW_H
