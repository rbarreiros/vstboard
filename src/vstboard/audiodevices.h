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

class MainHost;
class AudioDevices : public QObject
{
    Q_OBJECT
public:
    explicit AudioDevices(MainHost *myHost);
    ~AudioDevices();
    ListAudioInterfacesModel * GetModel();
    QHash<qint32,QSharedPointer<Connectables::AudioDevice> >listAudioDevices;
    QTimer fakeRenderTimer;

    QPersistentModelIndex AsioIndex;

private:
    void BuildModel();
    ListAudioInterfacesModel *model;
    int countActiveDevices;
    MainHost *myHost;
public slots:
    void OnToggleDeviceInUse(const ObjectInfo &objInfo, bool opened);
};

#endif // AUDIODEVICES_H
