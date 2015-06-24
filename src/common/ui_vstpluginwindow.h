/********************************************************************************
** Form generated from reading UI file 'vstpluginwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSTPLUGINWINDOW_H
#define UI_VSTPLUGINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VstPluginWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QFrame *VstPluginWindow)
    {
        if (VstPluginWindow->objectName().isEmpty())
            VstPluginWindow->setObjectName(QStringLiteral("VstPluginWindow"));
        VstPluginWindow->resize(94, 90);
        VstPluginWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        VstPluginWindow->setFrameShape(QFrame::NoFrame);
        horizontalLayout = new QHBoxLayout(VstPluginWindow);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(VstPluginWindow);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 94, 90));
        scrollAreaWidgetContents->setAutoFillBackground(false);
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(VstPluginWindow);

        QMetaObject::connectSlotsByName(VstPluginWindow);
    } // setupUi

    void retranslateUi(QFrame *VstPluginWindow)
    {
        Q_UNUSED(VstPluginWindow);
    } // retranslateUi

};

namespace Ui {
    class VstPluginWindow: public Ui_VstPluginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSTPLUGINWINDOW_H
