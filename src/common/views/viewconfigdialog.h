#ifndef VIEWCONFIGDIALOG_H
#define VIEWCONFIGDIALOG_H

#include <QDialog>

//#include "precomp.h"
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
        void GetColorFromConf();
        void UpdateSliders();
        void DisableSliders();
        void SaveColor();
        QColor currentColor;

        /// pointer to the dialog ui;
        Ui::ViewConfigDialog *ui;

        /// pointer to the MainHost
        MainHost *myHost;

        /// pointer to the config
        ViewConfig *conf;

        /// currently selected group
        ColorGroups::Enum currentGrp;

        /// currently selected color
        Colors::Enum currentCol;

        /// a copy of the original colors, restored if dialog is canceled
        QMap<ColorGroups::Enum,ColorGroup>backupColors;

        /// a copy of the colors, restored if saved in setup file is checked
        QMap<ColorGroups::Enum,ColorGroup>backupSetupColors;

        /// save in setup file, oriinal state
        bool backupSaveInSetup;

        /// set to true whan changes are made
        bool modified;

        bool StartUpdate();
        void EndUpdate();
        bool updateInProgress;

    public slots:
        void accept();
        void reject();

    private slots:
        void onPickerColorSelected(const QColor &color);
        void onPickerHueSelected(const QColor &color);
        void on_listPalettes_itemClicked(QListWidgetItem* item);
        void on_listRoles_itemClicked(QListWidgetItem* item);
        void on_checkSavedInSetupFile_clicked(bool checked);
        void on_RedSpinBox_valueChanged(int );
        void on_GreenSpinBox_valueChanged(int );
        void on_BlueSpinBox_valueChanged(int );
        void on_AlphaSpinBox_valueChanged(int );
    };
}
#endif // VIEWCONFIGDIALOG_H
