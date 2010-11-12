#include "programs.h"
#include "globals.h"
#include "mainhost.h"

Programs::Programs(QObject *parent) :
    QObject(parent),
    model(0),
    currentPrg(0),
    nextProgId(0)
{
    model=new ProgramsModel(this);
}

void Programs::BuildModel()
{
    if(model)
        model->clear();

    for(unsigned int grp=0; grp<1; grp++) {
        QStandardItem *grpItem = new QStandardItem(QString("Grp%1").arg(grp));
        grpItem->setData(NodeType::programGroup,UserRoles::nodeType);
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
            prgItem->setData(nextProgId,Qt::ToolTipRole);
            nextProgId++;
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        model->invisibleRootItem()->appendRow(grpItem);
    }
    currentPrg = model->item(0)->child(0)->child(0);
    emit ProgChanged( currentPrg->index() );
}

QStandardItem *Programs::CopyProgram(QStandardItem *progOri)
{
    int oriId = progOri->data(UserRoles::value).toInt();
    MainHost::Get()->programContainer->CopyProgram( oriId, nextProgId );

    QStandardItem *prgItem = new QStandardItem(progOri->text());
    prgItem->setData(NodeType::program,UserRoles::nodeType);
    prgItem->setData(nextProgId,UserRoles::value);
    prgItem->setData(nextProgId,Qt::ToolTipRole);
    nextProgId++;
    prgItem->setDragEnabled(true);
    prgItem->setDropEnabled(false);
    prgItem->setEditable(true);

    return prgItem;
}

QStandardItem *Programs::CopyGroup(QStandardItem *grpOri)
{
    QStandardItem *grpItem = new QStandardItem(grpOri->text());
    grpItem->setData(NodeType::programGroup,UserRoles::nodeType);
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
        MainHost::Get()->programContainer->RemoveProgram(prgId);
    }

    if(index.data(UserRoles::nodeType).toInt() == NodeType::programGroup) {
        QStandardItem *lstPrg = model->itemFromIndex(index.child(0,0));
        for(int i=0; i< lstPrg->rowCount(); i++) {
            QStandardItem *prg = lstPrg->child(i);
            int prgId = prg->data(UserRoles::value).toInt();
            MainHost::Get()->programContainer->RemoveProgram(prgId);
        }
    }
}

void Programs::ChangeProg(const QModelIndex &prgIndex)
{
    QStandardItem *newPrg = model->itemFromIndex( prgIndex );
    if(!newPrg)
        return;
    currentPrg = newPrg;
    emit ProgChanged( prgIndex );
}

void Programs::ChangeProg(int midiProgNum) {
    QStandardItem *newPrg = currentPrg->parent()->child(midiProgNum);
    if(!newPrg)
        return;

    currentPrg = newPrg;
    emit ProgChanged( newPrg->index() );
}

void Programs::ChangeGroup(int grpNum)
{
    QStandardItem *newGrp = model->item(grpNum);
    if(!newGrp)
        return;

    int prg = currentPrg->row();
    QStandardItem *newPrg = newGrp->child(0,0)->child( prg );
    if(!newPrg) {
        newPrg = newGrp->child(0)->child(0);
        if(!newPrg)
            return;
    }

    currentPrg = newPrg;

    emit ProgChanged( newPrg->index() );
}



QDataStream & operator<< (QDataStream& out, const Programs& value)
{
    QStandardItem *root = value.model->invisibleRootItem();
    out << (quint16)root->rowCount();
    for(int i=0; i<root->rowCount(); i++) {
        QStandardItem *grpItem = root->child(i);
        out << grpItem->text();

        QStandardItem *prgList = grpItem->child(0);
        out << (quint16)prgList->rowCount();

        for(int j=0; j<prgList->rowCount(); j++) {
            QStandardItem *prgItem = prgList->child(j);
            out << prgItem->text();
            out << (quint32)prgItem->data(UserRoles::value).toInt();
        }
    }

    out << (quint16)value.currentPrg->parent()->parent()->row();
    out << (quint16)value.currentPrg->row();

    return out;
}

QDataStream & operator>> (QDataStream& in, Programs& value)
{
    value.model->clear();
    value.currentPrg=0;
    QStandardItem *root = value.model->invisibleRootItem();
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
            prgItem->setData(prgId,Qt::ToolTipRole);

            if(prgId>=value.nextProgId)
                value.nextProgId=prgId+1;

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        root->appendRow(grpItem);
    }

    quint16 grp;
    in >> grp;
    quint16 prg;
    in >> prg;

    value.currentPrg=value.model->item(0)->child(0)->child(0);
    value.ChangeGroup(grp);
    value.ChangeProg(prg);

    return in;
}
