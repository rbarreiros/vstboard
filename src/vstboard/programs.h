#ifndef PROGRAMS_H
#define PROGRAMS_H

#include "precomp.h"
#include "models/programsmodel.h"
//#include <QObject>

class MainHost;
class Programs : public QObject
{
    Q_OBJECT
public:
    explicit Programs(MainHost *parent = 0);
    void BuildModel();
    ProgramsModel *GetModel() {return model;}
    QStandardItem *CopyProgram(QStandardItem *progOri);
    QStandardItem *CopyGroup(QStandardItem *grpOri);
    void RemoveIndex(const QModelIndex &index);
    friend QDataStream & operator<< (QDataStream&, const Programs&);
    friend QDataStream & operator>> (QDataStream&, Programs&);

private:
    ProgramsModel *model;
//    QStandardItem *currentGrp;
    QStandardItem *currentPrg;
//    unsigned int currentProgId;
    unsigned int nextProgId;
    MainHost *myHost;

signals:
    void ProgChanged(const QModelIndex &prgIndex);

public slots:
    void ChangeProg(int midiPrgId);
    void ChangeGroup(int grpNum);
    void ChangeProg(const QModelIndex &prgIndex);

};

QDataStream & operator<< (QDataStream& out, const Programs& value);
QDataStream & operator>> (QDataStream& in, Programs& value);

#endif // PROGRAMS_H
