/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "precomp.h"

#include "mainhost.h"
#include "models/listtoolsmodel.h"
#include "projectfile/projectfile.h"
#include "views/solverscene.h"
#include "views/sceneview.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow *Get(QWidget *parent = 0);
    ~MainWindow();
    static MainWindow *theMainWindow;
    QTimer *timerVu;

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *e);
    QGraphicsScene *sceneHostInputs;
    QGraphicsScene *sceneHostOutputs;

    ListToolsModel listToolsModel;

    QFileSystemModel listVstPluginsModel;
    View::SolverScene *solverScene;

    View::SceneView *mySceneView;

private:
    MainWindow(QWidget *parent = 0);
    bool userReallyWantsToQuit();
    void BuildListTools();
    void writeSettings();
    void readSettings();
    void restoreDefaultDocking();
    void updateRecentFileActions();

    QString currentProjectFile;
    QString currentSetupFile;

    QList<QAction*>listRecentProjects;
    QList<QAction*>listRecentSetups;

    Ui::MainWindow *ui;

    MainHost *mainHost;
    Project::ProjectFile *project;

public slots:
    void UpdateCpuLoad();
    void OnProgramChange(int prog);

private slots:
    void on_actionRefresh_Midi_devices_triggered();
    void on_actionRefresh_Audio_devices_triggered();
    void on_actionSave_Setup_As_triggered();
    void on_actionSave_Project_As_triggered();
    void on_actionAbout_triggered();
    void on_actionConfig_triggered();
    void on_actionNew_Setup_triggered();
    void on_actionSave_Setup_triggered();
    void on_actionShortcuts_toggled(bool );
    void on_actionNew_triggered();
    void on_treePrograms_clicked(QModelIndex index);
    void on_treePrograms_activated(QModelIndex index);
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void openRecentSetup();
    void openRecentProject();

};

#endif // MAINWINDOW_H
