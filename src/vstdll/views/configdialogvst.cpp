#include "configdialogvst.h"
#include "../common/ui_configdialog.h"
#include "mainhost.h"

ConfigDialogVst::ConfigDialogVst(MainHost *myHost, QWidget *parent) :
    ConfigDialog(myHost, parent)
{
    //ui->groupSampleFormat->setVisible(false);
    ui->sampleRate->setVisible(false);
    ui->labelsampleRate->setVisible(false);
}
