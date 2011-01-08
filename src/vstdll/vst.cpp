/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#include "vst.h"
#include "connectables/connectioninfo.h"
#include "connectables/objectinfo.h"
#include "mainhost.h"
#include "audioeffectx.h"
#include "projectfile/setupfile.h"
#include "projectfile/projectfile.h"
#include "views/configdialog.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new Vst (audioMaster);
}

Vst::Vst (audioMasterCallback audioMaster) :
    AudioEffectX (audioMaster, 1, 128),
    myApp(0),
    myHost(0),
    myWindow(0),
    bufferSize(0),
    listEvnts(0),
    hostSendVstEvents(false),
    hostSendVstMidiEvent(false),
    hostReportConnectionChanges(false),
    hostAcceptIOChanges(false),
    hostSendVstTimeInfo(false),
    hostReceiveVstEvents(false),
    hostReceiveVstMidiEvents(false),
    hostReceiveVstTimeInfo(false),
    opened(false)
{
    setNumInputs (DEFAULT_INPUTS*2);
    setNumOutputs (DEFAULT_OUTPUTS*2);
    setUniqueID (kUniqueID);
    canProcessReplacing(true);
    vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

    qRegisterMetaType<ConnectionInfo>("ConnectionInfo");
    qRegisterMetaType<ObjectInfo>("ObjectInfo");
    qRegisterMetaType<int>("ObjType::Enum");
    qRegisterMetaType<orderedNodes>("orderedNodes");
    qRegisterMetaType<QVariant>("QVariant");

    qRegisterMetaTypeStreamOperators<ObjectInfo>("ObjectInfo");

    QCoreApplication::setOrganizationName("CtrlBrk");
    QCoreApplication::setApplicationName(APP_NAME);

    myHost = new MainHost(this);
    myWindow = new MainWindow(myHost);
    qEditor = new Gui(this);
    setEditor(qEditor);
    qEditor->SetMainWindow(myWindow);
}

Vst::~Vst ()
{
    if(myHost) {
        delete myHost;
        myHost=0;
    }
}


void Vst::open()
{
//     char str[64];
//     getHostVendorString(str);
//     getHostProductString(str);
//     VstInt32 hostVer = getHostVendorVersion();

     hostSendVstEvents = (bool)canHostDo("sendVstEvents");
     hostSendVstMidiEvent = (bool)canHostDo("sendVstMidiEvent");
     hostSendVstTimeInfo = (bool)canHostDo((char *)"sendVstTimeInfo");
     hostReceiveVstEvents = (bool)canHostDo("receiveVstEvents");
     hostReceiveVstMidiEvents = (bool)canHostDo("receiveVstMidiEvents");
     hostReceiveVstTimeInfo = (bool)canHostDo("receiveVstTimeInfo");
     hostReportConnectionChanges = (bool)canHostDo("reportConnectionChanges");
     hostAcceptIOChanges = (bool)canHostDo((char *)"acceptIOChanges");

//     long hostSizeWindow = canHostDo("sizeWindow");
//     long hostAsyncProcessing = canHostDo("asyncProcessing");
//     long hostOffline = canHostDo("offline");
//     long hostSupplyIdle = canHostDo("supplyIdle");
//     long hostSupportShell = canHostDo("supportShell");
//     long hostOpenFileSelector = canHostDo("openFileSelector");
//     long hostEditFile = canHostDo("editFile");
//     long hostCloseFileSelector = canHostDo("closeFileSelector");

     if(hostReceiveVstTimeInfo) {
        connect(myHost,SIGNAL(TempoChanged()),
             this,SLOT(OnMainHostTempoChange()));
    }

     myHost->Open();

     //load default setup file
     QString currentSetupFile = ConfigDialog::defaultSetupFile();
     if(!currentSetupFile.isEmpty()) {
         if(!SetupFile::LoadFromFile(myHost,currentSetupFile))
             currentSetupFile = "";
     }

     //load default project file
     QString currentProjectFile = ConfigDialog::defaultProjectFile();
     if(!currentProjectFile.isEmpty()) {
         if(!ProjectFile::LoadFromFile(myHost,currentProjectFile))
             currentProjectFile = "";
     }

    opened=true;
    myWindow->readSettings();
}

void Vst::close()
{
    foreach(Connectables::VstAudioDeviceIn* dev, lstAudioIn) {
        removeAudioIn(dev);
    }
    foreach(Connectables::VstAudioDeviceOut* dev, lstAudioOut) {
        removeAudioOut(dev);
    }

    opened=false;

    if(myHost)
        myHost->mainWindow=0;
    myWindow->setParent(0);
    qEditor->SetMainWindow(0);
    delete myWindow;
}

bool Vst::addAudioIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);

    if(lstAudioIn.contains(dev))
        return true;

    if(!hostAcceptIOChanges && lstAudioIn.count()>=DEFAULT_INPUTS)
        return false;

    lstAudioIn << dev;
    dev->setObjectName( QString("Vst audio in %1").arg(lstAudioIn.count()) );
    setNumInputs(lstAudioIn.count()*2);
    ioChanged();
    return true;
}

bool Vst::addAudioOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);

    if(lstAudioOut.contains(dev))
        return true;

    if(!hostAcceptIOChanges && lstAudioOut.count()>=DEFAULT_OUTPUTS)
        return false;

    lstAudioOut << dev;
    dev->setObjectName( QString("Vst audio out %1").arg(lstAudioOut.count()) );
    setNumOutputs(lstAudioOut.count()*2);
    ioChanged();
    return true;
}

bool Vst::removeAudioIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);
    lstAudioIn.removeAll(dev);
    setNumInputs(lstAudioIn.count()*2);
    if(hostAcceptIOChanges && opened)
        ioChanged();
    return true;
}

bool Vst::removeAudioOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);
    lstAudioOut.removeAll(dev);
    setNumOutputs(lstAudioOut.count()*2);
    if(hostAcceptIOChanges && opened)
        ioChanged();
    return true;
}

void Vst::addMidiIn(Connectables::VstMidiDevice *dev)
{
    lstMidiIn << dev;
 }

void Vst::addMidiOut(Connectables::VstMidiDevice *dev)
{
    lstMidiOut << dev;
}

void Vst::removeMidiIn(Connectables::VstMidiDevice *dev)
{
    lstMidiIn.removeAll(dev);
}

void Vst::removeMidiOut(Connectables::VstMidiDevice *dev)
{
    lstMidiOut.removeAll(dev);
}

void Vst::addVstAutomation(Connectables::VstAutomation *dev)
{
    lstVstAutomation << dev;
}

void Vst::removeVstAutomation(Connectables::VstAutomation *dev)
{
    lstVstAutomation.removeAll(dev);
}

VstInt32 Vst::getNumMidiInputChannels()
{
    return 15;
}

VstInt32 Vst::getNumMidiOutputChannels()
{
    return 15;
}

void Vst::setProgramName (char* name)
{
    vst_strncpy (programName, name, kVstMaxProgNameLen);
}

void Vst::getProgramName (char* name)
{
    vst_strncpy (name, programName, kVstMaxProgNameLen);
}

void Vst::setParameter (VstInt32 index, float value)
{
    foreach(Connectables::VstAutomation *dev, lstVstAutomation) {
        dev->ValueFromHost(index,value);
    }
}

float Vst::getParameter (VstInt32 index)
{
    if(lstParameters.count()>index)
        return lstParameters.at(index);
    return .0f;
}

void Vst::getParameterName (VstInt32 index, char* label)
{
    vst_strncpy (label, QString("p%1").arg(index).toAscii(), kVstMaxParamStrLen);
}

void Vst::getParameterDisplay (VstInt32 index, char* text)
{
    dB2string (0, text, kVstMaxParamStrLen);
}

void Vst::getParameterLabel (VstInt32 index, char* label)
{
    vst_strncpy (label, "nd", kVstMaxParamStrLen);
}

bool Vst::getEffectName (char* name)
{
    vst_strncpy (name, "VstBoard", kVstMaxEffectNameLen);
    return true;
}

bool Vst::getProductString (char* text)
{
    vst_strncpy (text, "VstBoard", kVstMaxProductStrLen);
    return true;
}

bool Vst::getVendorString (char* text)
{
    vst_strncpy (text, "CtrlBrk", kVstMaxVendorStrLen);
    return true;
}

VstInt32 Vst::getVendorVersion ()
{
    return 0002;
}

VstInt32 Vst::canDo(char* text)
{
//     "offline", plug-in supports offline functions (offlineNotify, offlinePrepare, offlineRun)
//     "midiProgramNames", plug-in supports function getMidiProgramName ()
//     "bypass", plug-in supports function setBypass ()

    if (
            (!strcmp(text, "sendVstEvents")) ||
            (!strcmp(text, "sendVstMidiEvent")) ||
            (!strcmp(text, "receiveVstEvents")) ||
            (!strcmp(text, "receiveVstMidiEvent")) ||
            (!strcmp(text, "receiveVstTimeInfo"))
         )
         return 1;

    return 0;

}

VstInt32 Vst::processEvents(VstEvents* events)
{
    if(!hostSendVstEvents || !hostSendVstMidiEvent)
        return 0;

    VstEvent *evnt=0;

    for(int i=0; i<events->numEvents; i++) {
        evnt=events->events[i];
        if( evnt->type==kVstMidiType) {
            VstMidiEvent *midiEvnt = (VstMidiEvent*)evnt;

            foreach(Connectables::VstMidiDevice *dev, lstMidiIn) {
                long msg;
                memcpy(&msg, midiEvnt->midiData, sizeof(midiEvnt->midiData));
                dev->midiQueue << msg;
            }
        } else {
            debug("other vst event")
        }
    }

    return 1;
}

void Vst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    if(hostSendVstTimeInfo)
        myHost->SetTimeInfo( getTimeInfo( -1));

    if(bufferSize!=sampleFrames) {
        myHost->SetBufferSize((unsigned long)sampleFrames);
        bufferSize=sampleFrames;
    }

    int cpt=0;
    foreach(Connectables::VstAudioDeviceIn* dev, lstAudioIn) {
        dev->SetBuffers(inputs,cpt,sampleFrames);
    }

    myHost->Render();

    cpt=0;
    foreach(Connectables::VstAudioDeviceOut* dev, lstAudioOut) {
        dev->GetBuffers(outputs,cpt,sampleFrames);
    }

    if(hostReceiveVstEvents || hostReceiveVstMidiEvents) {
        //free last buffer
        if(listEvnts) {
            free(listEvnts);
            listEvnts = 0;
        }

        cpt=0;
        foreach(Connectables::VstMidiDevice *dev, lstMidiOut) {
            foreach(long msg, dev->midiQueue) {

                //allocate a new buffer
                if(!listEvnts)
                    listEvnts = (VstEvents*)malloc(sizeof(VstEvents) + sizeof(VstEvents*)*(VST_EVENT_BUFFER_SIZE-2));

                VstMidiEvent *evnt = new VstMidiEvent;
                memset(evnt, 0, sizeof(VstMidiEvent));
                evnt->type = kVstMidiType;
                evnt->flags = kVstMidiEventIsRealtime;
                evnt->byteSize = sizeof(VstMidiEvent);
                //memcpy(evnt->midiData, &msg.message, sizeof(evnt->midiData));
                memcpy(evnt->midiData, &msg, sizeof(evnt->midiData));
                listEvnts->events[cpt]=(VstEvent*)evnt;
                cpt++;
            }
            dev->midiQueue.clear();
        }

        if(cpt>0) {
            listEvnts->numEvents=cpt;
            sendVstEventsToHost(listEvnts);
        }
    }
}

void Vst::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{

}

void Vst::setSampleRate(float sampleRate)
{
    myHost->SetSampleRate(sampleRate);
}

void Vst::setBlockSize (VstInt32 blockSize)
{
    bufferSize=blockSize;
    myHost->SetBufferSize((unsigned long)blockSize);
}

void Vst::suspend()
{

}

void Vst::resume()
{

}

void Vst::OnMainHostTempoChange()
{
    if (!audioMaster)
        return;

    audioMaster (&cEffect, audioMasterSetTime, 0, 0, &myHost->vstHost->vstTimeInfo, 0);
}
