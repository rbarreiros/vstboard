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

#include "comdiscardchanges.h"
#include "models/programsmodel.h"

ComDiscardChanges::ComDiscardChanges(ProgramsModel *model,
                                     int row,
                                     int groupNum,
                                     QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    groupNum(groupNum),
    done(false)

{
    setText(QObject::tr("Discard changes"));


}

void ComDiscardChanges::undo()
{
    QModelIndex idx = model->index(row,0, model->index(groupNum,0) );

    QDataStream stream(&data, QIODevice::ReadOnly);
    if(groupNum==-1) {
        model->GroupFromStream(stream,idx);
    } else {
        model->ProgramFromStream(stream,idx);
    }
}

void ComDiscardChanges::redo()
{
    QDataStream stream(&data, QIODevice::WriteOnly);
    if(groupNum==-1)
        model->GroupToStream(stream, model->index(row,0) );
    else {
        model->ProgramToStream(stream, model->index(groupNum,0).child(row,0) );
    }
}
