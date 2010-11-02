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

        virtual void ReceiveMsg(const int msgType=0,void *data=0) {}
        virtual float GetValue() {return value;}

        void setObjectName(const QString &name);

//        void SetName(const QString &newName);
//        QString &GetName();

        void SetDisplayedText(const QString &txt);
        QString GetDisplayedText();

        Object *GetParentObj() {return parent;}

        const ConnectionInfo &GetConnectionInfo() const {return connectInfo;}
//        void SetBridge(bool bridge);
        bool event(QEvent *event);

        void SetParentModelNode(QStandardItem *parent_Node);
//        QList<QStandardItem*> GetModelNode() {return modelNode;}

        inline bool GetVisible() {return visible;}
        void SetVisible(bool vis);
        void Close();

    protected:
        ConnectionInfo connectInfo;
        float value;
        float falloff;
        float stepSize;
        Object *parent;
        bool visible;
        QStandardItem *parentNode;
        QStandardItem *modelPin;

        QMutex objMutex;
        bool closed;

    private:
        QMutex txtMutex;
//        QString name;
        QString displayedText;

    public slots:
        virtual void updateView();
        void SendMsg(int msgType,void *data=0);

    signals:
    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // CONNECTABLEPIN_H
