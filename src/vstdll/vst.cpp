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


#ifndef QT_NO_DEBUG

    void myMessageOutput(QtMsgType type, const char *msg)
     {
        qInstallMsgHandler(0);
        qDebug(msg);
        qInstallMsgHandler(myMessageOutput);

         switch (type) {

         case QtWarningMsg:
#ifdef _MSC_VER
                __asm int 3
#else
                __asm("int3");
#endif
            break;

         case QtCriticalMsg:
         case QtFatalMsg:
#ifdef _MSC_VER
                __asm int 3
#else
                __asm("int3");
#endif
             abort();
             break;

         case QtDebugMsg:
             break;
         }
     }
#endif

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
   return new Vst (audioMaster);
}

bool MyDllApp::notify(QObject *rec, QEvent *ev) {
       try {
           return QApplication::notify(rec, ev);
       }
       catch (...) {
           debug("Unknown exception!")
#ifdef _MSC_VER
            __asm int 3
#else
            __asm("int3");
#endif
           abort();
       }
   }

Vst::Vst (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 0),	// 1 program, 0 parameter
deviceIn(0),
deviceOut(0),
bufferSize(0),
myHost(0),
myWindow(0)
{
#ifndef QT_NO_DEBUG
    qInstallMsgHandler(myMessageOutput);
#endif

        setNumInputs (2);		// stereo in
        setNumOutputs (2);		// stereo out
        setUniqueID ('VbPl');	// identify
        canProcessReplacing ();	// supports replacing output
//        canDoubleReplacing ();	// supports double precision processing

        vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name


        //create a QApplication if needed
        if(!qApp) {
            argc=0;
            argv="";
            app = new MyDllApp(argc,&argv);
        } else {
            app=0;
        }

        qEditor = new Gui(this);
        setEditor(qEditor);

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
        qEditor->SetMainWindow(myWindow);
}

Vst::~Vst ()
{
    debug("delete TestVst")


    //crash at deletion if we don't open the mainwindow once
    if(app) {
        setEditor(0);
        qApp->removePostedEvents(myHost);
        delete myHost;

        myWindow->resize(0,0);
        myWindow->show();
        qApp->removePostedEvents(myWindow);
        qApp->processEvents();

        delete myWindow;
        delete qEditor;

        delete app;
    } else {
//        MainWindow::Get()->setParent(0);
//        delete qEditor;
//        MainHost::Get()->deleteLater();
//        MainWindow::Get()->deleteLater();
//        qEditor->deleteLater();
//        qApp->removePostedEvents(MainHost::Get());
//        qApp->processEvents();
    }
}

bool Vst::setDeviceIn(Connectables::VstAudioDeviceIn *dev)
{
    QMutexLocker l(&mutexDevices);

    if(deviceIn && dev)
        return false;
    deviceIn=dev;
    return true;
}

bool Vst::setDeviceOut(Connectables::VstAudioDeviceOut *dev)
{
    QMutexLocker l(&mutexDevices);

    if(deviceOut && dev)
        return false;
    deviceOut=dev;
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
    if(deviceIn)
        deviceIn->SetBuffers(inputs);
    if(deviceOut)
        deviceOut->SetBuffers(outputs);
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
    setDeviceIn(0);
    setDeviceOut(0);
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
