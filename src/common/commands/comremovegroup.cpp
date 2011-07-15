#include "comremovegroup.h"
#include "mainhost.h"

ComRemoveGroup::ComRemoveGroup(MainHost *myHost,
                               const QModelIndexList &listToRemove,
                               QUndoCommand *parent) :
    QUndoCommand(parent),
    myHost(myHost)
{
    setText(QObject::tr("Remove group"));

    foreach(QModelIndex index, listToRemove) {
        rows << index.row();
    }

    qSort(rows);
}

void ComRemoveGroup::undo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QDataStream stream(&progData, QIODevice::ReadWrite);

    for(int i=0; i<rows.size(); i++) {
        int progId = progIds.at(i);

        QStandardItem *grpItem = new QStandardItem( progNames.at(i) );
        grpItem->setData(progId, UserRoles::value);
        grpItem->setData(NodeType::programGroup, UserRoles::nodeType);
        grpItem->setData(0,UserRoles::type);
        grpItem->setDragEnabled(true);
        grpItem->setDropEnabled(false);
        grpItem->setEditable(true);

        QStandardItem *prgList = new QStandardItem();
        prgList->setDragEnabled(false);
        prgList->setDropEnabled(true);
        prgList->setEditable(false);

        grpItem->appendRow(prgList);

        model->fromCom=true;
        model->insertRow( rows.at(i), grpItem );
        model->fromCom=false;

        myHost->groupContainer->ProgramFromStream(progId,stream);
    }

    progData.resize(0);
    progIds.clear();
    progNames.clear();
}

void ComRemoveGroup::redo()
{
    ProgramsModel *model = myHost->programList->GetModel();
    QDataStream stream(&progData, QIODevice::ReadWrite);

    if(rows.contains( myHost->programList->GetCurrentMidiProg() )) {
        //go to another program
        int i=0;
        while(i<model->rowCount() && !rows.contains(i)) {
            i++;
        }
        myHost->programList->ChangeGroup(i);
    }

    for(int i=0; i<rows.size(); i++) {
        if(rows.at(i)>=0 || rows.at(i)<model->rowCount()) {
            QModelIndex groupIndex = model->index(rows.at(i),0);
            progNames << groupIndex.data().toString();
            int progId = groupIndex.data(UserRoles::value).toInt();
            progIds << progId;
            myHost->groupContainer->ProgramToStream(progId, stream);

            if(!myHost->programList->RemoveIndex(groupIndex)) {
                rows.removeAt(i);
                i++;
            }
        } else {
            rows.removeAt(i);
            i--;
        }
    }

    model->fromCom=true;
    for(int i=rows.size()-1; i>=0; i--) {
        model->removeRows( rows.at(i), 1 );
    }
    model->fromCom=false;
}
