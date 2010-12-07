/********************************************************************************
** Form generated from reading UI file 'vstshellselect.ui'
**
** Created: Thu 2. Dec 01:56:30 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSTSHELLSELECT_H
#define UI_VSTSHELLSELECT_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

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
            VstShellSelect->setObjectName(QString::fromUtf8("VstShellSelect"));
        VstShellSelect->resize(234, 297);
        VstShellSelect->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(VstShellSelect);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listPlugins = new QListWidget(VstShellSelect);
        listPlugins->setObjectName(QString::fromUtf8("listPlugins"));
        listPlugins->setUniformItemSizes(true);

        gridLayout->addWidget(listPlugins, 0, 0, 1, 2);

        buttonCancel = new QPushButton(VstShellSelect);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        gridLayout->addWidget(buttonCancel, 1, 0, 1, 1);

        buttonOk = new QPushButton(VstShellSelect);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setDefault(true);

        gridLayout->addWidget(buttonOk, 1, 1, 1, 1);


        retranslateUi(VstShellSelect);
        QObject::connect(listPlugins, SIGNAL(itemActivated(QListWidgetItem*)), buttonOk, SLOT(click()));

        QMetaObject::connectSlotsByName(VstShellSelect);
    } // setupUi

    void retranslateUi(QWidget *VstShellSelect)
    {
        VstShellSelect->setWindowTitle(QApplication::translate("VstShellSelect", "VstShell", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("VstShellSelect", "Cancel", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("VstShellSelect", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VstShellSelect: public Ui_VstShellSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSTSHELLSELECT_H
