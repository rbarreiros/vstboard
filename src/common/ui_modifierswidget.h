/********************************************************************************
** Form generated from reading UI file 'modifierswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFIERSWIDGET_H
#define UI_MODIFIERSWIDGET_H

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

class Ui_ModifiersWidget
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *shift;
    QCheckBox *ctrl;
    QCheckBox *alt;

    void setupUi(QWidget *ModifiersWidget)
    {
        if (ModifiersWidget->objectName().isEmpty())
            ModifiersWidget->setObjectName(QStringLiteral("ModifiersWidget"));
        ModifiersWidget->resize(45, 54);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ModifiersWidget->sizePolicy().hasHeightForWidth());
        ModifiersWidget->setSizePolicy(sizePolicy);
        ModifiersWidget->setMinimumSize(QSize(0, 54));
        ModifiersWidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 127);"));
        ModifiersWidget->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ModifiersWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        shift = new QCheckBox(ModifiersWidget);
        shift->setObjectName(QStringLiteral("shift"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(shift->sizePolicy().hasHeightForWidth());
        shift->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(shift);

        ctrl = new QCheckBox(ModifiersWidget);
        ctrl->setObjectName(QStringLiteral("ctrl"));
        sizePolicy1.setHeightForWidth(ctrl->sizePolicy().hasHeightForWidth());
        ctrl->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(ctrl);

        alt = new QCheckBox(ModifiersWidget);
        alt->setObjectName(QStringLiteral("alt"));
        sizePolicy1.setHeightForWidth(alt->sizePolicy().hasHeightForWidth());
        alt->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(alt);


        retranslateUi(ModifiersWidget);

        QMetaObject::connectSlotsByName(ModifiersWidget);
    } // setupUi

    void retranslateUi(QWidget *ModifiersWidget)
    {
        ModifiersWidget->setWindowTitle(QApplication::translate("ModifiersWidget", "Form", 0));
        shift->setText(QApplication::translate("ModifiersWidget", "Shift", 0));
        ctrl->setText(QApplication::translate("ModifiersWidget", "Ctrl", 0));
        alt->setText(QApplication::translate("ModifiersWidget", "Alt", 0));
    } // retranslateUi

};

namespace Ui {
    class ModifiersWidget: public Ui_ModifiersWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFIERSWIDGET_H
