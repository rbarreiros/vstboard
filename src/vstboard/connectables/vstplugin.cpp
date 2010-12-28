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

#include "vstplugin.h"
#include "../globals.h"
#include "../audiobuffer.h"
#include "../mainwindow.h"
#include "../vst/cvsthost.h"
#include "../views/vstpluginwindow.h"
#include "imagecollection.h"

using namespace Connectables;

VstPlugin *VstPlugin::pluginLoading = 0;
QMap<AEffect*,VstPlugin*>VstPlugin::mapPlugins;
View::VstShellSelect *VstPlugin::shellSelectView=0;

VstPlugin::VstPlugin(MainHost *myHost,int index, const ObjectInfo & info) :
    Object(myHost,index, info),
    CEffect(),
    editorWnd(0),
    sampleRate(44100.0),
    bufferSize(1024),
    listEvnts(0),
    isShell(false)
{
    for(int i=0;i<128;i++) {
        listValues << i;
    }
    listParameterPinIn->AddPin(FixedPinNumber::vstProgNumber);
}

VstPlugin::~VstPlugin()
{

    Close();
}

bool VstPlugin::Close()
{
    if(editorWnd) {
        objMutex.lock();
        EffEditClose();
        objMutex.unlock();
        editorWnd->close();
        editorWnd->deleteLater();
        editorWnd=0;
    }
    mapPlugins.remove(pEffect);

    if(listEvnts) {
        free(listEvnts);
        listEvnts = 0;
    }
    return true;
}

void VstPlugin::SetSleep(bool sleeping)
{
    Lock();

    if(sleeping) {
        EffStopProcess();
        EffSuspend();
    } else {
        EffResume();
        EffStartProcess();
    }

    Unlock();

    Object::SetSleep(sleeping);
}

void VstPlugin::SetBufferSize(unsigned long size)
{
    if(!pEffect)
        return;

    if(size==bufferSize)
        return;

    bool wasSleeping = GetSleep();
    SetSleep(true);

    debug("VstPlugin::SetBufferSize %d size %ld -> %ld",index,bufferSize,size)
    foreach(Pin *pin, listAudioPinIn->listPins) {
        static_cast<AudioPinIn*>(pin)->buffer->SetSize(size);
    }
    foreach(Pin *pin, listAudioPinOut->listPins) {
        static_cast<AudioPinOut*>(pin)->buffer->SetSize(size);
    }

    EffSetBlockSize((long)size);

    bufferSize = size;

    SetSleep(wasSleeping);
}

void VstPlugin::SetSampleRate(float rate)
{
    sampleRate = rate;

    if(!pEffect)
        return;

    bool wasSleeping = GetSleep();
    SetSleep(true);

    EffSetSampleRate(rate);

    SetSleep(wasSleeping);
}

void VstPlugin::Render()
{
    if(closed)
        return;

    QMutexLocker lock(&objMutex);

    //midi events
    //=========================
    if(bWantMidi) {
        midiEventsMutex.lock();

        if(listVstMidiEvents.size()!=0) {
            int nbEvents = listVstMidiEvents.size();

            if(listEvnts) {
                free(listEvnts);
                listEvnts = 0;
            }

            listEvnts = (VstEvents*)malloc(sizeof(VstEvents) + sizeof(VstEvents*)*(nbEvents-2));
            listEvnts->numEvents = nbEvents;

            int cpt=0;
            foreach(VstMidiEvent *evnt, listVstMidiEvents) {
                listEvnts->events[cpt] = (VstEvent*)evnt;
                cpt++;
            }

            EffProcessEvents(listEvnts);
        }
    }

    //audio buffers
    //=========================
    float **tmpBufOut = new float*[listAudioPinOut->listPins.size()];

    int cpt=0;
    foreach(Pin* pin,listAudioPinOut->listPins) {
        tmpBufOut[cpt] = static_cast<AudioPinOut*>(pin)->buffer->GetPointer(true);
        cpt++;
    }

    float **tmpBufIn;
    if(listAudioPinIn->listPins.size()==0) {
        //no input, use outputs as fake buffers... don't know what we're supposed to do...
        tmpBufIn = tmpBufOut;
    } else {
        tmpBufIn = new float*[listAudioPinIn->listPins.size()];

        cpt=0;
        foreach(Pin* pin,listAudioPinIn->listPins) {
            tmpBufIn[cpt] = static_cast<AudioPinOut*>(pin)->buffer->ConsumeStack();
            cpt++;
        }
    }

    //process
    //=========================

    if (pEffect->flags & effFlagsCanReplacing) {
        EffProcessReplacing(tmpBufIn, tmpBufOut, bufferSize);
    } else {
        EffProcess(tmpBufIn, tmpBufOut, bufferSize);
    }

    //clear midi events
    //=========================
    if (bWantMidi) {
        listVstMidiEvents.clear();
        midiEventsMutex.unlock();
    }


    //clear buffers
    //=========================
    if(tmpBufOut!=tmpBufIn)
        delete[] tmpBufIn;

    delete[] tmpBufOut;


    //send result
    //=========================
    foreach(Pin* pin,listAudioPinOut->listPins) {
        static_cast<AudioPinOut*>(pin)->buffer->ConsumeStack();
        static_cast<AudioPinOut*>(pin)->SendAudioBuffer();
    }



    EffIdle();
}

bool VstPlugin::Open()
{
    {
        QMutexLocker lock(&objMutex);
        VstPlugin::pluginLoading = this;

        if(!Load(myHost, objInfo.filename )) {
            VstPlugin::pluginLoading = 0;
            errorMessage=tr("Error while loading plugin");
            return true;
        }

        closed=false;
        mapPlugins.insert(pEffect, this);
        VstPlugin::pluginLoading = 0;

        //a shell plugin should have asked for shellCategory capability before we get there (or else what are they waiting for ?)
        if(isShell && objInfo.id==0) {
            if (EffGetPlugCategory() == kPlugCategShell ) {
                QMap<ulong,QString> listPlugins;
                char szName[1024];
                ulong id;
                while ((id = EffGetNextShellPlugin(szName))) {
                    listPlugins.insert(id,QString::fromAscii(szName));
                }

                if(VstPlugin::shellSelectView) {
                    debug("VstPlugin::Open shell selection already opened")
                    return false;
                }

                VstPlugin::shellSelectView = new View::VstShellSelect(myHost->objFactory);
                VstPlugin::shellSelectView->SetListPlugins(objInfo.name, listPlugins);
                VstPlugin::shellSelectView->show();

                //this is a shell, return false to delete this object
                return false;
            } //else {
                //if not, why did he asked for shellCategory capability !?
            //}
        }

        long ver = EffGetVstVersion();

        bufferSize = myHost->GetBufferSize();
        sampleRate = myHost->GetSampleRate();

        EffSetSampleRate(sampleRate);
        EffSetBlockSize(bufferSize);
        EffOpen();
        EffSetSampleRate(sampleRate);
        EffSetBlockSize(bufferSize);

        //long canSndMidiEvnt = pEffect->EffCanDo("sendVstMidiEvent");
        bWantMidi = (EffCanDo("receiveVstMidiEvent") == 1);

     //   long midiPrgNames = EffCanDo("midiProgramNames");
        VstPinProperties pinProp;
        EffGetInputProperties(0,&pinProp);

        //stereo input
        bool stereoIn = false;
        if(pinProp.flags & kVstPinIsStereo)
                stereoIn = true;
        //speaker arrangement
        //pinProp->arrangementType

        EffGetOutputProperties(0,&pinProp);
        //stereo output
        bool stereoOut = false;
        if(pinProp.flags & kVstPinIsStereo)
                stereoOut = true;

        //speaker arrangement
        //pinProp->arrangementType

        EffResume();
        EffSuspend();

        if(stereoIn)
        {
                EffGetInputProperties(0,&pinProp);
                EffGetInputProperties(1,&pinProp);
        }
        if(stereoOut)
        {
                EffGetOutputProperties(0,&pinProp);
                EffGetOutputProperties(1,&pinProp);
        }

        if(bWantMidi)
        {
                EffGetNumMidiInputChannels();
                EffGetNumMidiOutputChannels();
        }

        if(ver>=2000 && ver<2400)
        {
                EffConnectInput(0,1);
                if(stereoIn)
                        EffConnectInput(1,1);

                EffConnectOutput(0,1);
                if(stereoOut)
                        EffConnectOutput(1,1);
        }

        EffSetProgram(0);

        char szBuf[256] = "";
        if ((EffGetProductString(szBuf)) && (*szBuf)) {
            setObjectName( QString("%1%2").arg(szBuf).arg(index) );
        } else {
            sName = sName.section("/",-1);
            sName = sName.section(".",0,-2);
            setObjectName( sName % QString::number(index) );
        }
        objInfo.name=objectName();

        if(bWantMidi) {
            listMidiPinIn->AddPin(0);
            listMidiPinOut->AddPin(0);
        }
    }

    listAudioPinIn->ChangeNumberOfPins(pEffect->numInputs);
    listAudioPinOut->ChangeNumberOfPins(pEffect->numOutputs);

    //int nbParam = pEffect->numParams;
    bool nameCanChange=false;
    bool defVisible = true;

    //plugin with gui :
    if((pEffect->flags & effFlagsHasEditor) != 0) {
        //plugin may change the parameter name
        nameCanChange=true;

        //only show learned parameters
        defVisible=false;
    }

    //create all parameters pins
  /*  for(int i=0;i<nbParam;i++) {
        if(listParameterPinIn->listPins.contains(i)) {
            ParameterPinIn *pin = static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(i));
            pin->SetDefaultVisible(defVisible);
            pin->SetNameCanChange(nameCanChange);
        } else {
            listParameterPinIn->AddPin(i);
        }
    }
*/
    Object::Open();
    CreateEditorWindow();
    return true;
}

void VstPlugin::RaiseEditor()
{
    if(!editorWnd || !editorWnd->isVisible())
        return;

    editorWnd->raise();
}

//void VstPlugin::TakeScreenshot()
//{
//    const QPixmap backgroundPic = editorWnd->GetScreenshot();
//    ImageCollection::Get()->AddImage( QString::number( pEffect->uniqueID ), backgroundPic );
//    modelNode->setData( QString::number( pEffect->uniqueID ), UserRoles::editorImage );
//}

void VstPlugin::CreateEditorWindow()
{
    //already done
    if(editorWnd)
        return;

    //no gui
    if((pEffect->flags & effFlagsHasEditor) == 0)
        return;

    static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::editorVisible))->SetAlwaysVisible(true);
    static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::learningMode))->SetAlwaysVisible(true);

    editorWnd = new View::VstPluginWindow(myHost->mainWindow);
    connect(this,SIGNAL(CloseEditorWindow()),
            editorWnd,SLOT(hide()),
            Qt::QueuedConnection);

    editorWnd->setAttribute(Qt::WA_ShowWithoutActivating);

    if(!editorWnd->SetPlugin(this)) {
        editorWnd=0;
        OnHideEditor();
        return;
    }

    //no screenshot in db, create one
//    if(!ImageCollection::Get()->ImageExists(QString::number( pEffect->uniqueID ))) {
//        OnEditorVisibilityChanged(true);
//        connect(MainHost::Get()->updateViewTimer,SIGNAL(timeout()),
//                this,SLOT(EditIdle()));
//        QTimer::singleShot(100,this,SLOT(TakeScreenshot()));
//    }
}

void VstPlugin::OnShowEditor()
{
    if(!editorWnd || editorWnd->isVisible())
        return;

    editorWnd->show();
//    editorWnd->raise();
    connect(myHost->updateViewTimer,SIGNAL(timeout()),
            this,SLOT(EditIdle()));
}

void VstPlugin::OnHideEditor()
{
    if(!editorWnd || !editorWnd->isVisible())
        return;

    disconnect(myHost->updateViewTimer,SIGNAL(timeout()),
            this,SLOT(EditIdle()));
    emit CloseEditorWindow();
}

void VstPlugin::SetContainerAttribs(const ObjectContainerAttribs &attr)
{
    Object::SetContainerAttribs(attr);

    if(editorWnd) {
        if(attr.editorVisible != editorWnd->isVisible()) {
            editorWnd->setVisible(attr.editorVisible);
        }
        if(attr.editorVisible)
            editorWnd->LoadAttribs();
//        editorWnd->move(attr.editorPosition);
//        editorWnd->resize(attr.editorSize);
//        editorWnd->SetScrollValue(attr.editorHScroll,attr.editorVScroll);
    }
}

void VstPlugin::GetContainerAttribs(ObjectContainerAttribs &attr)
{
    if(editorWnd && editorWnd->isVisible())
        editorWnd->SaveAttribs();

    Object::GetContainerAttribs(attr);
    attr.editorVisible=editorWnd->isVisible();
}

void VstPlugin::EditorDestroyed()
{
    editorWnd = 0;
//    MainHost::GetModel()->setData(modelIndex, false, UserRoles::editorVisible);
    listParameterPinIn->listPins.value(FixedPinNumber::editorVisible)->SetVisible(false);
}

void VstPlugin::EditIdle()
{
    if(editorWnd && editorWnd->isVisible())
        EffEditIdle();
}

QString VstPlugin::GetParameterName(ConnectionInfo pinInfo)
{
    if(pinInfo.pinNumber < pEffect->numParams)
        return EffGetParamName( pinInfo.pinNumber );
    else
        debug("VstPlugin::GetParameterName : parameter id out of range")

    return "";
}

void VstPlugin::MidiMsgFromInput(long msg)
{

    VstMidiEvent *evnt = new VstMidiEvent;
    memset(evnt, 0, sizeof(VstMidiEvent));
    evnt->type = kVstMidiType;
    evnt->byteSize = sizeof(VstMidiEvent);
    memcpy(evnt->midiData, &msg, sizeof(evnt->midiData));
    evnt->flags = kVstMidiEventIsRealtime;

    midiEventsMutex.lock();
    listVstMidiEvents << evnt;
    midiEventsMutex.unlock();
}

long VstPlugin::OnMasterCallback(long opcode, long index, long value, void *ptr, float opt, long currentReturnCode)
{
    switch(opcode) {
        case audioMasterAutomate : //0
            //create the parameter pin if needed
            switch(GetLearningMode()) {
                case LearningMode::unlearn :
                    listParameterPinIn->AsyncRemovePin(index);
                    break;
                case LearningMode::learn :
                    listParameterPinIn->AsyncAddPin(index);
                case LearningMode::off :
                    if(listParameterPinIn->listPins.contains(index))
                        static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(index))->ChangeValue(opt);
                    break;
            }


            break;

        case audioMasterCurrentId : //2
            isShell=true;
            return objInfo.id;
            break;

        case audioMasterIdle : //3
            QApplication::processEvents();
            break;

        case audioMasterGetNumAutomatableParameters : //11
            return 128L;
            break;

        case audioMasterIOChanged : //13
            if(!pEffect)
                return 0L;
            listAudioPinIn->ChangeNumberOfPins(pEffect->numInputs);
            listAudioPinOut->ChangeNumberOfPins(pEffect->numOutputs);
            UpdateModelNode();
            return  1L;

        case audioMasterSizeWindow : //15
            emit WindowSizeChange((int)index,(int)value);
            break;

        case audioMasterCanDo : //37
            if(!strcmp((const char*)ptr, "shellCategory")) {
                isShell=true;
                return  1L;
            }
            break;

//        case audioMasterUpdateDisplay : //42
//            break;
    }

    return CEffect::OnMasterCallback(opcode, index, value, ptr, opt, currentReturnCode);
}

void VstPlugin::OnParameterChanged(ConnectionInfo pinInfo, float value)
{
    Object::OnParameterChanged(pinInfo,value);

    if(pinInfo.direction == PinDirection::Input) {
        if(pinInfo.pinNumber==FixedPinNumber::vstProgNumber) {
            //program pin
            EffSetProgram( static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::vstProgNumber))->GetIndex() );
            return;
        }

        if(pinInfo.pinNumber<200) {
            if(GetLearningMode() == LearningMode::unlearn) {
                listParameterPinIn->AsyncRemovePin(pinInfo.pinNumber);
                return;
            }
            if(EffCanBeAutomated(pinInfo.pinNumber)!=1) {
                debug2(<< "vst parameter can't be automated " << pinInfo.pinNumber)
                return;
            }
            EffSetParameter(pinInfo.pinNumber,value);
        }
    }
}

Pin* VstPlugin::CreatePin(const ConnectionInfo &info)
{
    Pin *newPin = Object::CreatePin(info);
    if(newPin)
        return newPin;

    switch(info.direction) {
        case PinDirection::Input :
            if(info.pinNumber==FixedPinNumber::vstProgNumber) {
                ParameterPinIn *pin = new ParameterPinIn(this,info.pinNumber,0,&listValues,true,"prog",false);
                pin->SetLimitsEnabled(false);
                return pin;
            } else {
                ParameterPinIn *pin = new ParameterPinIn(this,info.pinNumber,EffGetParameter(info.pinNumber),true,EffGetParamName(info.pinNumber),true);
                return pin;
            }
            break;

        default :
            debug("VstPlugin::CreatePin PinDirection")
            return 0;
            break;
    }

    return 0;
}
