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

#define SETUP_FILE_VERSION 7
#define SETUP_FILE_KEY 0x7575e711

#include "setupfile.h"
#include "../mainhost.h"
#include "../mainwindow.h"

bool SetupFile::SaveToFile(MainHost *myHost, QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox msgBox;
        msgBox.setText(tr("Unable to open %1").arg(filePath));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }
    QDataStream out(&file);

    out << (quint32)SETUP_FILE_KEY;
    out << (quint32)SETUP_FILE_VERSION;
    out.setVersion(QDataStream::Qt_4_7);

    myHost->EnableSolverUpdate(false);
    myHost->hostContainer->SaveProgram();

    for(myHost->filePass=0; myHost->filePass<LOADSAVE_STAGES ; myHost->filePass++) {
        out << *myHost->hostContainer;
    }

    if(myHost->mainWindow) {
        out << (quint8)1;
        myHost->mainWindow->mySceneView->viewHost->SaveProgram();
        out << *myHost->mainWindow->mySceneView->viewHost;
    } else {
        out << (quint8)0;
    }

    myHost->EnableSolverUpdate(true);

    return true;
}

void SetupFile::Clear(MainHost *myHost)
{
    myHost->EnableSolverUpdate(false);
    myHost->SetupHostContainer();
    myHost->EnableSolverUpdate(true);
    if(myHost->mainWindow) {
        myHost->mainWindow->mySceneView->viewHost->ClearPrograms();
    }
}

bool SetupFile::LoadFromFile(MainHost *myHost, QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setText(tr("Unable to open %1").arg(filePath));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }
    QDataStream in(&file);

    quint32 magic;
    in >> magic;
    if(magic != SETUP_FILE_KEY) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(filePath);
        msgBox.setText( tr("Unknown file format.") );
        msgBox.setInformativeText( tr("Not a VstBoard setup file") );
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    quint32 version;
    in >> version;
    if(version != SETUP_FILE_VERSION && version!=5) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(filePath);
        msgBox.setText( tr("Wrong file version.") );
        msgBox.setInformativeText( tr("Setup file format v%1 can't be converted to the current file format v%2").arg(version).arg(SETUP_FILE_VERSION) );
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    in.setVersion(QDataStream::Qt_4_7);

    myHost->EnableSolverUpdate(false);
    myHost->SetupHostContainer();

    for(myHost->filePass=0; myHost->filePass<LOADSAVE_STAGES ; myHost->filePass++) {
        in >> *myHost->hostContainer;
    }

    myHost->objFactory->ResetSavedId();

    quint8 gui;
    in >> gui;
    if(gui && myHost->mainWindow) {
        myHost->mainWindow->mySceneView->viewHost->SetProgram(EMPTY_PROGRAM);
        in >> *myHost->mainWindow->mySceneView->viewHost;
        myHost->mainWindow->mySceneView->viewHost->SetProgram(0);
    }

    myHost->EnableSolverUpdate(true);
    return true;
}
