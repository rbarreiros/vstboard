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

#include "shortcutkeys.h"
#include "mainconfig.h"

ShortcutKeys::ShortcutKeys() :
        mouseButtons(0),
        mouseWheel(0)
{
};

ShortcutKeys::ShortcutKeys(const ShortcutKeys &c)
{
    mouseButtons = c.mouseButtons;
    mouseWheel = c.mouseWheel;
    listKeys = c.listKeys;
}

QString ShortcutKeys::ToString() const
{
    QString keyStr;

    foreach(int k, listKeys) {
        keyStr += MainConfig::KeyToString(k);
        keyStr += " ";
    }

    if(mouseButtons & Qt::LeftButton)
        keyStr += "mouse.1 ";
    if(mouseButtons & Qt::RightButton)
        keyStr += "mouse.2 ";
    if(mouseButtons & Qt::MidButton)
        keyStr += "mouse.3 ";
    if(mouseButtons & Qt::XButton1)
        keyStr += "mouse.4 ";
    if(mouseButtons & Qt::XButton2)
        keyStr += "mouse.5 ";

    if(mouseWheel>0)
        keyStr += "wheel.up ";
    if(mouseWheel<0)
        keyStr += "wheel.down ";

    return keyStr;
}
