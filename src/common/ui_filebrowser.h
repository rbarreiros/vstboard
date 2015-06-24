/********************************************************************************
** Form generated from reading UI file 'filebrowser.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEBROWSER_H
#define UI_FILEBROWSER_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileBrowser
{
public:
    QGridLayout *gridLayout;
    QLineEdit *path;
    QPushButton *parentDir;
    QPushButton *rootDir;
    QPushButton *previousDir;
    QTreeView *treeFiles;
    QPushButton *nextDir;

    void setupUi(QWidget *FileBrowser)
    {
        if (FileBrowser->objectName().isEmpty())
            FileBrowser->setObjectName(QStringLiteral("FileBrowser"));
        FileBrowser->resize(145, 300);
        FileBrowser->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(FileBrowser);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        path = new QLineEdit(FileBrowser);
        path->setObjectName(QStringLiteral("path"));

        gridLayout->addWidget(path, 0, 0, 1, 1);

        parentDir = new QPushButton(FileBrowser);
        parentDir->setObjectName(QStringLiteral("parentDir"));
        parentDir->setEnabled(true);
        parentDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(parentDir, 0, 1, 1, 1);

        rootDir = new QPushButton(FileBrowser);
        rootDir->setObjectName(QStringLiteral("rootDir"));
        rootDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(rootDir, 0, 2, 1, 1);

        previousDir = new QPushButton(FileBrowser);
        previousDir->setObjectName(QStringLiteral("previousDir"));
        previousDir->setEnabled(false);
        previousDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(previousDir, 0, 3, 1, 1);

        treeFiles = new QTreeView(FileBrowser);
        treeFiles->setObjectName(QStringLiteral("treeFiles"));
        treeFiles->setContextMenuPolicy(Qt::CustomContextMenu);
        treeFiles->setFrameShape(QFrame::NoFrame);
        treeFiles->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        treeFiles->setDragDropMode(QAbstractItemView::DragDrop);
        treeFiles->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeFiles->setUniformRowHeights(true);
        treeFiles->setSortingEnabled(false);
        treeFiles->setAnimated(true);
        treeFiles->header()->setVisible(false);

        gridLayout->addWidget(treeFiles, 1, 0, 1, 5);

        nextDir = new QPushButton(FileBrowser);
        nextDir->setObjectName(QStringLiteral("nextDir"));
        nextDir->setEnabled(false);
        nextDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(nextDir, 0, 4, 1, 1);


        retranslateUi(FileBrowser);

        QMetaObject::connectSlotsByName(FileBrowser);
    } // setupUi

    void retranslateUi(QWidget *FileBrowser)
    {
        FileBrowser->setWindowTitle(QString());
        parentDir->setText(QApplication::translate("FileBrowser", "..", 0));
        rootDir->setText(QApplication::translate("FileBrowser", "/", 0));
        previousDir->setText(QApplication::translate("FileBrowser", "<", 0));
        nextDir->setText(QApplication::translate("FileBrowser", ">", 0));
    } // retranslateUi

};

namespace Ui {
    class FileBrowser: public Ui_FileBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEBROWSER_H
