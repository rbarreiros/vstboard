/******************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#ifndef AUDIODEVICES_H
#define AUDIODEVICES_H

#include "precomp.h"
#include "portaudio.h"
#include "models/listaudiointerfacesmodel.h"

class AudioDevices : public QObject
{
    Q_OBJECT
public:
    explicit AudioDevices(QObject *parent=0);
    ~AudioDevices();
    ListAudioInterfacesModel * GetModel();
private:
    void BuildModel();
    ListAudioInterfacesModel *model;

private slots:
    void UpdateCpuUsage();
};

#endif // AUDIODEVICES_H
