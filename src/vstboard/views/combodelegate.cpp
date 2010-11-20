/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "combodelegate.h"
#include "precomp.h"
#include "globals.h"
#include <QtGui/QComboBox>

ComboDelegate::ComboDelegate(QWidget *parent) :
    QItemDelegate(parent)
{
}

QWidget *ComboDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QComboBox *editor = new QComboBox(parent);


     return editor;
 }

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
 {
     QComboBox *box = static_cast<QComboBox*>(editor);
     box->addItems(index.data(UserRoles::value).toStringList());
     QString str = index.data().toString();
     int select = box->findData(str,Qt::DisplayRole);
     box->setCurrentIndex( select );
 }

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
 {
     QComboBox *box = static_cast<QComboBox*>(editor);
     model->setData(index, box->itemData(box->currentIndex(),Qt::DisplayRole), Qt::EditRole);
 }

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
