#ifndef PROGRAMS_H
#define PROGRAMS_H

#include "precomp.h"
#include "globals.h"
#include "models/programsmodel.h"

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
    QDataStream & toStream (QDataStream &) const;
    QDataStream & fromStream (QDataStream &);

    ProgramsModel *model;
    QStandardItem *currentGrp;
    QStandardItem *currentPrg;
//    unsigned int currentProgId;
    unsigned int nextGroupId;
    unsigned int nextProgId;
    MainHost *myHost;
    Autosave::Enum progAutosaveState;
    Autosave::Enum groupAutosaveState;

signals:
    void ProgChanged(const QModelIndex &prgIndex);
    void GroupChanged(const QModelIndex &grpIndex);
    void ProgCopy(int ori, int dest);
    void GroupCopy(int ori, int dest);
    void ProgAutosaveChanged(const Autosave::Enum state);
    void GroupAutosaveChanged(const Autosave::Enum state);

public slots:
    void ChangeProg(int midiPrgId);
    void ChangeGroup(int grpNum);
    void ChangeProg(const QModelIndex &prgIndex);
    void SetProgAutosave(const Autosave::Enum state);
    void SetGroupAutosave(const Autosave::Enum state);
};

QDataStream & operator<< (QDataStream& out, const Programs& value);
QDataStream & operator>> (QDataStream& in, Programs& value);

#endif // PROGRAMS_H
