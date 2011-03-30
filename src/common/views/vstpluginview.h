#ifndef VSTPLUGINVIEW_H
#define VSTPLUGINVIEW_H

#include "connectableobjectview.h"

namespace View {
    class VstPluginView : public ConnectableObjectView
    {
    Q_OBJECT
    public:
        VstPluginView(MainHost *myHost,QAbstractItemModel *model,QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
        void SetModelIndex(QPersistentModelIndex index);
        void UpdateModelIndex();

    protected:
        QAction *actSaveBank;
        QAction *actSaveBankAs;

    public slots:
        void SaveBankAs();
        void SaveBank();

    };
}

#endif // VSTPLUGINVIEW_H
