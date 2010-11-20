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

#include "shortcutdelegate.h"
#include "../globals.h"
#include "../mainconfig.h"
#include "../views/configlineedit.h"

ShortcutDelegate::ShortcutDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *ShortcutDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/) const
{
    ConfigLineEdit *edit = new ConfigLineEdit(parent);
    return edit;
}

void ShortcutDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ConfigLineEdit *edit = static_cast<ConfigLineEdit*>(editor);

    if(edit->value().ToString().isEmpty())
        return;

    model->setData(index, edit->value().ToString());
    model->setData(index, QVariant::fromValue(edit->value()), UserRoles::shortcut);
}

void ShortcutDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex & /*index*/) const
{
    editor->setGeometry(option.rect);
}
