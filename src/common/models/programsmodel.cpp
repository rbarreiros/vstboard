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

#include "programsmodel.h"
#include "globals.h"
#include "mainhost.h"
#include "commands/comchangeprogramitem.h"
#include "commands/comremoveprogram.h"
#include "commands/comaddprogram.h"
#include "commands/comremovegroup.h"
#include "commands/comaddgroup.h"
#include "commands/comchangeprogram.h"
#include "mainwindow.h"

ProgramsModel::ProgramsModel(MainHost *parent) :
    QStandardItemModel(parent),
    myHost(parent),
    currentGrp(QModelIndex()),
    currentPrg(QModelIndex()),
    nextGroupId(1),
    nextProgId(1),
    progAutosaveState(Qt::Checked),
    groupAutosaveState(Qt::Checked),
    dirtyFlag(false),
    currentProgColor(Qt::green),
    fromCom(false),
    currentCommand(0),
    countItemsToMove(0),
    openedPrompt(false)
{
}

void ProgramsModel::NewGroup(int row)
{
    if(row<0 || row>rowCount())
        row=rowCount();

    QByteArray tmpBa;
    QDataStream stream(&tmpBa,QIODevice::WriteOnly);
    stream << QString(tr("New"));

    QUndoCommand *com = new QUndoCommand( tr("New group") );
    new ComAddGroup(this,row,&tmpBa,com);
    new ComAddProgram(this,row,0,&tmpBa,com);
    myHost->undoStack.push( com );
}

void ProgramsModel::NewProgram(int groupNum, int row)
{
    if(!item(groupNum))
        return;

    QStandardItem *grpItem = item(groupNum);
    if(!grpItem)
        return;

    if( row < 0 || row > grpItem->rowCount() )
        row=grpItem->rowCount();

    QByteArray tmpBa;
    QDataStream stream(&tmpBa,QIODevice::WriteOnly);
    stream << QString(tr("New"));

    myHost->undoStack.push( new ComAddProgram(this,groupNum,row,&tmpBa) );
}

bool ProgramsModel::AddGroup(QModelIndex &index, int row)
{
    int groupId = myHost->programsModel->GetNextGroupId();

    QString name("New grp");

    //create the group
    QStandardItem *groupItem = new QStandardItem( name );
    groupItem->setData(GroupNode,NodeType);
    groupItem->setData(groupId,ProgramId);
    groupItem->setDragEnabled(true);
    groupItem->setDropEnabled(false);
    groupItem->setEditable(true);

    if(row==-1)
        row=rowCount();
    insertRow( row, groupItem );

    index = groupItem->index();

    return true;
}

bool ProgramsModel::AddProgram(int groupNum, QModelIndex &index, int row)
{
    QStandardItem *groupItem = item(groupNum);

    int progId = myHost->programsModel->GetNextProgId();

    QString name("New prog");

    //create the program item
    QStandardItem *prgItem = new QStandardItem( name );
    prgItem->setData(ProgramNode,NodeType);
    prgItem->setData(progId,ProgramId);
    prgItem->setDragEnabled(true);
    prgItem->setDropEnabled(false);
    prgItem->setEditable(true);

    if(row==-1)
        row=groupItem->rowCount();
    groupItem->insertRow(row, prgItem);

    if(!groupItem->isSelectable()) {
        groupItem->setSelectable(false);
        groupItem->setBackground(Qt::transparent);
        groupItem->setToolTip("");
    }

    index = prgItem->index();

    myHost->programsModel->ValidateProgChange(index);

    return true;
}

bool ProgramsModel::GroupFromStream( QDataStream &stream, int row)
{
    QModelIndex grpIndex;
    if(!AddGroup(grpIndex, row))
        return false;

    if(!grpIndex.isValid())
        return false;

    QString name;
    stream >> name;
    itemFromIndex(grpIndex)->setText(name);

    //send the group data to the host
    int groupId = grpIndex.data(ProgramId).toInt();
    myHost->groupContainer->ProgramFromStream( groupId, stream );
    myHost->mainWindow->mySceneView->viewGroup->ProgramFromStream( groupId, stream );

    //add the programs
    QByteArray programs;
    stream >> programs;
    QDataStream streamProgs( &programs, QIODevice::ReadOnly);
    int i=0;
    while(!streamProgs.atEnd()) {
        if(!ProgramFromStream(streamProgs, i, grpIndex.row()))
            return false;
        ++i;
    }
    return true;
}

bool ProgramsModel::ProgramFromStream( QDataStream &stream, int row, int groupNum)
{
    QModelIndex prgIndex;
    if(!AddProgram(groupNum,prgIndex,row))
        return false;

    if(!prgIndex.isValid())
        return false;

    QString name;
    stream >> name;
    itemFromIndex(prgIndex)->setText(name);

    //send the data to the host
    int progId = prgIndex.data(ProgramId).toInt();
    myHost->programContainer->ProgramFromStream( progId, stream );
    myHost->mainWindow->mySceneView->viewProgram->ProgramFromStream( progId, stream );
    return true;
}

bool ProgramsModel::GroupToStream( QDataStream &stream, int row) const
{
    return GroupToStream(stream, index(row,0));
}

bool ProgramsModel::GroupToStream( QDataStream &stream, const QModelIndex &groupIndex) const
{
    if(!groupIndex.isValid()) {
        debug2(<<"ProgramsModel::GroupToStream invalid index")
        return false;
    }

    //add the group name
    stream << groupIndex.data().toString();

    //add group datas
    int groupId = groupIndex.data(ProgramId).toInt();
    myHost->groupContainer->ProgramToStream( groupId, stream );
    myHost->mainWindow->mySceneView->viewGroup->ProgramToStream( groupId, stream );

    //add programs datas
    QStandardItem *grpItem = itemFromIndex(groupIndex);
    if(!grpItem)
        return false;

    QByteArray programs;
    QDataStream streamProgs( &programs, QIODevice::WriteOnly);
    for(int i=0; i<grpItem->rowCount(); i++) {
        if(!ProgramToStream( streamProgs, grpItem->child(i)->index() ))
            return false;
    }
    stream << programs;
    return true;
}

bool ProgramsModel::ProgramToStream( QDataStream &stream, int row, int groupNum) const
{
    return ProgramToStream(stream, item(groupNum)->child(row)->index() );
}

bool ProgramsModel::ProgramToStream( QDataStream &stream, const QModelIndex &progIndex) const
{
    if(!progIndex.isValid()) {
        debug2(<<"ProgramsModel::ProgramToStream invalid index")
        return false;
    }

    //add the program name
    stream << progIndex.data().toString();

    //add the datas
    int progId = progIndex.data(ProgramId).toInt();
    myHost->programContainer->ProgramToStream( progId, stream );
    myHost->mainWindow->mySceneView->viewProgram->ProgramToStream( progId, stream );

    return true;
}

bool ProgramsModel::RemoveProgram( int row, int groupNum )
{
    if(!item(groupNum) ) {
        debug2(<<"ProgramsModel::RemoveProgram can't remove"<<groupNum<<row)
        return false;
    }

    return removeRow(row, item(groupNum)->index() );
}

bool ProgramsModel::RemoveGroup( int row )
{
    if(!item(row)) {
        debug2(<<"ProgramsModel::RemoveGroup can't remove"<<row)
        return false;
    }
    return removeRow(row);
}

bool ProgramsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    switch(action) {
    case Qt::CopyAction:
        currentCommand = new QUndoCommand(tr("Copy programs"));
        break;
    case Qt::MoveAction:
        currentCommand = new QUndoCommand(tr("Move programs"));
        break;
    default:
        debug2(<<"ProgramsModel::dropMimeData unsupported action"<<action)
        return false;
    }

    int countRows=0;

    if(row==-1) {
        if(parent.isValid())
            row=itemFromIndex(parent)->rowCount();
        else
            row=rowCount();
    }

    if(data->hasFormat(MIMETYPE_GROUP)) {
        QDataStream stream( &data->data(MIMETYPE_GROUP), QIODevice::ReadOnly);
        while(!stream.atEnd()) {
            QByteArray tmpBa;
            stream >> tmpBa;
            new ComAddGroup( this, row+countRows, &tmpBa, currentCommand );
            ++countRows;
        }
    } else {
        if(data->hasFormat(MIMETYPE_PROGRAM)) {
            int groupNum;
            if(parent.isValid()) {
                groupNum = parent.row();
            } else {
                //drop programs on a group
                groupNum = row;
                if(!item(groupNum)) {
                    delete currentCommand;
                    currentCommand=0;
                    return false;
                }
                row = item(groupNum)->rowCount();
            }

            QDataStream stream( &data->data(MIMETYPE_PROGRAM), QIODevice::ReadOnly);
            while(!stream.atEnd()) {
                QByteArray tmpBa;
                stream >> tmpBa;
                new ComAddProgram( this, groupNum, row+countRows, &tmpBa, currentCommand );
                ++countRows;
            }
        }
    }

    switch(action) {
    case Qt::CopyAction :
        myHost->undoStack.push( currentCommand );
        currentCommand = 0;
        countItemsToMove = 0;
        break;
    case Qt::MoveAction :
        countItemsToMove = countRows;
        break;
    }

    return true;
}

bool ProgramsModel::removeRows ( int row, int count, const QModelIndex & parent )
{
    static int removedItems = 0;

    if(fromCom) {
        //check if we can remove those rows
        for(int i=0; i<count; i++) {
            QModelIndex idx = index(row,0, parent);
            if( !myHost->programsModel->RemoveIndex( idx ) )
                count = i;
        }
        removedItems += count;

        //removing last programs from a group, it will be disabled
        if(parent.isValid()) {
            QStandardItem *parentItem = item(parent.row());
            if( parentItem->rowCount() == count ) {
                parentItem->setSelectable(false);
                parentItem->setBackground(Qt::gray);
                parentItem->setToolTip( tr("This group has no program and is disabled.\nDrag & drop some programs here to enable it.") );
            }
        }

        return QStandardItemModel::removeRows(row,count,parent);
    }

    if(currentCommand) {

        countItemsToMove-=count;

        while(count>0) {
            if(parent.isValid()) {
                if( RemoveIndex( parent.child(row+count-1,0) ) )
                    new ComRemoveProgram( this, row+count-1, parent.row(), currentCommand);
            } else {
                if( RemoveIndex( index(row+count-1,0) ) )
                    new ComRemoveGroup( this, row+count-1, currentCommand);
            }
            --count;
        }

        if(countItemsToMove==0) {
            if(removedItems)
                myHost->undoStack.push( currentCommand );
            else
                delete currentCommand;

            currentCommand=0;
            removedItems=0;
        }
        return true;
    }

    debug2(<<"ProgramsModel::removeRows remove row with no undoCommand")
    return false;
}

void ProgramsModel::removeRows ( const QModelIndexList &listToRemove, const QModelIndex & parent )
{
    QList<int>rows;
    foreach(QModelIndex index, listToRemove) {
        rows << index.row();
    }

    currentCommand = new QUndoCommand(tr("Remove programs"));
    countItemsToMove = rows.size();

    qSort(rows.begin(),rows.end(),qGreater<int>());
    foreach(int r, rows)
        removeRow( r, parent);
}

bool ProgramsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //redirect renaming to a command
    if(!fromCom && role == Qt::EditRole) {
        int groupNum=-1;
        if( index.parent().isValid() )
            groupNum=index.parent().row();

        if(index.data(role)==value)
            return true;

        myHost->undoStack.push( new ComChangeProgramItem( this, index.row(), groupNum, index.data(role), value, role ) );
        return true;
    }

    return QStandardItemModel::setData(index,value,role);
}

QStringList ProgramsModel::mimeTypes () const
{
    QStringList types;
    types << MIMETYPE_PROGRAM;
    types << MIMETYPE_GROUP;
    return types;

}

QMimeData * ProgramsModel::mimeData ( const QModelIndexList & indexes ) const
{
    QByteArray groups;
    QDataStream streamGroup( &groups, QIODevice::WriteOnly);
    QByteArray programs;
    QDataStream streamProg( &programs, QIODevice::WriteOnly);

    foreach(QModelIndex index, indexes) {
        if(index.data(NodeType).toInt() == GroupNode) {
            QByteArray tmpBa;
            QDataStream tmpStream(&tmpBa, QIODevice::WriteOnly);
            GroupToStream(tmpStream,index);
            streamGroup << tmpBa;
        }
        if(index.data(NodeType).toInt() == ProgramNode) {
            QByteArray tmpBa;
            QDataStream tmpStream(&tmpBa, QIODevice::WriteOnly);
            ProgramToStream(tmpStream,index);
            streamProg << tmpBa;
        }
    }
    QMimeData *mime = new QMimeData();
    if(!groups.isEmpty())
        mime->setData(MIMETYPE_GROUP, groups);
    if(!programs.isEmpty())
        mime->setData(MIMETYPE_PROGRAM, programs);
    return mime;
}

void ProgramsModel::UserChangeProg(const QModelIndex &newPrg)
{
    if(newPrg==currentPrg)
        return;

    if(!newPrg.isValid() || !newPrg.parent().isValid()) {
        debug2(<<"ProgramsModel::UserChangeProg invalid prog")
        return;
    }

    if(myHost->undoProgramChanges()) {
        myHost->undoStack.push( new ComChangeProgram(this,
                                                     currentGrp.row(),
                                                     currentPrg.row(),
                                                     newPrg.parent().row(),
                                                     newPrg.row()) );
    } else {
        ValidateProgChange(newPrg);
    }
}

void ProgramsModel::UserChangeGroup(const QModelIndex &newGrp)
{
    QModelIndex newPrg = newGrp.child(0,0);
    if(currentPrg.isValid() && newGrp.child( currentPrg.row(),0 ).isValid() ) {
        newPrg = newGrp.child( currentPrg.row(),0 );
    }
    UserChangeProg(newPrg);
}

void ProgramsModel::UserChangeProg(int prg)
{
    UserChangeProg( currentPrg.sibling(prg,0) );
}

void ProgramsModel::UserChangeGroup(int grp)
{
    UserChangeGroup( index(grp,0) );
}

bool ProgramsModel::ValidateProgChange(const QModelIndex &newPrg)
{
    if(!newPrg.isValid()) {
        debug("ProgramsModel::ValidateProgChange invalid program")
        return false;
    }

    if(newPrg==currentPrg)
        return false;

    if(!userWantsToUnloadProgram()) {
        emit ProgChanged( currentPrg );
        emit ProgChanged( currentPrg.data(ProgramId).toInt() );
        return false;
    }

    QModelIndex newgrp = newPrg.parent();
    if(newgrp!=currentGrp) {
        if(!userWantsToUnloadGroup()) {
            emit ProgChanged( currentPrg );
            emit ProgChanged( currentPrg.data(ProgramId).toInt() );
            return false;
        }

        if(currentGrp.isValid()) {
            itemFromIndex( currentGrp )->setBackground(Qt::transparent);
        }

        currentGrp=newgrp;

        QStandardItem *newGrpItem = itemFromIndex(currentGrp);
        if(newGrpItem) {
            newGrpItem->setBackground(currentProgColor);
        }

        emit GroupChanged( currentGrp );
        emit GroupChanged( currentGrp.data(ProgramId).toInt() );
    }


    if(currentPrg.isValid()) {
        itemFromIndex(currentPrg)->setBackground(Qt::transparent);
    }

    itemFromIndex(newPrg)->setBackground(currentProgColor);

    currentPrg = newPrg;
    emit ProgChanged( currentPrg );
    emit ProgChanged( currentPrg.data(ProgramId).toInt() );

    SetDirty();
    return true;
}

/*!
  change program, returns when the program is fully loaded
  */
bool ProgramsModel::ChangeProgNow(int midiGroupNum, int midiProgNum)
{
    if( !index(midiGroupNum,0).child(midiProgNum,0).isValid() )
        return false;

    //if the program has not been changed, just return
    if( !ValidateProgChange( index(midiGroupNum,0).child(midiProgNum,0) ) )
        return false;

    //if program changed, force the host to update
    myHost->UpdateSolverNow();
    return true;
}

void ProgramsModel::UserChangeProgAutosave(const Qt::CheckState state)
{
    progAutosaveState=state;
    SetDirty();
}

void ProgramsModel::UserChangeGroupAutosave(const Qt::CheckState state)
{
    groupAutosaveState=state;
    SetDirty();
}

bool ProgramsModel::userWantsToUnloadGroup()
{
    if(openedPrompt)
        return false;

    if(groupAutosaveState == Qt::Unchecked)
        return true;

    if(!myHost->groupContainer->IsDirty())
        return true;

    if(groupAutosaveState == Qt::Checked) {
        myHost->groupContainer->SaveProgram();
        SetDirty();
        return true;
    }

    //prompt
    QMessageBox msgBox;
    openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The group has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res = msgBox.exec();
    openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Save:
            myHost->groupContainer->SaveProgram();
            SetDirty();
            return true;
    }

    return true;
}

bool ProgramsModel::userWantsToUnloadProgram()
{
    if(openedPrompt)
        return false;

    if(progAutosaveState == Qt::Unchecked)
        return true;

    if(!myHost->programContainer->IsDirty())
        return true;

    if(progAutosaveState == Qt::Checked) {
        myHost->programContainer->SaveProgram();
        SetDirty();
        return true;
    }

    //prompt
    QMessageBox msgBox;
    openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The progarm has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res = msgBox.exec();
    openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Save:
            myHost->programContainer->SaveProgram();
            SetDirty();
            return true;
    }

    return true;
}

bool ProgramsModel::userWantsToUnloadProject()
{
    if(openedPrompt)
        return false;

    Qt::CheckState onUnsaved = (Qt::CheckState)myHost->GetSetting("onUnsavedProject",Qt::PartiallyChecked).toInt();

    if(onUnsaved == Qt::Unchecked)
        return true;

    if(!userWantsToUnloadProgram())
        return false;

    if(!userWantsToUnloadGroup())
        return false;

    if(!IsDirty())
        return true;

    if(onUnsaved == Qt::Checked) {
        myHost->SaveProjectFile();
        return true;
    }

    //prompt
    QMessageBox msgBox;
    openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The project has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res=msgBox.exec();
    openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel :
            return false;
        case QMessageBox::Save :
            myHost->SaveProjectFile();
            return true;
    }

    return true;
}

bool ProgramsModel::userWantsToUnloadSetup()
{
    if(openedPrompt)
        return false;

    Qt::CheckState onUnsaved = (Qt::CheckState)myHost->GetSetting("onUnsavedSetup",Qt::PartiallyChecked).toInt();

    if(onUnsaved == Qt::Unchecked)
        return true;

    if(!myHost->hostContainer->IsDirty())
        return true;

    if(onUnsaved == Qt::Checked) {
        myHost->SaveSetupFile();
        return true;
    }

    //prompt
    QMessageBox msgBox;
    openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The setup has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res=msgBox.exec();
    openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel :
            return false;
        case QMessageBox::Save :
            myHost->SaveSetupFile();
            break;
        default :
            break;
    }

    return true;
}


/*!
  move to another program or another group (before deletion)
  */
bool ProgramsModel::GoAwayFromIndex( const QModelIndex &idx)
{
    QModelIndex target;

    if(idx == currentPrg) {
        //next program valid ?
        target = currentPrg.sibling( currentPrg.row()+1, 0 );
        if(target.isValid())
            return ValidateProgChange( target );

        //previous program ?
        target = currentPrg.sibling( currentPrg.row()-1, 0 );
        if(target.isValid())
            return ValidateProgChange( target );
    }

    //no valid program in this group

    //try next group
    QModelIndex groupTarget = index( currentGrp.row()+1, 0 );
    if(groupTarget.isValid()) {
        target = index( currentPrg.row(), 0, groupTarget );
        if(target.isValid())
            return ValidateProgChange( target );

        target = index( currentPrg.row()+1, 0, groupTarget );
        if(target.isValid())
            return ValidateProgChange( target );

        target = index( currentPrg.row()-1, 0, groupTarget );
        if(target.isValid())
            return ValidateProgChange( target );
    }

    //try previous group
    groupTarget = index( currentGrp.row()-1, 0 );
    if(groupTarget.isValid()) {
        target = index( currentPrg.row(), 0, groupTarget );
        if(target.isValid())
            return ValidateProgChange( target );

        target = index( currentPrg.row()+1, 0, groupTarget );
        if(target.isValid())
            return ValidateProgChange( target );

        target = index( currentPrg.row()-1, 0, groupTarget );
        if(target.isValid())
            return ValidateProgChange( target );
    }

    debug2(<<"ProgramsModel::GoAwayFromIndex no good prog found"<<currentGrp.row()<<currentPrg.row())
    return false;
}

bool ProgramsModel::RemoveIndex(const QModelIndex &idx)
{
    if(openedPrompt)
        return false;

    if(idx.data(NodeType).toInt() == ProgramNode) {

        //move to another program
        if(idx == currentPrg && !GoAwayFromIndex(idx))
            return false;

        //delete program
        int prgId = idx.data(ProgramId).toInt();
        emit ProgDelete(prgId);

        SetDirty();
        return true;
    }
    if(idx.data(NodeType).toInt() == GroupNode) {

        //move to another program
        if(idx == currentGrp && !GoAwayFromIndex(idx))
            return false;

        //delete all programs from group
        QStandardItem *lstPrg = itemFromIndex(idx);
        for(int i=0; i< lstPrg->rowCount(); i++) {
            QStandardItem *prg = lstPrg->child(i);
            int prgId = prg->data(ProgramId).toInt();
            emit ProgDelete(prgId);
        }

        //delete group
        int grpId = idx.data(ProgramId).toInt();
        emit GroupDelete(grpId);

        SetDirty();
        return true;
    }

    debug("ProgramsModel::RemoveIndex unknown type")
    return false;
}

void ProgramsModel::BuildDefaultModel()
{
    clear();

    nextGroupId=1;
    nextProgId=1;

    invisibleRootItem()->setDropEnabled(true);

    for(unsigned int grp=0; grp<3; grp++) {
        int groupId = GetNextGroupId();
        QStandardItem *grpItem = new QStandardItem(QString("Grp%1").arg(grp));
        grpItem->setData(ProgramsModel::GroupNode,ProgramsModel::NodeType);
        grpItem->setData(groupId,ProgramsModel::ProgramId);
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);

        for(unsigned int prg=0; prg<5; prg++) {
            int progId = GetNextProgId();
            QStandardItem *prgItem = new QStandardItem(QString("Prg%1").arg(prg));
            prgItem->setData(ProgramsModel::ProgramNode,ProgramsModel::NodeType);
            prgItem->setData(progId,ProgramsModel::ProgramId);
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);
            grpItem->appendRow(prgItem);
        }
        appendRow(grpItem);
    }
    currentGrp = item(0)->index();
    currentPrg = item(0)->child(0)->index();
    item(0)->setBackground(currentProgColor);
    item(0)->child(0)->setBackground(currentProgColor);

    emit GroupChanged( currentGrp );
    emit GroupChanged( currentGrp.data(ProgramsModel::ProgramId).toInt() );
    emit ProgChanged( currentPrg );
    emit ProgChanged( currentPrg.data(ProgramsModel::ProgramId).toInt() );

    groupAutosaveState=Qt::Checked;
    progAutosaveState=Qt::Checked;
    emit GroupAutosaveChanged(groupAutosaveState);
    emit ProgAutosaveChanged(progAutosaveState);

    SetDirty(false);
}


QDataStream & ProgramsModel::toStream (QDataStream &out)
{
    out << (quint16)rowCount();
    for(int i=0; i<rowCount(); i++) {
        QStandardItem *grpItem = item(i);
        out << grpItem->text();
        out << (quint32)grpItem->data(ProgramId).toInt();

        QStandardItem *prgList = grpItem;

        out << (quint16)prgList->rowCount();
        for(int j=0; j<prgList->rowCount(); j++) {
            QStandardItem *prgItem = prgList->child(j);
            out << prgItem->text();
            out << (quint32)prgItem->data(ProgramId).toInt();
        }
    }

    out << (quint16)currentGrp.row();
    out << (quint16)currentPrg.row();
    out << (quint8)groupAutosaveState;
    out << (quint8)progAutosaveState;

    SetDirty(false);
    return out;
}

QDataStream & ProgramsModel::fromStream (QDataStream &in)
{
    clear();

    quint16 nbgrp;
    in >> nbgrp;
    for(unsigned int i=0; i<nbgrp; i++) {
        QStandardItem *grpItem = new QStandardItem();
        grpItem->setData(GroupNode,NodeType);
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);

        QString str;
        in >> str;
        grpItem->setText(str);

        quint32 groupId;
        in >> groupId;
        if(groupId>=nextGroupId)
            nextGroupId=groupId+1;
        grpItem->setData(groupId,ProgramsModel::ProgramId);

        quint16 nbprog;
        in >> nbprog;
        for(unsigned int j=0; j<nbprog; j++) {
            QStandardItem *prgItem = new QStandardItem();
            prgItem->setData(ProgramNode,NodeType);
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);

            QString prgStr;
            in >> prgStr;
            prgItem->setText(prgStr);

            quint32 prgId;
            in >> prgId;
            if(prgId>=nextProgId)
                nextProgId=prgId+1;
            prgItem->setData(prgId,ProgramsModel::ProgramId);

            grpItem->appendRow(prgItem);
        }
        appendRow(grpItem);
    }

    quint16 grp;
    in >> grp;
    quint16 prg;
    in >> prg;

    currentGrp=QModelIndex();
    currentPrg=QModelIndex();

    ValidateProgChange( index(grp,0).child(prg,0) );

    in >> (quint8&)groupAutosaveState;
    emit GroupAutosaveChanged(groupAutosaveState);

    in >> (quint8&)progAutosaveState;
    emit ProgAutosaveChanged(progAutosaveState);

    SetDirty(false);
    return in;
}


QDataStream & operator<< (QDataStream& out, ProgramsModel& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream& in, ProgramsModel& value)
{
    return value.fromStream(in);
}

void ProgramsModel::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId==ColorGroups::Programs && colorId==Colors::HighlightBackground) {
        currentProgColor = color;
        if(currentGrp.isValid())
            itemFromIndex( currentGrp )->setBackground(currentProgColor);
        if(currentPrg.isValid())
            itemFromIndex( currentPrg )->setBackground(currentProgColor);
    }
}
