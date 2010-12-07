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

#ifndef AUDIODEVICEIN_H
#define AUDIODEVICEIN_H

#include "../precomp.h"
#include "object.h"
#include "objectinfo.h"

class AudioBuffer;

namespace Connectables {

    class AudioDevice;
    class AudioDeviceIn : public Object
    {
    Q_OBJECT
    public:
        AudioDeviceIn(MainHost *myHost,int index, const ObjectInfo &info);
        ~AudioDeviceIn();

        bool Open();
        bool Close();
        void Render();
//        int GetProcessingTime() {return 10;}

        bool bufferReady;

    protected:
        QSharedPointer<AudioDevice>parentDevice;

    public slots:
        void SetBufferSize(long size);

    friend class AudioDevice;
    };
}

#endif // AUDIODEVICEIN_H
