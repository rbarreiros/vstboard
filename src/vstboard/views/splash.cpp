#include "splash.h"
#include "ui_splash.h"
#include "precomp.h"

#ifndef APP_NAME
#define APP_NAME "noname ?"
#endif

#ifndef APP_VERSION
#define APP_VERSION "local-dev"
#endif

Splash::Splash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Splash)
{
    ui->setupUi(this);
    ui->labelApp->setText(APP_NAME);
    ui->labelVersion->setText(APP_VERSION);
}

Splash::~Splash()
{
    QSettings settings;
    settings.setValue("splashHide",ui->checkHideIt->isChecked());
    delete ui;
}
