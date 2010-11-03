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

#ifndef SHORTCUTKEYS_H
#define SHORTCUTKEYS_H

#include "precomp.h"
#include "globals.h"

class ShortcutEvent : public QEvent
{
public:
    ShortcutEvent(const int id, const QVariant &value) : QEvent(Event::Shortcut),id(id),value(value) {};
    const int id;
    const QVariant value;
};

class Action
{
public:
    Action(int id, const QString &name, const QString &displayedName) :
            id(id),
            name(name),
            displayedName(displayedName) {}

    int id;
    QString name;
    QString displayedName;
};

class ShortcutKeys
{
public:
    ShortcutKeys();
    ShortcutKeys(const ShortcutKeys &c);

    Qt::MouseButtons mouseButtons;
    int mouseWheel;
    QSet<int>listKeys;

    QString ToString() const;
};

Q_DECLARE_METATYPE(ShortcutKeys)

inline bool operator==(const ShortcutKeys &c1, const ShortcutKeys &c2)
{
    return c1.mouseButtons == c2.mouseButtons &&
            c1.mouseWheel == c2.mouseWheel &&
            c1.listKeys == c2.listKeys;
}

inline uint qHash(const ShortcutKeys &key)
{
    return key.mouseButtons ^ key.mouseWheel ^ key.listKeys.size();
}

#endif // SHORTCUTKEYS_H
