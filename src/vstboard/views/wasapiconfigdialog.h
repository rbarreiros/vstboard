#ifndef WASAPICONFIGDIALOG_H
#define WASAPICONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class WasapiConfigDialog;
}

class MainHost;
class WasapiConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WasapiConfigDialog(MainHost *myHost, QWidget *parent = 0);
    ~WasapiConfigDialog();

private:
    Ui::WasapiConfigDialog *ui;
    MainHost *myHost;


public slots:
    void accept();

};

#endif // WASAPICONFIGDIALOG_H
