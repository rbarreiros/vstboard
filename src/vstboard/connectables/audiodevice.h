/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include "../precomp.h"
#include "object.h"
#include "objectinfo.h"
#include "portaudio.h"

#ifndef bzero
#define bzero(memArea, len)  memset((memArea), 0, (len))
#endif

namespace Connectables {

    class AudioDeviceIn;
    class AudioDeviceOut;
    class AudioDevice : QObject //: public Object
    {
        Q_OBJECT
    public:
        AudioDevice(const ObjectInfo &info, QObject *parent=0);
        ~AudioDevice();

        bool Open();
        bool Close();
        void Render();

        bool SetObjectInput(AudioDeviceIn *obj);
        bool SetObjectOutput(AudioDeviceOut *obj);

        static QHash<qint32,AudioDevice*>listAudioDevices;

        void SetSleep(bool sleeping);

    protected:

        static int paCallback( const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData );

        bool FindDeviceFromName();
        bool OpenStream(double sampleRate);
        bool CloseStream();

        float sampleRate;
        unsigned long bufferSize;

        PaStream *stream;
        const PaDeviceInfo *devInfo;

        AudioDeviceIn *devIn;
        AudioDeviceOut *devOut;

        bool closeFlag;

        QMutex objMutex;
        bool closed;

        ObjectInfo objInfo;

    public slots:
        void SetSampleRate(float rate=44100.0);

        friend class AudioDeviceIn;
        friend class AudioDeviceOut;
    };
}

#endif // AUDIODEVICE_H
