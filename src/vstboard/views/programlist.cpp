#include <QMenu>
//#include <QStringListModel>
//#include <QSortFilterProxyModel>

#include "programlist.h"
#include "ui_programlist.h"

ProgramList::ProgramList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramList),
    model(0)
{
    ui->setupUi(this);

    ui->listGrps->setDragDropMode(QAbstractItemView::DragDrop);
    ui->listGrps->setDefaultDropAction(Qt::MoveAction);
    ui->listGrps->setFrameShape(QFrame::NoFrame);

    connect( ui->listGrps, SIGNAL(DragOverItemFromWidget(QWidget*,QModelIndex)),
             this, SLOT(OnDragOverGroups(QWidget*,QModelIndex)));
}

ProgramList::~ProgramList()
{
    delete ui;
}

void ProgramList::SetModel(QStandardItemModel *model)
{
    this->model=model;
    ui->listGrps->setModel(model);

     ui->listProgs->setModel(model);
     ui->listProgs->setRootIndex(model->invisibleRootItem()->child(0)->child(0)->index());
}

void ProgramList::OnDragOverGroups( QWidget *source, QModelIndex index)
{
    if(source == ui->listProgs) {
        //ui->listGrps->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect );
        ui->listGrps->setCurrentIndex(index);
        ui->listGrps->scrollTo(index);
        ui->listProgs->setRootIndex(index.child(0,0));
    }
}

void ProgramList::OnProgChange(const QModelIndex &index)
{
    //change group if needed
    QModelIndex parIndex = index.parent().parent();
//    ui->listGrps->selectionModel()->select(parIndex, QItemSelectionModel::ClearAndSelect );
    ui->listGrps->setCurrentIndex(parIndex);
    ui->listGrps->scrollTo(parIndex);
    ui->listProgs->setRootIndex(parIndex.child(0,0));

    //select current program
//    ui->listProgs->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect );
    ui->listProgs->setCurrentIndex(index);
    ui->listProgs->scrollTo(index);
}

void ProgramList::on_listGrps_clicked(QModelIndex index)
{
    //get the current program
//    QItemSelectionModel *sel = ui->listProgs->selectionModel();
//    QStandardItem *item = model->itemFromIndex( sel->selectedIndexes().first() );

    //move to the corresponding program in the selected group
    emit ChangeProg( index.child(0,0).child( ui->listProgs->currentIndex().row() ,0 ) );
}

void ProgramList::on_listProgs_clicked(QModelIndex index)
{
    emit ChangeProg(index);
}
