/********************************************************************************
** Form generated from reading UI file 'programlist.ui'
**
** Created: Thu 2. Dec 01:56:30 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRAMLIST_H
#define UI_PROGRAMLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "views/grouplistview.h"
#include "views/proglistview.h"

QT_BEGIN_NAMESPACE

class Ui_ProgramList
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    GroupListView *listGrps;
    ProgListView *listProgs;

    void setupUi(QWidget *ProgramList)
    {
        if (ProgramList->objectName().isEmpty())
            ProgramList->setObjectName(QString::fromUtf8("ProgramList"));
        ProgramList->resize(150, 119);
        verticalLayout = new QVBoxLayout(ProgramList);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(ProgramList);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        listGrps = new GroupListView(splitter);
        listGrps->setObjectName(QString::fromUtf8("listGrps"));
        listGrps->setAcceptDrops(true);
        splitter->addWidget(listGrps);
        listProgs = new ProgListView(splitter);
        listProgs->setObjectName(QString::fromUtf8("listProgs"));
        splitter->addWidget(listProgs);

        verticalLayout->addWidget(splitter);


        retranslateUi(ProgramList);

        QMetaObject::connectSlotsByName(ProgramList);
    } // setupUi

    void retranslateUi(QWidget *ProgramList)
    {
        ProgramList->setWindowTitle(QApplication::translate("ProgramList", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgramList: public Ui_ProgramList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRAMLIST_H
