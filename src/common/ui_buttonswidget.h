/********************************************************************************
** Form generated from reading UI file 'buttonswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONSWIDGET_H
#define UI_BUTTONSWIDGET_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ButtonsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *lmb;
    QCheckBox *mmb;
    QCheckBox *rmb;
    QCheckBox *mb4;
    QCheckBox *mb5;

    void setupUi(QWidget *ButtonsWidget)
    {
        if (ButtonsWidget->objectName().isEmpty())
            ButtonsWidget->setObjectName(QStringLiteral("ButtonsWidget"));
        ButtonsWidget->resize(48, 90);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ButtonsWidget->sizePolicy().hasHeightForWidth());
        ButtonsWidget->setSizePolicy(sizePolicy);
        ButtonsWidget->setMinimumSize(QSize(0, 90));
        ButtonsWidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 127);"));
        ButtonsWidget->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ButtonsWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lmb = new QCheckBox(ButtonsWidget);
        lmb->setObjectName(QStringLiteral("lmb"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lmb->sizePolicy().hasHeightForWidth());
        lmb->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(lmb);

        mmb = new QCheckBox(ButtonsWidget);
        mmb->setObjectName(QStringLiteral("mmb"));
        sizePolicy1.setHeightForWidth(mmb->sizePolicy().hasHeightForWidth());
        mmb->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(mmb);

        rmb = new QCheckBox(ButtonsWidget);
        rmb->setObjectName(QStringLiteral("rmb"));
        sizePolicy1.setHeightForWidth(rmb->sizePolicy().hasHeightForWidth());
        rmb->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(rmb);

        mb4 = new QCheckBox(ButtonsWidget);
        mb4->setObjectName(QStringLiteral("mb4"));
        sizePolicy1.setHeightForWidth(mb4->sizePolicy().hasHeightForWidth());
        mb4->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(mb4);

        mb5 = new QCheckBox(ButtonsWidget);
        mb5->setObjectName(QStringLiteral("mb5"));
        sizePolicy1.setHeightForWidth(mb5->sizePolicy().hasHeightForWidth());
        mb5->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(mb5);


        retranslateUi(ButtonsWidget);

        QMetaObject::connectSlotsByName(ButtonsWidget);
    } // setupUi

    void retranslateUi(QWidget *ButtonsWidget)
    {
        lmb->setText(QApplication::translate("ButtonsWidget", "Left", 0));
        mmb->setText(QApplication::translate("ButtonsWidget", "Mid", 0));
        rmb->setText(QApplication::translate("ButtonsWidget", "Right", 0));
        mb4->setText(QApplication::translate("ButtonsWidget", "Bt 4", 0));
        mb5->setText(QApplication::translate("ButtonsWidget", "Bt 5", 0));
        Q_UNUSED(ButtonsWidget);
    } // retranslateUi

};

namespace Ui {
    class ButtonsWidget: public Ui_ButtonsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONSWIDGET_H
