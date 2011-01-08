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

#ifndef BRIDGEPINVIEW_H
#define BRIDGEPINVIEW_H

#define PINSIZE 12
#define BRPIN_MRG 3

#include "pinview.h"

namespace View {

    class BridgePinView : public PinView
    {
    Q_OBJECT
    public:
        BridgePinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, const ConnectionInfo &pinInfo);
        const QPointF pinPos() const;
        void UpdateModelIndex(const QModelIndex &index);

    protected:
        QAbstractGraphicsShapeItem *vuValue;
        float value;
        PinType::Enum valueType;

    public slots:
        void updateVu();
    };

}
#endif // BRIDGEPINVIEW_H
