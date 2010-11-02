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
#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include "../precomp.h"

namespace Ui {
    class FileBrowser;
}

class FileBrowser : public QWidget {
    Q_OBJECT
public:
    FileBrowser(QWidget *parent = 0);
    ~FileBrowser();

    void setModel(QFileSystemModel *model);
    void setPath(QString dir);
    QString path();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FileBrowser *ui;
    QStringList dirHistory;
    int historyPosition;
    QFileSystemModel *model;

private slots:
    void on_nextDir_clicked();
    void on_previousDir_clicked();
    void on_rootDir_clicked();
    void on_parentDir_clicked();
    void on_treeFiles_doubleClicked(QModelIndex index);
    void on_path_textEdited(QString );
};

#endif // FILEBROWSER_H
