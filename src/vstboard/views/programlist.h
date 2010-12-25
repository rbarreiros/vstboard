#ifndef PROGRAMLIST_H
#define PROGRAMLIST_H

//#include <QWidget>
#include "precomp.h"
#include "globals.h"

namespace Ui {
    class ProgramList;
}

class ProgramList : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramList(QWidget *parent = 0);
    ~ProgramList();

    void SetModel(QStandardItemModel *model);
    void writeSettings();
    void readSettings();
    void resetSettings();

private:
    Ui::ProgramList *ui;
    QStandardItemModel *model;
    int currentGrp;
    int currentPrg;
    int currentGrpDragging;

signals:
    void ChangeProg(const QModelIndex &index);
    void ProgAutoSave(const Autosave::Enum state);
    void GroupAutoSave(const Autosave::Enum state);

public slots:
    void OnProgChange(const QModelIndex &index);
    void OnDragOverGroups( QWidget *source, const QModelIndex & index);
    void OnGrpStartDrag(const QModelIndex & index);
    void ShowCurrentGroup();
    void rowsInserted ( const QModelIndex & parent, int start, int end );
    void OnProgAutoSaveChanged(const Autosave::Enum state);
    void OnGroupAutoSaveChanged(const Autosave::Enum state);

private slots:
    void on_grpAutosaveOff_clicked();
    void on_grpAutosaveAsk_clicked();
    void on_grpAutosaveOn_clicked();
    void on_progAutosaveOff_clicked();
    void on_progAutosaveAsk_clicked();
    void on_progAutosaveOn_clicked();
    void on_listProgs_clicked(QModelIndex index);
    void on_listGrps_clicked(QModelIndex index);
};

#endif // PROGRAMLIST_H
