/********************************************************************************
** Form generated from reading UI file 'programlist.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRAMLIST_H
#define UI_PROGRAMLIST_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "views/grouplistview.h"
#include "views/proglistview.h"

QT_BEGIN_NAMESPACE

class Ui_ProgramList
{
public:
    QVBoxLayout *verticalLayout_3;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *btAddGroup;
    QToolButton *btDelGroup;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QCheckBox *GroupAutosave;
    GroupListView *listGrps;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QToolButton *btAddProgram;
    QToolButton *btDelProgram;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *ProgAutosave;
    ProgListView *listProgs;

    void setupUi(QWidget *ProgramList)
    {
        if (ProgramList->objectName().isEmpty())
            ProgramList->setObjectName(QStringLiteral("ProgramList"));
        ProgramList->resize(158, 198);
        ProgramList->setMinimumSize(QSize(1, 0));
        ProgramList->setWindowTitle(QStringLiteral("Form"));
        ProgramList->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout_3 = new QVBoxLayout(ProgramList);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        splitter = new QSplitter(ProgramList);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(1);
        splitter->setChildrenCollapsible(false);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btAddGroup = new QToolButton(layoutWidget);
        btAddGroup->setObjectName(QStringLiteral("btAddGroup"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btAddGroup->sizePolicy().hasHeightForWidth());
        btAddGroup->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/img16x16/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btAddGroup->setIcon(icon);
        btAddGroup->setIconSize(QSize(10, 10));

        horizontalLayout_2->addWidget(btAddGroup);

        btDelGroup = new QToolButton(layoutWidget);
        btDelGroup->setObjectName(QStringLiteral("btDelGroup"));
        sizePolicy.setHeightForWidth(btDelGroup->sizePolicy().hasHeightForWidth());
        btDelGroup->setSizePolicy(sizePolicy);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/img16x16/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        btDelGroup->setIcon(icon1);
        btDelGroup->setIconSize(QSize(10, 10));

        horizontalLayout_2->addWidget(btDelGroup);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(1, 0));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        GroupAutosave = new QCheckBox(layoutWidget);
        GroupAutosave->setObjectName(QStringLiteral("GroupAutosave"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(GroupAutosave->sizePolicy().hasHeightForWidth());
        GroupAutosave->setSizePolicy(sizePolicy2);
        GroupAutosave->setTristate(true);

        horizontalLayout_2->addWidget(GroupAutosave);


        verticalLayout_4->addLayout(horizontalLayout_2);

        listGrps = new GroupListView(layoutWidget);
        listGrps->setObjectName(QStringLiteral("listGrps"));
        listGrps->setMinimumSize(QSize(1, 0));
        listGrps->setContextMenuPolicy(Qt::CustomContextMenu);
        listGrps->setFrameShape(QFrame::NoFrame);
        listGrps->setDragDropMode(QAbstractItemView::DragDrop);
        listGrps->setDefaultDropAction(Qt::MoveAction);
        listGrps->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listGrps->setUniformItemSizes(true);

        verticalLayout_4->addWidget(listGrps);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btAddProgram = new QToolButton(layoutWidget1);
        btAddProgram->setObjectName(QStringLiteral("btAddProgram"));
        sizePolicy.setHeightForWidth(btAddProgram->sizePolicy().hasHeightForWidth());
        btAddProgram->setSizePolicy(sizePolicy);
        btAddProgram->setIcon(icon);
        btAddProgram->setIconSize(QSize(10, 10));

        horizontalLayout->addWidget(btAddProgram);

        btDelProgram = new QToolButton(layoutWidget1);
        btDelProgram->setObjectName(QStringLiteral("btDelProgram"));
        sizePolicy.setHeightForWidth(btDelProgram->sizePolicy().hasHeightForWidth());
        btDelProgram->setSizePolicy(sizePolicy);
        btDelProgram->setIcon(icon1);
        btDelProgram->setIconSize(QSize(10, 10));

        horizontalLayout->addWidget(btDelProgram);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setMinimumSize(QSize(1, 0));

        horizontalLayout->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        ProgAutosave = new QCheckBox(layoutWidget1);
        ProgAutosave->setObjectName(QStringLiteral("ProgAutosave"));
        sizePolicy2.setHeightForWidth(ProgAutosave->sizePolicy().hasHeightForWidth());
        ProgAutosave->setSizePolicy(sizePolicy2);
        ProgAutosave->setTristate(true);

        horizontalLayout->addWidget(ProgAutosave);


        verticalLayout_2->addLayout(horizontalLayout);

        listProgs = new ProgListView(layoutWidget1);
        listProgs->setObjectName(QStringLiteral("listProgs"));
        listProgs->setMinimumSize(QSize(1, 0));
        listProgs->setContextMenuPolicy(Qt::CustomContextMenu);
        listProgs->setFrameShape(QFrame::NoFrame);
        listProgs->setDragDropMode(QAbstractItemView::DragDrop);
        listProgs->setDefaultDropAction(Qt::MoveAction);
        listProgs->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listProgs->setUniformItemSizes(true);

        verticalLayout_2->addWidget(listProgs);

        splitter->addWidget(layoutWidget1);

        verticalLayout_3->addWidget(splitter);


        retranslateUi(ProgramList);
        QObject::connect(btAddGroup, SIGNAL(clicked()), listGrps, SLOT(InsertItem()));
        QObject::connect(btDelGroup, SIGNAL(clicked()), listGrps, SLOT(DeleteItem()));
        QObject::connect(btAddProgram, SIGNAL(clicked()), listProgs, SLOT(InsertItem()));
        QObject::connect(btDelProgram, SIGNAL(clicked()), listProgs, SLOT(DeleteItem()));

        QMetaObject::connectSlotsByName(ProgramList);
    } // setupUi

    void retranslateUi(QWidget *ProgramList)
    {
#ifndef QT_NO_TOOLTIP
        btAddGroup->setToolTip(QApplication::translate("ProgramList", "Add a group", 0));
#endif // QT_NO_TOOLTIP
        btAddGroup->setText(QApplication::translate("ProgramList", "...", 0));
#ifndef QT_NO_TOOLTIP
        btDelGroup->setToolTip(QApplication::translate("ProgramList", "Remove a group", 0));
#endif // QT_NO_TOOLTIP
        btDelGroup->setText(QApplication::translate("ProgramList", "...", 0));
        label->setText(QApplication::translate("ProgramList", "Groups", 0));
#ifndef QT_NO_TOOLTIP
        GroupAutosave->setToolTip(QApplication::translate("ProgramList", "Autosave groups", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        btAddProgram->setToolTip(QApplication::translate("ProgramList", "Add a program", 0));
#endif // QT_NO_TOOLTIP
        btAddProgram->setText(QApplication::translate("ProgramList", "...", 0));
#ifndef QT_NO_TOOLTIP
        btDelProgram->setToolTip(QApplication::translate("ProgramList", "Remove a program", 0));
#endif // QT_NO_TOOLTIP
        btDelProgram->setText(QApplication::translate("ProgramList", "...", 0));
        label_2->setText(QApplication::translate("ProgramList", "Programs", 0));
#ifndef QT_NO_TOOLTIP
        ProgAutosave->setToolTip(QApplication::translate("ProgramList", "Autosave programs", 0));
#endif // QT_NO_TOOLTIP
        ProgAutosave->setText(QString());
        Q_UNUSED(ProgramList);
    } // retranslateUi

};

namespace Ui {
    class ProgramList: public Ui_ProgramList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRAMLIST_H
