#include "mmeconfigdialog.h"
#include "ui_mmeconfigdialog.h"
#include "mainhost.h"
#include "pa_win_wmme.h"

MmeConfigDialog::MmeConfigDialog( MainHost *myHost, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MmeConfigDialog),
    myHost(myHost)
{
    ui->setupUi(this);
    setWindowTitle(tr("MME Config"));

    unsigned int flags = myHost->GetSetting("api/wmme_flags", MME_DFAULT_FLAGS).toUInt();
    unsigned int framesPerBuffer = myHost->GetSetting("api/wmme_bufferSize", MME_DEFAULT_BUFFER_SIZE).toUInt();
    unsigned int bufferCount = myHost->GetSetting("api/wmme_bufferCount", MME_DEFAULT_BUFFER_COUNT).toUInt();

    ui->UseLowLevelLatencyParameters->setChecked( flags & paWinMmeUseLowLevelLatencyParameters );
    ui->DontThrottleOverloadedProcessingThread->setChecked( flags & paWinMmeDontThrottleOverloadedProcessingThread );

    ui->framesPerBuffer->setValue( framesPerBuffer );
    ui->framesPerBuffer->setEnabled( ui->UseLowLevelLatencyParameters->isChecked() );
    ui->bufferCount->setValue( bufferCount );
    ui->bufferCount->setEnabled( ui->UseLowLevelLatencyParameters->isChecked() );
}

MmeConfigDialog::~MmeConfigDialog()
{
    delete ui;
}

void MmeConfigDialog::accept()
{
    unsigned int flags=0;
    if(ui->UseLowLevelLatencyParameters->isChecked())
        flags += paWinMmeUseLowLevelLatencyParameters;
    if(ui->DontThrottleOverloadedProcessingThread->isChecked())
        flags += paWinMmeDontThrottleOverloadedProcessingThread;

    unsigned int framesPerBuffer = ui->framesPerBuffer->value();
    unsigned int bufferCount = ui->bufferCount->value();

    myHost->SetSetting("api/wmme_flags", flags);
    myHost->SetSetting("api/wmme_bufferSize", framesPerBuffer);
    myHost->SetSetting("api/wmme_bufferCount", bufferCount);

    QDialog::accept();
}
