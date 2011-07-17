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

#ifndef PROGRAMS_H
#define PROGRAMS_H

//#include "precomp.h"
#include "globals.h"
#include "models/programsmodel.h"
#include "views/viewconfig.h"

class MainWindow;
class MainHost;
class Optimizer;
class Programs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int prog READ GetCurrentMidiProg WRITE ChangeProg)
    Q_PROPERTY(int group READ GetCurrentMidiGroup WRITE ChangeGroup)
    Q_PROPERTY(int nbProgs READ GetNbOfProgs)
    Q_PROPERTY(int nbGroups READ GetNbOfGroups)


public:
    explicit Programs(MainHost *parent = 0);
    void BuildModel();
    ProgramsModel *GetModel() {return model;}

    bool RemoveIndex(const QModelIndex &index);
    bool userWantsToUnloadGroup();
    bool userWantsToUnloadProgram();
    bool isDirty() {return projectDirty;}

    inline int GetCurrentMidiGroup() const {
        if(!currentGrp.isValid())
            return 0;
        return currentGrp.row();
    }

    inline int GetCurrentMidiProg() const {
        if(!currentPrg.isValid())
            return 0;
        return currentPrg.row();
    }

    void SetMainWindow(MainWindow *win) {mainWindow=win;}

    bool ChangeProgNow(int midiGroupNum, int midiProgNum);

    int GetNbOfProgs();
    int GetNbOfGroups();

    bool GetIndexFromProgNum(int midiGroupNum, int midiProgNum, QModelIndex &index);

    inline uint GetNextProgId() {
        return nextProgId++;
    }

    inline uint GetNextGroupId() {
        return nextGroupId++;
    }

    Optimizer *optimizer;

private:
    bool GoAwayFromIndex( const QModelIndex &index);

    QDataStream & toStream (QDataStream &);
    QDataStream & fromStream (QDataStream &);

    ProgramsModel *model;
    QPersistentModelIndex currentGrp;
    QPersistentModelIndex currentPrg;

    unsigned int nextGroupId;
    unsigned int nextProgId;
    MainHost *myHost;
    Autosave::Enum progAutosaveState;
    Autosave::Enum groupAutosaveState;

    bool projectDirty;

    MainWindow *mainWindow;
    QBrush currentProgColor;

signals:
    void ProgChanged(int prgId);
    void ProgChanged(const QModelIndex &prgIndex);
    void GroupChanged(int prgId);
    void GroupChanged(const QModelIndex &grpIndex);
    void ProgCopy(int ori, int dest);
    void GroupCopy(int ori, int dest);
    void ProgDelete(int prg);
    void GroupDelete(int prg);
    void ProgAutosaveChanged(const Autosave::Enum state);
    void GroupAutosaveChanged(const Autosave::Enum state);

public slots:
    bool ChangeProg(int midiProgNum);
    bool ChangeGroup(int midiGroupNum);
    bool ChangeProg(const QModelIndex &newPrg);
    bool ChangeGroup(const QModelIndex &newGrp);
    void SetProgAutosave(const Autosave::Enum state);
    void SetGroupAutosave(const Autosave::Enum state);

    void UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);

    friend QDataStream & operator<< (QDataStream&, Programs&);
    friend QDataStream & operator>> (QDataStream&, Programs&);
};

QDataStream & operator<< (QDataStream& out, Programs& value);
QDataStream & operator>> (QDataStream& in, Programs& value);

#endif // PROGRAMS_H
