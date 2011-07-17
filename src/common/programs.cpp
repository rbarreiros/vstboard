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

#include "programs.h"
#include "globals.h"
#include "mainhost.h"
#include "mainwindow.h"


Programs::Programs(MainHost *parent) :
    QObject(parent),
    model(0),
    nextGroupId(1),
    nextProgId(1),
    myHost(parent),
    progAutosaveState(Autosave::save),
    groupAutosaveState(Autosave::save),
    projectDirty(false),
    mainWindow(0)
{
    setObjectName("Programs");
    model=new ProgramsModel(parent);

#ifdef SCRIPTENGINE
    QScriptValue scriptObj = myHost->scriptEngine->newQObject(this);
    myHost->scriptEngine->globalObject().setProperty("Programs", scriptObj);
#endif
    //currentProgColor = myHost->mainWindow->viewConfig.GetColor(View::ColorGroups::Programs,View::Colors::HighlightBackground);
//    connect( &myHost->mainWindow->viewConfig, SIGNAL(ColorChanged(View::ColorGroups::Enum,View::Colors::Enum,QColor)),
//            this, SLOT(UpdateColor(View::ColorGroups::Enum,View::Colors::Enum,QColor)) );
}

void Programs::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId==ColorGroups::Programs && colorId==Colors::HighlightBackground) {
        currentProgColor = color;
        if(currentGrp.isValid())
            model->itemFromIndex( currentGrp )->setBackground(currentProgColor);
        if(currentPrg.isValid())
            model->itemFromIndex( currentPrg )->setBackground(currentProgColor);
    }
}

void Programs::BuildModel()
{
    if(model)
        model->clear();

    nextGroupId=1;
    nextProgId=1;
    groupAutosaveState=Autosave::save;
    progAutosaveState=Autosave::save;

    for(unsigned int grp=0; grp<3; grp++) {
        int groupId = GetNextGroupId();
        QStandardItem *grpItem = new QStandardItem(QString("Grp%1").arg(grp));
        grpItem->setData(ProgramsModel::GroupNode,ProgramsModel::NodeType);
        grpItem->setData(groupId,ProgramsModel::ProgramId);
        grpItem->setData(groupId,Qt::ToolTipRole);
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);

        QStandardItem *prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);

        for(unsigned int prg=0; prg<5; prg++) {
            int progId = GetNextProgId();
            QStandardItem *prgItem = new QStandardItem(QString("Prg%1").arg(prg));
            prgItem->setData(ProgramsModel::ProgramNode,ProgramsModel::NodeType);
            prgItem->setData(progId,ProgramsModel::ProgramId);
            prgItem->setData(progId,Qt::ToolTipRole);
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        model->invisibleRootItem()->appendRow(grpItem);
    }
    currentGrp = model->item(0)->index();
    currentPrg = model->item(0)->child(0)->child(0)->index();
    model->item(0)->setBackground(currentProgColor);
    model->item(0)->child(0)->child(0)->setBackground(currentProgColor);


    emit GroupChanged( currentGrp );
    emit GroupChanged( currentGrp.data(ProgramsModel::ProgramId).toInt() );
    emit ProgChanged( currentPrg );
    emit ProgChanged( currentPrg.data(ProgramsModel::ProgramId).toInt() );

    emit GroupAutosaveChanged(groupAutosaveState);
    emit ProgAutosaveChanged(progAutosaveState);

    projectDirty=false;
}

bool Programs::userWantsToUnloadGroup()
{
    if(mainWindow->openedPrompt)
        return false;

    if(groupAutosaveState == Autosave::discard)
        return true;

    if(!myHost->groupContainer->IsDirty())
        return true;

    if(groupAutosaveState == Autosave::save) {
        myHost->groupContainer->SaveProgram();
        projectDirty=true;
        return true;
    }

    //prompt
    QMessageBox msgBox;
    mainWindow->openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The group has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res = msgBox.exec();
    mainWindow->openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Save:
            myHost->groupContainer->SaveProgram();
            projectDirty=true;
            return true;
    }

    return true;
}

bool Programs::userWantsToUnloadProgram()
{
    if(mainWindow->openedPrompt)
        return false;

    if(progAutosaveState == Autosave::discard)
        return true;

    if(!myHost->programContainer->IsDirty())
        return true;

    if(progAutosaveState == Autosave::save) {
        myHost->programContainer->SaveProgram();
        projectDirty=true;
        return true;
    }

    //prompt
    QMessageBox msgBox;
    mainWindow->openedPrompt=true;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(tr("The progarm has been modified."));
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);

    int res = msgBox.exec();
    mainWindow->openedPrompt=false;

    switch(res) {
        case QMessageBox::Cancel:
            return false;
        case QMessageBox::Save:
            myHost->programContainer->SaveProgram();
            projectDirty=true;
            return true;
    }

    return true;
}

/*!
  move to another program or another group (before deletion)
  */
bool Programs::GoAwayFromIndex( const QModelIndex &index)
{
    QModelIndex target;

    if(index == currentPrg) {
        //find another program
        int i=0;
        target = currentPrg.sibling(i,0);
        while( target.isValid() && currentPrg==target) {
            ++i;
            target = currentPrg.sibling(i,0);
        }

        if(target.isValid())
            return ChangeProg( target );
    }

    //no valid program in this group, find another group
    int i=0;
    QModelIndex groupTarget = currentGrp.sibling(i,0);
    while( groupTarget.isValid() && !target.isValid()) {
        groupTarget = currentGrp.sibling(i,0);
        if(currentGrp==groupTarget) {
            ++i;
            continue;
        }

        if(groupTarget.isValid() && groupTarget.child(0,0).isValid())
            target = groupTarget.child(0,0).child(0,0);

        ++i;
    }

    if(!target.isValid()) {
        debug2(<<"Programs::GoAwayFromIndex no good prog found"<<currentGrp.row()<<currentPrg.row())
        return false;
    }
    return ChangeProg( target );
}

bool Programs::RemoveIndex(const QModelIndex &index)
{
    if(mainWindow->openedPrompt)
        return false;

    //move to another program
    if( (index == currentPrg || index == currentGrp)
            && !GoAwayFromIndex(index) )
        return false;

    if(index.data(ProgramsModel::NodeType).toInt() == ProgramsModel::ProgramNode) {

        //delete program
        int prgId = index.data(ProgramsModel::ProgramId).toInt();
        emit ProgDelete(prgId);

        projectDirty=true;
        return true;
    } else
    if(index.data(ProgramsModel::NodeType).toInt() == ProgramsModel::GroupNode) {

        //delete all programs from group
        QStandardItem *lstPrg = model->itemFromIndex(index.child(0,0));
        for(int i=0; i< lstPrg->rowCount(); i++) {
            QStandardItem *prg = lstPrg->child(i);
            int prgId = prg->data(ProgramsModel::ProgramId).toInt();
            emit ProgDelete(prgId);
        }

        //delete group
        int grpId = index.data(ProgramsModel::ProgramId).toInt();
        emit GroupDelete(grpId);

        projectDirty=true;
        return true;
    } else {
        debug("Programs::RemoveIndex unknown type")
        return false;
    }

    return true;
}

bool Programs::ChangeProg(const QModelIndex &newPrg)
{
    if(!newPrg.isValid()) {
        debug("Programs::ChangeProg newprg not valid")
        return false;
    }

    if(newPrg==currentPrg)
        return false;

    if(!userWantsToUnloadProgram()) {
        emit ProgChanged( currentPrg );
        emit ProgChanged( currentPrg.data(ProgramsModel::ProgramId).toInt() );
        return false;
    }

    QModelIndex newgrp = newPrg.parent().parent();
    if(newgrp!=currentGrp) {
        if(!userWantsToUnloadGroup()) {
            emit ProgChanged( currentPrg );
            emit ProgChanged( currentPrg.data(ProgramsModel::ProgramId).toInt() );
            return false;
        }

        if(currentGrp.isValid()) {
            model->itemFromIndex( currentGrp )->setBackground(Qt::transparent);
        }

        currentGrp=newgrp;

        QStandardItem *newGrpItem = model->itemFromIndex(currentGrp);
        if(newGrpItem) {
            newGrpItem->setBackground(currentProgColor);
        }

        emit GroupChanged( currentGrp );
        emit GroupChanged( currentGrp.data(ProgramsModel::ProgramId).toInt() );
    }


    if(currentPrg.isValid()) {
        model->itemFromIndex(currentPrg)->setBackground(Qt::transparent);
    }

    model->itemFromIndex(newPrg)->setBackground(currentProgColor);

    currentPrg = newPrg;
    emit ProgChanged( currentPrg );
    emit ProgChanged( currentPrg.data(ProgramsModel::ProgramId).toInt() );

    projectDirty=true;
    return true;
}

bool Programs::ChangeProg(int midiProgNum) {
    if(!currentGrp.isValid())
        return false;

    QStandardItem* progList = model->itemFromIndex(currentGrp)->child(0,0);

    if(midiProgNum>=progList->rowCount())
        midiProgNum=progList->rowCount()-1;

    QModelIndex index = progList->child(midiProgNum,0)->index();
    return ChangeProg(index);
}

bool Programs::ChangeGroup(const QModelIndex &newGrp)
{
    if(!newGrp.isValid())
        return false;

    if(newGrp==currentGrp)
        return false;

    QModelIndex newPrg = newGrp.child(0,0).child(0,0);

    if(currentPrg.isValid() && newGrp.child(0,0).child( currentPrg.row(),0 ).isValid() ) {
        newPrg = newGrp.child(0,0).child( currentPrg.row(),0 );
    }

    if(!newPrg.isValid()) {
        debug("Programs::ChangeGroup prog 0 not found")
        return false;
    }

    return ChangeProg(newPrg);
}

bool Programs::ChangeGroup(int midiGroupNum)
{
    if(midiGroupNum>=model->rowCount())
        midiGroupNum=model->rowCount()-1;

    QModelIndex newGrp = model->item(midiGroupNum)->index();
    return ChangeGroup(newGrp);
}

bool Programs::GetIndexFromProgNum(int midiGroupNum, int midiProgNum, QModelIndex &index)
{
    index = QModelIndex();

    if(midiGroupNum>=model->rowCount() || midiGroupNum<0)
        return false;

    QStandardItem* progList = model->item(midiGroupNum)->child(0,0);
    if(midiProgNum>=progList->rowCount() || midiProgNum<0)
        return false;

    QStandardItem* progItem = progList->child(midiProgNum);
    if(!progItem)
        return false;

    index = progItem->index();
    return true;
}

bool Programs::ChangeProgNow(int midiGroupNum, int midiProgNum)
{
    QModelIndex index;
    if(!GetIndexFromProgNum(midiGroupNum,midiProgNum,index))
        return false;

    //if the program has not been changed, just return
    if( !ChangeProg( index ) )
        return false;

    //if program changed, force the host to update
    myHost->UpdateSolverNow();
    return true;
}

int Programs::GetNbOfProgs()
{
    if(!currentGrp.isValid())
        return 0;

    if(!currentGrp.child(0,0).isValid())
        return 0;

    return model->rowCount( currentGrp.child(0,0) );
}

int Programs::GetNbOfGroups()
{
    return model->rowCount();
}

void Programs::SetProgAutosave(const Autosave::Enum state)
{
    progAutosaveState=state;
    projectDirty=true;
}

void Programs::SetGroupAutosave(const Autosave::Enum state)
{
    groupAutosaveState=state;
    projectDirty=true;
}

QDataStream & Programs::toStream (QDataStream &out)
{
    QStandardItem *root = model->invisibleRootItem();
    out << (quint16)root->rowCount();
    for(int i=0; i<root->rowCount(); i++) {
        QStandardItem *grpItem = root->child(i);
        out << grpItem->text();
        out << (quint32)grpItem->data(ProgramsModel::ProgramId).toInt();

        QStandardItem *prgList = grpItem->child(0);
        out << (quint16)prgList->rowCount();

        for(int j=0; j<prgList->rowCount(); j++) {
            QStandardItem *prgItem = prgList->child(j);
            out << prgItem->text();
            out << (quint32)prgItem->data(ProgramsModel::ProgramId).toInt();
        }
    }

    out << (quint16)currentGrp.row();
    out << (quint16)currentPrg.row();
    out << (quint8)groupAutosaveState;
    out << (quint8)progAutosaveState;

    projectDirty=false;
    return out;
}

QDataStream & Programs::fromStream (QDataStream &in)
{
    model->clear();
    QStandardItem *root = model->invisibleRootItem();
    quint16 nbgrp;
    in >> nbgrp;
    for(unsigned int i=0; i<nbgrp; i++) {
        QStandardItem *grpItem = new QStandardItem();
        grpItem->setData(ProgramsModel::GroupNode,ProgramsModel::NodeType);
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
        grpItem->setData(groupId,Qt::ToolTipRole);

        QStandardItem *prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);

        quint16 nbprog;
        in >> nbprog;
        for(unsigned int j=0; j<nbprog; j++) {
            QStandardItem *prgItem = new QStandardItem();
            prgItem->setData(ProgramsModel::ProgramNode,ProgramsModel::NodeType);
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
            prgItem->setData(prgId,Qt::ToolTipRole);

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        root->appendRow(grpItem);
    }

    quint16 grp;
    in >> grp;
    quint16 prg;
    in >> prg;

    currentGrp=QModelIndex();
    currentPrg=QModelIndex();
    ChangeGroup(grp);
    ChangeProg(prg);


    in >> (quint8&)groupAutosaveState;
    emit GroupAutosaveChanged(groupAutosaveState);

    in >> (quint8&)progAutosaveState;
    emit ProgAutosaveChanged(progAutosaveState);

    projectDirty=false;
    return in;
}


QDataStream & operator<< (QDataStream& out, Programs& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream& in, Programs& value)
{
    return value.fromStream(in);
}
