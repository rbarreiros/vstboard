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

#include "audiodevices.h"
#include "globals.h"
#include "connectables/objectinfo.h"

AudioDevices::AudioDevices() :
        model(0)
{
    PaError paRet =Pa_Initialize();
    if(paRet!=paNoError) {
        debug(Pa_GetErrorText(paRet))
    }

    BuildModel();
}

AudioDevices::~AudioDevices()
{
    model->clear();
    model->deleteLater();
}

void AudioDevices::BuildModel()
{
    QStringList headerLabels;
    headerLabels << "Name";
    headerLabels << "In";
    headerLabels << "Out";

    model = new ListAudioInterfacesModel();
    model->setHorizontalHeaderLabels(  headerLabels );

    QStandardItem *parentItem = model->invisibleRootItem();

    //APIs
    for (int i = 0; i < Pa_GetHostApiCount(); ++i) {
        QStandardItem *api = new QStandardItem(Pa_GetHostApiInfo(i)->name);
        api->setColumnCount(1);

        parentItem->appendRow(api);
    }

    //devices
    QString lastName;
    int cptDuplicateNames=0;

    for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
        QList<QStandardItem *>  items;
        const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(i);

        QString devName(devInfo->name);
        if(lastName == devName) {
            cptDuplicateNames++;
        } else {
            cptDuplicateNames=0;
        }
        lastName = devName;

        ObjectInfo obj;
        obj.nodeType = NodeType::object;
        obj.objType = ObjType::AudioInterface;
        obj.id = i;
        obj.name = devName;
        obj.api = QString::fromStdString( Pa_GetHostApiInfo(devInfo->hostApi)->name );
        obj.duplicateNamesCounter = cptDuplicateNames;
        obj.inputs = devInfo->maxInputChannels;
        obj.outputs = devInfo->maxOutputChannels;

        QStandardItem *dev = new QStandardItem( devName);
        dev->setEditable(false);
        dev->setData(QVariant::fromValue(obj), UserRoles::objInfo);
        items << dev;

        QStandardItem *ins = new QStandardItem( QString::number(devInfo->maxInputChannels));
        ins->setEditable(false);
        items << ins;

        QStandardItem *outs = new QStandardItem( QString::number(devInfo->maxOutputChannels));
        outs->setEditable(false);
        items << outs;

        QStandardItem *parent = model->item(devInfo->hostApi,0);
        parent->appendRow(items);
    }
}
