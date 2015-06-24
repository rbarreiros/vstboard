/********************************************************************************
** Form generated from reading UI file 'wasapiconfigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WASAPICONFIGDIALOG_H
#define UI_WASAPICONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_WasapiConfigDialog
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *inputLatency;
    QLabel *label_2;
    QSpinBox *outputLatency;
    QDialogButtonBox *buttonBox;
    QCheckBox *exclusiveMode;

    void setupUi(QDialog *WasapiConfigDialog)
    {
        if (WasapiConfigDialog->objectName().isEmpty())
            WasapiConfigDialog->setObjectName(QStringLiteral("WasapiConfigDialog"));
        WasapiConfigDialog->resize(225, 116);
        formLayout = new QFormLayout(WasapiConfigDialog);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(WasapiConfigDialog);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        inputLatency = new QSpinBox(WasapiConfigDialog);
        inputLatency->setObjectName(QStringLiteral("inputLatency"));
        inputLatency->setMaximum(5000);

        formLayout->setWidget(1, QFormLayout::FieldRole, inputLatency);

        label_2 = new QLabel(WasapiConfigDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        outputLatency = new QSpinBox(WasapiConfigDialog);
        outputLatency->setObjectName(QStringLiteral("outputLatency"));
        outputLatency->setMaximum(5000);

        formLayout->setWidget(2, QFormLayout::FieldRole, outputLatency);

        buttonBox = new QDialogButtonBox(WasapiConfigDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        formLayout->setWidget(3, QFormLayout::SpanningRole, buttonBox);

        exclusiveMode = new QCheckBox(WasapiConfigDialog);
        exclusiveMode->setObjectName(QStringLiteral("exclusiveMode"));

        formLayout->setWidget(0, QFormLayout::SpanningRole, exclusiveMode);


        retranslateUi(WasapiConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), WasapiConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), WasapiConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(WasapiConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *WasapiConfigDialog)
    {
        WasapiConfigDialog->setWindowTitle(QApplication::translate("WasapiConfigDialog", "Dialog", 0));
        label->setText(QApplication::translate("WasapiConfigDialog", "Input latency (0=auto)", 0));
        label_2->setText(QApplication::translate("WasapiConfigDialog", "Output latency (0=auto)", 0));
        exclusiveMode->setText(QApplication::translate("WasapiConfigDialog", "Exclusive mode", 0));
    } // retranslateUi

};

namespace Ui {
    class WasapiConfigDialog: public Ui_WasapiConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WASAPICONFIGDIALOG_H
