/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#ifndef PINSLIST_H
#define PINSLIST_H

#include "precomp.h"
#include "globals.h"
#include "connectioninfo.h"
#include "objectinfo.h"
#include "audiobuffer.h"

namespace Connectables {

    class Pin;
    class Object;

    class PinsList : public QObject
    {
        Q_OBJECT
    public:
        PinsList(MainHost *myHost, Object *parent);
        void SetContainerId(const quint16 id);
        void Hide();
        void SetVisible(bool visible);
        bool ChangeNumberOfPins(const int newNbIn);
        void SetBridge(bool bridge);
        void SetInfo(Object *parent,const ConnectionInfo &connInfo, const ObjectInfo &objInfo);
        Pin * GetPin(int pinNumber, bool autoCreate=false);
        void UpdateModelNode(QStandardItem *parentNode=0);
        void ConnectAllTo(PinsList *other, bool hidden=false);
        void AsyncAddPin(int nb);

        AudioBuffer *GetBuffer(int pinNumber);

        QMap<quint16,Pin*> listPins;
        ConnectionInfo connInfo;
        ObjectInfo objInfo;

        virtual QDataStream & toStream (QDataStream &) const;
        virtual QDataStream & fromStream (QDataStream &);

    protected:
        QPersistentModelIndex modelList;
        Object *parent;
        quint16 containerId;
        MainHost *myHost;

    signals :
        void PinAdded(int nb);

    public slots:
        Pin * AddPin(int nb);
    };
}

QDataStream & operator<< (QDataStream & out, const Connectables::PinsList& value);
QDataStream & operator>> (QDataStream & in, Connectables::PinsList& value);

#endif // PINSLIST_H
