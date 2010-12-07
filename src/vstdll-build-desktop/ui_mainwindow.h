/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu 2. Dec 01:56:30 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "views/filebrowser.h"
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
    QAction *actionInsert_panel;
    QAction *actionProgram_panel;
    QAction *actionRefresh_Audio_devices;
    QAction *actionRefresh_Midi_devices;
    QAction *actionRestore_default_layout;
    QAction *actionStatus_bar;
    QAction *actionTool_bar;
    QAction *actionZoom_buttons;
    QAction *actionProject_panel;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_9;
    QSplitter *splitterPanels;
    QGroupBox *HostBox;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushZoomInHost;
    QPushButton *pushZoomResetHost;
    QPushButton *pushZoomOutHost;
    QSpacerItem *verticalSpacer;
    MainGraphicsView *hostView;
    QGroupBox *ProjectBox;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_13;
    QPushButton *pushZoomInProj;
    QPushButton *pushZoomResetProj;
    QPushButton *pushZoomOutProj;
    QSpacerItem *verticalSpacer_4;
    MainGraphicsView *projectView;
    QGroupBox *ProgramBox;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_12;
    QPushButton *pushZoomInProg;
    QPushButton *pushZoomResetProg;
    QPushButton *pushZoomOutProg;
    QSpacerItem *verticalSpacer_3;
    MainGraphicsView *programView;
    QGroupBox *InsertBox;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayoutBoutInsert;
    QPushButton *pushZoomInIns;
    QPushButton *pushZoomResetIns;
    QPushButton *pushZoomOutIns;
    QSpacerItem *verticalSpacer_2;
    MainGraphicsView *insertView;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuEdit;
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
    QTreeView *treeAudioInterfaces;
    QDockWidget *dockSolver;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_5;
    QTreeView *solverView;
    QDockWidget *dockTools;
    QWidget *dockWidgetContents_7;
    QVBoxLayout *verticalLayout_4;
    QTreeView *treeTools;
    QDockWidget *dockHostModel;
    QWidget *dockWidgetContents_8;
    QVBoxLayout *verticalLayout_3;
    QTreeView *treeHostModel;
    QDockWidget *dockParking;
    QWidget *dockWidgetContents_9;
    QVBoxLayout *verticalLayout_2;
    QListView *listParking;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(846, 570);
        QPalette palette;
        QBrush brush(QColor(248, 248, 248, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        QBrush brush1(QColor(236, 236, 236, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        QBrush brush2(QColor(212, 212, 212, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        MainWindow->setPalette(palette);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        MainWindow->setToolButtonStyle(Qt::ToolButtonFollowStyle);
        MainWindow->setDockNestingEnabled(true);
        MainWindow->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img16x16/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img16x16/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad->setIcon(icon1);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img16x16/filenew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon2);
        actionConfig = new QAction(MainWindow);
        actionConfig->setObjectName(QString::fromUtf8("actionConfig"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img16x16/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfig->setIcon(icon3);
        actionShortcuts = new QAction(MainWindow);
        actionShortcuts->setObjectName(QString::fromUtf8("actionShortcuts"));
        actionShortcuts->setCheckable(true);
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img16x16/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon4);
        actionSave_Setup = new QAction(MainWindow);
        actionSave_Setup->setObjectName(QString::fromUtf8("actionSave_Setup"));
        actionSave_Setup->setIcon(icon);
        actionNew_Setup = new QAction(MainWindow);
        actionNew_Setup->setObjectName(QString::fromUtf8("actionNew_Setup"));
        actionNew_Setup->setIcon(icon2);
        actionHost_panel = new QAction(MainWindow);
        actionHost_panel->setObjectName(QString::fromUtf8("actionHost_panel"));
        actionHost_panel->setCheckable(true);
        actionHost_panel->setChecked(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img16x16/file_setup.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHost_panel->setIcon(icon5);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/img16x16/documentinfo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon6);
        actionSave_Project_As = new QAction(MainWindow);
        actionSave_Project_As->setObjectName(QString::fromUtf8("actionSave_Project_As"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img16x16/filesaveas.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Project_As->setIcon(icon7);
        actionSave_Setup_As = new QAction(MainWindow);
        actionSave_Setup_As->setObjectName(QString::fromUtf8("actionSave_Setup_As"));
        actionSave_Setup_As->setIcon(icon7);
        actionInsert_panel = new QAction(MainWindow);
        actionInsert_panel->setObjectName(QString::fromUtf8("actionInsert_panel"));
        actionInsert_panel->setCheckable(true);
        actionInsert_panel->setChecked(true);
        actionInsert_panel->setIcon(icon5);
        actionProgram_panel = new QAction(MainWindow);
        actionProgram_panel->setObjectName(QString::fromUtf8("actionProgram_panel"));
        actionProgram_panel->setCheckable(true);
        actionProgram_panel->setChecked(true);
        actionProgram_panel->setIcon(icon5);
        actionRefresh_Audio_devices = new QAction(MainWindow);
        actionRefresh_Audio_devices->setObjectName(QString::fromUtf8("actionRefresh_Audio_devices"));
        actionRefresh_Midi_devices = new QAction(MainWindow);
        actionRefresh_Midi_devices->setObjectName(QString::fromUtf8("actionRefresh_Midi_devices"));
        actionRestore_default_layout = new QAction(MainWindow);
        actionRestore_default_layout->setObjectName(QString::fromUtf8("actionRestore_default_layout"));
        actionStatus_bar = new QAction(MainWindow);
        actionStatus_bar->setObjectName(QString::fromUtf8("actionStatus_bar"));
        actionStatus_bar->setCheckable(true);
        actionTool_bar = new QAction(MainWindow);
        actionTool_bar->setObjectName(QString::fromUtf8("actionTool_bar"));
        actionTool_bar->setCheckable(true);
        actionZoom_buttons = new QAction(MainWindow);
        actionZoom_buttons->setObjectName(QString::fromUtf8("actionZoom_buttons"));
        actionZoom_buttons->setCheckable(true);
        actionProject_panel = new QAction(MainWindow);
        actionProject_panel->setObjectName(QString::fromUtf8("actionProject_panel"));
        actionProject_panel->setCheckable(true);
        actionProject_panel->setChecked(true);
        actionProject_panel->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout_9 = new QVBoxLayout(centralWidget);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        splitterPanels = new QSplitter(centralWidget);
        splitterPanels->setObjectName(QString::fromUtf8("splitterPanels"));
        splitterPanels->setOrientation(Qt::Vertical);
        HostBox = new QGroupBox(splitterPanels);
        HostBox->setObjectName(QString::fromUtf8("HostBox"));
        HostBox->setAlignment(Qt::AlignCenter);
        HostBox->setFlat(true);
        horizontalLayout_2 = new QHBoxLayout(HostBox);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushZoomInHost = new QPushButton(HostBox);
        pushZoomInHost->setObjectName(QString::fromUtf8("pushZoomInHost"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/img16x16/viewmag+.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushZoomInHost->setIcon(icon8);
        pushZoomInHost->setAutoRepeat(true);

        verticalLayout->addWidget(pushZoomInHost);

        pushZoomResetHost = new QPushButton(HostBox);
        pushZoomResetHost->setObjectName(QString::fromUtf8("pushZoomResetHost"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/img16x16/viewmagfit.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushZoomResetHost->setIcon(icon9);

        verticalLayout->addWidget(pushZoomResetHost);

        pushZoomOutHost = new QPushButton(HostBox);
        pushZoomOutHost->setObjectName(QString::fromUtf8("pushZoomOutHost"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/img16x16/viewmag-.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushZoomOutHost->setIcon(icon10);
        pushZoomOutHost->setAutoRepeat(true);

        verticalLayout->addWidget(pushZoomOutHost);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);

        hostView = new MainGraphicsView(HostBox);
        hostView->setObjectName(QString::fromUtf8("hostView"));
        hostView->setFrameShape(QFrame::NoFrame);
        hostView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hostView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hostView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        hostView->setDragMode(QGraphicsView::ScrollHandDrag);
        hostView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout_2->addWidget(hostView);

        splitterPanels->addWidget(HostBox);
        ProjectBox = new QGroupBox(splitterPanels);
        ProjectBox->setObjectName(QString::fromUtf8("ProjectBox"));
        ProjectBox->setAlignment(Qt::AlignCenter);
        ProjectBox->setFlat(true);
        horizontalLayout_5 = new QHBoxLayout(ProjectBox);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        pushZoomInProj = new QPushButton(ProjectBox);
        pushZoomInProj->setObjectName(QString::fromUtf8("pushZoomInProj"));
        pushZoomInProj->setIcon(icon8);
        pushZoomInProj->setAutoRepeat(true);

        verticalLayout_13->addWidget(pushZoomInProj);

        pushZoomResetProj = new QPushButton(ProjectBox);
        pushZoomResetProj->setObjectName(QString::fromUtf8("pushZoomResetProj"));
        pushZoomResetProj->setIcon(icon9);

        verticalLayout_13->addWidget(pushZoomResetProj);

        pushZoomOutProj = new QPushButton(ProjectBox);
        pushZoomOutProj->setObjectName(QString::fromUtf8("pushZoomOutProj"));
        pushZoomOutProj->setIcon(icon10);
        pushZoomOutProj->setAutoRepeat(true);

        verticalLayout_13->addWidget(pushZoomOutProj);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_4);


        horizontalLayout_5->addLayout(verticalLayout_13);

        projectView = new MainGraphicsView(ProjectBox);
        projectView->setObjectName(QString::fromUtf8("projectView"));
        projectView->setFrameShape(QFrame::NoFrame);
        projectView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        projectView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        projectView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        projectView->setDragMode(QGraphicsView::ScrollHandDrag);
        projectView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout_5->addWidget(projectView);

        splitterPanels->addWidget(ProjectBox);
        ProgramBox = new QGroupBox(splitterPanels);
        ProgramBox->setObjectName(QString::fromUtf8("ProgramBox"));
        ProgramBox->setAlignment(Qt::AlignCenter);
        ProgramBox->setFlat(true);
        horizontalLayout_4 = new QHBoxLayout(ProgramBox);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        pushZoomInProg = new QPushButton(ProgramBox);
        pushZoomInProg->setObjectName(QString::fromUtf8("pushZoomInProg"));
        pushZoomInProg->setIcon(icon8);
        pushZoomInProg->setAutoRepeat(true);

        verticalLayout_12->addWidget(pushZoomInProg);

        pushZoomResetProg = new QPushButton(ProgramBox);
        pushZoomResetProg->setObjectName(QString::fromUtf8("pushZoomResetProg"));
        pushZoomResetProg->setIcon(icon9);

        verticalLayout_12->addWidget(pushZoomResetProg);

        pushZoomOutProg = new QPushButton(ProgramBox);
        pushZoomOutProg->setObjectName(QString::fromUtf8("pushZoomOutProg"));
        pushZoomOutProg->setIcon(icon10);
        pushZoomOutProg->setAutoRepeat(true);

        verticalLayout_12->addWidget(pushZoomOutProg);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_3);


        horizontalLayout_4->addLayout(verticalLayout_12);

        programView = new MainGraphicsView(ProgramBox);
        programView->setObjectName(QString::fromUtf8("programView"));
        programView->setFrameShape(QFrame::NoFrame);
        programView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        programView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        programView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        programView->setDragMode(QGraphicsView::ScrollHandDrag);
        programView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout_4->addWidget(programView);

        splitterPanels->addWidget(ProgramBox);
        InsertBox = new QGroupBox(splitterPanels);
        InsertBox->setObjectName(QString::fromUtf8("InsertBox"));
        InsertBox->setAlignment(Qt::AlignCenter);
        InsertBox->setFlat(true);
        horizontalLayout_3 = new QHBoxLayout(InsertBox);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayoutBoutInsert = new QVBoxLayout();
        verticalLayoutBoutInsert->setSpacing(0);
        verticalLayoutBoutInsert->setObjectName(QString::fromUtf8("verticalLayoutBoutInsert"));
        pushZoomInIns = new QPushButton(InsertBox);
        pushZoomInIns->setObjectName(QString::fromUtf8("pushZoomInIns"));
        pushZoomInIns->setIcon(icon8);
        pushZoomInIns->setAutoRepeat(true);

        verticalLayoutBoutInsert->addWidget(pushZoomInIns);

        pushZoomResetIns = new QPushButton(InsertBox);
        pushZoomResetIns->setObjectName(QString::fromUtf8("pushZoomResetIns"));
        pushZoomResetIns->setIcon(icon9);

        verticalLayoutBoutInsert->addWidget(pushZoomResetIns);

        pushZoomOutIns = new QPushButton(InsertBox);
        pushZoomOutIns->setObjectName(QString::fromUtf8("pushZoomOutIns"));
        pushZoomOutIns->setIcon(icon10);
        pushZoomOutIns->setAutoRepeat(true);

        verticalLayoutBoutInsert->addWidget(pushZoomOutIns);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutBoutInsert->addItem(verticalSpacer_2);


        horizontalLayout_3->addLayout(verticalLayoutBoutInsert);

        insertView = new MainGraphicsView(InsertBox);
        insertView->setObjectName(QString::fromUtf8("insertView"));
        insertView->setFrameShape(QFrame::NoFrame);
        insertView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        insertView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        insertView->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        insertView->setDragMode(QGraphicsView::ScrollHandDrag);
        insertView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout_3->addWidget(insertView);

        splitterPanels->addWidget(InsertBox);

        verticalLayout_9->addWidget(splitterPanels);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 846, 21));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuRecent_Setups = new QMenu(menuFile);
        menuRecent_Setups->setObjectName(QString::fromUtf8("menuRecent_Setups"));
        menuRecent_Projects = new QMenu(menuFile);
        menuRecent_Projects->setObjectName(QString::fromUtf8("menuRecent_Projects"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setEnabled(true);
        MainWindow->setStatusBar(statusBar);
        dockPrograms = new QDockWidget(MainWindow);
        dockPrograms->setObjectName(QString::fromUtf8("dockPrograms"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockPrograms->sizePolicy().hasHeightForWidth());
        dockPrograms->setSizePolicy(sizePolicy1);
        dockPrograms->setFloating(false);
        dockPrograms->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockPrograms->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_10 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        Programs = new ProgramList(dockWidgetContents_2);
        Programs->setObjectName(QString::fromUtf8("Programs"));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Programs->sizePolicy().hasHeightForWidth());
        Programs->setSizePolicy(sizePolicy2);

        verticalLayout_10->addWidget(Programs);

        dockPrograms->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockPrograms);
        dockMidiDevices = new QDockWidget(MainWindow);
        dockMidiDevices->setObjectName(QString::fromUtf8("dockMidiDevices"));
        dockMidiDevices->setAcceptDrops(false);
        dockMidiDevices->setFloating(false);
        dockMidiDevices->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockMidiDevices->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
        verticalLayout_8 = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        treeMidiInterfaces = new QTreeView(dockWidgetContents_6);
        treeMidiInterfaces->setObjectName(QString::fromUtf8("treeMidiInterfaces"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(treeMidiInterfaces->sizePolicy().hasHeightForWidth());
        treeMidiInterfaces->setSizePolicy(sizePolicy3);
        treeMidiInterfaces->setFrameShape(QFrame::NoFrame);
        treeMidiInterfaces->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeMidiInterfaces->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeMidiInterfaces->setProperty("showDropIndicator", QVariant(false));
        treeMidiInterfaces->setDragDropMode(QAbstractItemView::DragOnly);
        treeMidiInterfaces->setIndentation(10);
        treeMidiInterfaces->setUniformRowHeights(true);
        treeMidiInterfaces->setSortingEnabled(true);
        treeMidiInterfaces->setAnimated(true);
        treeMidiInterfaces->header()->setStretchLastSection(false);

        verticalLayout_8->addWidget(treeMidiInterfaces);

        dockMidiDevices->setWidget(dockWidgetContents_6);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockMidiDevices);
        dockVstBrowser = new QDockWidget(MainWindow);
        dockVstBrowser->setObjectName(QString::fromUtf8("dockVstBrowser"));
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        verticalLayout_7 = new QVBoxLayout(dockWidgetContents_5);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        VstBrowser = new FileBrowser(dockWidgetContents_5);
        VstBrowser->setObjectName(QString::fromUtf8("VstBrowser"));
        sizePolicy3.setHeightForWidth(VstBrowser->sizePolicy().hasHeightForWidth());
        VstBrowser->setSizePolicy(sizePolicy3);

        verticalLayout_7->addWidget(VstBrowser);

        dockVstBrowser->setWidget(dockWidgetContents_5);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockVstBrowser);
        dockAudioDevices = new QDockWidget(MainWindow);
        dockAudioDevices->setObjectName(QString::fromUtf8("dockAudioDevices"));
        dockAudioDevices->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        treeAudioInterfaces = new QTreeView(dockWidgetContents);
        treeAudioInterfaces->setObjectName(QString::fromUtf8("treeAudioInterfaces"));
        sizePolicy3.setHeightForWidth(treeAudioInterfaces->sizePolicy().hasHeightForWidth());
        treeAudioInterfaces->setSizePolicy(sizePolicy3);
        treeAudioInterfaces->setFrameShape(QFrame::NoFrame);
        treeAudioInterfaces->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeAudioInterfaces->setProperty("showDropIndicator", QVariant(false));
        treeAudioInterfaces->setDragDropMode(QAbstractItemView::DragOnly);
        treeAudioInterfaces->setIndentation(10);
        treeAudioInterfaces->setUniformRowHeights(true);
        treeAudioInterfaces->setSortingEnabled(true);
        treeAudioInterfaces->setAnimated(true);
        treeAudioInterfaces->header()->setStretchLastSection(false);

        verticalLayout_6->addWidget(treeAudioInterfaces);

        dockAudioDevices->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockAudioDevices);
        dockSolver = new QDockWidget(MainWindow);
        dockSolver->setObjectName(QString::fromUtf8("dockSolver"));
        sizePolicy1.setHeightForWidth(dockSolver->sizePolicy().hasHeightForWidth());
        dockSolver->setSizePolicy(sizePolicy1);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        solverView = new QTreeView(dockWidgetContents_4);
        solverView->setObjectName(QString::fromUtf8("solverView"));
        sizePolicy2.setHeightForWidth(solverView->sizePolicy().hasHeightForWidth());
        solverView->setSizePolicy(sizePolicy2);
        solverView->setFrameShape(QFrame::NoFrame);

        verticalLayout_5->addWidget(solverView);

        dockSolver->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockSolver);
        dockTools = new QDockWidget(MainWindow);
        dockTools->setObjectName(QString::fromUtf8("dockTools"));
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_7);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        treeTools = new QTreeView(dockWidgetContents_7);
        treeTools->setObjectName(QString::fromUtf8("treeTools"));
        sizePolicy3.setHeightForWidth(treeTools->sizePolicy().hasHeightForWidth());
        treeTools->setSizePolicy(sizePolicy3);
        treeTools->setFrameShape(QFrame::NoFrame);
        treeTools->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeTools->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeTools->setProperty("showDropIndicator", QVariant(false));
        treeTools->setDragDropMode(QAbstractItemView::DragOnly);
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
        dockHostModel->setObjectName(QString::fromUtf8("dockHostModel"));
        sizePolicy1.setHeightForWidth(dockHostModel->sizePolicy().hasHeightForWidth());
        dockHostModel->setSizePolicy(sizePolicy1);
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QString::fromUtf8("dockWidgetContents_8"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_8);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        treeHostModel = new QTreeView(dockWidgetContents_8);
        treeHostModel->setObjectName(QString::fromUtf8("treeHostModel"));
        sizePolicy2.setHeightForWidth(treeHostModel->sizePolicy().hasHeightForWidth());
        treeHostModel->setSizePolicy(sizePolicy2);
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
        dockParking = new QDockWidget(MainWindow);
        dockParking->setObjectName(QString::fromUtf8("dockParking"));
        sizePolicy1.setHeightForWidth(dockParking->sizePolicy().hasHeightForWidth());
        dockParking->setSizePolicy(sizePolicy1);
        dockWidgetContents_9 = new QWidget();
        dockWidgetContents_9->setObjectName(QString::fromUtf8("dockWidgetContents_9"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_9);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listParking = new QListView(dockWidgetContents_9);
        listParking->setObjectName(QString::fromUtf8("listParking"));
        sizePolicy2.setHeightForWidth(listParking->sizePolicy().hasHeightForWidth());
        listParking->setSizePolicy(sizePolicy2);
        listParking->setFrameShape(QFrame::NoFrame);
        listParking->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        listParking->setDragEnabled(true);
        listParking->setDragDropMode(QAbstractItemView::DragDrop);
        listParking->setDefaultDropAction(Qt::IgnoreAction);
        listParking->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listParking->setUniformItemSizes(true);

        verticalLayout_2->addWidget(listParking);

        dockParking->setWidget(dockWidgetContents_9);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockParking);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuView->addAction(actionHost_panel);
        menuView->addAction(actionProject_panel);
        menuView->addAction(actionProgram_panel);
        menuView->addAction(actionInsert_panel);
        menuView->addSeparator();
        menuView->addAction(actionRestore_default_layout);
        menuView->addAction(actionStatus_bar);
        menuView->addAction(actionTool_bar);
        menuView->addAction(actionZoom_buttons);
        menuEdit->addAction(actionConfig);
        menuEdit->addAction(actionShortcuts);
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionRefresh_Audio_devices);
        menuEdit->addAction(actionRefresh_Midi_devices);
        menuFile->addAction(actionLoad);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_Project_As);
        menuFile->addAction(actionNew);
        menuFile->addAction(menuRecent_Projects->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Setup);
        menuFile->addAction(actionSave_Setup_As);
        menuFile->addAction(actionNew_Setup);
        menuFile->addAction(menuRecent_Setups->menuAction());
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionNew_Setup);
        mainToolBar->addAction(actionLoad);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionSave_Setup);
        mainToolBar->addAction(actionConfig);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionHost_panel);
        mainToolBar->addAction(actionProject_panel);
        mainToolBar->addAction(actionProgram_panel);
        mainToolBar->addAction(actionInsert_panel);

        retranslateUi(MainWindow);
        QObject::connect(pushZoomInProg, SIGNAL(pressed()), programView, SLOT(zoomIn()));
        QObject::connect(pushZoomResetProg, SIGNAL(clicked()), programView, SLOT(zoomReset()));
        QObject::connect(pushZoomOutProg, SIGNAL(pressed()), programView, SLOT(zoomOut()));
        QObject::connect(pushZoomInIns, SIGNAL(pressed()), insertView, SLOT(zoomIn()));
        QObject::connect(pushZoomResetIns, SIGNAL(clicked()), insertView, SLOT(zoomReset()));
        QObject::connect(pushZoomOutIns, SIGNAL(pressed()), insertView, SLOT(zoomOut()));
        QObject::connect(pushZoomInHost, SIGNAL(pressed()), hostView, SLOT(zoomIn()));
        QObject::connect(pushZoomResetHost, SIGNAL(clicked()), hostView, SLOT(zoomReset()));
        QObject::connect(pushZoomOutHost, SIGNAL(pressed()), hostView, SLOT(zoomOut()));
        QObject::connect(actionHost_panel, SIGNAL(toggled(bool)), HostBox, SLOT(setVisible(bool)));
        QObject::connect(actionInsert_panel, SIGNAL(toggled(bool)), InsertBox, SLOT(setVisible(bool)));
        QObject::connect(actionProgram_panel, SIGNAL(toggled(bool)), ProgramBox, SLOT(setVisible(bool)));
        QObject::connect(actionStatus_bar, SIGNAL(toggled(bool)), statusBar, SLOT(setVisible(bool)));
        QObject::connect(actionTool_bar, SIGNAL(toggled(bool)), mainToolBar, SLOT(setVisible(bool)));
        QObject::connect(pushZoomInProj, SIGNAL(pressed()), projectView, SLOT(zoomIn()));
        QObject::connect(pushZoomOutProj, SIGNAL(pressed()), projectView, SLOT(zoomOut()));
        QObject::connect(pushZoomResetProj, SIGNAL(clicked()), projectView, SLOT(zoomReset()));
        QObject::connect(actionProject_panel, SIGNAL(toggled(bool)), ProjectBox, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "VstBoard", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionSave->setStatusTip(QApplication::translate("MainWindow", "Save the program and insert panels in a project file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionLoad->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionLoad->setStatusTip(QApplication::translate("MainWindow", "Open a project file or a setup file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionLoad->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "New Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionNew->setStatusTip(QApplication::translate("MainWindow", "Clear the program and insert panels", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionConfig->setText(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionConfig->setStatusTip(QApplication::translate("MainWindow", "Show the configuration dialog", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionConfig->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionShortcuts->setText(QApplication::translate("MainWindow", "Edit Shortcuts (testing)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionShortcuts->setStatusTip(QApplication::translate("MainWindow", "Edit Shortcuts (testing)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionDelete->setStatusTip(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave_Setup->setText(QApplication::translate("MainWindow", "Save Setup", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionSave_Setup->setStatusTip(QApplication::translate("MainWindow", "Save the host panel in a setup file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave_Setup->setShortcut(QApplication::translate("MainWindow", "Alt+S", 0, QApplication::UnicodeUTF8));
        actionNew_Setup->setText(QApplication::translate("MainWindow", "New Setup", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionNew_Setup->setStatusTip(QApplication::translate("MainWindow", "Clear the host panel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionNew_Setup->setShortcut(QApplication::translate("MainWindow", "Alt+N", 0, QApplication::UnicodeUTF8));
        actionHost_panel->setText(QApplication::translate("MainWindow", "Toggle Host panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionHost_panel->setStatusTip(QApplication::translate("MainWindow", "Show or hide the host panel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionHost_panel->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionAbout->setStatusTip(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave_Project_As->setText(QApplication::translate("MainWindow", "Save Project As ...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionSave_Project_As->setStatusTip(QApplication::translate("MainWindow", "Save Project As", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave_Project_As->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionSave_Setup_As->setText(QApplication::translate("MainWindow", "Save Setup As ...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionSave_Setup_As->setStatusTip(QApplication::translate("MainWindow", "Save Setup As", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionSave_Setup_As->setShortcut(QApplication::translate("MainWindow", "Alt+Shift+S", 0, QApplication::UnicodeUTF8));
        actionInsert_panel->setText(QApplication::translate("MainWindow", "Toggle Insert panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionInsert_panel->setToolTip(QApplication::translate("MainWindow", "Toggle Insert panel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionInsert_panel->setStatusTip(QApplication::translate("MainWindow", "Show or hide the insert panel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionInsert_panel->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionProgram_panel->setText(QApplication::translate("MainWindow", "Toggle Program panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionProgram_panel->setStatusTip(QApplication::translate("MainWindow", "Show or hide the program panel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionProgram_panel->setShortcut(QApplication::translate("MainWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionRefresh_Audio_devices->setText(QApplication::translate("MainWindow", "Refresh Audio devices", 0, QApplication::UnicodeUTF8));
        actionRefresh_Midi_devices->setText(QApplication::translate("MainWindow", "Refresh Midi devices", 0, QApplication::UnicodeUTF8));
        actionRestore_default_layout->setText(QApplication::translate("MainWindow", "Restore default layout", 0, QApplication::UnicodeUTF8));
        actionStatus_bar->setText(QApplication::translate("MainWindow", "Status bar", 0, QApplication::UnicodeUTF8));
        actionTool_bar->setText(QApplication::translate("MainWindow", "Tool bar", 0, QApplication::UnicodeUTF8));
        actionZoom_buttons->setText(QApplication::translate("MainWindow", "Zoom buttons", 0, QApplication::UnicodeUTF8));
        actionProject_panel->setText(QApplication::translate("MainWindow", "Toggle Project panel", 0, QApplication::UnicodeUTF8));
        HostBox->setTitle(QApplication::translate("MainWindow", "Host Panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushZoomInHost->setToolTip(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomResetHost->setToolTip(QApplication::translate("MainWindow", "Reset Zoom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomOutHost->setToolTip(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        hostView->setWhatsThis(QApplication::translate("MainWindow", "The Host panel is where you normally put your audio and midi interfaces.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        ProjectBox->setTitle(QApplication::translate("MainWindow", "Project Panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushZoomInProj->setToolTip(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomResetProj->setToolTip(QApplication::translate("MainWindow", "Reset Zoom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomOutProj->setToolTip(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ProgramBox->setTitle(QApplication::translate("MainWindow", "Program Panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushZoomInProg->setToolTip(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomResetProg->setToolTip(QApplication::translate("MainWindow", "Reset Zoom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomOutProg->setToolTip(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        InsertBox->setTitle(QApplication::translate("MainWindow", "Insert Panel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushZoomInIns->setToolTip(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomResetIns->setToolTip(QApplication::translate("MainWindow", "Reset Zoom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushZoomOutIns->setToolTip(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuRecent_Setups->setTitle(QApplication::translate("MainWindow", "Recent Setups", 0, QApplication::UnicodeUTF8));
        menuRecent_Projects->setTitle(QApplication::translate("MainWindow", "Recent Projects", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        dockPrograms->setWindowTitle(QApplication::translate("MainWindow", "Programs", 0, QApplication::UnicodeUTF8));
        dockMidiDevices->setWindowTitle(QApplication::translate("MainWindow", "Midi devices", 0, QApplication::UnicodeUTF8));
        dockVstBrowser->setWindowTitle(QApplication::translate("MainWindow", "Vst Plugins", 0, QApplication::UnicodeUTF8));
        dockAudioDevices->setWindowTitle(QApplication::translate("MainWindow", "Audio devices", 0, QApplication::UnicodeUTF8));
        dockSolver->setWindowTitle(QApplication::translate("MainWindow", "Solver", 0, QApplication::UnicodeUTF8));
        dockTools->setWindowTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        dockHostModel->setWindowTitle(QApplication::translate("MainWindow", "HostModel", 0, QApplication::UnicodeUTF8));
        dockParking->setWindowTitle(QApplication::translate("MainWindow", "Parking", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
