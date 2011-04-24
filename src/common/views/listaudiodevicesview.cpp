#include "listaudiodevicesview.h"
#include "globals.h"

ListAudioDevicesView::ListAudioDevicesView(QWidget *parent) :
    QTreeView(parent)
{
    audioDevConfig = new QAction(QIcon(":/img16x16/configure.png"), tr("Config device"), this);
    connect(audioDevConfig, SIGNAL(triggered()),
            this, SLOT(ConfigCurrentDevice()));

    updateList = new QAction(QIcon(":/img16x16/viewmag+.png"), tr("Refresh list"), this);
    connect(updateList, SIGNAL(triggered()),
            this, SIGNAL(UpdateList()));

    connect(this, SIGNAL(customContextMenuRequested( const QPoint& )),
            this, SLOT(AudioDevContextMenu(const QPoint &)));
}

void ListAudioDevicesView::AudioDevContextMenu(const QPoint &pt)
{
    currentDevice = indexAt(pt).sibling(0,0);

    QList<QAction *> lstActions;

    if(currentDevice.data(UserRoles::objInfo).isValid())
        lstActions << audioDevConfig;

    lstActions << updateList;
    QMenu::exec(lstActions, mapToGlobal(pt));
}

void ListAudioDevicesView::ConfigCurrentDevice()
{
    emit Config(currentDevice);
}
