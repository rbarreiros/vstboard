#include "precomp.h"

#include "splash.h"
#include "ui_splash.h"

#include "../_version.h"

Splash::Splash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Splash)
{
    QString ver = APP_VERSION;
    ver=ver.section("-",0,1);

    ui->setupUi(this);
    ui->labelApp->setText(APP_NAME);
    ui->labelVersion->setText(" "+ver);

    setWindowTitle(APP_NAME);

    setFixedSize(size());

}

Splash::~Splash()
{
    QSettings settings;
    settings.setValue("splashHide",ui->checkHideIt->isChecked());
    delete ui;
}
