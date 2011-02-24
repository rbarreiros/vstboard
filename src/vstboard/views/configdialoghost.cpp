#include "configdialoghost.h"
#include "../common/ui_configdialog.h"
#include "mainhost.h"

ConfigDialogHost::ConfigDialogHost(MainHost *myHost, QWidget *parent) :
    ConfigDialog(myHost, parent)
{
    ui->sampleRate->addItem("44.1 kHz",44100);
    ui->sampleRate->addItem("48 kHz",48000);
    ui->sampleRate->addItem("88.2 kHz",88200);
    ui->sampleRate->addItem("96 kHz",96000);

    int index=ui->sampleRate->findData( myHost->GetSampleRate() );
    if(index==-1) {
        debug("ConfigDialog invalid sample rate")
        return;
    }
    ui->sampleRate->setCurrentIndex(index);

//buffer size
//    ui->bufferSize->addItem("Auto",0);
//    index = ui->bufferSize->findData( (unsigned int)myHost->GetBufferSize() );
//    if(index==-1) {
//        debug("ConfigDialog invalid buffer size")
//        index=0;
//    }
//    ui->bufferSize->setCurrentIndex(index);
}

void ConfigDialogHost::accept()
{
    float rate = ui->sampleRate->itemData(ui->sampleRate->currentIndex()).toFloat();
    if(rate!=defaultSampleRate(myHost)) {
        myHost->SetSampleRate( rate );
    }
    myHost->SetSetting("sampleRate", rate);

//    int buffer = ui->bufferSize->itemData(ui->bufferSize->currentIndex()).toInt();
//    settings.setValue("bufferSize", buffer);

    ConfigDialog::accept();
}
