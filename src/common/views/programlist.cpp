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

#include "programlist.h"
#include "mainhost.h"
#include "ui_programlist.h"
#include "commands/comchangeprogram.h"
#include "commands/comchangegroup.h"

ProgramList::ProgramList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramList),
    model(0),
    myHost(0)
{
    ui->setupUi(this);

    connect( ui->listGrps, SIGNAL(DragOverItemFromWidget(QModelIndex)),
             this, SLOT(OnGroupHovered(QModelIndex)));
    connect( ui->listProgs, SIGNAL(DragFinished()),
             this, SLOT(BackToCurrentGroup()));
}

ProgramList::~ProgramList()
{
    delete ui;
}

void ProgramList::SetModel(MainHost *myHost, QAbstractItemModel *model)
{
    this->myHost=myHost;
    this->model=model;
    ui->listGrps->setModel(model);
    ui->listProgs->setModel(model);
}

void ProgramList::OnGroupHovered(const QModelIndex & index)
{
    ui->listGrps->setCurrentIndex(index);
    ui->listGrps->scrollTo(index);
    ui->listProgs->setRootIndex( index.child(0,0) );
}

void ProgramList::BackToCurrentGroup()
{
    ui->listGrps->setCurrentIndex( currentPrg.parent().parent() );
    ui->listGrps->scrollTo( currentPrg.parent().parent() );
    ui->listGrps->selectionModel()->clear();

    ui->listProgs->setRootIndex( currentPrg.parent() );
    ui->listGrps->setCurrentIndex( currentPrg );
    ui->listGrps->scrollTo( currentPrg );
    ui->listProgs->selectionModel()->clear();
}

void ProgramList::OnProgChange(const QModelIndex &index)
{
    if(!index.isValid()) {
        debug("ProgramList::OnProgChange invalid index")
        return;
    }

    //change group if needed
    QModelIndex parIndex = index.parent().parent();
    if(!parIndex.isValid()) {
        debug("ProgramList::OnProgChange invalid group")
        return;
    }

    currentPrg = index;

    ui->listGrps->scrollTo(parIndex);
    ui->listProgs->setRootIndex( currentPrg.parent() );
    ui->listProgs->scrollTo(currentPrg);
}

void ProgramList::on_listGrps_activated(QModelIndex index)
{
    if(index==currentPrg.parent().parent())
        return;

    if(myHost->undoProgramChanges()) {
        myHost->undoStack.push( new ComChangeGroup(myHost, index) );
    } else {
        emit ChangeGroup(index);
    }
}

void ProgramList::on_listProgs_activated(QModelIndex index)
{
    if(index==currentPrg)
        return;

    if(myHost->undoProgramChanges()) {
        myHost->undoStack.push( new ComChangeProgram(myHost, index) );
    } else {
        emit ChangeProg(index);
    }
}

void ProgramList::writeSettings(MainHost *myHost)
{
    myHost->SetSetting("ProgramList/geometry", saveGeometry());
    myHost->SetSetting("ProgramList/state", ui->splitter->saveState());
}

void ProgramList::readSettings(MainHost *myHost)
{
    if(myHost->SettingDefined("ProgramList/geometry")) {
        restoreGeometry(myHost->GetSetting("ProgramList/geometry").toByteArray());
        ui->splitter->restoreState(myHost->GetSetting("ProgramList/state").toByteArray());
    } else {
        resetSettings();
    }
}

void ProgramList::resetSettings()
{
    int w = ui->splitter->width();
    QList<int>widths;
    widths << w << w;
    ui->splitter->setSizes(widths);
}

void ProgramList::on_progAutosaveOn_clicked()
{
    emit ProgAutoSave(Autosave::save);
}

void ProgramList::on_progAutosaveAsk_clicked()
{
    emit ProgAutoSave(Autosave::prompt);
}

void ProgramList::on_progAutosaveOff_clicked()
{
    emit ProgAutoSave(Autosave::discard);
}

void ProgramList::on_grpAutosaveOn_clicked()
{
    emit GroupAutoSave(Autosave::save);
}

void ProgramList::on_grpAutosaveAsk_clicked()
{
    emit GroupAutoSave(Autosave::prompt);
}

void ProgramList::on_grpAutosaveOff_clicked()
{
    emit GroupAutoSave(Autosave::discard);
}

void ProgramList::OnProgAutoSaveChanged(const Autosave::Enum state)
{
    switch(state) {
    case Autosave::save :
        ui->progAutosaveOn->setChecked(true);
        break;
    case Autosave::discard :
        ui->progAutosaveOff->setChecked(true);
        break;
    default :
        ui->progAutosaveAsk->setChecked(true);
        break;
    }
}

void ProgramList::OnGroupAutoSaveChanged(const Autosave::Enum state)
{
    switch(state) {
    case Autosave::save :
        ui->grpAutosaveOn->setChecked(true);
        break;
    case Autosave::discard :
        ui->grpAutosaveOff->setChecked(true);
        break;
    default :
        ui->grpAutosaveAsk->setChecked(true);
        break;
    }
}
