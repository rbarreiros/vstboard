#ifndef PROGRAMS_H
#define PROGRAMS_H

#include "precomp.h"
//#include <QObject>

class Programs : public QObject
{
    Q_OBJECT
public:
    explicit Programs(QObject *parent = 0);
    void BuildModel();
    QStandardItemModel *GetModel() {return model;}

    friend QDataStream & operator<< (QDataStream&, const Programs&);
    friend QDataStream & operator>> (QDataStream&, Programs&);

private:
    QStandardItemModel *model;
    int currentGrp;
    int currentPrg;
    int currentProgId;

signals:
    void ProgChanged(const QModelIndex &prgIndex);

public slots:
    void ChangeProg(int midiPrgId, int grpNum=-1);
    void ChangeGroup(int grpNum);
    void ChangeProg(const QModelIndex &prgIndex);

};

QDataStream & operator<< (QDataStream& out, const Programs& value);
QDataStream & operator>> (QDataStream& in, Programs& value);

#endif // PROGRAMS_H
