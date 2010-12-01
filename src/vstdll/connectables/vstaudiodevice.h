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

#ifndef VSTAUDIODEVICE_H
#define VSTAUDIODEVICE_H

#include <QWaitCondition>

#include "precomp.h"
#include "connectables/object.h"
#include "connectables/objectinfo.h"

#ifndef bzero
#define bzero(memArea, len)  memset((memArea), 0, (len))
#endif

namespace Connectables {

    class VstAudioDeviceIn;
    class VstAudioDeviceOut;
    class VstAudioDevice : public QObject
    {
        Q_OBJECT
    public:
        VstAudioDevice(const ObjectInfo &info, QObject *parent=0);
        ~VstAudioDevice();

        bool Open();
        bool Close();
        bool SetObjectInput(VstAudioDeviceIn *obj);
        bool SetObjectOutput(VstAudioDeviceOut *obj);
        void SetSleep(bool sleeping);
        bool bufferReady;
        static int countDevicesReady;
        static int countInputDevices;
        static QMutex listDevMutex;

    protected:
        bool OpenStream(double sampleRate);
        bool CloseStream();
        void DeleteCircualBuffers();
        bool isClosing;
        bool devOutClosing;
        float sampleRate;
        unsigned long bufferSize;
        VstAudioDeviceIn *devIn;
        VstAudioDeviceOut *devOut;
        bool closed;
        QMutex devicesMutex;
        ObjectInfo objInfo;

    signals:
        void InUseChanged(const ObjectInfo &objInfo, bool inUse);

    public slots:
        void SetSampleRate(float rate=44100.0);

//        friend class VstAudioDeviceIn;
//        friend class VstAudioDeviceOut;

        void DeleteIfUnused();
    };
}

#endif // VSTAUDIODEVICE_H
