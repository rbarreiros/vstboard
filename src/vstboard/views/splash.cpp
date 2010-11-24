#include "precomp.h"

#include "../_version.h"

#include "splash.h"
#include "ui_splash.h"

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
