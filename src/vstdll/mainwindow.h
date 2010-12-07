/**************************************************************************
#    Copyright 2010 Rapha�l Fran�ois
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

#include <QMainWindow>

#include "../vstboard/mainhost.h"
#include "../vstboard/views/sceneview.h"
#include "../vstboard/models/listtoolsmodel.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MainHost * myHost, QWidget *parent = 0);
    ~MainWindow();

private:
    MainHost * myHost;

    void writeSettings();
    void readSettings();
    void resetSettings();

    QString currentProjectFile;
    QString currentSetupFile;

    Ui::MainWindow *ui;

    ListToolsModel *listToolsModel;
    void BuildListTools();

    QList<QAction*>listRecentProjects;
    QList<QAction*>listRecentSetups;
    void updateRecentFileActions();

    QFileSystemModel *listVstPluginsModel;
    View::SceneView *mySceneView;

private slots:
    void on_actionRestore_default_layout_triggered();
    void on_actionSave_Setup_As_triggered();
    void on_actionSave_Project_As_triggered();
    void on_actionConfig_triggered();
    void on_actionNew_Setup_triggered();
    void on_actionSave_Setup_triggered();
    void on_actionNew_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionAbout_triggered();
    void openRecentSetup();
    void openRecentProject();
};

#endif // MAINWINDOW_H
