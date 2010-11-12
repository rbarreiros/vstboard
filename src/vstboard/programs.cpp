#include "programs.h"
#include "globals.h"

Programs::Programs(QObject *parent) :
    QObject(parent),
    model(0),
    currentGrp(0),
    currentPrg(0)
{
    model=new QStandardItemModel(this);
    BuildModel();
}

void Programs::BuildModel()
{
    if(model)
        model->clear();

    for(int grp=0; grp<128; grp++) {
        QStandardItem *grpItem = new QStandardItem(QString("Group%1").arg(grp));
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);

        QStandardItem *prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);

        for(int prg=0; prg<128; prg++) {
            QStandardItem *prgItem = new QStandardItem(QString("Prog%1").arg(prg));
            prgItem->setData(grp*128 + prg,UserRoles::value);
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        model->invisibleRootItem()->appendRow(grpItem);
    }
}

void Programs::ChangeProg(int midiProgNum, int grpNum) {
    if(grpNum!=-1)
        currentGrp=grpNum;

    currentPrg = midiProgNum;
    currentProgId = currentGrp*128 + currentPrg;

    QStandardItem *grpItem = model->invisibleRootItem()->child(currentGrp);
    if(!grpItem) {
        debug("Programs::ChangeProg prog not found")
        return;
    }

    emit ProgChanged( grpItem->child(0)->child(currentPrg)->index() );
}

void Programs::ChangeGroup(int grpNum)
{
    currentGrp=grpNum;
    currentProgId = currentGrp*128 + currentPrg;

    QStandardItem *grpItem = model->invisibleRootItem()->child(currentGrp);
    emit ProgChanged( grpItem->child(0)->child(currentPrg)->index() );
}

void Programs::ChangeProg(const QModelIndex &prgIndex)
{
    currentPrg = prgIndex.row();
    currentGrp = prgIndex.parent().parent().row();
    currentProgId = prgIndex.data(UserRoles::value).toInt();
    emit ProgChanged( prgIndex );
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
            out << (quint16)prgItem->data(UserRoles::value).toInt();
        }
    }
    return out;
}

QDataStream & operator>> (QDataStream& in, Programs& value)
{
    value.model->clear();
    QStandardItem *root = value.model->invisibleRootItem();
    quint16 nbgrp;
    in >> nbgrp;
    for(int i=0; i<nbgrp; i++) {
        QStandardItem *grpItem = new QStandardItem();
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
        for(int j=0; j<nbprog; j++) {
            QStandardItem *prgItem = new QStandardItem();
            prgItem->setDragEnabled(true);
            prgItem->setDropEnabled(false);
            prgItem->setEditable(true);
            QString prgStr;
            in >> prgStr;
            prgItem->setText(prgStr);
            quint16 prgId;
            in >> prgId;
            prgItem->setData(prgId,UserRoles::value);

            prgList->appendRow(prgItem);
        }
        grpItem->appendRow(prgList);
        root->appendRow(grpItem);
    }
    return in;
}
