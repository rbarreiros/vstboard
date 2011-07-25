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

#include "comchangeautosave.h"
#include "models/programsmodel.h"

ComChangeAutosave::ComChangeAutosave(ProgramsModel *model,
                                     int type,
                                     Qt::CheckState newState,
                                     QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    type(type),
    newState(newState)
{
    setText(QObject::tr("Change Autosave"));

    if(type==0) {
        oldState = model->groupAutosaveState;
    }
    if(type==1) {
        oldState = model->progAutosaveState;
    }
}

void ComChangeAutosave::undo()
{
    model->fromCom=true;
    if(type==0) {
        model->UserChangeGroupAutosave(oldState);
    }
    if(type==1) {
        model->UserChangeProgAutosave(oldState);
    }
    model->fromCom=false;
}

void ComChangeAutosave::redo()
{
    model->fromCom=true;
    if(type==0) {
        model->UserChangeGroupAutosave(newState);
    }
    if(type==1) {
        model->UserChangeProgAutosave(newState);
    }
    model->fromCom=false;
}
