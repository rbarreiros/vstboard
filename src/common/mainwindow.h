/**************************************************************************
#    Copyright 2010-2011 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "precomp.h"

#include "mainhost.h"
#include "models/listtoolsmodel.h"
#include "views/sceneview.h"
#include "views/viewconfig.h"


namespace Ui {
    class MainWindow;
}

namespace View {
    class ViewConfigDialog;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(MainHost * myHost, QWidget *parent = 0);
    virtual ~MainWindow();
    virtual void readSettings();
    void writeSettings();
    bool openedPrompt;

    View::SceneView *mySceneView;
    View::ViewConfig *viewConfig;

    bool userWantsToUnloadProject();
    bool userWantsToUnloadSetup();

protected:
    void changeEvent(QEvent *e);
    void SetupBrowsersModels(const QString &vstPath, const QString &browserPath);

    ListToolsModel *listToolsModel;

    QFileSystemModel *listVstPluginsModel;
    QFileSystemModel *listVstBanksModel;

    virtual void BuildListTools();
    virtual void resetSettings();
    void updateRecentFileActions();

    QList<QAction*>listRecentProjects;
    QList<QAction*>listRecentSetups;

    Ui::MainWindow *ui;
    MainHost *myHost;

    View::ViewConfigDialog *viewConfigDlg;

public slots:
    void programParkingModelChanges(QStandardItemModel *model);
    void groupParkingModelChanges(QStandardItemModel *model);
    void currentFileChanged();
    void OnViewConfigClosed();
    void LoadDefaultFiles();

private slots:
    void on_actionLoad_Setup_triggered();
    void on_actionRestore_default_layout_triggered();
    void on_actionSave_Setup_As_triggered();
    void on_actionSave_Project_As_triggered();
    void on_actionAbout_triggered();
    void on_actionNew_Setup_triggered();
    void on_actionSave_Setup_triggered();
    void on_actionShortcuts_toggled(bool );
    void on_actionNew_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void openRecentSetup();
    void openRecentProject();
    virtual void on_actionConfig_triggered();
    virtual void on_actionRefresh_Midi_devices_triggered() {}
    virtual void on_actionRefresh_Audio_devices_triggered() {}
    void InitColors();
    void UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);

    void on_solverView_clicked(const QModelIndex &index);
    void on_actionAppearance_toggled(bool arg1);
};

#endif // MAINWINDOW_H
