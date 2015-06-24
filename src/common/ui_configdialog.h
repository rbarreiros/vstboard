/********************************************************************************
** Form generated from reading UI file 'configdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
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
    QComboBox *defaultBankPath;
    QPushButton *browseBank;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_3;
    QLabel *label_5;
    QSpinBox *nbThreads;
    QCheckBox *fastEditorOpenClose;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout;
    QLabel *label_7;
    QComboBox *onUnsavedSetup;
    QLabel *label_8;
    QComboBox *onUnsavedProject;
    QGroupBox *groupSampleFormat;
    QFormLayout *formLayout_2;
    QLabel *labelsampleRate;
    QComboBox *sampleRate;
    QComboBox *bufferSize;
    QLabel *labelsamplePrecision;
    QComboBox *samplePrecision;
    QLabel *labelbufferSize;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QStringLiteral("ConfigDialog"));
        ConfigDialog->resize(513, 370);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConfigDialog->sizePolicy().hasHeightForWidth());
        ConfigDialog->setSizePolicy(sizePolicy);
        ConfigDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout_2 = new QGridLayout(ConfigDialog);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(ConfigDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        defaultSetup = new QComboBox(groupBox);
        defaultSetup->setObjectName(QStringLiteral("defaultSetup"));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(defaultSetup->sizePolicy().hasHeightForWidth());
        defaultSetup->setSizePolicy(sizePolicy2);
        defaultSetup->setEditable(true);

        gridLayout->addWidget(defaultSetup, 0, 1, 1, 1);

        browseSetup = new QPushButton(groupBox);
        browseSetup->setObjectName(QStringLiteral("browseSetup"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(browseSetup->sizePolicy().hasHeightForWidth());
        browseSetup->setSizePolicy(sizePolicy3);
        QIcon icon;
        icon.addFile(QStringLiteral(":/img16x16/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        browseSetup->setIcon(icon);

        gridLayout->addWidget(browseSetup, 0, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        defaultProject = new QComboBox(groupBox);
        defaultProject->setObjectName(QStringLiteral("defaultProject"));
        sizePolicy2.setHeightForWidth(defaultProject->sizePolicy().hasHeightForWidth());
        defaultProject->setSizePolicy(sizePolicy2);
        defaultProject->setEditable(true);

        gridLayout->addWidget(defaultProject, 1, 1, 1, 1);

        browseProject = new QPushButton(groupBox);
        browseProject->setObjectName(QStringLiteral("browseProject"));
        sizePolicy3.setHeightForWidth(browseProject->sizePolicy().hasHeightForWidth());
        browseProject->setSizePolicy(sizePolicy3);
        browseProject->setIcon(icon);

        gridLayout->addWidget(browseProject, 1, 2, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        defaultVstPath = new QComboBox(groupBox);
        defaultVstPath->setObjectName(QStringLiteral("defaultVstPath"));
        sizePolicy2.setHeightForWidth(defaultVstPath->sizePolicy().hasHeightForWidth());
        defaultVstPath->setSizePolicy(sizePolicy2);
        defaultVstPath->setEditable(true);

        gridLayout->addWidget(defaultVstPath, 2, 1, 1, 1);

        browseVst = new QPushButton(groupBox);
        browseVst->setObjectName(QStringLiteral("browseVst"));
        sizePolicy3.setHeightForWidth(browseVst->sizePolicy().hasHeightForWidth());
        browseVst->setSizePolicy(sizePolicy3);
        browseVst->setIcon(icon);

        gridLayout->addWidget(browseVst, 2, 2, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        defaultBankPath = new QComboBox(groupBox);
        defaultBankPath->setObjectName(QStringLiteral("defaultBankPath"));
        sizePolicy2.setHeightForWidth(defaultBankPath->sizePolicy().hasHeightForWidth());
        defaultBankPath->setSizePolicy(sizePolicy2);
        defaultBankPath->setEditable(true);

        gridLayout->addWidget(defaultBankPath, 3, 1, 1, 1);

        browseBank = new QPushButton(groupBox);
        browseBank->setObjectName(QStringLiteral("browseBank"));
        sizePolicy3.setHeightForWidth(browseBank->sizePolicy().hasHeightForWidth());
        browseBank->setSizePolicy(sizePolicy3);
        browseBank->setIcon(icon);

        gridLayout->addWidget(browseBank, 3, 2, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 3, 1, 6);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 5, 3, 1);

        groupBox_2 = new QGroupBox(ConfigDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout_3 = new QFormLayout(groupBox_2);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_5);

        nbThreads = new QSpinBox(groupBox_2);
        nbThreads->setObjectName(QStringLiteral("nbThreads"));
        sizePolicy3.setHeightForWidth(nbThreads->sizePolicy().hasHeightForWidth());
        nbThreads->setSizePolicy(sizePolicy3);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, nbThreads);

        fastEditorOpenClose = new QCheckBox(groupBox_2);
        fastEditorOpenClose->setObjectName(QStringLiteral("fastEditorOpenClose"));

        formLayout_3->setWidget(1, QFormLayout::SpanningRole, fastEditorOpenClose);


        gridLayout_2->addWidget(groupBox_2, 3, 3, 2, 2);

        groupBox_3 = new QGroupBox(ConfigDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy4);
        formLayout = new QFormLayout(groupBox_3);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_7);

        onUnsavedSetup = new QComboBox(groupBox_3);
        onUnsavedSetup->setObjectName(QStringLiteral("onUnsavedSetup"));
        onUnsavedSetup->setFrame(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, onUnsavedSetup);

        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_8);

        onUnsavedProject = new QComboBox(groupBox_3);
        onUnsavedProject->setObjectName(QStringLiteral("onUnsavedProject"));

        formLayout->setWidget(1, QFormLayout::FieldRole, onUnsavedProject);


        gridLayout_2->addWidget(groupBox_3, 2, 4, 1, 1);

        groupSampleFormat = new QGroupBox(ConfigDialog);
        groupSampleFormat->setObjectName(QStringLiteral("groupSampleFormat"));
        sizePolicy4.setHeightForWidth(groupSampleFormat->sizePolicy().hasHeightForWidth());
        groupSampleFormat->setSizePolicy(sizePolicy4);
        formLayout_2 = new QFormLayout(groupSampleFormat);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        labelsampleRate = new QLabel(groupSampleFormat);
        labelsampleRate->setObjectName(QStringLiteral("labelsampleRate"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, labelsampleRate);

        sampleRate = new QComboBox(groupSampleFormat);
        sampleRate->setObjectName(QStringLiteral("sampleRate"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, sampleRate);

        bufferSize = new QComboBox(groupSampleFormat);
        bufferSize->setObjectName(QStringLiteral("bufferSize"));
        bufferSize->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, bufferSize);

        labelsamplePrecision = new QLabel(groupSampleFormat);
        labelsamplePrecision->setObjectName(QStringLiteral("labelsamplePrecision"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, labelsamplePrecision);

        samplePrecision = new QComboBox(groupSampleFormat);
        samplePrecision->setObjectName(QStringLiteral("samplePrecision"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, samplePrecision);

        labelbufferSize = new QLabel(groupSampleFormat);
        labelbufferSize->setObjectName(QStringLiteral("labelbufferSize"));
        labelbufferSize->setEnabled(false);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelbufferSize);


        gridLayout_2->addWidget(groupSampleFormat, 2, 3, 1, 1);

        buttonBox = new QDialogButtonBox(ConfigDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        sizePolicy3.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy3);
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 4, 7, 1, 1);


        retranslateUi(ConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Config", 0));
        groupBox->setTitle(QApplication::translate("ConfigDialog", "Default files and directories", 0));
        label_3->setText(QApplication::translate("ConfigDialog", "Setup file", 0));
#ifndef QT_NO_TOOLTIP
        browseSetup->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseSetup->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label_2->setText(QApplication::translate("ConfigDialog", "Project file", 0));
#ifndef QT_NO_TOOLTIP
        browseProject->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseProject->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("ConfigDialog", "VST directory", 0));
#ifndef QT_NO_TOOLTIP
        browseVst->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseVst->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        label_4->setText(QApplication::translate("ConfigDialog", "Bank directory", 0));
#ifndef QT_NO_TOOLTIP
        browseBank->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        browseBank->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        groupBox_2->setTitle(QApplication::translate("ConfigDialog", "Engine", 0));
        label_5->setText(QApplication::translate("ConfigDialog", "Number of threads (0=auto) :", 0));
#ifndef QT_NO_TOOLTIP
        fastEditorOpenClose->setToolTip(QApplication::translate("ConfigDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Allows faster program change by hiding windows instead of closing them. May increase cpu and memory usage</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        fastEditorOpenClose->setText(QApplication::translate("ConfigDialog", "Fast plugins windows open/close", 0));
        groupBox_3->setTitle(QApplication::translate("ConfigDialog", "On unsaved changes", 0));
        label_7->setText(QApplication::translate("ConfigDialog", "Setup", 0));
        label_8->setText(QApplication::translate("ConfigDialog", "Project", 0));
        groupSampleFormat->setTitle(QApplication::translate("ConfigDialog", "Sample format", 0));
        labelsampleRate->setText(QApplication::translate("ConfigDialog", "Sample rate", 0));
        labelsamplePrecision->setText(QApplication::translate("ConfigDialog", "Precision", 0));
        labelbufferSize->setText(QApplication::translate("ConfigDialog", "Buffer size", 0));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
