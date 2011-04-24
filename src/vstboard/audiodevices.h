/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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

#ifndef AUDIODEVICES_H
#define AUDIODEVICES_H

#define FAKE_RENDER_TIMER_MS 10

#include "precomp.h"
#include "portaudio.h"
#include "models/listaudiointerfacesmodel.h"
#include "connectables/objectinfo.h"
#include "connectables/audiodevice.h"

class MainHostHost;
class AudioDevices : public QObject
{
    Q_OBJECT
public:
    explicit AudioDevices(MainHostHost *myHost);
    ~AudioDevices();
    ListAudioInterfacesModel * GetModel();
    Connectables::AudioDevice * AddDevice(ObjectInfo &objInfo, QString *errMsg=0);
    void RemoveDevice(PaDeviceIndex devId);

    void PutPinsBuffersInRingBuffers();

    /// timer to launch the rendering loop when no audio devices are opened
    QTimer fakeRenderTimer;

    /// model index of the asio devices, used by the view to expand this branch only
    QPersistentModelIndex AsioIndex;

    bool FindPortAudioDevice(ObjectInfo &objInfo, PaDeviceInfo *dInfo);
private:
    void BuildModel();

    /// list of opened AudioDevice
    QHash<qint32,Connectables::AudioDevice* >listAudioDevices;

    /// model pointer
    ListAudioInterfacesModel *model;

    /// number of opened devices
    int countActiveDevices;

    /// pointer to the MainHost
    MainHostHost *myHost;

    QMutex mutexDevices;


public slots:
    void OnToggleDeviceInUse(PaHostApiIndex apiId, PaDeviceIndex devId, bool inUse);
    void ConfigDevice(const QModelIndex &dev);
};

#endif // AUDIODEVICES_H
