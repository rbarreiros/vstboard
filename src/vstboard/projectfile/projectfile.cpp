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

#include "projectfile.h"
//#include "program.h"
#include "../mainhost.h"

using namespace Project;

ProjectFile *ProjectFile::theProjectFile=0;

ProjectFile::ProjectFile(QObject *parent) :
    QObject(parent)
{
    theProjectFile = this;
    fileName = "session.dat";
/*
    int cpt=0;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            Program *prog = new Program(cpt,this,i,j);
            listPrograms.insert(cpt,prog);
            cpt++;
        }
    }

    BuildModel();*/
}
/*
void ProjectFile::BuildModel()
{
    programModel.clear();

    QStringList headerLabels;
    headerLabels << "Group/Prog";
    programModel.setHorizontalHeaderLabels(  headerLabels );

    QStandardItem *parentItem = programModel.invisibleRootItem();

    QStandardItem *grpItem;

    foreach(Program *prog,listPrograms) {
        bool grpExists=false;
        for(int i=0;i<parentItem->rowCount();i++) {
            if(parentItem->child(i,0)->data(UserRoles::programGroup) == prog->progGroup) {
                grpExists = true;
                break;
            }
        }

        if(!grpExists) {
            grpItem = new QStandardItem(QString("Group %1").arg(prog->progGroup));
            grpItem->setData(prog->progGroup,UserRoles::programGroup);
            parentItem->appendRow(grpItem);
        }

        QStandardItem *item = new QStandardItem(QString("Prog %1").arg(prog->progIndex));
        item->setData(prog->progIndex,UserRoles::programNumber);
        grpItem->appendRow(item);
    }

}

Program* ProjectFile::GetProgram(int prog)
{
    if(listPrograms.contains(prog))
        return listPrograms.value(prog);
    return 0;
}

QStandardItem *ProjectFile::ItemFromProgId(int progId)
{
    Program *prg = listPrograms.value(progId);
    if(!prg)
        return 0;

    QStandardItem *grp = programModel.item( prg->progGroup);
    if(!grp)
        return 0;

    return grp->child(prg->progPlaceInGroup);
}
*/
void ProjectFile::SaveToFile(QString filePath)
{
    if(filePath.isNull())
        filePath = fileName;

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);

    stream << (quint32)0x757b0a5d;  //key
    stream << (quint32)1;           //version
    stream.setVersion(QDataStream::Qt_4_6);
    stream << *MainHost::Get();
}

bool ProjectFile::LoadFromFile(QString filePath)
{
    if(filePath.isNull())
        filePath = fileName;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);

    quint32 magic;
    stream >> magic;
    if(magic != 0x757b0a5d) {
        QMessageBox msgBox;
        msgBox.setText(tr("Not a project file."));
        msgBox.exec();
        return false;
    }

    quint32 version;
    stream >> version;
    if(version != 1) {
        QMessageBox msgBox;
        msgBox.setText(tr("Wrong file version."));
        msgBox.exec();
        return false;
    }

    stream.setVersion(QDataStream::Qt_4_6);
    stream >> *MainHost::Get();
    return true;
}

