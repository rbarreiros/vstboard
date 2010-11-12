#include "programlist.h"
#include "ui_programlist.h"

ProgramList::ProgramList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramList),
    model(0)
{
    currentGrp=0;
    currentPrg=0;
    currentGrpDragging=-1;
    ui->setupUi(this);

    ui->listGrps->setDragDropMode(QAbstractItemView::DragDrop);
    ui->listGrps->setDefaultDropAction(Qt::MoveAction);
    ui->listGrps->setFrameShape(QFrame::NoFrame);

    connect( ui->listGrps, SIGNAL(DragOverItemFromWidget(QWidget*,QModelIndex)),
             this, SLOT(OnDragOverGroups(QWidget*,QModelIndex)));
    connect( ui->listGrps, SIGNAL(StartDrag(QModelIndex)),
             this,SLOT(OnGrpStartDrag(QModelIndex)));
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
     ui->listProgs->setRootIndex( model->invisibleRootItem()->child(0)->child(0)->index() );

    connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(rowsInserted(QModelIndex,int,int)));
}

void ProgramList::OnDragOverGroups( QWidget *source, QModelIndex index)
{
    if(source == ui->listProgs) {
        ui->listGrps->setCurrentIndex(index);
        ui->listGrps->scrollTo(index);
        ui->listProgs->setRootIndex( index.child(0,0) );
    }
}

void ProgramList::OnGrpStartDrag(QModelIndex index)
{
    currentGrpDragging=index.row();
}

void ProgramList::rowsInserted ( const QModelIndex & parent, int start, int end )
{
    if(currentGrpDragging!=-1) {
        if(currentGrpDragging == currentGrp) {
            if(start > currentGrp)
                currentGrp = start-1;
            else
                currentGrp = start;
        } else {
            if(currentGrpDragging < currentGrp && start > currentGrp)
                currentGrp--;
            if(currentGrpDragging > currentGrp && start < currentGrp)
                currentGrp++;
        }
        currentGrpDragging=-1;
        QTimer::singleShot(0, this, SLOT( ShowCurrentGroup()) );
    }
}

void ProgramList::ShowCurrentGroup()
{
    QModelIndex idx = model->index(currentPrg, 0, model->index(currentGrp,0).child(0,0) );
    OnProgChange(idx);
}

void ProgramList::OnProgChange(const QModelIndex &index)
{
    //change group if needed
    QModelIndex parIndex = index.parent().parent();
    if(!parIndex.isValid()) {
        debug("ProgramList::OnProgChange invalid parent")
        return;
    }

    currentGrp = parIndex.row();
    currentPrg = index.row();

    ui->listGrps->setCurrentIndex(parIndex);
    ui->listGrps->scrollTo(parIndex);

    ui->listProgs->setRootIndex( index.parent() );

    //select current program
    ui->listProgs->setCurrentIndex(index);
    ui->listProgs->scrollTo(index);
}

void ProgramList::on_listGrps_clicked(QModelIndex index)
{
    QModelIndex newProg = index.child(0,0).child( currentPrg ,0 );
    if(!newProg.isValid()) {
        debug("ProgramList::on_listGrps_clicked invalid prog")

    }
    emit ChangeProg( newProg );
}

void ProgramList::on_listProgs_clicked(QModelIndex index)
{
    emit ChangeProg(index);
}
