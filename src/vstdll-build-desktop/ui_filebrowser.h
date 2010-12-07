/********************************************************************************
** Form generated from reading UI file 'filebrowser.ui'
**
** Created: Thu 2. Dec 01:56:30 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEBROWSER_H
#define UI_FILEBROWSER_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

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
            FileBrowser->setObjectName(QString::fromUtf8("FileBrowser"));
        FileBrowser->resize(145, 300);
        FileBrowser->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(FileBrowser);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        path = new QLineEdit(FileBrowser);
        path->setObjectName(QString::fromUtf8("path"));

        gridLayout->addWidget(path, 0, 0, 1, 1);

        parentDir = new QPushButton(FileBrowser);
        parentDir->setObjectName(QString::fromUtf8("parentDir"));
        parentDir->setEnabled(true);
        parentDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(parentDir, 0, 1, 1, 1);

        rootDir = new QPushButton(FileBrowser);
        rootDir->setObjectName(QString::fromUtf8("rootDir"));
        rootDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(rootDir, 0, 2, 1, 1);

        previousDir = new QPushButton(FileBrowser);
        previousDir->setObjectName(QString::fromUtf8("previousDir"));
        previousDir->setEnabled(false);
        previousDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(previousDir, 0, 3, 1, 1);

        treeFiles = new QTreeView(FileBrowser);
        treeFiles->setObjectName(QString::fromUtf8("treeFiles"));
        treeFiles->setFrameShape(QFrame::NoFrame);
        treeFiles->setDragDropMode(QAbstractItemView::DragOnly);
        treeFiles->header()->setVisible(false);

        gridLayout->addWidget(treeFiles, 1, 0, 1, 5);

        nextDir = new QPushButton(FileBrowser);
        nextDir->setObjectName(QString::fromUtf8("nextDir"));
        nextDir->setEnabled(false);
        nextDir->setMaximumSize(QSize(18, 16777215));

        gridLayout->addWidget(nextDir, 0, 4, 1, 1);


        retranslateUi(FileBrowser);

        QMetaObject::connectSlotsByName(FileBrowser);
    } // setupUi

    void retranslateUi(QWidget *FileBrowser)
    {
        FileBrowser->setWindowTitle(QString());
        parentDir->setText(QApplication::translate("FileBrowser", "..", 0, QApplication::UnicodeUTF8));
        rootDir->setText(QApplication::translate("FileBrowser", "/", 0, QApplication::UnicodeUTF8));
        previousDir->setText(QApplication::translate("FileBrowser", "<", 0, QApplication::UnicodeUTF8));
        nextDir->setText(QApplication::translate("FileBrowser", ">", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileBrowser: public Ui_FileBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEBROWSER_H
