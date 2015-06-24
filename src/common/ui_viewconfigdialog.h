/********************************************************************************
** Form generated from reading UI file 'viewconfigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWCONFIGDIALOG_H
#define UI_VIEWCONFIGDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "views/gradientwidget.h"
#include "views/gradientwidgethue.h"

QT_BEGIN_NAMESPACE

class Ui_ViewConfigDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QCheckBox *checkSavedInSetupFile;
    QListWidget *listPresets;
    QLabel *label_5;
    QToolButton *addPreset;
    QToolButton *delPreset;
    QWidget *layoutWidget_2;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QLabel *label_7;
    QListWidget *listPalettes;
    QListWidget *listRoles;
    QWidget *layoutWidget_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupColor;
    QGridLayout *gridLayout;
    QLabel *label;
    QSlider *RedSlider;
    QSpinBox *RedSpinBox;
    QSlider *GreenSlider;
    QSpinBox *GreenSpinBox;
    QLabel *label_2;
    QSlider *BlueSlider;
    QLabel *label_4;
    QSpinBox *BlueSpinBox;
    QSlider *AlphaSlider;
    QLabel *label_3;
    QSpinBox *AlphaSpinBox;
    View::GradientWidget *picker;
    View::GradientWidgetHue *picker_hue;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QLabel *label_8;
    QDoubleSpinBox *fontProgSize;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer;
    QLabel *label_12;
    QCheckBox *fontProgItalic;
    QLabel *label_13;
    QComboBox *fontProgFamily;
    QLabel *label_11;
    QLabel *label_9;
    QDoubleSpinBox *fontProgStretch;
    QCheckBox *fontProgBold;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ViewConfigDialog)
    {
        if (ViewConfigDialog->objectName().isEmpty())
            ViewConfigDialog->setObjectName(QStringLiteral("ViewConfigDialog"));
        ViewConfigDialog->resize(722, 333);
        ViewConfigDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout_2 = new QVBoxLayout(ViewConfigDialog);
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(ViewConfigDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(5);
        gridLayout_4->setContentsMargins(5, 5, 5, 5);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        splitter = new QSplitter(groupBox);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(false);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        checkSavedInSetupFile = new QCheckBox(layoutWidget);
        checkSavedInSetupFile->setObjectName(QStringLiteral("checkSavedInSetupFile"));

        gridLayout_2->addWidget(checkSavedInSetupFile, 2, 0, 1, 5);

        listPresets = new QListWidget(layoutWidget);
        listPresets->setObjectName(QStringLiteral("listPresets"));
        listPresets->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        listPresets->setUniformItemSizes(true);

        gridLayout_2->addWidget(listPresets, 1, 0, 1, 5);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 3);

        addPreset = new QToolButton(layoutWidget);
        addPreset->setObjectName(QStringLiteral("addPreset"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/img16x16/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addPreset->setIcon(icon);

        gridLayout_2->addWidget(addPreset, 0, 3, 1, 1);

        delPreset = new QToolButton(layoutWidget);
        delPreset->setObjectName(QStringLiteral("delPreset"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/img16x16/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        delPreset->setIcon(icon1);

        gridLayout_2->addWidget(delPreset, 0, 4, 1, 1);

        splitter->addWidget(layoutWidget);
        layoutWidget_2 = new QWidget(splitter);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        gridLayout_3 = new QGridLayout(layoutWidget_2);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(layoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 0, 0, 1, 1);

        label_7 = new QLabel(layoutWidget_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 0, 1, 1, 1);

        listPalettes = new QListWidget(layoutWidget_2);
        listPalettes->setObjectName(QStringLiteral("listPalettes"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listPalettes->sizePolicy().hasHeightForWidth());
        listPalettes->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(listPalettes, 1, 0, 1, 1);

        listRoles = new QListWidget(layoutWidget_2);
        listRoles->setObjectName(QStringLiteral("listRoles"));
        sizePolicy1.setHeightForWidth(listRoles->sizePolicy().hasHeightForWidth());
        listRoles->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(listRoles, 1, 1, 1, 1);

        splitter->addWidget(layoutWidget_2);
        layoutWidget_3 = new QWidget(splitter);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        verticalLayout = new QVBoxLayout(layoutWidget_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupColor = new QGroupBox(layoutWidget_3);
        groupColor->setObjectName(QStringLiteral("groupColor"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupColor->sizePolicy().hasHeightForWidth());
        groupColor->setSizePolicy(sizePolicy2);
        gridLayout = new QGridLayout(groupColor);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupColor);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        RedSlider = new QSlider(groupColor);
        RedSlider->setObjectName(QStringLiteral("RedSlider"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(RedSlider->sizePolicy().hasHeightForWidth());
        RedSlider->setSizePolicy(sizePolicy3);
        RedSlider->setMaximum(255);
        RedSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(RedSlider, 1, 1, 1, 1);

        RedSpinBox = new QSpinBox(groupColor);
        RedSpinBox->setObjectName(QStringLiteral("RedSpinBox"));
        RedSpinBox->setMaximum(255);

        gridLayout->addWidget(RedSpinBox, 1, 2, 1, 1);

        GreenSlider = new QSlider(groupColor);
        GreenSlider->setObjectName(QStringLiteral("GreenSlider"));
        sizePolicy3.setHeightForWidth(GreenSlider->sizePolicy().hasHeightForWidth());
        GreenSlider->setSizePolicy(sizePolicy3);
        GreenSlider->setMaximum(255);
        GreenSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(GreenSlider, 2, 1, 1, 1);

        GreenSpinBox = new QSpinBox(groupColor);
        GreenSpinBox->setObjectName(QStringLiteral("GreenSpinBox"));
        GreenSpinBox->setMaximum(255);

        gridLayout->addWidget(GreenSpinBox, 2, 2, 1, 1);

        label_2 = new QLabel(groupColor);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        BlueSlider = new QSlider(groupColor);
        BlueSlider->setObjectName(QStringLiteral("BlueSlider"));
        sizePolicy3.setHeightForWidth(BlueSlider->sizePolicy().hasHeightForWidth());
        BlueSlider->setSizePolicy(sizePolicy3);
        BlueSlider->setMaximum(255);
        BlueSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(BlueSlider, 3, 1, 1, 1);

        label_4 = new QLabel(groupColor);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        BlueSpinBox = new QSpinBox(groupColor);
        BlueSpinBox->setObjectName(QStringLiteral("BlueSpinBox"));
        BlueSpinBox->setMaximum(255);

        gridLayout->addWidget(BlueSpinBox, 3, 2, 1, 1);

        AlphaSlider = new QSlider(groupColor);
        AlphaSlider->setObjectName(QStringLiteral("AlphaSlider"));
        sizePolicy3.setHeightForWidth(AlphaSlider->sizePolicy().hasHeightForWidth());
        AlphaSlider->setSizePolicy(sizePolicy3);
        AlphaSlider->setMaximum(255);
        AlphaSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(AlphaSlider, 4, 1, 1, 1);

        label_3 = new QLabel(groupColor);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        AlphaSpinBox = new QSpinBox(groupColor);
        AlphaSpinBox->setObjectName(QStringLiteral("AlphaSpinBox"));
        AlphaSpinBox->setMaximum(255);

        gridLayout->addWidget(AlphaSpinBox, 4, 2, 1, 1);

        picker = new View::GradientWidget(groupColor);
        picker->setObjectName(QStringLiteral("picker"));
        sizePolicy.setHeightForWidth(picker->sizePolicy().hasHeightForWidth());
        picker->setSizePolicy(sizePolicy);

        gridLayout->addWidget(picker, 6, 0, 1, 3);

        picker_hue = new View::GradientWidgetHue(groupColor);
        picker_hue->setObjectName(QStringLiteral("picker_hue"));
        sizePolicy.setHeightForWidth(picker_hue->sizePolicy().hasHeightForWidth());
        picker_hue->setSizePolicy(sizePolicy);

        gridLayout->addWidget(picker_hue, 5, 0, 1, 3);


        verticalLayout->addWidget(groupColor);

        splitter->addWidget(layoutWidget_3);

        gridLayout_4->addWidget(splitter, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(ViewConfigDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy4);
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setSpacing(5);
        gridLayout_5->setContentsMargins(5, 5, 5, 5);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(label_8, 2, 0, 1, 1);

        fontProgSize = new QDoubleSpinBox(groupBox_2);
        fontProgSize->setObjectName(QStringLiteral("fontProgSize"));
        fontProgSize->setDecimals(0);
        fontProgSize->setMinimum(0);
        fontProgSize->setValue(0);

        gridLayout_5->addWidget(fontProgSize, 2, 2, 1, 1);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy5.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(label_10, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 2, 9, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));
        sizePolicy5.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(label_12, 0, 7, 1, 1);

        fontProgItalic = new QCheckBox(groupBox_2);
        fontProgItalic->setObjectName(QStringLiteral("fontProgItalic"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(fontProgItalic->sizePolicy().hasHeightForWidth());
        fontProgItalic->setSizePolicy(sizePolicy6);

        gridLayout_5->addWidget(fontProgItalic, 2, 7, 1, 1);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        sizePolicy5.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(label_13, 0, 1, 1, 1);

        fontProgFamily = new QComboBox(groupBox_2);
        fontProgFamily->setObjectName(QStringLiteral("fontProgFamily"));

        gridLayout_5->addWidget(fontProgFamily, 2, 1, 1, 1);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        sizePolicy5.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(label_11, 0, 5, 1, 1);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy5.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy5);

        gridLayout_5->addWidget(label_9, 0, 8, 1, 1);

        fontProgStretch = new QDoubleSpinBox(groupBox_2);
        fontProgStretch->setObjectName(QStringLiteral("fontProgStretch"));
        fontProgStretch->setDecimals(0);
        fontProgStretch->setMinimum(5);
        fontProgStretch->setMaximum(1000);
        fontProgStretch->setSingleStep(10);
        fontProgStretch->setValue(100);

        gridLayout_5->addWidget(fontProgStretch, 2, 8, 1, 1);

        fontProgBold = new QCheckBox(groupBox_2);
        fontProgBold->setObjectName(QStringLiteral("fontProgBold"));
        sizePolicy6.setHeightForWidth(fontProgBold->sizePolicy().hasHeightForWidth());
        fontProgBold->setSizePolicy(sizePolicy6);

        gridLayout_5->addWidget(fontProgBold, 2, 5, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(ViewConfigDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy7);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);

        QWidget::setTabOrder(buttonBox, listPresets);
        QWidget::setTabOrder(listPresets, checkSavedInSetupFile);
        QWidget::setTabOrder(checkSavedInSetupFile, addPreset);
        QWidget::setTabOrder(addPreset, delPreset);
        QWidget::setTabOrder(delPreset, listPalettes);
        QWidget::setTabOrder(listPalettes, listRoles);
        QWidget::setTabOrder(listRoles, RedSlider);
        QWidget::setTabOrder(RedSlider, RedSpinBox);
        QWidget::setTabOrder(RedSpinBox, GreenSlider);
        QWidget::setTabOrder(GreenSlider, GreenSpinBox);
        QWidget::setTabOrder(GreenSpinBox, BlueSlider);
        QWidget::setTabOrder(BlueSlider, BlueSpinBox);
        QWidget::setTabOrder(BlueSpinBox, AlphaSlider);
        QWidget::setTabOrder(AlphaSlider, AlphaSpinBox);
        QWidget::setTabOrder(AlphaSpinBox, fontProgFamily);
        QWidget::setTabOrder(fontProgFamily, fontProgSize);
        QWidget::setTabOrder(fontProgSize, fontProgBold);
        QWidget::setTabOrder(fontProgBold, fontProgItalic);
        QWidget::setTabOrder(fontProgItalic, fontProgStretch);

        retranslateUi(ViewConfigDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ViewConfigDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ViewConfigDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ViewConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ViewConfigDialog)
    {
        ViewConfigDialog->setWindowTitle(QApplication::translate("ViewConfigDialog", "VstBoard Appearance", 0));
        groupBox->setTitle(QApplication::translate("ViewConfigDialog", "Colors", 0));
#ifndef QT_NO_TOOLTIP
        checkSavedInSetupFile->setToolTip(QApplication::translate("ViewConfigDialog", "Customize colors of the current setup file", 0));
#endif // QT_NO_TOOLTIP
        checkSavedInSetupFile->setText(QApplication::translate("ViewConfigDialog", "Saved in setup file", 0));
        label_5->setText(QApplication::translate("ViewConfigDialog", "Presets :", 0));
#ifndef QT_NO_TOOLTIP
        addPreset->setToolTip(QApplication::translate("ViewConfigDialog", "Add new preset", 0));
#endif // QT_NO_TOOLTIP
        addPreset->setText(QApplication::translate("ViewConfigDialog", "...", 0));
        addPreset->setShortcut(QApplication::translate("ViewConfigDialog", "Ins", 0));
#ifndef QT_NO_TOOLTIP
        delPreset->setToolTip(QApplication::translate("ViewConfigDialog", "Remove preset", 0));
#endif // QT_NO_TOOLTIP
        delPreset->setText(QApplication::translate("ViewConfigDialog", "...", 0));
        delPreset->setShortcut(QApplication::translate("ViewConfigDialog", "Del", 0));
        label_6->setText(QApplication::translate("ViewConfigDialog", "Objects :", 0));
        label_7->setText(QApplication::translate("ViewConfigDialog", "Elements :", 0));
        groupColor->setTitle(QApplication::translate("ViewConfigDialog", "Color", 0));
        label->setText(QApplication::translate("ViewConfigDialog", "Red", 0));
        label_2->setText(QApplication::translate("ViewConfigDialog", "Green", 0));
        label_4->setText(QApplication::translate("ViewConfigDialog", "Blue", 0));
        label_3->setText(QApplication::translate("ViewConfigDialog", "Alpha", 0));
        groupBox_2->setTitle(QApplication::translate("ViewConfigDialog", "Font", 0));
        label_8->setText(QApplication::translate("ViewConfigDialog", "Programs list :", 0));
        label_10->setText(QApplication::translate("ViewConfigDialog", "Size", 0));
        label_12->setText(QApplication::translate("ViewConfigDialog", "Italic", 0));
        fontProgItalic->setText(QString());
        label_13->setText(QApplication::translate("ViewConfigDialog", "Family", 0));
        label_11->setText(QApplication::translate("ViewConfigDialog", "Bold", 0));
        label_9->setText(QApplication::translate("ViewConfigDialog", "Stretch", 0));
        fontProgBold->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ViewConfigDialog: public Ui_ViewConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWCONFIGDIALOG_H
