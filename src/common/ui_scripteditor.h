/********************************************************************************
** Form generated from reading UI file 'scripteditor.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRIPTEDITOR_H
#define UI_SCRIPTEDITOR_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScriptEditor
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btExecute;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *ScriptEditor)
    {
        if (ScriptEditor->objectName().isEmpty())
            ScriptEditor->setObjectName(QStringLiteral("ScriptEditor"));
        ScriptEditor->resize(400, 300);
        ScriptEditor->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(ScriptEditor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btExecute = new QPushButton(ScriptEditor);
        btExecute->setObjectName(QStringLiteral("btExecute"));

        horizontalLayout->addWidget(btExecute);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        plainTextEdit = new QPlainTextEdit(ScriptEditor);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setDocumentTitle(QStringLiteral(""));
        plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        plainTextEdit->setPlainText(QStringLiteral(""));
        plainTextEdit->setTabStopWidth(30);

        verticalLayout->addWidget(plainTextEdit);


        retranslateUi(ScriptEditor);

        QMetaObject::connectSlotsByName(ScriptEditor);
    } // setupUi

    void retranslateUi(QWidget *ScriptEditor)
    {
        ScriptEditor->setWindowTitle(QApplication::translate("ScriptEditor", "Form", 0));
        btExecute->setText(QApplication::translate("ScriptEditor", "Execute", 0));
        btExecute->setShortcut(QApplication::translate("ScriptEditor", "Ctrl+E", 0));
    } // retranslateUi

};

namespace Ui {
    class ScriptEditor: public Ui_ScriptEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTEDITOR_H
