/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created: Thu 2. Dec 01:56:30 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QComboBox *defaultSetup;
    QPushButton *browseSetup;
    QLabel *label_2;
    QComboBox *defaultProject;
    QPushButton *browseProject;
    QLabel *label;
    QComboBox *defaultVstPath;
    QPushButton *browseVst;
    QLabel *label_4;
    QComboBox *sampleRate;
    QLabel *label_5;
    QComboBox *bufferSize;
    QLabel *label_6;
    QComboBox *samplePrecision;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QString::fromUtf8("ConfigDialog"));
        ConfigDialog->resize(283, 282);
        ConfigDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(ConfigDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(ConfigDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        defaultSetup = new QComboBox(ConfigDialog);
        defaultSetup->setObjectName(QString::fromUtf8("defaultSetup"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(defaultSetup->sizePolicy().hasHeightForWidth());
        defaultSetup->setSizePolicy(sizePolicy1);
        defaultSetup->setEditable(true);

        gridLayout->addWidget(defaultSetup, 0, 1, 1, 2);

        browseSetup = new QPushButton(ConfigDialog);
        browseSetup->setObjectName(QString::fromUtf8("browseSetup"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(browseSetup->sizePolicy().hasHeightForWidth());
        browseSetup->setSizePolicy(sizePolicy2);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img16x16/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        browseSetup->setIcon(icon);

        gridLayout->addWidget(browseSetup, 0, 3, 1, 1);

        label_2 = new QLabel(ConfigDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        defaultProject = new QComboBox(ConfigDialog);
        defaultProject->setObjectName(QString::fromUtf8("defaultProject"));
        sizePolicy1.setHeightForWidth(defaultProject->sizePolicy().hasHeightForWidth());
        defaultProject->setSizePolicy(sizePolicy1);
        defaultProject->setEditable(true);

        gridLayout->addWidget(defaultProject, 1, 1, 1, 2);

        browseProject = new QPushButton(ConfigDialog);
        browseProject->setObjectName(QString::fromUtf8("browseProject"));
        sizePolicy2.setHeightForWidth(browseProject->sizePolicy().hasHeightForWidth());
        browseProject->setSizePolicy(sizePolicy2);
        browseProject->setIcon(icon);

        gridLayout->addWidget(browseProject, 1, 3, 1, 1);

        label = new QLabel(ConfigDialog);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        defaultVstPath = new QComboBox(ConfigDialog);
        defaultVstPath->setObjectName(QString::fromUtf8("defaultVstPath"));
        sizePolicy1.setHeightForWidth(defaultVstPath->sizePolicy().hasHeightForWidth());
        defaultVstPath->setSizePolicy(sizePolicy1);
        defaultVstPath->setEditable(true);

        gridLayout->addWidget(defaultVstPath, 2, 1, 1, 2);

        browseVst = new QPushButton(ConfigDialog);
        browseVst->setObjectName(QString::fromUtf8("browseVst"));
        sizePolicy2.setHeightForWidth(browseVst->sizePolicy().hasHeightForWidth());
        browseVst->setSizePolicy(sizePolicy2);
        browseVst->setIcon(icon);

        gridLayout->addWidget(browseVst, 2, 3, 1, 1);

        label_4 = new QLabel(ConfigDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 2);

        sampleRate = new QComboBox(ConfigDialog);
        sampleRate->setObjectName(QString::fromUtf8("sampleRate"));

        gridLayout->addWidget(sampleRate, 3, 2, 1, 2);

        label_5 = new QLabel(ConfigDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setEnabled(false);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 2);

        bufferSize = new QComboBox(ConfigDialog);
        bufferSize->setObjectName(QString::fromUtf8("bufferSize"));
        bufferSize->setEnabled(false);

        gridLayout->addWidget(bufferSize, 4, 2, 1, 2);

        label_6 = new QLabel(ConfigDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setEnabled(false);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 5, 0, 1, 2);

        samplePrecision = new QComboBox(ConfigDialog);
        samplePrecision->setObjectName(QString::fromUtf8("samplePrecision"));
        samplePrecision->setEnabled(false);

        gridLayout->addWidget(samplePrecision, 5, 2, 1, 2);

        buttonBox = new QDialogButtonBox(ConfigDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 6, 0, 1, 4);


        retranslateUi(ConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Config", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ConfigDialog", "Setup file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        browseSetup->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseSetup->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label_2->setText(QApplication::translate("ConfigDialog", "Project file", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        browseProject->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseProject->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("ConfigDialog", "VST path", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        browseVst->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseVst->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label_4->setText(QApplication::translate("ConfigDialog", "Sample rate", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ConfigDialog", "Buffer size", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ConfigDialog", "Precision", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
