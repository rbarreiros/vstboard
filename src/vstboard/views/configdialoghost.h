#ifndef CONFIGDIALOGHOST_H
#define CONFIGDIALOGHOST_H

#include "views/configdialog.h"

class ConfigDialogHost : public ConfigDialog
{
public:
    ConfigDialogHost(MainHost *myHost, QWidget *parent = 0);

public slots:
    void accept();
};

#endif // CONFIGDIALOGHOST_H
