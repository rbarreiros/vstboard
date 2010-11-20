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

#include "configlineedit.h"

ConfigLineEdit::ConfigLineEdit(QWidget *parent) :
    QLineEdit(parent),
    configInProgress(false)
{
}

void ConfigLineEdit::setValue(const ShortcutKeys &shortcut)
{
    savedConfig = shortcut;
}

ShortcutKeys ConfigLineEdit::value()
{
    return savedConfig;
}

void ConfigLineEdit::focusInEvent( QFocusEvent * event )
{
    event->accept();
    if(!configInProgress)
        StartConfig();
}

void ConfigLineEdit::focusOutEvent ( QFocusEvent * event )
{
    event->accept();
    CancelConfig();
}

void ConfigLineEdit::mousePressEvent( QMouseEvent * event )
{
    event->accept();

    if(!configInProgress) {
        configInProgress=true;
        StartConfig();
    }

    if(!rect().contains(event->pos())) {
        CancelConfig();
        return;
    }

    tempConfig.mouseButtons = event->buttons();
    setText(tempConfig.ToString());
}

void ConfigLineEdit::mouseReleaseEvent ( QMouseEvent * event )
{
    event->accept();

    if(!configInProgress) {
        configInProgress=true;
        StartConfig();

        //the first mouse release may be the double click who openned the editor
        return;
    }

    SaveConfig();
}

void ConfigLineEdit::keyPressEvent ( QKeyEvent * event )
{
    event->accept();

    if(event->isAutoRepeat())
        return;

    if(!configInProgress) {
        configInProgress=true;
        StartConfig();
    }


    if(event->key() == Qt::Key_Escape) {
        CancelConfig();
        return;
    }

    tempConfig.listKeys << event->key();
    setText(tempConfig.ToString());
}

void ConfigLineEdit::keyReleaseEvent ( QKeyEvent * event )
{
    event->accept();

    if(event->isAutoRepeat())
        return;

    if(!configInProgress) {
        configInProgress=true;
        StartConfig();

        //the first key release may be the edit-key F2
        return;
    }

    SaveConfig();
}

void ConfigLineEdit::wheelEvent( QWheelEvent * event )
{
    event->accept();
    tempConfig.mouseWheel = event->delta();
    SaveConfig();
}

void ConfigLineEdit::StartConfig()
{
    tempConfig = ShortcutKeys();
    setText(tempConfig.ToString());
    grabKeyboard();
    grabMouse();
}

void ConfigLineEdit::SaveConfig()
{
    savedConfig = tempConfig;
    releaseKeyboard();
    releaseMouse();
    close();
}

void ConfigLineEdit::CancelConfig()
{
    releaseKeyboard();
    releaseMouse();
    close();
}

