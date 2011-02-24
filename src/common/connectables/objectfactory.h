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

#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "../precomp.h"
#include "../globals.h"
#include "object.h"
#include "objectinfo.h"

class MainHost;

namespace Connectables {

    typedef QHash<int,QWeakPointer<Object> > hashObjects;

    class ObjectFactory : public QObject
    {
    Q_OBJECT
    public:
        ObjectFactory(MainHost *myHost);
        ~ObjectFactory();

        QSharedPointer<Object> NewObject(const ObjectInfo &info);
        QSharedPointer<Object> GetObj(const QModelIndex & index);
        void RemoveObject(int id);
        void Clear();

        QSharedPointer<Object> GetObjectFromId(int id);
        int IdFromSavedId(int savedId);
        void ResetSavedId();
        Pin *GetPin(const ConnectionInfo &pinInfo);
        Pin *GetPin(const QModelIndex & index);

        const hashObjects &GetListObjects() {return listObjects;}

    protected:
        virtual Object *CreateOtherObjects(const ObjectInfo &info)=0;

        hashObjects listObjects;
        int cptListObjects;
        MainHost *myHost;

//        QScriptValue scriptObj;
    };

}

#endif // OBJECTFACTORY_H
