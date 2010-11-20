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

#ifndef PARAMETERPIN_H
#define PARAMETERPIN_H

#include "../precomp.h"
#include "pin.h"
#include "objectprogram.h"

namespace Connectables {

    class Object;
    class ParameterPin : public Pin
    {
    Q_OBJECT
    public:
        ParameterPin(Object *parent, PinDirection::Enum direction, int number, float defaultValue, bool defaultVisible, QString name="", bool nameCanChange=false, bool bridge=false);
        ParameterPin(Object *parent, PinDirection::Enum direction, int number, QVariant defaultVariantValue, QList<QVariant> *listValues, bool defaultVisible, QString name="", bool nameCanChange=false, bool bridge=false);

        void ChangeValue(float val);
        void ChangeValue(int index);
        void ChangeValue(QVariant variant);
        void Load(const ObjectParameter *param);

        virtual void OnValueChanged(float val);

        inline int GetIndex() {return stepIndex;}
        inline QVariant GetVariantValue() {return listValues->at(stepIndex);}

        void GetDefault(ObjectParameter &param);
        void GetValues(ObjectParameter &param);

        void SetDefaultValue(float value) {defaultValue = value;}
        void SetDefaultVisible(bool visible) {defaultVisible = visible;}
        void SetFixedName(QString fixedName);
        void SetNameCanChange(bool canChange) {nameCanChange = canChange;}

    protected:
        QList<QVariant> *listValues;
        int stepIndex;
        float defaultValue;
        int defaultIndex;
        bool defaultVisible;
        bool loading;
        bool nameCanChange;
        bool dirty;

    signals:
        void ParameterChanged(ConnectionInfo pinInfo, float value);
        void SetProgDirty();

    public slots:
        void OnStep(int delta);

    };
}

#endif // PARAMETERPIN_H
