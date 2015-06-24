/********************************************************************************
** Form generated from reading UI file 'mmeconfigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MMECONFIGDIALOG_H
#define UI_MMECONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MmeConfigDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QSpinBox *framesPerBuffer;
    QLabel *label_2;
    QSpinBox *bufferCount;
    QLabel *label;
    QCheckBox *UseLowLevelLatencyParameters;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QCheckBox *DontThrottleOverloadedProcessingThread;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MmeConfigDialog)
    {
        if (MmeConfigDialog->objectName().isEmpty())
            MmeConfigDialog->setObjectName(QStringLiteral("MmeConfigDialog"));
        MmeConfigDialog->resize(400, 234);
        verticalLayout_2 = new QVBoxLayout(MmeConfigDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(MmeConfigDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        framesPerBuffer = new QSpinBox(groupBox);
        framesPerBuffer->setObjectName(QStringLiteral("framesPerBuffer"));
        framesPerBuffer->setMinimum(1);
        framesPerBuffer->setMaximum(4096);

        formLayout->setWidget(2, QFormLayout::FieldRole, framesPerBuffer);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_2);

        bufferCount = new QSpinBox(groupBox);
        bufferCount->setObjectName(QStringLiteral("bufferCount"));
        bufferCount->setMinimum(1);

        formLayout->setWidget(3, QFormLayout::FieldRole, bufferCount);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label);

        UseLowLevelLatencyParameters = new QCheckBox(groupBox);
        UseLowLevelLatencyParameters->setObjectName(QStringLiteral("UseLowLevelLatencyParameters"));

        formLayout->setWidget(1, QFormLayout::SpanningRole, UseLowLevelLatencyParameters);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MmeConfigDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        DontThrottleOverloadedProcessingThread = new QCheckBox(groupBox_2);
        DontThrottleOverloadedProcessingThread->setObjectName(QStringLiteral("DontThrottleOverloadedProcessingThread"));

        verticalLayout->addWidget(DontThrottleOverloadedProcessingThread);


        verticalLayout_2->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(MmeConfigDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(MmeConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MmeConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MmeConfigDialog, SLOT(reject()));
        QObject::connect(UseLowLevelLatencyParameters, SIGNAL(toggled(bool)), framesPerBuffer, SLOT(setEnabled(bool)));
        QObject::connect(UseLowLevelLatencyParameters, SIGNAL(toggled(bool)), bufferCount, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(MmeConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *MmeConfigDialog)
    {
        MmeConfigDialog->setWindowTitle(QApplication::translate("MmeConfigDialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("MmeConfigDialog", "Buffers", 0));
        label_2->setText(QApplication::translate("MmeConfigDialog", "Buffer Count", 0));
        label->setText(QApplication::translate("MmeConfigDialog", "Buffer Size", 0));
        UseLowLevelLatencyParameters->setText(QApplication::translate("MmeConfigDialog", "Use Low Level Latency Parameters", 0));
        groupBox_2->setTitle(QApplication::translate("MmeConfigDialog", "Options", 0));
        DontThrottleOverloadedProcessingThread->setText(QApplication::translate("MmeConfigDialog", "Don't Throttle Overloaded Processing Thread", 0));
    } // retranslateUi

};

namespace Ui {
    class MmeConfigDialog: public Ui_MmeConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MMECONFIGDIALOG_H
