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

#define PROJECT_FILE_VERSION 4
#define PROJECT_FILE_KEY 0x757b0a5d

#include "projectfile.h"
#include "../mainhost.h"

bool ProjectFile::SaveToFile(QString filePath)
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

    out << (quint32)PROJECT_FILE_KEY;
    out << (quint32)PROJECT_FILE_VERSION;
    out.setVersion(QDataStream::Qt_4_6);

    MainHost *host = MainHost::Get();

    host->EnableSolverUpdate(false);

    host->insertContainer->SaveProgram();
    host->programContainer->SaveProgram();
    host->parkingContainer->SaveProgram();

    for(host->filePass=0; host->filePass<LOADSAVE_STAGES ; host->filePass++) {
        out << *host->parkingContainer;
        out << *host->insertContainer;
        out << *host->programContainer;
    }

    out << *host->programList;

    host->EnableSolverUpdate(true);

    return true;
}

void ProjectFile::Clear()
{
    MainHost *host = MainHost::Get();
    host->EnableSolverUpdate(false);
    host->SetupInsertContainer();
    host->SetupProgramContainer();
    host->SetupParking();
    host->EnableSolverUpdate(true);
    host->programList->BuildModel();
}

bool ProjectFile::LoadFromFile(QString filePath)
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
    if(magic != PROJECT_FILE_KEY) {
        QMessageBox msgBox;
        msgBox.setText(tr("%1 is not a project file.").arg(filePath));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    quint32 version;
    in >> version;
    if(version != PROJECT_FILE_VERSION) {
        QMessageBox msgBox;
        msgBox.setText(tr("%1 : wrong file version.").arg(filePath));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    in.setVersion(QDataStream::Qt_4_6);

    MainHost *host = MainHost::Get();

    host->EnableSolverUpdate(false);

    host->SetupInsertContainer();
    host->SetupProgramContainer();
    host->SetupParking();

    for(host->filePass=0; host->filePass<LOADSAVE_STAGES ; host->filePass++) {
        in >> *host->parkingContainer;
        in >> *host->insertContainer;
        in >> *host->programContainer;
    }

    Connectables::ObjectFactory::Get()->ResetSavedId();
    in >> *host->programList;
    host->EnableSolverUpdate(true);

    return true;
}

