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

#ifndef CONNECTABLEPINVIEW_H
#define CONNECTABLEPINVIEW_H

#include "pinview.h"
//#include "outlinedtext.h"

namespace View {

    class ConnectablePinView : public PinView
    {
    Q_OBJECT
    public:
        ConnectablePinView(float angle, QAbstractItemModel *model,QGraphicsItem * parent, Connectables::Pin *pin);
        virtual void UpdateModelIndex(const QModelIndex &index);

    protected:
        void wheelEvent ( QGraphicsSceneWheelEvent * event );
        QGraphicsSimpleTextItem *textItem;
        QGraphicsRectItem *rectVu;
        QString text;
        float value;
        bool isParameter;

    public slots:
        void updateVu();
    };

}
#endif // CONNECTABLEPINVIEW_H
