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

#ifndef MAINWINDOWHOST_H
#define MAINWINDOWHOST_H

#ifndef APP_NAME
#define APP_NAME "noname ?"
#endif

#include "mainwindow.h"
#include "mainhosthost.h"
#include "../common/ui_mainwindow.h"
class MainWindowHost : public MainWindow {
    Q_OBJECT

public:
    MainWindowHost(MainHostHost * myHost, QWidget *parent = 0);
    void readSettings();

protected:
    void closeEvent(QCloseEvent *event);
    void resetSettings();

    ListToolsModel *listAudioDevModel;
    ListToolsModel *listMidiDevModel;

public slots:
    void UpdateAudioDevices();
    void UpdateMidiDevices();

private slots:
    void on_actionConfig_triggered();
    void on_actionRefresh_Midi_devices_triggered();
    void on_actionRefresh_Audio_devices_triggered();

};

#endif // MAINWINDOWHOST_H
