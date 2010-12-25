#include "programs.h"
#include "globals.h"
#include "mainhost.h"

Programs::Programs(MainHost *parent) :
    QObject(parent),
    model(0),
    currentGrp(0),
    currentPrg(0),
    nextGroupId(0),
    nextProgId(0),
    myHost(parent),
    progAutosaveState(Autosave::save),
    groupAutosaveState(Autosave::save)
{
    model=new ProgramsModel(parent);
}

void Programs::BuildModel()
{
    if(model)
        model->clear();

    groupAutosaveState=Autosave::save;
    progAutosaveState=Autosave::save;

    for(unsigned int grp=0; grp<1; grp++) {
        QStandardItem *grpItem = new QStandardItem(QString("Grp%1").arg(grp));
        grpItem->setData(NodeType::programGroup,UserRoles::nodeType);
        grpItem->setData(nextGroupId,UserRoles::value);
        nextGroupId++;
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);

        QStandardItem *prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);

        for(unsigned int prg=0; prg<5; prg++) {
            QStandardItem *prgItem = new QStandardItem(QString("Prg%1").arg(prg));
            prgItem->setData(NodeType::program,UserRoles::nodeType);
            prgItem->setData(nextProgId,UserRoles::value);
//            prgItem->setData(nextProgId,Qt::ToolTipRole);
            nextProgId++;
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        model->invisibleRootItem()->appendRow(grpItem);
    }
    currentGrp = model->item(0);
    currentPrg = model->item(0)->child(0)->child(0);

    emit GroupChanged( currentGrp->index() );
    emit ProgChanged( currentPrg->index() );

    emit GroupAutosaveChanged(groupAutosaveState);
    emit ProgAutosaveChanged(progAutosaveState);
}

QStandardItem *Programs::CopyProgram(QStandardItem *progOri)
{
    int oriId = progOri->data(UserRoles::value).toInt();
    emit ProgCopy(oriId,nextProgId);

    QStandardItem *prgItem = new QStandardItem(progOri->text());
    prgItem->setData(NodeType::program,UserRoles::nodeType);
    prgItem->setData(nextProgId,UserRoles::value);
    nextProgId++;
    prgItem->setDragEnabled(true);
    prgItem->setDropEnabled(false);
    prgItem->setEditable(true);

    return prgItem;
}

QStandardItem *Programs::CopyGroup(QStandardItem *grpOri)
{
    int oriId = grpOri->data(UserRoles::value).toInt();
    emit GroupCopy(oriId,nextGroupId);

    QStandardItem *grpItem = new QStandardItem(grpOri->text());
    grpItem->setData(NodeType::programGroup,UserRoles::nodeType);
    grpItem->setData(nextGroupId,UserRoles::value);
    nextGroupId++;
    grpItem->setDragEnabled(true);
    grpItem->setDropEnabled(false);
    grpItem->setEditable(true);

    QStandardItem *prgList = new QStandardItem();
    prgList->setDragEnabled(false);
    prgList->setDropEnabled(true);
    prgList->setEditable(false);

    QStandardItem *prgListOri = grpOri->child(0);
    for(int prg=0; prg<prgListOri->rowCount(); prg++) {
        prgList->appendRow( CopyProgram( prgListOri->child(prg) ) );
    }

    grpItem->appendRow(prgList);
    return grpItem;
}

void Programs::RemoveIndex(const QModelIndex &index)
{
    if(index.data(UserRoles::nodeType).toInt() == NodeType::program) {
        int prgId = index.data(UserRoles::value).toInt();
        myHost->programContainer->RemoveProgram(prgId);
    }

    if(index.data(UserRoles::nodeType).toInt() == NodeType::programGroup) {
        QStandardItem *lstPrg = model->itemFromIndex(index.child(0,0));
        for(int i=0; i< lstPrg->rowCount(); i++) {
            QStandardItem *prg = lstPrg->child(i);
            int prgId = prg->data(UserRoles::value).toInt();
            myHost->programContainer->RemoveProgram(prgId);
        }
    }
}

void Programs::ChangeProg(const QModelIndex &prgIndex)
{
    QStandardItem *newPrg = model->itemFromIndex( prgIndex );
    if(!newPrg || newPrg==currentPrg)
        return;


    if(progAutosaveState != Autosave::discard && myHost->programContainer->IsDirty()) {

        int save=0;

        if(progAutosaveState == Autosave::save) {
            //auto save
            save = QMessageBox::Save;
        } else {
            //prompt
            QMessageBox msgBox;
            msgBox.setText(tr("The progarm has been modified."));
            msgBox.setInformativeText(tr("Do you want to save your changes?"));
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);
            save = msgBox.exec();
        }

        switch(save) {
            case QMessageBox::Cancel:
                //go back to current program
                emit ProgChanged( currentPrg->index() );
                return;
            case QMessageBox::Save:
                myHost->programContainer->SaveProgram();
        }

    }

    QStandardItem *newgrp = newPrg->parent()->parent();
    if(newgrp!=currentGrp) {

        if(groupAutosaveState != Autosave::discard && myHost->groupContainer->IsDirty()) {

            int save=0;

            if(groupAutosaveState == Autosave::save) {
                //auto save
                save = QMessageBox::Save;
            } else {
                //prompt
                QMessageBox msgBox;
                msgBox.setText(tr("The group has been modified."));
                msgBox.setInformativeText(tr("Do you want to save your changes?"));
                msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Save);
                save = msgBox.exec();
            }

            switch(save) {
                case QMessageBox::Cancel:
                    //go back to current program
                    emit ProgChanged( currentPrg->index() );
                    return;
                case QMessageBox::Save:
                    myHost->groupContainer->SaveProgram();
            }
        }

        currentGrp=newgrp;
        emit GroupChanged( currentGrp->index() );
    }

    currentPrg = newPrg;
    emit ProgChanged( currentPrg->index() );
}

void Programs::ChangeProg(int midiProgNum) {
    QStandardItem *newPrg = currentPrg->parent()->child(midiProgNum);
    if(!newPrg || newPrg==currentPrg)
        return;
    currentPrg = newPrg;

    QStandardItem *newgrp = newPrg->parent()->parent();
    if(newgrp!=currentGrp) {
        currentGrp=newgrp;
        emit GroupChanged( currentGrp->index() );
    }

    emit ProgChanged( currentPrg->index() );
}

void Programs::ChangeGroup(int grpNum)
{
    QStandardItem *newGrp = model->item(grpNum);
    if(!newGrp)
        return;

    if(newGrp==currentGrp)
        return;

    currentGrp=newGrp;


    int prg = currentPrg->row();
    QStandardItem *newPrg = currentGrp->child(0,0)->child( prg );
    if(!newPrg) {
        newPrg = currentGrp->child(0)->child(0);
        if(!newPrg)
            return;
    }

    currentPrg = newPrg;

    emit GroupChanged( currentGrp->index() );
    emit ProgChanged( currentPrg->index() );
}

void Programs::SetProgAutosave(const Autosave::Enum state)
{
    progAutosaveState=state;
}

void Programs::SetGroupAutosave(const Autosave::Enum state)
{
    groupAutosaveState=state;
}

QDataStream & Programs::toStream (QDataStream &out) const
{
    const quint16 file_version = 1;
    out << file_version;

    QStandardItem *root = model->invisibleRootItem();
    out << (quint16)root->rowCount();
    for(int i=0; i<root->rowCount(); i++) {
        QStandardItem *grpItem = root->child(i);
        out << grpItem->text();
        out << (quint32)grpItem->data(UserRoles::value).toInt();

        QStandardItem *prgList = grpItem->child(0);
        out << (quint16)prgList->rowCount();

        for(int j=0; j<prgList->rowCount(); j++) {
            QStandardItem *prgItem = prgList->child(j);
            out << prgItem->text();
            out << (quint32)prgItem->data(UserRoles::value).toInt();
        }
    }

    out << (quint16)currentPrg->parent()->parent()->row();
    out << (quint16)currentPrg->row();

    out << (quint8)groupAutosaveState;
    out << (quint8)progAutosaveState;

    return out;
}

QDataStream & Programs::fromStream (QDataStream &in)
{
    quint16 file_version;
    in >> file_version;

    model->clear();
    currentPrg=0;
    QStandardItem *root = model->invisibleRootItem();
    quint16 nbgrp;
    in >> nbgrp;
    for(unsigned int i=0; i<nbgrp; i++) {
        QStandardItem *grpItem = new QStandardItem();
        grpItem->setData(NodeType::programGroup,UserRoles::nodeType);
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);
        QString str;
        in >> str;
        grpItem->setText(str);
        quint32 prgId;
        in >> prgId;
        grpItem->setData(prgId,UserRoles::value);

        QStandardItem *prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);

        quint16 nbprog;
        in >> nbprog;
        for(unsigned int j=0; j<nbprog; j++) {
            QStandardItem *prgItem = new QStandardItem();
            prgItem->setData(NodeType::program,UserRoles::nodeType);
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);
            QString prgStr;
            in >> prgStr;
            prgItem->setText(prgStr);
            quint32 prgId;
            in >> prgId;
            prgItem->setData(prgId,UserRoles::value);

            if(prgId>=nextProgId)
                nextProgId=prgId+1;

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        root->appendRow(grpItem);
    }

    quint16 grp;
    in >> grp;
    quint16 prg;
    in >> prg;

    currentPrg=model->item(0)->child(0)->child(0);
    ChangeGroup(grp);
    ChangeProg(prg);

    in >> (quint8&)groupAutosaveState;
    emit GroupAutosaveChanged(groupAutosaveState);

    in >> (quint8&)progAutosaveState;
    emit ProgAutosaveChanged(progAutosaveState);

    return in;
}

QDataStream & operator<< (QDataStream& out, const Programs& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream& in, Programs& value)
{
    return value.fromStream(in);
}
