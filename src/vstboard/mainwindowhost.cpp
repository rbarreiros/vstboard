/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#include "mainwindowhost.h"
#include "mainhosthost.h"
#include "views/configdialoghost.h"
#include "views/splash.h"
#include "audiodevices.h"
#include "mididevices.h"
#include "models/programsmodel.h"

MainWindowHost::MainWindowHost(MainHostHost * myHost,QWidget *parent) :
    MainWindow(myHost,parent)
{
    setWindowTitle(APP_NAME);

    //audio devices
    ui->treeAudioInterfaces->setModel(myHost->audioDevices->GetModel());
    ui->treeAudioInterfaces->header()->setSectionResizeMode(0,QHeaderView::Stretch);
    //ui->treeAudioInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);

    //ui->treeAudioInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setSectionResizeMode(1,QHeaderView::Fixed);

    //ui->treeAudioInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setSectionResizeMode(2,QHeaderView::Fixed);

    //ui->treeAudioInterfaces->header()->setResizeMode(3,QHeaderView::Fixed);
    ui->treeAudioInterfaces->header()->setSectionResizeMode(3,QHeaderView::Fixed);

    ui->treeAudioInterfaces->header()->resizeSection(1,30);
    ui->treeAudioInterfaces->header()->resizeSection(2,30);
    ui->treeAudioInterfaces->header()->resizeSection(3,40);
    ui->treeAudioInterfaces->expand( myHost->audioDevices->AsioIndex );

    //midi devices
    ui->treeMidiInterfaces->setModel(myHost->midiDevices->GetModel());
    //ui->treeMidiInterfaces->header()->setResizeMode(0,QHeaderView::Stretch);
    ui->treeMidiInterfaces->header()->setSectionResizeMode(0,QHeaderView::Stretch);

    //ui->treeMidiInterfaces->header()->setResizeMode(1,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->setSectionResizeMode(1,QHeaderView::Fixed);

    //ui->treeMidiInterfaces->header()->setResizeMode(2,QHeaderView::Fixed);
    ui->treeMidiInterfaces->header()->setSectionResizeMode(2,QHeaderView::Fixed);

    ui->treeMidiInterfaces->header()->resizeSection(1,30);
    ui->treeMidiInterfaces->header()->resizeSection(2,30);

    BuildListTools();

    connect(ui->treeAudioInterfaces, SIGNAL(Config(const QModelIndex &)),
            myHost->audioDevices, SLOT(ConfigDevice(const QModelIndex &)));
    connect(ui->treeAudioInterfaces, SIGNAL(UpdateList()),
            this, SLOT(UpdateAudioDevices()));

    QAction *updateMidiList = new QAction(QIcon(":/img16x16/viewmag+.png"), tr("Refresh list"), ui->treeMidiInterfaces);
    updateMidiList->setShortcut(Qt::Key_F5);
    updateMidiList->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect( updateMidiList, SIGNAL(triggered()),
             this, SLOT(UpdateMidiDevices()));
    ui->treeMidiInterfaces->addAction( updateMidiList );

    myHost->SetSampleRate( ConfigDialog::defaultSampleRate(myHost) );
}

void MainWindowHost::closeEvent(QCloseEvent *event)
{
    if(!myHost->programsModel->userWantsToUnloadSetup()) {
        event->ignore();
        return;
    }

    if(!myHost->programsModel->userWantsToUnloadProject()) {
        event->ignore();
        return;
    }

    writeSettings();
    event->accept();

    qApp->exit(0);

}

void MainWindowHost::readSettings()
{
    MainWindow::readSettings();

    QSettings settings;
    if( !settings.value("splashHide",false).toBool() ) {
        Splash *spl = new Splash(this);
        spl->show();
    }

    ui->menuView->addAction(ui->dockMidiDevices->toggleViewAction());
    ui->mainToolBar->addAction(ui->dockMidiDevices->toggleViewAction());
    ui->dockMidiDevices->toggleViewAction()->setShortcut(viewConfig->keyBinding->GetMainShortcut(KeyBind::midiDevices));

    ui->menuView->addAction(ui->dockAudioDevices->toggleViewAction());
    ui->mainToolBar->addAction(ui->dockAudioDevices->toggleViewAction());
    ui->dockAudioDevices->toggleViewAction()->setShortcut(viewConfig->keyBinding->GetMainShortcut(KeyBind::audioDevices));
}

void MainWindowHost::resetSettings()
{
    MainWindow::resetSettings();

    QList<QDockWidget*>listDocksVisible;
    listDocksVisible << ui->dockMidiDevices;
    listDocksVisible << ui->dockAudioDevices;
    foreach(QDockWidget *dock, listDocksVisible) {
        dock->setFloating(false);
        dock->setVisible(true);
    }

    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockMidiDevices);
    addDockWidget(Qt::LeftDockWidgetArea,  ui->dockAudioDevices);

    tabifyDockWidget(ui->dockTools,ui->dockMidiDevices);
    tabifyDockWidget(ui->dockMidiDevices,ui->dockAudioDevices);
}

void MainWindowHost::UpdateAudioDevices()
{
    ui->treeAudioInterfaces->setModel(static_cast<MainHostHost*>(myHost)->audioDevices->GetModel());
    ui->treeAudioInterfaces->expand( static_cast<MainHostHost*>(myHost)->audioDevices->AsioIndex );
}

void MainWindowHost::UpdateMidiDevices()
{
    ui->treeMidiInterfaces->setModel(static_cast<MainHostHost*>(myHost)->midiDevices->GetModel());
}

void MainWindowHost::on_actionRefresh_Audio_devices_triggered()
{
    UpdateAudioDevices();
}

void MainWindowHost::on_actionRefresh_Midi_devices_triggered()
{
    UpdateMidiDevices();
}

void MainWindowHost::on_actionConfig_triggered()
{
    ConfigDialogHost conf(myHost,this);
    conf.exec();
}
