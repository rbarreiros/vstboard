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

#define SETUP_FILE_KEY 0x7575e711

#include "setupfile.h"
#include "../mainhost.h"
#include "../mainwindow.h"
#include "fileversion.h"
#include "../views/viewconfig.h"

void SetupFile::Clear(MainHost *myHost)
{
    myHost->EnableSolverUpdate(false);
    myHost->SetupHostContainer();
    myHost->EnableSolverUpdate(true);
    if(myHost->mainWindow) {
        myHost->mainWindow->mySceneView->viewHost->ClearPrograms();
    }
    myHost->mainWindow->viewConfig.LoadFromRegistry(myHost);
}

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
    return ToStream(myHost,out);
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
    return FromStream(myHost,in);

}

bool SetupFile::ToStream(MainHost *myHost,QDataStream &out)
{
    out.setVersion(QDataStream::Qt_4_7);

    myHost->currentFileVersion=PROJECT_AND_SETUP_FILE_VERSION;

    out << (quint32)SETUP_FILE_KEY;
    out << myHost->currentFileVersion;

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

    out << myHost->mainWindow->viewConfig.savedInSetupFile;
    if(myHost->mainWindow->viewConfig.savedInSetupFile)
        out << myHost->mainWindow->viewConfig;

    return true;
}

bool SetupFile::FromStream(MainHost *myHost,QDataStream &in)
{
    in.setVersion(QDataStream::Qt_4_7);

    quint32 magic;
    in >> magic;
    if(magic != SETUP_FILE_KEY) {
        QMessageBox msgBox;
        //msgBox.setWindowTitle(filePath);
        msgBox.setText( tr("Unknown file format.") );
        msgBox.setInformativeText( tr("Not a VstBoard setup file") );
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    in >> myHost->currentFileVersion;

    if(myHost->currentFileVersion < 11) {
        QMessageBox msgBox;
//        msgBox.setWindowTitle(filePath);
        msgBox.setText( tr("Wrong file version.") );
        msgBox.setInformativeText( tr("Setup file format v%1 can't be converted to the current file format v%2").arg(myHost->currentFileVersion).arg(PROJECT_AND_SETUP_FILE_VERSION) );
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    myHost->EnableSolverUpdate(false);
    myHost->renderer->SetEnabled(false);
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

    myHost->renderer->SetEnabled(true);
    myHost->EnableSolverUpdate(true);


    if(myHost->currentFileVersion >= 12) {
        bool colorsInSetupFile;
        in >> colorsInSetupFile;
        myHost->mainWindow->viewConfig.savedInSetupFile=colorsInSetupFile;

        if(colorsInSetupFile)
            in >> myHost->mainWindow->viewConfig;
        else
            myHost->mainWindow->viewConfig.LoadFromRegistry(myHost);
    }

    return true;
}
