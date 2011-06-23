#include "vstpluginview.h"

using namespace View;

VstPluginView::VstPluginView(MainHost *myHost,QAbstractItemModel *model,QGraphicsItem * parent, Qt::WindowFlags wFlags) :
    ConnectableObjectView(myHost,model,parent,wFlags)
{
    setObjectName("vstPluginView");

    setAcceptDrops(true);

    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::VstPlugin,Colors::Background) );
    setPalette( pal );
}

void VstPluginView::UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if(groupId!=ColorGroups::VstPlugin)
        return;

    switch(colorId) {
        case Colors::Background: {
            if(!highlighted) {
                QPalette pal(palette());
                pal.setColor(QPalette::Window,color);
                setPalette( pal );
            }
            break;
        }
        case Colors::HighlightBackground: {
            if(highlighted) {
                QPalette pal(palette());
                pal.setColor(QPalette::Window,color);
                setPalette( pal );
            }
            break;
        }
        default:
            break;
    }
}

void VstPluginView::SetModelIndex(QPersistentModelIndex index)
{
    ConnectableObjectView::SetModelIndex(index);

    actSaveBank = new QAction(QIcon(":/img16x16/filesave.png"),tr("Save Bank"),this);
    connect(actSaveBank,SIGNAL(triggered()),
            this,SLOT(SaveBank()));
    addAction(actSaveBank);

    actSaveBankAs = new QAction(QIcon(":/img16x16/filesaveas.png"),tr("Save Bank As..."),this);
    connect(actSaveBankAs,SIGNAL(triggered()),
            this,SLOT(SaveBankAs()));
    actSaveBankAs->setEnabled(false);
    addAction(actSaveBankAs);

    actSaveProgram = new QAction(QIcon(":/img16x16/filesave.png"),tr("Save Program"),this);
    connect(actSaveProgram,SIGNAL(triggered()),
            this,SLOT(SaveProgram()));
    addAction(actSaveProgram);

    actSaveProgramAs = new QAction(QIcon(":/img16x16/filesaveas.png"),tr("Save Program As..."),this);
    connect(actSaveProgramAs,SIGNAL(triggered()),
            this,SLOT(SaveProgramAs()));
    actSaveProgramAs->setEnabled(false);
    addAction(actSaveProgramAs);
}

void VstPluginView::UpdateModelIndex()
{
    ConnectableObjectView::UpdateModelIndex();
    if(objIndex.data(UserRoles::bankFile).isValid() && !objIndex.data(UserRoles::bankFile).toString().isEmpty()) {
        QFileInfo info;
        info.setFile( objIndex.data(UserRoles::bankFile).toString() );
        actSaveBank->setText( QString(tr("Save Bank (%1)")).arg(info.fileName()) );
        actSaveBankAs->setEnabled(true);
    } else {
        actSaveBank->setText( tr("Save Bank") );
        actSaveBankAs->setEnabled(false);
    }

    if(objIndex.data(UserRoles::programFile).isValid() && !objIndex.data(UserRoles::programFile).toString().isEmpty()) {
        QFileInfo info;
        info.setFile( objIndex.data(UserRoles::programFile).toString() );
        actSaveProgram->setText( QString(tr("Save Program (%1)")).arg(info.fileName()) );
        actSaveProgramAs->setEnabled(true);
    } else {
        actSaveProgram->setText( tr("Save Program") );
        actSaveProgramAs->setEnabled(false);
    }
}

void VstPluginView::SaveBankAs()
{
    QString lastDir = myHost->GetSetting("lastBankPath").toString();
    QString filename = QFileDialog::getSaveFileName(0, tr("Save Bank"), lastDir, tr("Bank File (*.%1)").arg(VST_BANK_FILE_EXTENSION) );
    if(filename.isEmpty())
        return;

    if( !filename.endsWith( QString(".")+VST_BANK_FILE_EXTENSION, Qt::CaseInsensitive ) ) {
        filename.append( QString(".")+VST_BANK_FILE_EXTENSION );
    }

    myHost->SetSetting("lastBankPath",QFileInfo(filename).absolutePath());
    model->setData(objIndex,filename,UserRoles::bankFile);
}

void VstPluginView::SaveBank()
{
    if(objIndex.data(UserRoles::bankFile).isValid() && !objIndex.data(UserRoles::bankFile).toString().isEmpty()) {
        model->setData(objIndex, objIndex.data(UserRoles::bankFile).toString(), UserRoles::bankFile);
    } else {
        SaveBankAs();
    }
}

void VstPluginView::SaveProgramAs()
{
    QString lastDir = myHost->GetSetting("lastBankPath").toString();
    QString filename = QFileDialog::getSaveFileName(0, tr("Save Program"), lastDir, tr("Program File (*.%1)").arg(VST_PROGRAM_FILE_EXTENSION) );
    if(filename.isEmpty())
        return;

    if( !filename.endsWith(QString(".")+VST_PROGRAM_FILE_EXTENSION, Qt::CaseInsensitive) ) {
        filename.append(QString(".")+VST_PROGRAM_FILE_EXTENSION);
    }

    myHost->SetSetting("lastBankPath",QFileInfo(filename).absolutePath());
    model->setData(objIndex,filename,UserRoles::programFile);
}

void VstPluginView::SaveProgram()
{
    if(objIndex.data(UserRoles::programFile).isValid() && !objIndex.data(UserRoles::programFile).toString().isEmpty()) {
        model->setData(objIndex, objIndex.data(UserRoles::programFile).toString(), UserRoles::programFile);
    } else {
        SaveProgramAs();
    }
}

void VstPluginView::dragEnterEvent( QGraphicsSceneDragDropEvent *event)
{
    //accept fxp files and replacement plugin
    if (event->mimeData()->hasUrls()) {
        QString fName;
        QFileInfo info;

        QStringList acceptedFiles;
        acceptedFiles << "fxb" << "fxp" << "dll";

        foreach(QUrl url,event->mimeData()->urls()) {
            fName = url.toLocalFile();
            info.setFile( fName );
            if ( info.isFile() && info.isReadable() ) {
                if( acceptedFiles.contains( info.suffix(), Qt::CaseInsensitive) ) {
                    event->setDropAction(Qt::CopyAction);
                    event->accept();

                    HighlightStart();

                    return;
                }
            }
        }
    }
    event->ignore();
}

void VstPluginView::dragLeaveEvent( QGraphicsSceneDragDropEvent *event)
{
    HighlightStop();
}

void VstPluginView::dropEvent( QGraphicsSceneDragDropEvent *event)
{
    HighlightStop();

    QGraphicsWidget::dropEvent(event);
    event->setAccepted(model->dropMimeData(event->mimeData(), event->proposedAction(), 0, 0, objIndex));
}

void VstPluginView::HighlightStart()
{
    border = new QGraphicsRectItem(0,0,geometry().width(),geometry().height(),this,scene());
    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::VstPlugin,Colors::HighlightBackground) );
    setPalette( pal );
}

void VstPluginView::HighlightStop()
{
    if(border) {
        scene()->removeItem(border);
        delete border;
    }
    QPalette pal(palette());
    pal.setColor(QPalette::Window, config->GetColor(ColorGroups::VstPlugin,Colors::Background) );
    setPalette( pal );
}
