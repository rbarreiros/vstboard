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
QList<VstPlugin*>VstPlugin::listPlugins;

View::VstShellSelect *VstPlugin::shellSelectView=0;

VstPlugin::VstPlugin(int index, const ObjectInfo & info) :
    Object(index, info),
    CEffect(),
    editorWnd(0),
    sampleRate(44100.0),
    bufferSize(1024),
    listEvnts(0),
    isShell(false)
{
    listPlugins << this;
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

//        modelEditor=0;
//        modelLearningMode=0;
        editorWnd->close();
        editorWnd->deleteLater();
        editorWnd=0;
    }

    if(!Object::Close())
        return false;

    listPlugins.removeAll(this);

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

void VstPlugin::SetBufferSize(long size)
{
    if(!pEffect)
        return;

    if(size==bufferSize)
        return;

    bool wasSleeping = GetSleep();
    SetSleep(true);

    debug("VstPlugin::SetBufferSize %d size %ld -> %ld",index,bufferSize,size)
    foreach(AudioPinIn *pin, listAudioPinIn) {
        pin->buffer->SetSize(size);
    }
    foreach(AudioPinOut *pin, listAudioPinOut) {
        pin->buffer->SetSize(size);
    }

    EffSetBlockSize(size);

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
    float **tmpBufOut = new float*[listAudioPinOut.size()];

    int cpt=0;
    foreach(AudioPinOut* pin,listAudioPinOut) {
        tmpBufOut[cpt] = pin->buffer->GetPointer(true);
        cpt++;
    }

    float **tmpBufIn;
    if(listAudioPinIn.size()==0) {
        //no input, use outputs as fake buffers... don't know what we're supposed to do...
        tmpBufIn = tmpBufOut;
    } else {
        tmpBufIn = new float*[listAudioPinIn.size()];

        cpt=0;
        foreach(AudioPinIn* pin,listAudioPinIn) {
            tmpBufIn[cpt] = pin->buffer->ConsumeStack();
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
    foreach(AudioPinOut* pin,listAudioPinOut) {
        pin->buffer->ConsumeStack();
        pin->SendAudioBuffer();
    }



    EffIdle();
}

bool VstPlugin::Open()
{
    closed=false;

    {
        QMutexLocker lock(&objMutex);
        VstPlugin::pluginLoading = this;

        if(!Load( objInfo.filename )) {
            VstPlugin::pluginLoading = 0;
            return false;
        }

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

                VstPlugin::shellSelectView = new View::VstShellSelect();
                VstPlugin::shellSelectView->SetListPlugins(objInfo.name, listPlugins);
                VstPlugin::shellSelectView->show();
                return false;
            } //else {
                //if not, why did he asked for shellCategory capability !?
            //}
        }

        long ver = EffGetVstVersion();

        bufferSize = MainHost::Get()->GetBufferSize();
        sampleRate = MainHost::Get()->GetSampleRate();

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


        for(int i=0;i<pEffect->numInputs;i++) {
            AudioPinIn *pin = new AudioPinIn(this,i);
            pin->buffer->SetSize(bufferSize);
            listAudioPinIn << pin;
        }
        for(int i=0;i<pEffect->numOutputs;i++) {
            AudioPinOut *pin = new AudioPinOut(this,i);
            pin->buffer->SetSize(bufferSize);
            listAudioPinOut << pin;
        }

        if(bWantMidi) {
            MidiPinIn *pinin = new MidiPinIn(this);
            pinin->setObjectName(QString("Midi in"));
            listMidiPinIn << pinin;

            MidiPinOut *pinout = new MidiPinOut(this);
            listMidiPinOut << pinout;
        }
    }

    int nbParam = pEffect->numParams;
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
    for(int i=0;i<nbParam;i++) {
        if(listParameterPinIn.contains(i)) {
            listParameterPinIn.value(i)->SetDefaultVisible(defVisible);
            listParameterPinIn.value(i)->SetNameCanChange(nameCanChange);
        } else {
            listParameterPinIn.insert(i, new ParameterPinIn(this, i, EffGetParameter(i), defVisible, EffGetParamName(i), nameCanChange) );
        }
    }

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

    hasEditor=true;
    canLearn=true;

    editorWnd = new View::VstPluginWindow(MainWindow::theMainWindow);
    connect(this,SIGNAL(CloseEditorWindow()),
            editorWnd,SLOT(hide()),
            Qt::QueuedConnection);

    editorWnd->setAttribute(Qt::WA_ShowWithoutActivating);

    if(!editorWnd->SetPlugin(this)) {
        editorWnd=0;
        OnEditorVisibilityChanged(false);
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

void VstPlugin::OnEditorVisibilityChanged(bool visible)
{
    Object::OnEditorVisibilityChanged(visible);

    if(!editorWnd)
        return;

    if(visible) {
        editorWnd->show();
        editorWnd->raise();
        connect(MainHost::Get()->updateViewTimer,SIGNAL(timeout()),
                this,SLOT(EditIdle()));

//        UpdateEditorNode();
    } else {
        disconnect(MainHost::Get()->updateViewTimer,SIGNAL(timeout()),
                this,SLOT(EditIdle()));
        emit CloseEditorWindow();
//        UpdateEditorNode();
    }
}

void VstPlugin::SetContainerAttribs(const ObjectConatinerAttribs &attr)
{
    Object::SetContainerAttribs(attr);

    if(editorWnd && editorWnd->isVisible()) {
        editorWnd->move(attr.editorPosition);
        editorWnd->resize(attr.editorSize);
    }
}

void VstPlugin::GetContainerAttribs(ObjectConatinerAttribs &attr)
{
    if(editorWnd && editorWnd->isVisible())
        editorWnd->SavePosSize();

    Object::GetContainerAttribs(attr);
}

void VstPlugin::EditorDestroyed()
{
    editorWnd = 0;
//    modelNode->setData(false, UserRoles::editorVisible);
    MainHost::GetModel()->setData(modelIndex, false, UserRoles::editorVisible);
//    editorVisible = false;
}

void VstPlugin::EditIdle()
{
    if(editorWnd && editorWnd->isVisible())
        EffEditIdle();
}

//void VstPlugin::UpdateEditorNode()
//{
////    bool vis=false;
////    if(editorWnd && editorWnd->isVisible())
////        vis=true;

////    if(modelEditor) {
////        modelEditor->setData(vis,UserRoles::value);
////        if(backgroundPic)
////            modelEditor->setData(objectName(), UserRoles::objInfo);
////    }
//    if(modelLearningMode && editorWnd)
//        modelLearningMode->setData(parameterLearning, UserRoles::value);
//}

void VstPlugin::SetParentModeIndex(const QModelIndex &parentIndex)
{
    Object::SetParentModeIndex(parentIndex);

    if(hasEditor) {
        QStandardItem *item = MainHost::GetModel()->itemFromIndex(modelIndex);
        item->setData(editorWnd->isVisible(), UserRoles::editorVisible);
        item->setData(false,UserRoles::paramLearning);
        item->setData(pEffect->uniqueID, UserRoles::editorImage);
    }
}

void VstPlugin::SetParkingIndex(const QModelIndex &parentIndex)
{
    if(editorWnd && editorWnd->isVisible())
        editorWnd->close();
    Object::SetParkingIndex(parentIndex);
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
            if(!listParameterPinIn.contains(index)) {
//                float v=.0f;
//                QString n="";
//                try {
//                    v=EffGetParameter(index);
//                    n=EffGetParamName(index);
//                } catch(...) {
//                    debug("VstPlugin::OnMasterCallback exception")
//                }

                listParameterPinIn.insert(index, new ParameterPinIn(this, index, (float)opt, false, "n/d", true) );
            }
            listParameterPinIn.value(index)->OnValueChanged(opt);
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
            return  1L;
            break;

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
    if(pinInfo.direction == PinDirection::Input) {
        if(EffCanBeAutomated(pinInfo.pinNumber)!=1) {
            debug(QString("vst parameter can't be automated %1").arg(pinInfo.pinNumber).toAscii())
            return;
        }
        EffSetParameter(pinInfo.pinNumber,value);
    }
}

