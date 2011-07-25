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

#include "comremoveprogram.h"
#include "models/programsmodel.h"

ComRemoveProgram::ComRemoveProgram(ProgramsModel *model,
                                   int row,
                                   int groupNum,
                                   QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    groupNum(groupNum),
    done(false)

{
    setText(QObject::tr("Remove program"));

    done=true;
    redo();
    done=false;
}

void ComRemoveProgram::undo()
{
    QModelIndex prgIndex;
    if(!model->AddProgram(groupNum,prgIndex,row))
        return;

    QDataStream stream(&data, QIODevice::ReadOnly);
    model->ProgramFromStream(stream,prgIndex);
}

void ComRemoveProgram::redo()
{
    if(!done) {
        done=true;
        return;
    }

    QDataStream stream(&data, QIODevice::WriteOnly);
    model->ProgramToStream(stream, model->index(groupNum,0).child(row,0) );

    model->fromCom=true;
    model->removeRow(row, model->index(groupNum,0) );
    model->fromCom=false;
}
