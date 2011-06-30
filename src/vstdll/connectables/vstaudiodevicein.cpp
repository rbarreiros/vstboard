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

#include "vstaudiodevicein.h"
#include "globals.h"
#include "audiobuffer.h"
#include "mainhostvst.h"
#include "vst.h"

using namespace Connectables;

VstAudioDeviceIn::VstAudioDeviceIn(MainHost *myHost, int index, const ObjectInfo &info) :
    Object(myHost,index, info)
{
}

VstAudioDeviceIn::~VstAudioDeviceIn()
{
    Close();
}

bool VstAudioDeviceIn::Close()
{
    static_cast<MainHostVst*>(myHost)->myVstPlugin->removeAudioIn(this);
    if(!Object::Close())
        return false;
    return true;
}

void VstAudioDeviceIn::Render()
{
    foreach(Pin* pin,listAudioPinOut->listPins) {
        static_cast<AudioPin*>(pin)->GetBuffer()->ConsumeStack();
        static_cast<AudioPin*>(pin)->SendAudioBuffer();
    }
}

void VstAudioDeviceIn::SetBufferSize(unsigned long size)
{
    foreach(Pin *pin, listAudioPinOut->listPins) {
        static_cast<AudioPin*>(pin)->GetBuffer()->SetSize(size);
    }
}

bool VstAudioDeviceIn::Open()
{
    if(!static_cast<MainHostVst*>(myHost)->myVstPlugin->addAudioIn(this))
        return false;

    listAudioPinOut->ChangeNumberOfPins(2);
    SetBufferSize(myHost->GetBufferSize());
    Object::Open();
    return true;
}

void VstAudioDeviceIn::SetBuffers(float **buf, int &cpt, int sampleFrames)
{
    foreach(Pin *pin, listAudioPinOut->listPins) {
        AudioBuffer *abuf= static_cast<AudioPin*>(pin)->GetBuffer();
        abuf->SetBufferContent(buf[cpt],sampleFrames);
        cpt++;
    }
}

void VstAudioDeviceIn::SetBuffersD(double **buf, int &cpt, int sampleFrames)
{
    foreach(Pin *pin, listAudioPinOut->listPins) {
        AudioBuffer *abuf= static_cast<AudioPin*>(pin)->GetBuffer();
        abuf->SetBufferContent(buf[cpt],sampleFrames);
        cpt++;
    }
}

QStandardItem *VstAudioDeviceIn::GetFullItem()
{
    QStandardItem *modelNode = Object::GetFullItem();
    modelNode->setData(doublePrecision, UserRoles::isDoublePrecision);
    return modelNode;
}
