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


#include "testvst.h"
#include "connectables/connectioninfo.h"
#include "connectables/objectinfo.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{


        return new TestVst (audioMaster);
}

TestVst *TestVst::theVstPlugin=0;

TestVst::TestVst (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1),	// 1 program, 1 parameter only
deviceIn(0),
deviceOut(0),
bufferSize(0)
{
    theVstPlugin = this;
        setNumInputs (2);		// stereo in
        setNumOutputs (2);		// stereo out
        setUniqueID ('CbVb');	// identify
        canProcessReplacing ();	// supports replacing output
        canDoubleReplacing ();	// supports double precision processing

        vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

//  QtTestVst* TestVst = new QtTestVst(this);
        Gui *editor = new Gui(this);

        setEditor(editor);
//        connect(this, SIGNAL(update(float)), TestVst, SIGNAL(update(float)));


        qRegisterMetaType<ConnectionInfo>("ConnectionInfo");
        qRegisterMetaType<ObjectInfo>("ObjectInfo");
        qRegisterMetaType<int>("ObjType::Enum");
        qRegisterMetaType<orderedNodes>("orderedNodes");
        qRegisterMetaType<QVariant>("QVariant");

        qRegisterMetaTypeStreamOperators<ObjectInfo>("ObjectInfo");

//        QCoreApplication::setOrganizationName("CtrlBrk");
//        QCoreApplication::setApplicationName(APP_NAME);

//        static int argc=0;
//        static char *argv=0;
//        app = new QApplication(argc,&argv);
        MainHost::Create();

    //    MainHost::Get()->Open();
        MainWindow::Get();


    //    //load default setup file
    //    currentSetupFile = ConfigDialog::defaultSetupFile();
    //    if(!currentSetupFile.isEmpty()) {
    //        if(!SetupFile::LoadFromFile(currentSetupFile))
    //            currentSetupFile = "";
    //    }

    //    //load default project file
    //    currentProjectFile = ConfigDialog::defaultProjectFile();
    //    if(!currentProjectFile.isEmpty()) {
    //        if(!ProjectFile::LoadFromFile(currentProjectFile))
    //            currentProjectFile = "";
    //    }

    //    win = MainWindow::Get();
    //    win->show();
}

TestVst::~TestVst ()
{
//    app->closeAllWindows();
//    app->processEvents();
//    app->sendPostedEvents();
//    app->exit(0);

}

bool TestVst::setDeviceIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);

    if(deviceIn && dev)
        return false;
    deviceIn=dev;
    return true;
}

bool TestVst::setDeviceOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);

    if(deviceOut && dev)
        return false;
    deviceOut=dev;
    return true;
}

void TestVst::setProgramName (char* name)
{
        vst_strncpy (programName, name, kVstMaxProgNameLen);
}

void TestVst::getProgramName (char* name)
{
        vst_strncpy (name, programName, kVstMaxProgNameLen);
}

void TestVst::setParameter (VstInt32 index, float value)
{
    emit update(value);
}

float TestVst::getParameter (VstInt32 index)
{
        return 0;
}

void TestVst::getParameterName (VstInt32 index, char* label)
{
        vst_strncpy (label, "Gain", kVstMaxParamStrLen);
}

void TestVst::getParameterDisplay (VstInt32 index, char* text)
{
        dB2string (0, text, kVstMaxParamStrLen);
}

void TestVst::getParameterLabel (VstInt32 index, char* label)
{
        vst_strncpy (label, "dB", kVstMaxParamStrLen);
}

bool TestVst::getEffectName (char* name)
{
        vst_strncpy (name, "VstBoard", kVstMaxEffectNameLen);
        return true;
}

bool TestVst::getProductString (char* text)
{
        vst_strncpy (text, "VstBoard", kVstMaxProductStrLen);
        return true;
}

bool TestVst::getVendorString (char* text)
{
        vst_strncpy (text, "CtrlBrk", kVstMaxVendorStrLen);
        return true;
}

VstInt32 TestVst::getVendorVersion ()
{
        return 1000;
}

void TestVst::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    if(bufferSize!=sampleFrames) {
        MainHost::Get()->SetBufferSize(sampleFrames);
        bufferSize=sampleFrames;
    }

    mutexDevices.lock();
    if(deviceIn)
        deviceIn->SetBuffers(inputs);
    if(deviceOut)
        deviceOut->SetBuffers(outputs);
    mutexDevices.unlock();

    MainHost::Get()->Render();

}

void TestVst::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{

}

void TestVst::open()
{

}

void TestVst::close()
{
//    win->deleteLater();
//    app->deleteLater();
    setDeviceIn(0);
    setDeviceOut(0);

}

void TestVst::setSampleRate(float sampleRate)
{
    MainHost::Get()->SetSampleRate(sampleRate);
}

void TestVst::setBlockSize (VstInt32 blockSize)
{
    bufferSize=blockSize;
    MainHost::Get()->SetBufferSize(blockSize);
}
void TestVst::suspend()
{

}

void TestVst::resume()
{

}
