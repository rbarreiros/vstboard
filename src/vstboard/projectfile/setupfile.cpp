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

#define SETUP_FILE_VERSION 1
#define SETUP_FILE_KEY 0x7575e711

#include "setupfile.h"
#include "../mainhost.h"

SetupFile::SetupFile(QObject *parent) :
        QObject(parent)
{
    fileName = "setup.dat";
}

void SetupFile::SaveToFile(QString filePath)
{
    if(filePath.isNull())
        filePath = fileName;

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);

    stream << (quint32)SETUP_FILE_KEY;
    stream << (quint32)SETUP_FILE_VERSION;
    stream.setVersion(QDataStream::Qt_4_6);

    MainHost *host = MainHost::Get();

    host->EnableSolverUpdate(false);

    host->hostContainer->SaveProgram();

    MainHost::Get()->filePass=0;
    stream << *host->hostContainer;

    MainHost::Get()->filePass=1;
    stream << *host->hostContainer;

    host->EnableSolverUpdate(true);
}

void SetupFile::Clear()
{
    MainHost *host = MainHost::Get();
    host->EnableSolverUpdate(false);
    host->SetupHostContainer();
    host->hostContainer->listenProgramChanges=false;
    host->EnableSolverUpdate(true);
}

bool SetupFile::LoadFromFile(QString filePath)
{
    if(filePath.isNull())
        filePath = fileName;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);

    quint32 magic;
    stream >> magic;
    if(magic != SETUP_FILE_KEY) {
        QMessageBox msgBox;
        msgBox.setText(tr("Not a setup file."));
        msgBox.exec();
        return false;
    }

    quint32 version;
    stream >> version;
    if(version != SETUP_FILE_VERSION) {
        QMessageBox msgBox;
        msgBox.setText(tr("Wrong file version."));
        msgBox.exec();
        return false;
    }

    stream.setVersion(QDataStream::Qt_4_6);

    MainHost *host = MainHost::Get();
    host->EnableSolverUpdate(false);
    host->SetupHostContainer();
    host->hostContainer->LoadProgram(TEMP_PROGRAM);

    MainHost::Get()->filePass=0;
    stream >> *host->hostContainer;
    MainHost::Get()->filePass=1;
    stream >> *host->hostContainer;

    Connectables::ObjectFactory::Get()->ResetSavedId();

    host->hostContainer->listenProgramChanges=false;
    host->hostContainer->LoadProgram(0);

    host->EnableSolverUpdate(true);
    return true;
}
