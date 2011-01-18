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

#ifndef PROGRAMLIST_H
#define PROGRAMLIST_H

//#include <QWidget>
#include "precomp.h"
#include "globals.h"

namespace Ui {
    class ProgramList;
}

class ProgramList : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramList(QWidget *parent = 0);
    ~ProgramList();

    void SetModel(QStandardItemModel *model);
    void writeSettings();
    void readSettings();
    void resetSettings();

private:
    Ui::ProgramList *ui;
    QStandardItemModel *model;
    int currentGrp;
    int currentPrg;
    int currentGrpDragging;

signals:
    void ChangeProg(const QModelIndex &index);
    void ProgAutoSave(const Autosave::Enum state);
    void GroupAutoSave(const Autosave::Enum state);

public slots:
    void OnProgChange(const QModelIndex &index);
    void OnDragOverGroups( QWidget *source, const QModelIndex & index);
    void OnGrpStartDrag(const QModelIndex & index);
    void ShowCurrentGroup();
    void rowsInserted ( const QModelIndex & parent, int start, int end );
    void OnProgAutoSaveChanged(const Autosave::Enum state);
    void OnGroupAutoSaveChanged(const Autosave::Enum state);

private slots:
    void on_grpAutosaveOff_clicked();
    void on_grpAutosaveAsk_clicked();
    void on_grpAutosaveOn_clicked();
    void on_progAutosaveOff_clicked();
    void on_progAutosaveAsk_clicked();
    void on_progAutosaveOn_clicked();
    void on_listProgs_clicked(QModelIndex index);
    void on_listGrps_clicked(QModelIndex index);
};

#endif // PROGRAMLIST_H
