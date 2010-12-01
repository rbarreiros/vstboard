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

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QWaitCondition>

#include "../precomp.h"
#include "object.h"
#include "objectinfo.h"
#ifndef VST_PLUGIN
    #include "portaudio.h"
    #include "pa_win_wmme.h"
    #include "pa_win_ds.h"
#endif

#include "../circularbuffer.h"

#ifndef bzero
#define bzero(memArea, len)  memset((memArea), 0, (len))
#endif

namespace Connectables {

    class AudioDeviceIn;
    class AudioDeviceOut;
    class AudioDevice : public QObject
    {
        Q_OBJECT
    public:
        AudioDevice(const ObjectInfo &info, QObject *parent=0);
        ~AudioDevice();

        bool Open();
        bool Close();

        float GetCpuUsage();

        bool SetObjectInput(AudioDeviceIn *obj);
        bool SetObjectOutput(AudioDeviceOut *obj);

//        static QHash<qint32,QSharedPointer<AudioDevice> >listAudioDevices;

        void SetSleep(bool sleeping);

        bool bufferReady;

        static bool FindDeviceByName(ObjectInfo &objInfo, PaDeviceInfo *devInfo=0);

        static int countDevicesReady;
        static int countInputDevices;

//        QWeakPointer<AudioDevice> GetSharedPointer() {return this->sharedPointer;}

        static QMutex listDevMutex;

    protected:
//        QWeakPointer<AudioDevice>sharedPointer;

        static int paCallback( const void *inputBuffer, void *outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData );

        static void paStreamFinished( void* userData );

        bool OpenStream(double sampleRate);
        bool CloseStream();

        void DeleteCircualBuffers();

        bool isClosing;
        bool devOutClosing;

        float sampleRate;
        unsigned long bufferSize;

        PaStream *stream;
        PaDeviceInfo devInfo;

        AudioDeviceIn *devIn;
        AudioDeviceOut *devOut;

        bool closed;

        QMutex devicesMutex;
        ObjectInfo objInfo;

        PaWinMmeStreamInfo wmmeStreamInfo;
        PaWinDirectSoundStreamInfo directSoundStreamInfo;

        QList<CircularBuffer*>listCircularBuffersIn;
        QList<CircularBuffer*>listCircularBuffersOut;

        float cpuUsage;

    signals:
        void InUseChanged(const ObjectInfo &objInfo, bool inUse);

    public slots:
        void SetSampleRate(float rate=44100.0);

        friend class AudioDeviceIn;
        friend class AudioDeviceOut;

        void DeleteIfUnused();
    };
}

#endif // AUDIODEVICE_H
