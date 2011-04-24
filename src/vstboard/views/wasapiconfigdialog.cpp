#include "wasapiconfigdialog.h"
#include "ui_wasapiconfigdialog.h"
#include "mainhost.h"
#include "pa_win_wasapi.h"

WasapiConfigDialog::WasapiConfigDialog(MainHost *myHost, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WasapiConfigDialog),
    myHost(myHost)
{
    ui->setupUi(this);
    setWindowTitle(tr("WASAPI Config"));

    unsigned int flags = myHost->GetSetting("api/wasapi_flags", WASAPI_DEFAULT_FLAGS).toUInt();
    ui->exclusiveMode->setChecked( flags & paWinWasapiExclusive);

    ui->inputLatency->setValue( myHost->GetSetting("api/wasapi_inLatency", WASAPI_DEFAULT_INLATENCY).toUInt() );
    ui->outputLatency->setValue( myHost->GetSetting("api/wasapi_outLatency", WASAPI_DEFAULT_OUTLATENCY).toUInt() );
}

WasapiConfigDialog::~WasapiConfigDialog()
{
    delete ui;
}


void WasapiConfigDialog::accept()
{
    unsigned int flags=0;
    if(ui->exclusiveMode->isChecked())
        flags += paWinWasapiExclusive;
    myHost->SetSetting("api/wasapi_flags", flags);

    myHost->SetSetting("api/wasapi_inLatency", ui->inputLatency->value());
    myHost->SetSetting("api/wasapi_outLatency", ui->outputLatency->value());
    QDialog::accept();
}
