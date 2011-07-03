#ifndef LISTAUDIODEVICESVIEW_H
#define LISTAUDIODEVICESVIEW_H

//#include "precomp.h"

class ListAudioDevicesView : public QTreeView
{
    Q_OBJECT
public:
    ListAudioDevicesView(QWidget *parent = 0);

protected:
    QAction *audioDevConfig;
    QAction *updateList;
    QModelIndex currentDevice;

signals:
    void Config(const QModelIndex &device);
    void UpdateList();

public slots:
    void AudioDevContextMenu(const QPoint &pt);
private slots:
    void ConfigCurrentDevice();

};

#endif // LISTAUDIODEVICESVIEW_H
