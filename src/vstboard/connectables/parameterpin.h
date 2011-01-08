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
#include "objectparameter.h"

namespace Connectables {

    class Object;
    class ParameterPin : public Pin
    {
    Q_OBJECT
    public:
        ParameterPin(Object *parent, PinDirection::Enum direction, int number, float defaultValue, bool defaultVisible, const QString &name="", bool nameCanChange=false, bool bridge=false);
        ParameterPin(Object *parent, PinDirection::Enum direction, int number, const QVariant &defaultVariantValue, QList<QVariant> *listValues, bool defaultVisible, const QString &name="", bool nameCanChange=false, bool bridge=false);

        void ChangeValue(float val, bool fromObj=false);
        void ChangeValue(int index, bool fromObj=false);
        void ChangeValue(const QVariant &variant, bool fromObj=false);
        void Load(const ObjectParameter &param);

        virtual void OnValueChanged(float val);

        inline int GetIndex() {return outStepIndex;}
        inline QVariant GetVariantValue() {return listValues->at(outStepIndex);}
        void SetVariantValue(const QVariant &val);

        void GetDefault(ObjectParameter &param);
        void GetValues(ObjectParameter &param);

        void SetDefaultValue(float value) {defaultValue = value;}
        void SetDefaultVisible(bool visible) {defaultVisible = visible;}
        void SetFixedName(QString fixedName);
        void SetNameCanChange(bool canChange) {nameCanChange = canChange;}
        void SetAlwaysVisible(bool visible);
        void SetVisible(bool vis);

        void SetLimit(ObjType::Enum type, float newVal);
        void SetLimitsEnabled(bool enable) {limitsEnabled=enable;}

    protected:
        QList<QVariant> *listValues;
        int stepIndex;
        float defaultValue;
        int defaultIndex;
        bool defaultVisible;
        bool loading;
        bool nameCanChange;
        bool dirty;
        bool visibilityCanChange;
        float limitInMin;
        float limitInMax;
        float limitOutMin;
        float limitOutMax;

        int outStepIndex;
        float outValue;

        QPersistentModelIndex indexLimitInMin;
        QPersistentModelIndex indexLimitInMax;
        QPersistentModelIndex indexLimitOutMin;
        QPersistentModelIndex indexLimitOutMax;

        bool limitsEnabled;

    signals:
        void ParameterChanged(ConnectionInfo pinInfo, float value);
        void SetProgDirty();

    public slots:
        void OnStep(int delta);

    };
}

#endif // PARAMETERPIN_H
