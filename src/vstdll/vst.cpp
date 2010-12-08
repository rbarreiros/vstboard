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

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
   return new Vst (audioMaster);
}

Vst::Vst (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 0),	// 1 program, 0 parameter
myApp(0),
myHost(0),
myWindow(0),
bufferSize(0)
{
        setNumInputs (2);		// stereo in
        setNumOutputs (2);		// stereo out
        setUniqueID ('VbPl');	// identify
        canProcessReplacing ();	// supports replacing output
//        canDoubleReplacing ();	// supports double precision processing

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

bool Vst::addDeviceIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);

    if(lstDeviceIn.contains(dev))
        return true;

    lstDeviceIn << dev;
    dev->setObjectName( QString("Vst audio in %1").arg(lstDeviceIn.count()) );
    setNumInputs(lstDeviceIn.count()*2);
    ioChanged();
    return true;
}

bool Vst::addDeviceOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);

    if(lstDeviceOut.contains(dev))
        return true;

    lstDeviceOut << dev;
    dev->setObjectName( QString("Vst audio out %1").arg(lstDeviceOut.count()) );
    setNumOutputs(lstDeviceOut.count()*2);
    ioChanged();
    return true;
}

bool Vst::removeDeviceIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);
    lstDeviceIn.removeAll(dev);
    setNumInputs(lstDeviceIn.count()*2);
    ioChanged();
    return true;
}

bool Vst::removeDeviceOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);
    lstDeviceOut.removeAll(dev);
    setNumOutputs(lstDeviceOut.count()*2);
    ioChanged();
    return true;
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

void Vst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    if(bufferSize!=sampleFrames) {
        myHost->SetBufferSize(sampleFrames);
        bufferSize=sampleFrames;
    }

    mutexDevices.lock();

    int cpt=0;
    foreach(Connectables::VstAudioDeviceIn* dev, lstDeviceIn) {
        dev->SetBuffers(inputs,cpt);
    }

    cpt=0;
    foreach(Connectables::VstAudioDeviceOut* dev, lstDeviceOut) {
        dev->SetBuffers(outputs,cpt);
    }

    mutexDevices.unlock();

    myHost->Render();

}

void Vst::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{

}

void Vst::open()
{

}

void Vst::close()
{
    foreach(Connectables::VstAudioDeviceIn* dev, lstDeviceIn) {
        removeDeviceIn(dev);
    }
    foreach(Connectables::VstAudioDeviceOut* dev, lstDeviceOut) {
        removeDeviceOut(dev);
    }
}

void Vst::setSampleRate(float sampleRate)
{
    myHost->SetSampleRate(sampleRate);
}

void Vst::setBlockSize (VstInt32 blockSize)
{
    bufferSize=blockSize;
    myHost->SetBufferSize(blockSize);
}
void Vst::suspend()
{

}

void Vst::resume()
{

}
