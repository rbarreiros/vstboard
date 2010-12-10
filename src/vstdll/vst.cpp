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
#include "mainwindow.h"
#include "portmidi.h"
#include "pmutil.h"
#include "audioeffectx.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new Vst (audioMaster);
}

Vst::Vst (audioMasterCallback audioMaster)
    : AudioEffectX (audioMaster, 1, 0),	// 1 program, 0 parameter
    myApp(0),
    myHost(0),
    myWindow(0),
    bufferSize(0),
    listEvnts(0),
    hostAcceptIOChanges(0)
{
    VstInt32 ver = getMasterVersion();
    if(ver<2400) {
        QMessageBox msg;
        msg.setText(tr("host not supported (Vst version %1)").arg(ver));
        msg.exec();
    }

    char str[64];
    getHostVendorString(str);
    getHostProductString(str);
    VstInt32 hostVer = getHostVendorVersion();

    long hostSendVstEvents = canHostDo("sendVstEvents");
    long hostSendVstMidiEvent = canHostDo("sendVstMidiEvent");
    long hostSendVstTimeInfo = canHostDo("sendVstTimeInfo");
    long hostReceiveVstEvents = canHostDo("receiveVstEvents");
    long hostReceiveVstMidiEvents = canHostDo("receiveVstMidiEvents");
    long hostReceiveVstTimeInfo = canHostDo("receiveVstTimeInfo");
    long hostReportConnectionChanges = canHostDo("reportConnectionChanges");
    hostAcceptIOChanges = canHostDo("acceptIOChanges");
    long hostSizeWindow = canHostDo("sizeWindow");
    long hostAsyncProcessing = canHostDo("asyncProcessing");
    long hostOffline = canHostDo("offline");
    long hostSupplyIdle = canHostDo("supplyIdle");
    long hostSupportShell = canHostDo("supportShell");
    long hostOpenFileSelector = canHostDo("openFileSelector");
    long hostEditFile = canHostDo("editFile");
    long hostCloseFileSelector = canHostDo("closeFileSelector");


    //cEffect.flags |= effFlagsIsSynth;

    setNumInputs (DEFAULT_INPUTS*2);
    setNumOutputs (DEFAULT_OUTPUTS*2);
    setUniqueID (kUniqueID);
    canProcessReplacing(true);
    //        canDoubleReplacing ();

    vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

    qEditor = new Gui(this);
    setEditor(qEditor);

    qRegisterMetaType<ConnectionInfo>("ConnectionInfo");
    qRegisterMetaType<ObjectInfo>("ObjectInfo");
    qRegisterMetaType<int>("ObjType::Enum");
    qRegisterMetaType<orderedNodes>("orderedNodes");
    qRegisterMetaType<QVariant>("QVariant");

    qRegisterMetaTypeStreamOperators<ObjectInfo>("ObjectInfo");

    if(!qApp) {
        int argc=0;
        char *argv=0;
        myApp = new QApplication(argc,&argv);
    }

    //QCoreApplication::setOrganizationName("CtrlBrk");
    //QCoreApplication::setApplicationName(APP_NAME);

    myHost = new MainHost(this);
    myWindow = new MainWindow(myHost);
    qEditor->SetMainWindow(myWindow);
}

Vst::~Vst ()
{
    debug2(<< "delete Vst" << hex << (long)this)

            setEditor(0);
    myHost->mainWindow=0;
    myWindow->setParent(0);
    qApp->removePostedEvents(myWindow);
    delete myWindow;
    qApp->removePostedEvents(myHost);
    delete myHost;
    qApp->removePostedEvents(qEditor);
    delete qEditor;
    if(myApp)
        delete myApp;
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

    if(hostAcceptIOChanges)
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

    if(hostAcceptIOChanges)
        ioChanged();
    return true;
}

bool Vst::removeAudioIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);
    lstAudioIn.removeAll(dev);
    setNumInputs(lstAudioIn.count()*2);
    if(hostAcceptIOChanges)
        ioChanged();
    return true;
}

bool Vst::removeAudioOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);
    lstAudioOut.removeAll(dev);
    setNumOutputs(lstAudioOut.count()*2);
    if(hostAcceptIOChanges)
        ioChanged();
    return true;
}

void Vst::addMidiIn(Connectables::MidiDevice *dev)
{
    lstMidiIn << dev;
 }

void Vst::addMidiOut(Connectables::MidiDevice *dev)
{
    lstMidiOut << dev;
}

void Vst::removeMidiIn(Connectables::MidiDevice *dev)
{
    lstMidiIn.removeAll(dev);
}

void Vst::removeMidiOut(Connectables::MidiDevice *dev)
{
    lstMidiOut.removeAll(dev);
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
//    emit update(value);
}

float Vst::getParameter (VstInt32 index)
{
    return 0;
}

void Vst::getParameterName (VstInt32 index, char* label)
{
    vst_strncpy (label, "nd", kVstMaxParamStrLen);
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
            (!strcmp(text, "receiveVstMidiEvent") ||
            (!strcmp(text, "receiveVstTimeInfo")))
         )
         return 1;

    return 0;

}

VstInt32 Vst::processEvents(VstEvents* events)
{
    VstEvent *evnt=0;

    for(int i=0; i<events->numEvents; i++) {
        evnt=events->events[i];
        if( evnt->type==kVstMidiType) {
            VstMidiEvent *midiEvnt = (VstMidiEvent*)evnt;

            foreach(Connectables::MidiDevice *dev, lstMidiIn) {
                Pm_Enqueue(dev->queue, midiEvnt->midiData);
            }
        } else {
            debug("other vst event")
        }
    }

    return 1;
}

void Vst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    if(bufferSize!=sampleFrames) {
        myHost->SetBufferSize((unsigned long)sampleFrames);
        bufferSize=sampleFrames;
    }

    mutexDevices.lock();

    int cpt=0;
    foreach(Connectables::VstAudioDeviceIn* dev, lstAudioIn) {
        dev->SetBuffers(inputs,cpt);
    }

    cpt=0;
    foreach(Connectables::VstAudioDeviceOut* dev, lstAudioOut) {
        dev->SetBuffers(outputs,cpt);
    }

    mutexDevices.unlock();

    myHost->Render();


    //free last buffer
    if(listEvnts) {
        free(listEvnts);
        listEvnts = 0;
    }

    PmEvent msg;
    cpt=0;
    foreach(Connectables::MidiDevice *dev, lstMidiOut) {
        while (!Pm_QueueEmpty(dev->queue)) {

            //allocate a new buffer
            if(!listEvnts)
                listEvnts = (VstEvents*)malloc(sizeof(VstEvents) + sizeof(VstEvents*)*(VST_EVENT_BUFFER_SIZE-2));

            Pm_Dequeue( dev->queue, &msg);

            VstMidiEvent *evnt = new VstMidiEvent;
            memset(evnt, 0, sizeof(VstMidiEvent));
            evnt->type = kVstMidiType;
            evnt->flags = kVstMidiEventIsRealtime;
            evnt->byteSize = sizeof(VstMidiEvent);
            memcpy(evnt->midiData, &msg.message, sizeof(evnt->midiData));
            listEvnts->events[cpt]=(VstEvent*)evnt;
            cpt++;
        }
    }

    if(cpt>0) {
        listEvnts->numEvents=cpt;
        sendVstEventsToHost(listEvnts);
    }
}

void Vst::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{

}

void Vst::open()
{

}

void Vst::close()
{
    foreach(Connectables::VstAudioDeviceIn* dev, lstAudioIn) {
        removeAudioIn(dev);
    }
    foreach(Connectables::VstAudioDeviceOut* dev, lstAudioOut) {
        removeAudioOut(dev);
    }
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
