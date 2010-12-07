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

#include "parameterpin.h"
#include "object.h"
#include "../globals.h"

using namespace Connectables;

//parameter is a float
ParameterPin::ParameterPin(Object *parent, PinDirection::Enum direction, int number, float defaultValue, bool defaultVisible, const QString &name, bool nameCanChange, bool bridge) :
        Pin(parent,PinType::Parameter,direction,number,bridge),
        listValues(0),
        stepIndex(0),
        defaultValue(defaultValue),
        defaultIndex(0),
        defaultVisible(defaultVisible),
        loading(false),
        nameCanChange(nameCanChange),
        dirty(false),
        visibilityCanChange(true)
{
    SetVisible(false);

    value = defaultValue;
    falloff = 0.0f;
    setObjectName(name);

    loading=true;
    OnValueChanged( defaultValue );
    loading=false;
}

//parameter is a int with a list of possible values
ParameterPin::ParameterPin(Object *parent, PinDirection::Enum direction, int number, const QVariant &defaultVariantValue, QList<QVariant> *listValues, bool defaultVisible, const QString &name, bool nameCanChange, bool bridge) :
        Pin(parent,PinType::Parameter,direction,number,bridge),
        listValues(listValues),
        defaultValue( .0f ),
        defaultVisible(defaultVisible),
        loading(false),
        nameCanChange(nameCanChange),
        dirty(false),
        visibilityCanChange(true)
{
    SetVisible(false);
    falloff = 0.0f;
    setObjectName(name);

    loading=true;
    stepSize=1.0f/(listValues->size()-1);
    stepIndex=listValues->indexOf(defaultVariantValue);
    defaultIndex=stepIndex;
    OnValueChanged( stepIndex*stepSize );
    loading=false;
}

void ParameterPin::SetAlwaysVisible(bool /*visible*/)
{
    defaultVisible=true;
    visibilityCanChange=false;
}

void ParameterPin::GetDefault(ObjectParameter &param)
{
    param.index=defaultIndex;
    param.value=defaultValue;
    param.visible=defaultVisible;
}

void ParameterPin::GetValues(ObjectParameter &param)
{
    param.index=stepIndex;
    param.value=value;
    param.visible=visible;
}

//from float
void ParameterPin::ChangeValue(float val)
{
    if(listValues) {
        //we got a float, but we need a int
        ChangeValue((int)( 0.5f + val/stepSize ));
        return;
    }

    val = std::min(val,1.0f);
    val = std::max(val,0.0f);

    if(val==value)
        return;

    OnValueChanged(val);

    parent->OnParameterChanged(connectInfo,value);
}

//from int
void ParameterPin::ChangeValue(int index)
{
    index = std::min(index,listValues->size()-1);
    index = std::max(index,0);

    if(index==stepIndex)
        return;

    stepIndex=index;
    OnValueChanged(stepSize*index);

    parent->OnParameterChanged(connectInfo,value);
}

//from variant
void ParameterPin::ChangeValue(QVariant variant)
{
    ChangeValue(listValues->indexOf(variant));
}

void ParameterPin::OnStep(int delta)
{
    if(listValues) {
        ChangeValue(stepIndex + delta);
    } else {
        ChangeValue(value + delta*stepSize);
    }

}

//load from program
void ParameterPin::Load(const ObjectParameter &param)
{
    loading = true;

    if(listValues)
        ChangeValue(param.index);
    else
        ChangeValue(param.value);

    SetVisible(param.visible);

    dirty = false;
    loading = false;
}

void ParameterPin::OnValueChanged(float val)
{
    if(value!=val)
        valueChanged=true;

    value=val;

    if(!loading) {

        if(visibilityCanChange) {
            //parent is learning
            if(parent->GetLearningMode()==1)
                SetVisible(true);

            //parent is unlearning
            if(parent->GetLearningMode()==2)
                SetVisible(false);
        }

        if(!dirty) {
            dirty=true;
            parent->OnProgramDirty();
        }
    }

    if(visible) {
        if(nameCanChange)
            setObjectName(parent->GetParameterName(connectInfo));

        if(listValues) {
            //SetDisplayedText( QString("%1:%2").arg(objectName()).arg(listValues->at(stepIndex).toString()) );
            displayedText = QString("%1:%2").arg(objectName()).arg(listValues->at(stepIndex).toString());
        }
    }
}

void ParameterPin::SetFixedName(QString fixedName)
{
    setObjectName(fixedName);
    nameCanChange=false;
}
