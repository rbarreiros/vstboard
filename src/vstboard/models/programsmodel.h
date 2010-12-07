#ifndef PROGRAMSMODEL_H
#define PROGRAMSMODEL_H

#include <QStandardItemModel>
class MainHost;
class ProgramsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ProgramsModel(MainHost *parent = 0);
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
protected:
    bool movingItems;
    MainHost *myHost;
signals:

public slots:

};

#endif // PROGRAMSMODEL_H
