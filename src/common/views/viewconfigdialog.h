#ifndef VIEWCONFIGDIALOG_H
#define VIEWCONFIGDIALOG_H

#include <QDialog>

#include "precomp.h"
#include "mainhost.h"
#include "mainwindow.h"
#include "viewconfig.h"

namespace Ui {
    class ViewConfigDialog;
}

namespace View {

    class ViewConfigDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit ViewConfigDialog(MainHost *myHost, QWidget *parent = 0);
        ~ViewConfigDialog();

    private:
        void UpdateSliders();
        void UpdateColor();
        Ui::ViewConfigDialog *ui;
        MainHost *myHost;
        ViewConfig *conf;

        ColorGroups::Enum currentGrp;
        Colors::Enum currentCol;

    private slots:
        void on_listPalettes_itemClicked(QListWidgetItem* item);
        void on_listRoles_itemClicked(QListWidgetItem* item);
        void on_RedSlider_sliderMoved(int position);
        void on_GreenSlider_sliderMoved(int position);
        void on_BlueSlider_sliderMoved(int position);
        void on_AlphaSlider_sliderMoved(int position);

    };
}
#endif // VIEWCONFIGDIALOG_H
