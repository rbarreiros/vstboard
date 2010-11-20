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

#ifndef MAINCONFIG_H
#define MAINCONFIG_H

#include "precomp.h"
#include "shortcutkeys.h"

class MainConfig : public QObject
{
    Q_OBJECT
public:
    static MainConfig * Get(QObject *parent = 0);
    ~MainConfig();

    QStandardItemModel * GetAllActionsModel();

    QStandardItemModel * GetShortcutsModel();
    void UpdateShortcutsFromModel();
    static QString KeyToString(int key);

    void ListenToAction(QString const &action, QObject * listener);

    bool shortcutConfig;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    bool AddWidgetToActionsModel(QWidget *widg, QStandardItem *parentItem);
    QStandardItemModel actionsModel;

    void InitListActions();
    void ShortcutStateChanged(ShortcutKeys &shortcut, bool pressed);
    void SendAction(int actionId, const QVariant &value=0);

    QSet<Action*>listActions;
    QMultiHash<ShortcutKeys /*keycombination*/ , int /*action*/>globalShortcutBinding;
    QMultiHash<int /*action*/, QObject * /*listener*/>actionListeners;

    QStandardItemModel *shortcutsModel;
    ShortcutKeys currentShortcut;
    bool currentShortcutPressed;
    bool currentShortcutConstructing;

private:
    static QMutex singletonMutex;
    static MainConfig *theConfig;
    MainConfig(QObject *parent = 0);
    static void InitKeyStrings();
    static QHash<int,QString>listKeysToStrings;
};

#endif // MAINCONFIG_H
