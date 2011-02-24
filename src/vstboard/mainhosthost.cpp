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

#include "mainhosthost.h"

#include "connectables/mididevice.h"
#include "connectables/audiodevice.h"
#include "connectables/objectfactoryhost.h"

MainHostHost::MainHostHost(QObject *parent, QString settingsGroup) :
    MainHost(parent,settingsGroup)
{
    objFactory = new Connectables::ObjectFactoryHost(this);
    midiDevices = new MidiDevices(this);
    audioDevices = new AudioDevices(this);
    connect(this,SIGNAL(OnAudioDeviceToggleInUse(ObjectInfo,bool)),
            audioDevices,SLOT(OnToggleDeviceInUse(ObjectInfo,bool)));
    connect(&audioDevices->fakeRenderTimer,SIGNAL(timeout()),
            this, SLOT(Render()));
}

MainHostHost::~MainHostHost()
{
    delete audioDevices;
    audioDevices=0;
    delete midiDevices;
    midiDevices=0;
}

void MainHostHost::Render(unsigned long samples)
{
    #ifdef VSTSDK
        vstHost->UpdateTimeInfo(timeFromStart.elapsed(), samples, sampleRate);
    #endif

    MainHost::Render(samples);
}
