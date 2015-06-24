/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QUndoView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "views/filebrowser.h"
#include "views/listaudiodevicesview.h"
#include "views/maingraphicsview.h"
#include "views/programlist.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionLoad;
    QAction *actionNew;
    QAction *actionConfig;
    QAction *actionShortcuts;
    QAction *actionDelete;
    QAction *actionSave_Setup;
    QAction *actionNew_Setup;
    QAction *actionHost_panel;
    QAction *actionAbout;
    QAction *actionSave_Project_As;
    QAction *actionSave_Setup_As;
    QAction *actionGroup_panel;
    QAction *actionProgram_panel;
    QAction *actionRefresh_Audio_devices;
    QAction *actionRefresh_Midi_devices;
    QAction *actionRestore_default_layout;
    QAction *actionTool_bar;
    QAction *actionProject_panel;
    QAction *actionLoad_Setup;
    QAction *actionAppearance;
    QAction *actionCable;
    QAction *actionValue;
    QAction *actionKeyBinding;
    QAction *actionHide_all_editors;
    QAction *actionAutoShowGui;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_9;
    QSplitter *splitterPanels;
    QGroupBox *HostBox;
    QHBoxLayout *horizontalLayout_2;
    MainGraphicsView *hostView;
    QGroupBox *ProjectBox;
    QHBoxLayout *horizontalLayout_5;
    MainGraphicsView *projectView;
    QGroupBox *ProgramBox;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitterProg;
    MainGraphicsView *programView;
    QListView *programParkList;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QSplitter *splitterGroup;
    MainGraphicsView *groupView;
    QListView *groupParkList;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuEdit;
    QMenu *menuTool;
    QMenu *menuFile;
    QMenu *menuRecent_Setups;
    QMenu *menuRecent_Projects;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockPrograms;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_10;
    ProgramList *Programs;
    QDockWidget *dockMidiDevices;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout_8;
    QTreeView *treeMidiInterfaces;
    QDockWidget *dockVstBrowser;
    QWidget *dockWidgetContents_5;
    QVBoxLayout *verticalLayout_7;
    FileBrowser *VstBrowser;
    QDockWidget *dockAudioDevices;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_6;
    ListAudioDevicesView *treeAudioInterfaces;
    QDockWidget *dockSolver;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_5;
    QTableView *solverView;
    QDockWidget *dockTools;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_4;
    QTreeView *treeTools;
    QDockWidget *dockHostModel;
    QWidget *dockWidgetContents_8;
    QVBoxLayout *verticalLayout_3;
    QTreeView *treeHostModel;
    QDockWidget *dockBankBrowser;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout;
    FileBrowser *BankBrowser;
    QDockWidget *dockUndo;
    QWidget *dockWidgetContents_9;
    QVBoxLayout *verticalLayout_2;
    QUndoView *listUndo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(846, 570);
        MainWindow->setFocusPolicy(Qt::StrongFocus);
        QIcon icon;
        icon.addFile(QStringLiteral(":/vstboard32_8.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        MainWindow->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        MainWindow->setDockNestingEnabled(true);
        MainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/img16x16/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon1);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/img16x16/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad->setIcon(icon2);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/img16x16/filenew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon3);
        actionConfig = new QAction(MainWindow);
        actionConfig->setObjectName(QStringLiteral("actionConfig"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/img16x16/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfig->setIcon(icon4);
        actionShortcuts = new QAction(MainWindow);
        actionShortcuts->setObjectName(QStringLiteral("actionShortcuts"));
        actionShortcuts->setCheckable(true);
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QStringLiteral("actionDelete"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/img16x16/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon5);
        actionSave_Setup = new QAction(MainWindow);
        actionSave_Setup->setObjectName(QStringLiteral("actionSave_Setup"));
        actionSave_Setup->setIcon(icon1);
        actionNew_Setup = new QAction(MainWindow);
        actionNew_Setup->setObjectName(QStringLiteral("actionNew_Setup"));
        actionNew_Setup->setIcon(icon3);
        actionHost_panel = new QAction(MainWindow);
        actionHost_panel->setObjectName(QStringLiteral("actionHost_panel"));
        actionHost_panel->setCheckable(true);
        actionHost_panel->setChecked(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/img16x16/file_setup.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHost_panel->setIcon(icon6);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/img16x16/documentinfo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon7);
        actionSave_Project_As = new QAction(MainWindow);
        actionSave_Project_As->setObjectName(QStringLiteral("actionSave_Project_As"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/img16x16/filesaveas.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Project_As->setIcon(icon8);
        actionSave_Setup_As = new QAction(MainWindow);
        actionSave_Setup_As->setObjectName(QStringLiteral("actionSave_Setup_As"));
        actionSave_Setup_As->setIcon(icon8);
        actionGroup_panel = new QAction(MainWindow);
        actionGroup_panel->setObjectName(QStringLiteral("actionGroup_panel"));
        actionGroup_panel->setCheckable(true);
        actionGroup_panel->setChecked(true);
        actionGroup_panel->setIcon(icon6);
        actionProgram_panel = new QAction(MainWindow);
        actionProgram_panel->setObjectName(QStringLiteral("actionProgram_panel"));
        actionProgram_panel->setCheckable(true);
        actionProgram_panel->setChecked(true);
        actionProgram_panel->setIcon(icon6);
        actionRefresh_Audio_devices = new QAction(MainWindow);
        actionRefresh_Audio_devices->setObjectName(QStringLiteral("actionRefresh_Audio_devices"));
        actionRefresh_Midi_devices = new QAction(MainWindow);
        actionRefresh_Midi_devices->setObjectName(QStringLiteral("actionRefresh_Midi_devices"));
        actionRestore_default_layout = new QAction(MainWindow);
        actionRestore_default_layout->setObjectName(QStringLiteral("actionRestore_default_layout"));
        actionTool_bar = new QAction(MainWindow);
        actionTool_bar->setObjectName(QStringLiteral("actionTool_bar"));
        actionTool_bar->setCheckable(true);
        actionProject_panel = new QAction(MainWindow);
        actionProject_panel->setObjectName(QStringLiteral("actionProject_panel"));
        actionProject_panel->setCheckable(true);
        actionProject_panel->setChecked(true);
        actionProject_panel->setIcon(icon6);
        actionLoad_Setup = new QAction(MainWindow);
        actionLoad_Setup->setObjectName(QStringLiteral("actionLoad_Setup"));
        actionLoad_Setup->setIcon(icon2);
        actionAppearance = new QAction(MainWindow);
        actionAppearance->setObjectName(QStringLiteral("actionAppearance"));
        actionAppearance->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/img16x16/appearance.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAppearance->setIcon(icon9);
        actionCable = new QAction(MainWindow);
        actionCable->setObjectName(QStringLiteral("actionCable"));
        actionCable->setCheckable(true);
        actionCable->setChecked(true);
        actionValue = new QAction(MainWindow);
        actionValue->setObjectName(QStringLiteral("actionValue"));
        actionValue->setCheckable(true);
        actionKeyBinding = new QAction(MainWindow);
        actionKeyBinding->setObjectName(QStringLiteral("actionKeyBinding"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/img16x16/key_bindings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionKeyBinding->setIcon(icon10);
        actionHide_all_editors = new QAction(MainWindow);
        actionHide_all_editors->setObjectName(QStringLiteral("actionHide_all_editors"));
        actionHide_all_editors->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/img16x16/14_layer_lowerlayer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHide_all_editors->setIcon(icon11);
        actionAutoShowGui = new QAction(MainWindow);
        actionAutoShowGui->setObjectName(QStringLiteral("actionAutoShowGui"));
        actionAutoShowGui->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/img16x16/14_layer_raiselayer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAutoShowGui->setIcon(icon12);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout_9 = new QVBoxLayout(centralWidget);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        splitterPanels = new QSplitter(centralWidget);
        splitterPanels->setObjectName(QStringLiteral("splitterPanels"));
        splitterPanels->setOrientation(Qt::Vertical);
        splitterPanels->setChildrenCollapsible(false);
        HostBox = new QGroupBox(splitterPanels);
        HostBox->setObjectName(QStringLiteral("HostBox"));
        HostBox->setAlignment(Qt::AlignCenter);
        HostBox->setFlat(true);
        horizontalLayout_2 = new QHBoxLayout(HostBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        hostView = new MainGraphicsView(HostBox);
        hostView->setObjectName(QStringLiteral("hostView"));
        hostView->setFrameShape(QFrame::NoFrame);
        hostView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hostView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hostView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        hostView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        hostView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout_2->addWidget(hostView);

        splitterPanels->addWidget(HostBox);
        ProjectBox = new QGroupBox(splitterPanels);
        ProjectBox->setObjectName(QStringLiteral("ProjectBox"));
        ProjectBox->setAlignment(Qt::AlignCenter);
        ProjectBox->setFlat(true);
        horizontalLayout_5 = new QHBoxLayout(ProjectBox);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        projectView = new MainGraphicsView(ProjectBox);
        projectView->setObjectName(QStringLiteral("projectView"));
        projectView->setFrameShape(QFrame::NoFrame);
        projectView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        projectView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        projectView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        projectView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        projectView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout_5->addWidget(projectView);

        splitterPanels->addWidget(ProjectBox);
        ProgramBox = new QGroupBox(splitterPanels);
        ProgramBox->setObjectName(QStringLiteral("ProgramBox"));
        ProgramBox->setAlignment(Qt::AlignCenter);
        ProgramBox->setFlat(true);
        horizontalLayout = new QHBoxLayout(ProgramBox);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        splitterProg = new QSplitter(ProgramBox);
        splitterProg->setObjectName(QStringLiteral("splitterProg"));
        splitterProg->setOrientation(Qt::Horizontal);
        splitterProg->setHandleWidth(2);
        programView = new MainGraphicsView(splitterProg);
        programView->setObjectName(QStringLiteral("programView"));
        programView->setFrameShape(QFrame::NoFrame);
        programView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        programView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        programView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        programView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        programView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        splitterProg->addWidget(programView);
        programParkList = new QListView(splitterProg);
        programParkList->setObjectName(QStringLiteral("programParkList"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(programParkList->sizePolicy().hasHeightForWidth());
        programParkList->setSizePolicy(sizePolicy1);
        programParkList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        programParkList->setDragEnabled(true);
        programParkList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        programParkList->setUniformItemSizes(true);
        splitterProg->addWidget(programParkList);

        horizontalLayout->addWidget(splitterProg);

        splitterPanels->addWidget(ProgramBox);
        groupBox = new QGroupBox(splitterPanels);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(true);
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        splitterGroup = new QSplitter(groupBox);
        splitterGroup->setObjectName(QStringLiteral("splitterGroup"));
        splitterGroup->setOrientation(Qt::Horizontal);
        splitterGroup->setHandleWidth(2);
        groupView = new MainGraphicsView(splitterGroup);
        groupView->setObjectName(QStringLiteral("groupView"));
        groupView->setFrameShape(QFrame::NoFrame);
        groupView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        groupView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        groupView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        groupView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        groupView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        splitterGroup->addWidget(groupView);
        groupParkList = new QListView(splitterGroup);
        groupParkList->setObjectName(QStringLiteral("groupParkList"));
        sizePolicy1.setHeightForWidth(groupParkList->sizePolicy().hasHeightForWidth());
        groupParkList->setSizePolicy(sizePolicy1);
        groupParkList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        groupParkList->setDragEnabled(true);
        groupParkList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        groupParkList->setUniformItemSizes(true);
        splitterGroup->addWidget(groupParkList);

        horizontalLayout_3->addWidget(splitterGroup);

        splitterPanels->addWidget(groupBox);

        verticalLayout_9->addWidget(splitterPanels);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 846, 21));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuTool = new QMenu(menuEdit);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRecent_Setups = new QMenu(menuFile);
        menuRecent_Setups->setObjectName(QStringLiteral("menuRecent_Setups"));
        menuRecent_Projects = new QMenu(menuFile);
        menuRecent_Projects->setObjectName(QStringLiteral("menuRecent_Projects"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockPrograms = new QDockWidget(MainWindow);
        dockPrograms->setObjectName(QStringLiteral("dockPrograms"));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockPrograms->sizePolicy().hasHeightForWidth());
        dockPrograms->setSizePolicy(sizePolicy2);
        dockPrograms->setMinimumSize(QSize(80, 50));
        dockPrograms->setFloating(false);
        dockPrograms->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockPrograms->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        Programs = new ProgramList(dockWidgetContents_2);
        Programs->setObjectName(QStringLiteral("Programs"));
        QSizePolicy sizePolicy3(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Programs->sizePolicy().hasHeightForWidth());
        Programs->setSizePolicy(sizePolicy3);

        verticalLayout_10->addWidget(Programs);

        dockPrograms->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockPrograms);
        dockMidiDevices = new QDockWidget(MainWindow);
        dockMidiDevices->setObjectName(QStringLiteral("dockMidiDevices"));
        dockMidiDevices->setMinimumSize(QSize(80, 91));
        dockMidiDevices->setAcceptDrops(false);
        dockMidiDevices->setFloating(false);
        dockMidiDevices->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockMidiDevices->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        treeMidiInterfaces = new QTreeView(dockWidgetContents_6);
        treeMidiInterfaces->setObjectName(QStringLiteral("treeMidiInterfaces"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(treeMidiInterfaces->sizePolicy().hasHeightForWidth());
        treeMidiInterfaces->setSizePolicy(sizePolicy4);
        treeMidiInterfaces->setContextMenuPolicy(Qt::ActionsContextMenu);
        treeMidiInterfaces->setFrameShape(QFrame::NoFrame);
        treeMidiInterfaces->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeMidiInterfaces->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeMidiInterfaces->setProperty("showDropIndicator", QVariant(false));
        treeMidiInterfaces->setDragDropMode(QAbstractItemView::DragOnly);
        treeMidiInterfaces->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeMidiInterfaces->setIndentation(10);
        treeMidiInterfaces->setUniformRowHeights(true);
        treeMidiInterfaces->setSortingEnabled(true);
        treeMidiInterfaces->setAnimated(true);
        treeMidiInterfaces->header()->setStretchLastSection(false);

        verticalLayout_8->addWidget(treeMidiInterfaces);

        dockMidiDevices->setWidget(dockWidgetContents_6);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockMidiDevices);
        dockVstBrowser = new QDockWidget(MainWindow);
        dockVstBrowser->setObjectName(QStringLiteral("dockVstBrowser"));
        dockVstBrowser->setMinimumSize(QSize(80, 50));
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        verticalLayout_7 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        VstBrowser = new FileBrowser(dockWidgetContents_5);
        VstBrowser->setObjectName(QStringLiteral("VstBrowser"));
        sizePolicy4.setHeightForWidth(VstBrowser->sizePolicy().hasHeightForWidth());
        VstBrowser->setSizePolicy(sizePolicy4);

        verticalLayout_7->addWidget(VstBrowser);

        dockVstBrowser->setWidget(dockWidgetContents_5);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockVstBrowser);
        dockAudioDevices = new QDockWidget(MainWindow);
        dockAudioDevices->setObjectName(QStringLiteral("dockAudioDevices"));
        dockAudioDevices->setMinimumSize(QSize(80, 91));
        dockAudioDevices->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        treeAudioInterfaces = new ListAudioDevicesView(dockWidgetContents);
        treeAudioInterfaces->setObjectName(QStringLiteral("treeAudioInterfaces"));
        sizePolicy4.setHeightForWidth(treeAudioInterfaces->sizePolicy().hasHeightForWidth());
        treeAudioInterfaces->setSizePolicy(sizePolicy4);
        treeAudioInterfaces->setContextMenuPolicy(Qt::CustomContextMenu);
        treeAudioInterfaces->setFrameShape(QFrame::NoFrame);
        treeAudioInterfaces->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeAudioInterfaces->setProperty("showDropIndicator", QVariant(false));
        treeAudioInterfaces->setDragEnabled(true);
        treeAudioInterfaces->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeAudioInterfaces->setIndentation(10);
        treeAudioInterfaces->setUniformRowHeights(true);
        treeAudioInterfaces->setSortingEnabled(true);
        treeAudioInterfaces->setAnimated(true);
        treeAudioInterfaces->header()->setStretchLastSection(false);

        verticalLayout_6->addWidget(treeAudioInterfaces);

        dockAudioDevices->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockAudioDevices);
        dockSolver = new QDockWidget(MainWindow);
        dockSolver->setObjectName(QStringLiteral("dockSolver"));
        sizePolicy2.setHeightForWidth(dockSolver->sizePolicy().hasHeightForWidth());
        dockSolver->setSizePolicy(sizePolicy2);
        dockSolver->setMinimumSize(QSize(80, 91));
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        solverView = new QTableView(dockWidgetContents_4);
        solverView->setObjectName(QStringLiteral("solverView"));
        sizePolicy3.setHeightForWidth(solverView->sizePolicy().hasHeightForWidth());
        solverView->setSizePolicy(sizePolicy3);
        solverView->setFrameShape(QFrame::NoFrame);
        solverView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        solverView->setProperty("showDropIndicator", QVariant(false));
        solverView->setAlternatingRowColors(true);
        solverView->setSelectionMode(QAbstractItemView::NoSelection);
        solverView->setCornerButtonEnabled(false);

        verticalLayout_5->addWidget(solverView);

        dockSolver->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockSolver);
        dockTools = new QDockWidget(MainWindow);
        dockTools->setObjectName(QStringLiteral("dockTools"));
        dockTools->setMinimumSize(QSize(80, 91));
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QStringLiteral("dockWidgetContents_7"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        treeTools = new QTreeView(dockWidgetContents_7);
        treeTools->setObjectName(QStringLiteral("treeTools"));
        sizePolicy4.setHeightForWidth(treeTools->sizePolicy().hasHeightForWidth());
        treeTools->setSizePolicy(sizePolicy4);
        treeTools->setFrameShape(QFrame::NoFrame);
        treeTools->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeTools->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeTools->setProperty("showDropIndicator", QVariant(false));
        treeTools->setDragDropMode(QAbstractItemView::DragOnly);
        treeTools->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeTools->setIndentation(10);
        treeTools->setUniformRowHeights(true);
        treeTools->setSortingEnabled(false);
        treeTools->setAnimated(true);
        treeTools->setHeaderHidden(true);
        treeTools->header()->setStretchLastSection(false);

        verticalLayout_4->addWidget(treeTools);

        dockTools->setWidget(dockWidgetContents_7);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockTools);
        dockHostModel = new QDockWidget(MainWindow);
        dockHostModel->setObjectName(QStringLiteral("dockHostModel"));
        sizePolicy2.setHeightForWidth(dockHostModel->sizePolicy().hasHeightForWidth());
        dockHostModel->setSizePolicy(sizePolicy2);
        dockHostModel->setMinimumSize(QSize(80, 91));
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QStringLiteral("dockWidgetContents_8"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_8);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        treeHostModel = new QTreeView(dockWidgetContents_8);
        treeHostModel->setObjectName(QStringLiteral("treeHostModel"));
        sizePolicy3.setHeightForWidth(treeHostModel->sizePolicy().hasHeightForWidth());
        treeHostModel->setSizePolicy(sizePolicy3);
        treeHostModel->setFrameShape(QFrame::NoFrame);
        treeHostModel->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        treeHostModel->setDragEnabled(true);
        treeHostModel->setDragDropMode(QAbstractItemView::DragDrop);
        treeHostModel->setDefaultDropAction(Qt::IgnoreAction);
        treeHostModel->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeHostModel->setIndentation(10);
        treeHostModel->setUniformRowHeights(true);
        treeHostModel->setSortingEnabled(true);
        treeHostModel->setAnimated(true);

        verticalLayout_3->addWidget(treeHostModel);

        dockHostModel->setWidget(dockWidgetContents_8);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockHostModel);
        dockBankBrowser = new QDockWidget(MainWindow);
        dockBankBrowser->setObjectName(QStringLiteral("dockBankBrowser"));
        dockBankBrowser->setMinimumSize(QSize(80, 50));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        verticalLayout = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        BankBrowser = new FileBrowser(dockWidgetContents_3);
        BankBrowser->setObjectName(QStringLiteral("BankBrowser"));

        verticalLayout->addWidget(BankBrowser);

        dockBankBrowser->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockBankBrowser);
        dockUndo = new QDockWidget(MainWindow);
        dockUndo->setObjectName(QStringLiteral("dockUndo"));
        dockUndo->setMinimumSize(QSize(80, 91));
        dockWidgetContents_9 = new QWidget();
        dockWidgetContents_9->setObjectName(QStringLiteral("dockWidgetContents_9"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_9);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        listUndo = new QUndoView(dockWidgetContents_9);
        listUndo->setObjectName(QStringLiteral("listUndo"));
        listUndo->setFrameShape(QFrame::NoFrame);
        listUndo->setUniformItemSizes(true);

        verticalLayout_2->addWidget(listUndo);

        dockUndo->setWidget(dockWidgetContents_9);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockUndo);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuView->addAction(actionHost_panel);
        menuView->addAction(actionProject_panel);
        menuView->addAction(actionProgram_panel);
        menuView->addAction(actionGroup_panel);
        menuView->addSeparator();
        menuView->addAction(actionRestore_default_layout);
        menuView->addAction(actionTool_bar);
        menuView->addAction(actionAppearance);
        menuView->addAction(actionAutoShowGui);
        menuView->addAction(actionHide_all_editors);
        menuView->addSeparator();
        menuEdit->addAction(actionConfig);
        menuEdit->addAction(actionKeyBinding);
        menuEdit->addAction(actionRefresh_Audio_devices);
        menuEdit->addAction(actionRefresh_Midi_devices);
        menuEdit->addAction(menuTool->menuAction());
        menuTool->addAction(actionCable);
        menuTool->addAction(actionValue);
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_Project_As);
        menuFile->addAction(actionNew);
        menuFile->addAction(menuRecent_Projects->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_Setup);
        menuFile->addAction(actionSave_Setup);
        menuFile->addAction(actionSave_Setup_As);
        menuFile->addAction(actionNew_Setup);
        menuFile->addAction(menuRecent_Setups->menuAction());
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionLoad);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionHost_panel);
        mainToolBar->addAction(actionProject_panel);
        mainToolBar->addAction(actionProgram_panel);
        mainToolBar->addAction(actionGroup_panel);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionConfig);
        mainToolBar->addAction(actionAppearance);
        mainToolBar->addAction(actionKeyBinding);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCable);
        mainToolBar->addAction(actionValue);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAutoShowGui);
        mainToolBar->addAction(actionHide_all_editors);

        retranslateUi(MainWindow);
        QObject::connect(actionHost_panel, SIGNAL(toggled(bool)), HostBox, SLOT(setVisible(bool)));
        QObject::connect(actionGroup_panel, SIGNAL(toggled(bool)), groupBox, SLOT(setVisible(bool)));
        QObject::connect(actionProgram_panel, SIGNAL(toggled(bool)), ProgramBox, SLOT(setVisible(bool)));
        QObject::connect(actionTool_bar, SIGNAL(toggled(bool)), mainToolBar, SLOT(setVisible(bool)));
        QObject::connect(actionProject_panel, SIGNAL(toggled(bool)), ProjectBox, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "VstBoard", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save Project", 0));
#ifndef QT_NO_STATUSTIP
        actionSave->setStatusTip(QApplication::translate("MainWindow", "Save the Program and Group panels in a project file", 0));
#endif // QT_NO_STATUSTIP
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionLoad->setText(QApplication::translate("MainWindow", "Open Project", 0));
#ifndef QT_NO_TOOLTIP
        actionLoad->setToolTip(QApplication::translate("MainWindow", "Open a Project file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionLoad->setStatusTip(QApplication::translate("MainWindow", "Open a Project file", 0));
#endif // QT_NO_STATUSTIP
        actionLoad->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New Project", 0));
#ifndef QT_NO_STATUSTIP
        actionNew->setStatusTip(QApplication::translate("MainWindow", "Clear the Program and Group panels", 0));
#endif // QT_NO_STATUSTIP
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        actionConfig->setText(QApplication::translate("MainWindow", "Configuration", 0));
#ifndef QT_NO_STATUSTIP
        actionConfig->setStatusTip(QApplication::translate("MainWindow", "Show the configuration dialog", 0));
#endif // QT_NO_STATUSTIP
        actionShortcuts->setText(QApplication::translate("MainWindow", "Edit Shortcuts (testing)", 0));
#ifndef QT_NO_STATUSTIP
        actionShortcuts->setStatusTip(QApplication::translate("MainWindow", "Edit Shortcuts (testing)", 0));
#endif // QT_NO_STATUSTIP
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0));
#ifndef QT_NO_STATUSTIP
        actionDelete->setStatusTip(QApplication::translate("MainWindow", "Delete", 0));
#endif // QT_NO_STATUSTIP
        actionSave_Setup->setText(QApplication::translate("MainWindow", "Save Setup", 0));
#ifndef QT_NO_STATUSTIP
        actionSave_Setup->setStatusTip(QApplication::translate("MainWindow", "Save the Host panel in a setup file", 0));
#endif // QT_NO_STATUSTIP
        actionSave_Setup->setShortcut(QApplication::translate("MainWindow", "Alt+S", 0));
        actionNew_Setup->setText(QApplication::translate("MainWindow", "New Setup", 0));
#ifndef QT_NO_STATUSTIP
        actionNew_Setup->setStatusTip(QApplication::translate("MainWindow", "Clear the Host panel", 0));
#endif // QT_NO_STATUSTIP
        actionNew_Setup->setShortcut(QApplication::translate("MainWindow", "Alt+N", 0));
        actionHost_panel->setText(QApplication::translate("MainWindow", "Toggle Host panel", 0));
#ifndef QT_NO_STATUSTIP
        actionHost_panel->setStatusTip(QApplication::translate("MainWindow", "Show or hide the host panel", 0));
#endif // QT_NO_STATUSTIP
        actionHost_panel->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0));
#ifndef QT_NO_STATUSTIP
        actionAbout->setStatusTip(QApplication::translate("MainWindow", "About", 0));
#endif // QT_NO_STATUSTIP
        actionSave_Project_As->setText(QApplication::translate("MainWindow", "Save Project As ...", 0));
#ifndef QT_NO_STATUSTIP
        actionSave_Project_As->setStatusTip(QApplication::translate("MainWindow", "Save Project As", 0));
#endif // QT_NO_STATUSTIP
        actionSave_Project_As->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0));
        actionSave_Setup_As->setText(QApplication::translate("MainWindow", "Save Setup As ...", 0));
#ifndef QT_NO_STATUSTIP
        actionSave_Setup_As->setStatusTip(QApplication::translate("MainWindow", "Save Setup As", 0));
#endif // QT_NO_STATUSTIP
        actionSave_Setup_As->setShortcut(QApplication::translate("MainWindow", "Alt+Shift+S", 0));
        actionGroup_panel->setText(QApplication::translate("MainWindow", "Toggle Group panel", 0));
#ifndef QT_NO_TOOLTIP
        actionGroup_panel->setToolTip(QApplication::translate("MainWindow", "Toggle Group panel", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionGroup_panel->setStatusTip(QApplication::translate("MainWindow", "Show or hide the group panel", 0));
#endif // QT_NO_STATUSTIP
        actionGroup_panel->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", 0));
        actionProgram_panel->setText(QApplication::translate("MainWindow", "Toggle Program panel", 0));
#ifndef QT_NO_STATUSTIP
        actionProgram_panel->setStatusTip(QApplication::translate("MainWindow", "Show or hide the program panel", 0));
#endif // QT_NO_STATUSTIP
        actionProgram_panel->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0));
        actionRefresh_Audio_devices->setText(QApplication::translate("MainWindow", "Refresh Audio devices", 0));
        actionRefresh_Midi_devices->setText(QApplication::translate("MainWindow", "Refresh Midi devices", 0));
        actionRestore_default_layout->setText(QApplication::translate("MainWindow", "Restore default layout", 0));
        actionTool_bar->setText(QApplication::translate("MainWindow", "Tool bar", 0));
        actionProject_panel->setText(QApplication::translate("MainWindow", "Toggle Project panel", 0));
        actionLoad_Setup->setText(QApplication::translate("MainWindow", "Open Setup", 0));
#ifndef QT_NO_TOOLTIP
        actionLoad_Setup->setToolTip(QApplication::translate("MainWindow", "Open a Setup file", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionLoad_Setup->setStatusTip(QApplication::translate("MainWindow", "Open a Setup file", 0));
#endif // QT_NO_STATUSTIP
        actionLoad_Setup->setShortcut(QApplication::translate("MainWindow", "Alt+O", 0));
        actionAppearance->setText(QApplication::translate("MainWindow", "Appearance", 0));
        actionCable->setText(QApplication::translate("MainWindow", "Cable", 0));
        actionCable->setShortcut(QApplication::translate("MainWindow", "C", 0));
        actionValue->setText(QApplication::translate("MainWindow", "Value", 0));
        actionValue->setShortcut(QApplication::translate("MainWindow", "V", 0));
        actionKeyBinding->setText(QApplication::translate("MainWindow", "Keyboard mapping", 0));
#ifndef QT_NO_TOOLTIP
        actionKeyBinding->setToolTip(QApplication::translate("MainWindow", "Keyboard and Mouse mapping", 0));
#endif // QT_NO_TOOLTIP
        actionHide_all_editors->setText(QApplication::translate("MainWindow", "Hide all plugins editors", 0));
        actionHide_all_editors->setShortcut(QApplication::translate("MainWindow", "H", 0));
        actionAutoShowGui->setText(QApplication::translate("MainWindow", "Auto-open plugins editors", 0));
        actionAutoShowGui->setShortcut(QApplication::translate("MainWindow", "G", 0));
        HostBox->setTitle(QApplication::translate("MainWindow", "Host Panel", 0));
        ProjectBox->setTitle(QApplication::translate("MainWindow", "Project Panel", 0));
        ProgramBox->setTitle(QApplication::translate("MainWindow", "Program Panel", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Group Panel", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuTool->setTitle(QApplication::translate("MainWindow", "Tool", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuRecent_Setups->setTitle(QApplication::translate("MainWindow", "Recent Setups", 0));
        menuRecent_Projects->setTitle(QApplication::translate("MainWindow", "Recent Projects", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow", "Toolbar", 0));
        dockPrograms->setWindowTitle(QApplication::translate("MainWindow", "Programs", 0));
        dockMidiDevices->setWindowTitle(QApplication::translate("MainWindow", "Midi devices", 0));
        dockVstBrowser->setWindowTitle(QApplication::translate("MainWindow", "VST Plugins", 0));
        dockAudioDevices->setWindowTitle(QApplication::translate("MainWindow", "Audio devices", 0));
        dockSolver->setWindowTitle(QApplication::translate("MainWindow", "Solver", 0));
        dockTools->setWindowTitle(QApplication::translate("MainWindow", "Tools", 0));
        dockHostModel->setWindowTitle(QApplication::translate("MainWindow", "HostModel", 0));
        dockBankBrowser->setWindowTitle(QApplication::translate("MainWindow", "Browser", 0));
        dockUndo->setWindowTitle(QApplication::translate("MainWindow", "Undo history", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
