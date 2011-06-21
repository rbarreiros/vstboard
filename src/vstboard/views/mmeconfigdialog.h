#ifndef MMECONFIGDIALOG_H
#define MMECONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class MmeConfigDialog;
}

class MainHost;
class MmeConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MmeConfigDialog( MainHost *myHost, QWidget *parent = 0);
    ~MmeConfigDialog();

private:
    Ui::MmeConfigDialog *ui;
    MainHost *myHost;

public slots:
    void accept();

};

#endif // MMECONFIGDIALOG_H
