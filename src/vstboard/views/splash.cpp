#include "splash.h"
#include "ui_splash.h"
#include "precomp.h"

Splash::Splash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Splash)
{
    ui->setupUi(this);
}

Splash::~Splash()
{
    QSettings settings;
    settings.setValue("splashHide",ui->checkHideIt->isChecked());
    delete ui;
}
