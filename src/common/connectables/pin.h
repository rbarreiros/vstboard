/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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

namespace Connectables {

    class Object;
    class Cable;
    class Pin : public QObject
    {
    Q_OBJECT
    public:
        Pin(Object *parent,PinType::Enum type, PinDirection::Enum direction, int number, bool bridge=false );
        virtual ~Pin();

        virtual void SendMsg(const PinMessage::Enum msgType,void *data=0);

        /*!
         Message received from another pin
         \param msgType PinMessage
         \param data received data
         */
        virtual void ReceiveMsg(const PinMessage::Enum msgType,void *data=0) {
	  Q_UNUSED(msgType);
	  Q_UNUSED(data);
	}

        /*!
          Get the current value (vu meter or parameter value)
          \return current value
          */
        virtual float GetValue() {return value;}

        /*!
          get the pin infos
          \return ConnectionInfo
          */
        const ConnectionInfo &GetConnectionInfo() const {return connectInfo;}

        virtual void SetParentModelIndex(const QModelIndex &newParent);
        void SetContainerId(quint16 id);

        /// \return true if visible
        inline bool GetVisible() {return visible;}
        virtual void SetVisible(bool vis);
        void SetBridge(bool bridge);
        void UpdateModelNode();
        void Close();

        /*!
          Prepare for a new rendering
          Called one time at the beginning of the loop
           */
        virtual void NewRenderLoop() {}

        void AddCable(const QWeakPointer<Cable>&c) {cablesMutex.lock(); listCables << c; cablesMutex.unlock();}
        void RemoveCable(const QWeakPointer<Cable>&c) {cablesMutex.lock(); listCables.removeAll(c); cablesMutex.unlock();}

    protected:
        QMutex cablesMutex;
        QList<QWeakPointer<Cable> >listCables;

        /// ConnectionInfo og the pin
        ConnectionInfo connectInfo;

        /// current value
        float value;

        /// size of each steps (=1/nbvalues)
        float stepSize;

        /// pointer to the parent Object
        Object *parent;

        /// is displayed or not
        bool visible;

        /// index of the parent in the model
        QPersistentModelIndex parentIndex;

        /// index of this pin in the model
        QPersistentModelIndex modelIndex;

        /// global pin mutex
        QMutex objMutex;

        /// true if closed or closing (no send, no receive)
        bool closed;

        /// text to displaye
        QString displayedText;

        /// true if the value changes since the laste view update
        bool valueChanged;

    public slots:
        virtual void updateView();

    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // CONNECTABLEPIN_H
