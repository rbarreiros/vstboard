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

#ifndef CONFIGLINEEDIT_H
#define CONFIGLINEEDIT_H

#include "../precomp.h"
#include "../shortcutkeys.h"


class ConfigLineEdit : public QLineEdit
{
Q_OBJECT
public:
    explicit ConfigLineEdit(QWidget *parent = 0);
    void setValue(const ShortcutKeys &shortcut);
    ShortcutKeys value();

protected:
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );

    void mousePressEvent( QMouseEvent * event );
    void keyPressEvent ( QKeyEvent * event );
    void wheelEvent( QWheelEvent * event );

    void mouseReleaseEvent ( QMouseEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );

    void StartConfig();
    void SaveConfig();
    void CancelConfig();

    void InitKeyStrings();

    bool configInProgress;
    ShortcutKeys savedConfig;
    ShortcutKeys tempConfig;


signals:

public slots:

};

#endif // CONFIGLINEEDIT_H
