#include "vstpluginview.h"
#include "../mainhost.h"

using namespace View;

VstPluginView::VstPluginView(MainHost *myHost,QAbstractItemModel *model,QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    ConnectableObjectView(myHost,model,parent,wFlags)
{

}

void VstPluginView::SetModelIndex(QPersistentModelIndex index)
{
    ConnectableObjectView::SetModelIndex(index);

    actSaveBankAs = new QAction(QIcon(":/img16x16/filesave.png"),tr("Save Bank As"),this);
//    actSaveBankAs->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_B);
//    actSaveBankAs->setShortcutContext(Qt::WidgetShortcut);
    connect(actSaveBankAs,SIGNAL(triggered()),
            this,SLOT(SaveBankAs()));
    addAction(actSaveBankAs);

    actSaveBank = new QAction(QIcon(":/img16x16/filesave.png"),tr("Save Bank"),this);
//    actSaveBank->setShortcut( Qt::CTRL + Qt::Key_B);
//    actSaveBank->setShortcutContext(Qt::WidgetShortcut);
    connect(actSaveBank,SIGNAL(triggered()),
            this,SLOT(SaveBank()));
    actSaveBank->setEnabled(false);
    addAction(actSaveBank);
}

void VstPluginView::UpdateModelIndex()
{
    ConnectableObjectView::UpdateModelIndex();
    actSaveBank->setEnabled( !objIndex.data(UserRoles::bankFile).toString().isEmpty() );
}

void VstPluginView::SaveBankAs()
{
    QString lastDir = myHost->GetSetting("lastBankPath").toString();
    QString fileName = QFileDialog::getSaveFileName(0, tr("Save Bank"), lastDir, tr("Bank File (*.%1)").arg(VST_BANK_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(VST_BANK_FILE_EXTENSION, Qt::CaseInsensitive)) {
        fileName += ".";
        fileName += VST_BANK_FILE_EXTENSION;
    }

    myHost->SetSetting("lastBankPath",QFileInfo(fileName).absolutePath());
    model->setData(objIndex,fileName,UserRoles::bankFile);
    UpdateModelIndex();
}

void VstPluginView::SaveBank()
{
    model->setData(objIndex, objIndex.data(UserRoles::bankFile).toString(), UserRoles::bankFile);
}
