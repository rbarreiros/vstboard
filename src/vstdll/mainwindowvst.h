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

#ifndef MAINWINDOWVST_H
#define MAINWINDOWVST_H

#ifndef APP_NAME
#define APP_NAME "noname ?"
#endif

#include "mainwindow.h"
#include "../common/ui_mainwindow.h"

class MainHostVst;
class MainWindowVst : public MainWindow {
    Q_OBJECT

public:
    MainWindowVst(MainHost * myHost, QWidget *parent = 0);
    void readSettings();

protected:
    void closeEvent(QCloseEvent *event);
    void BuildListTools();
    void resetSettings();

private slots:
    void on_actionConfig_triggered();
};

#endif // MAINWINDOWVST_H
