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

#ifndef CONNECTABLEPIN_H
#define CONNECTABLEPIN_H

#include "../precomp.h"
#include "../globals.h"
#include "connectioninfo.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4100 )
#endif

class PinMessageEvent : public QEvent
{
public:
    PinMessageEvent(PinMessage::Enum msgType, void* data=0) : QEvent(Event::PinMessage),msgType(msgType),data(data) {}
    PinMessage::Enum msgType;
    void* data;
};

namespace Connectables {

    class Object;
    class Pin : public QObject
    {
    Q_OBJECT
    public:
        Pin(Object *parent,PinType::Enum type, PinDirection::Enum direction, int number, bool bridge=false );
        virtual ~Pin();

        virtual void SendMsg(const PinMessage::Enum msgType,void *data=0);
        virtual void ReceiveMsg(const PinMessage::Enum msgType,void *data=0) {}
        virtual float GetValue() {return value;}

        void setObjectName(const QString &name);

//        void SetDisplayedText(const QString &txt);
//        QString GetDisplayedText();

        Object *GetParentObj() {return parent;}

        const ConnectionInfo &GetConnectionInfo() const {return connectInfo;}
        bool event(QEvent *event);

        virtual void SetParentModelIndex(const QModelIndex &newParent);
        void SetContainerId(quint16 id);

        inline bool GetVisible() {return visible;}
        virtual void SetVisible(bool vis);
        void SetBridge(bool bridge);
        void UpdateModelNode();
        void Close();

    protected:
        ConnectionInfo connectInfo;
        float value;
        float stepSize;
        Object *parent;
        bool visible;
        QPersistentModelIndex parentIndex;
        QPersistentModelIndex modelIndex;

        QMutex objMutex;
        bool closed;
        QString displayedText;
        bool valueChanged;

    private:
//        QMutex txtMutex;


    public slots:
        virtual void updateView();


    signals:
    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // CONNECTABLEPIN_H
