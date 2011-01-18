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

#include "filebrowser.h"
#include "ui_filebrowser.h"

FileBrowser::FileBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowser)
{
    ui->setupUi(this);
    historyPosition = 0;
}

FileBrowser::~FileBrowser()
{
    delete ui;
}

void FileBrowser::setModel(QFileSystemModel *model)
{
    this->model = model;
    ui->treeFiles->setModel(model);

    ui->treeFiles->setColumnHidden(1,true);
    ui->treeFiles->setColumnHidden(2,true);
    ui->treeFiles->setColumnHidden(3,true);
    setPath(model->rootPath());
}

void FileBrowser::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FileBrowser::on_path_textEdited(QString txt)
{
    QFileInfo info(txt);
    if(info.isDir())
        setPath(txt);
}

void FileBrowser::setPath(QString dir)
{
    for(int i=historyPosition+1; i<dirHistory.size();i++)
        dirHistory.removeAt(i);

    dirHistory << dir;

    if(dirHistory.size()>20) dirHistory.removeFirst();
    historyPosition = dirHistory.size()-1;

    if(dirHistory.size()>1)
        ui->previousDir->setEnabled(true);
    ui->nextDir->setEnabled(false);

    ui->path->setText(dir);
    ui->treeFiles->setRootIndex(model->index(dir));
}

QString FileBrowser::path()
{
    return dirHistory.last();
}

void FileBrowser::on_treeFiles_doubleClicked(QModelIndex index)
{
    QFileInfo info = model->fileInfo(index);
    if(!info.isReadable() || !info.isDir())
        return;
    setPath(info.canonicalFilePath());
}

void FileBrowser::on_parentDir_clicked()
{
    QFileInfo info = model->fileInfo( ui->treeFiles->rootIndex() );
    setPath(info.canonicalPath());
}

void FileBrowser::on_rootDir_clicked()
{
    setPath(":");
}

void FileBrowser::on_previousDir_clicked()
{
    historyPosition--;

    ui->path->setText(dirHistory[historyPosition]);
    ui->treeFiles->setRootIndex(model->index(dirHistory[historyPosition]));

    if(historyPosition==0)
        ui->previousDir->setEnabled(false);
    if(historyPosition<dirHistory.size())
        ui->nextDir->setEnabled(true);
}

void FileBrowser::on_nextDir_clicked()
{
    historyPosition++;

    ui->path->setText(dirHistory[historyPosition]);
    ui->treeFiles->setRootIndex(model->index(dirHistory[historyPosition]));

    if(historyPosition>=0)
        ui->previousDir->setEnabled(true);
    if(historyPosition==dirHistory.size()-1)
        ui->nextDir->setEnabled(false);
}
