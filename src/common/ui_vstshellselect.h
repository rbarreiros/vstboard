/********************************************************************************
** Form generated from reading UI file 'vstshellselect.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSTSHELLSELECT_H
#define UI_VSTSHELLSELECT_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VstShellSelect
{
public:
    QGridLayout *gridLayout;
    QListWidget *listPlugins;
    QPushButton *buttonCancel;
    QPushButton *buttonOk;

    void setupUi(QWidget *VstShellSelect)
    {
        if (VstShellSelect->objectName().isEmpty())
            VstShellSelect->setObjectName(QStringLiteral("VstShellSelect"));
        VstShellSelect->setWindowModality(Qt::ApplicationModal);
        VstShellSelect->resize(234, 297);
        VstShellSelect->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(VstShellSelect);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listPlugins = new QListWidget(VstShellSelect);
        listPlugins->setObjectName(QStringLiteral("listPlugins"));
        listPlugins->setUniformItemSizes(true);

        gridLayout->addWidget(listPlugins, 0, 0, 1, 2);

        buttonCancel = new QPushButton(VstShellSelect);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        gridLayout->addWidget(buttonCancel, 1, 0, 1, 1);

        buttonOk = new QPushButton(VstShellSelect);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));
        buttonOk->setDefault(true);

        gridLayout->addWidget(buttonOk, 1, 1, 1, 1);


        retranslateUi(VstShellSelect);
        QObject::connect(listPlugins, SIGNAL(itemActivated(QListWidgetItem*)), buttonOk, SLOT(click()));

        QMetaObject::connectSlotsByName(VstShellSelect);
    } // setupUi

    void retranslateUi(QWidget *VstShellSelect)
    {
        VstShellSelect->setWindowTitle(QApplication::translate("VstShellSelect", "VstShell", 0));
        buttonCancel->setText(QApplication::translate("VstShellSelect", "Cancel", 0));
        buttonOk->setText(QApplication::translate("VstShellSelect", "Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class VstShellSelect: public Ui_VstShellSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSTSHELLSELECT_H
