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

#include <QAction>
#include "mainconfig.h"
#include "globals.h"
#include "mainwindow.h"

QMutex MainConfig::singletonMutex;
MainConfig *MainConfig::theConfig=0;
QHash<int,QString>MainConfig::listKeysToStrings;

MainConfig::MainConfig(QObject *parent) :
        QObject(parent),
        shortcutConfig(false),
        shortcutsModel(0),
        currentShortcutPressed(false),
        currentShortcutConstructing(false)
{
    if(listKeysToStrings.isEmpty())
        InitKeyStrings();

    InitListActions();

    ShortcutKeys j;
    j.listKeys << Qt::Key_F1;
    globalShortcutBinding.insert(j,0);

    ShortcutKeys k;
    k.listKeys << Qt::Key_F2;
    globalShortcutBinding.insert(k,1);

    ShortcutKeys l;
    l.listKeys << Qt::Key_F3;
    globalShortcutBinding.insert(l,2);

    qApp->installEventFilter(this);
}

MainConfig::~MainConfig()
{
    foreach(Action *act, listActions) {
        delete act;
    }
    listActions.clear();

    globalShortcutBinding.clear();
    actionListeners.clear();
}

MainConfig * MainConfig::Get(QObject *parent)
{
    singletonMutex.lock();
    if(!theConfig)
        theConfig=new MainConfig(parent);
    singletonMutex.unlock();

    return theConfig;
}

QStandardItemModel * MainConfig::GetAllActionsModel()
{
    actionsModel.clear();
    AddWidgetToActionsModel(MainWindow::theMainWindow,actionsModel.invisibleRootItem());
    return &actionsModel;
}

bool MainConfig::AddWidgetToActionsModel(QWidget *widg, QStandardItem *parentItem)
{
    bool childs =false;
    bool actions =false;

    //add widget to model
    QStandardItem *item  = new QStandardItem(widg->objectName());

    //add all childrens
    foreach(QObject *childObj, widg->children()) {
        QWidget *childWidget = qobject_cast<QWidget*>(childObj);
        if(childWidget) {
            if(AddWidgetToActionsModel(childWidget,item))
                childs = true;
        }
    }

    //add all actions
    foreach(QAction *act, widg->actions()) {
        actions = true;
        QStandardItem *itemAct = new QStandardItem(act->text());
        item->appendRow(itemAct);
    }

    //if(childs || actions) {
        parentItem->appendRow(item);
        return true;
    //}

    //return false;
}

QStandardItemModel * MainConfig::GetShortcutsModel()
{
    if(shortcutsModel)
        delete shortcutsModel;

    shortcutsModel = new QStandardItemModel(0,1,this);

    QStringList headerLabels;
    headerLabels << "Action";
    headerLabels << "Shortcut";

    shortcutsModel->setHorizontalHeaderLabels(  headerLabels );
    QStandardItem *parentItem = shortcutsModel->invisibleRootItem();

    //add all actions
    foreach(Action *act, listActions) {
        QStandardItem *itemAction  = new QStandardItem(act->displayedName);
        itemAction->setData(act->id);
        itemAction->setEditable(false);

        //add all shortcuts bound the action
        QHash<ShortcutKeys /*keycombination*/ , int /*action*/>::iterator i = globalShortcutBinding.begin();
        while(i!=globalShortcutBinding.end()) {
            if(i.value() == act->id) {
                QStandardItem *binding = new QStandardItem(i.key().ToString());
                binding->setData( QVariant::fromValue(i.key()) ,UserRoles::shortcut );
                itemAction->appendRow(binding);
            }
            ++i;
        }

        parentItem->appendRow(itemAction);
    }

    return shortcutsModel;
}

void MainConfig::UpdateShortcutsFromModel()
{
    if(!shortcutsModel)
        return;

    globalShortcutBinding.clear();

    QStandardItem *parentItem = shortcutsModel->invisibleRootItem();

    //list all actions
    while(parentItem->rowCount()>0) {
        QList<QStandardItem *>actionRow = parentItem->takeRow(0);
        if(!actionRow[0])
            continue;

        //list all bound shortcuts
        while(actionRow[0]->rowCount()>0) {
            QList<QStandardItem *>shortcutRow = actionRow[0]->takeRow(0);
            if(!shortcutRow[0])
                continue;

            globalShortcutBinding.insert(shortcutRow[0]->data(UserRoles::shortcut).value<ShortcutKeys>() ,actionRow[0]->data().toInt());
        }
    }

    //invalidate the model
    delete shortcutsModel;
    shortcutsModel=0;
}

void MainConfig::ListenToAction(QString const &action, QObject * listener)
{
    Action *act = 0;

    foreach(Action *a, listActions) {
        if(a->name == action) {
            act = a;
            break;
        }
    }

    if(!act) {
        debug("action not found %s",action.toAscii().constData());
        return;
    }

    //avoid duplicates
    QHash<int, QObject *>::iterator i = actionListeners.find(act->id);
    while(i!=actionListeners.end() && i.key()==act->id) {
        if(i.value() == listener)
            return;
        ++i;
    }

    actionListeners.insert(act->id,listener);
}

void MainConfig::SendAction(int actionId, const QVariant &value)
{
    QHash<int, QObject *>::iterator i = actionListeners.find(actionId);
    while(i!=actionListeners.end() && i.key()==actionId) {
//        QApplication::removePostedEvents(listener,Event::Shortcut);
        ShortcutEvent *event = new ShortcutEvent(actionId,value);
        QApplication::postEvent(i.value(),event);
        ++i;
    }
}

void MainConfig::ShortcutStateChanged(ShortcutKeys &shortcut, bool pressed)
{
    if(pressed) {
        currentShortcutPressed = true;
        debug("pressed shortcut %s",shortcut.ToString().toAscii().constData());

        QHash<ShortcutKeys /*keycombination*/ , int /*action*/>::iterator i = globalShortcutBinding.find(currentShortcut);
        while(i!=globalShortcutBinding.end() && i.key()==currentShortcut) {
            SendAction(i.value(),true);
            ++i;
        }

    } else {
        debug("released shortcut %s",shortcut.ToString().toAscii().constData());

        QHash<ShortcutKeys /*keycombination*/ , int /*action*/>::iterator i = globalShortcutBinding.find(currentShortcut);
        while(i!=globalShortcutBinding.end() && i.key()==currentShortcut) {
            SendAction(i.value(),false);
            ++i;
        }
        currentShortcutPressed = false;
    }
}

bool MainConfig::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type()) {
        case QEvent::KeyRelease :
            {
                QKeyEvent *e = static_cast<QKeyEvent *>(event);
//                    if(e->isAutoRepeat())
//                        break;
                currentShortcut.listKeys.remove(e->key());
            }
            if(currentShortcutPressed)
                ShortcutStateChanged(currentShortcut,false);
            return QObject::eventFilter(obj, event);

        case QEvent::MouseButtonRelease :
            {
                QMouseEvent *e = static_cast<QMouseEvent *>(event);
                currentShortcut.mouseButtons = e->buttons();
            }
            if(currentShortcutPressed)
                ShortcutStateChanged(currentShortcut,false);
            return QObject::eventFilter(obj, event);

        case QEvent::KeyPress :
            {
                QKeyEvent *e = static_cast<QKeyEvent *>(event);
//                    if(e->isAutoRepeat())
//                        break;
                currentShortcut.listKeys << e->key();
            }
            break;

        case QEvent::MouseButtonPress :
            {
                QMouseEvent *e = static_cast<QMouseEvent *>(event);
                currentShortcut.mouseButtons = e->buttons();
            }
            break;

        case QEvent::Wheel :
            {
                QWheelEvent *e = static_cast<QWheelEvent *>(event);
                currentShortcut.mouseWheel = e->delta();
                if(globalShortcutBinding.contains(currentShortcut)) {
                    ShortcutStateChanged(currentShortcut,true);
                    ShortcutStateChanged(currentShortcut,false);
                }
                currentShortcut.mouseWheel=0;
                return QObject::eventFilter(obj, event);
            }
            break;

        default :
            return QObject::eventFilter(obj, event);

    }

    if(globalShortcutBinding.contains(currentShortcut))
        ShortcutStateChanged(currentShortcut,true);

    return QObject::eventFilter(obj, event);
}

void MainConfig::InitListActions()
{
    int act = 0;
    listActions << new Action(act++, "sceneZoomIn", tr("Scene Zoom In"));
    listActions << new Action(act++, "sceneZoomOut", tr("Scene Zoom Out"));
    listActions << new Action(act++, "sceneDrag", tr("Drag Scene"));
    listActions << new Action(act++, "test2",       tr("test 2"));
    listActions << new Action(act++, "blah",        tr("blah"));
    listActions << new Action(act++, "test2aze",    tr("test 2 aze"));
    listActions << new Action(act++, "test2qsd",    tr("test 2 qsd"));
    listActions << new Action(act++, "test2wxc",    tr("test 2 wxc"));
}


QString MainConfig::KeyToString(int key)
{
    if(!listKeysToStrings.contains(key))
        return "";

    return listKeysToStrings.value(key);
}

void MainConfig::InitKeyStrings()
{
    listKeysToStrings.insert(Qt::Key_Escape,tr("Escape"));
    listKeysToStrings.insert(Qt::Key_Tab,tr("Tab"));
    listKeysToStrings.insert(Qt::Key_Backtab,tr("Backtab"));
    listKeysToStrings.insert(Qt::Key_Backspace,tr("Backspace"));
    listKeysToStrings.insert(Qt::Key_Return,tr("Return"));
    listKeysToStrings.insert(Qt::Key_Enter,tr("Enter"));
    listKeysToStrings.insert(Qt::Key_Insert,tr("Insert"));
    listKeysToStrings.insert(Qt::Key_Delete,tr("Delete"));
    listKeysToStrings.insert(Qt::Key_Pause,tr("Pause"));
    listKeysToStrings.insert(Qt::Key_Print,tr("Print"));
    listKeysToStrings.insert(Qt::Key_SysReq,tr("SysReq"));
    listKeysToStrings.insert(Qt::Key_Clear,tr("Clear"));
    listKeysToStrings.insert(Qt::Key_Home,tr("Home"));
    listKeysToStrings.insert(Qt::Key_End,tr("End"));
    listKeysToStrings.insert(Qt::Key_Left,tr("Left"));
    listKeysToStrings.insert(Qt::Key_Up,tr("Up"));
    listKeysToStrings.insert(Qt::Key_Right,tr("Right"));
    listKeysToStrings.insert(Qt::Key_Down,tr("Down"));
    listKeysToStrings.insert(Qt::Key_PageUp,tr("PageUp"));
    listKeysToStrings.insert(Qt::Key_PageDown,tr("PageDown"));
    listKeysToStrings.insert(Qt::Key_Shift,tr("Shift"));
    listKeysToStrings.insert(Qt::Key_Control,tr("Control"));
    listKeysToStrings.insert(Qt::Key_Meta,tr("Meta"));
    listKeysToStrings.insert(Qt::Key_Alt,tr("Alt"));
    listKeysToStrings.insert(Qt::Key_AltGr,tr("AltGr"));
    listKeysToStrings.insert(Qt::Key_CapsLock,tr("CapsLock"));
    listKeysToStrings.insert(Qt::Key_NumLock,tr("NumLock"));
    listKeysToStrings.insert(Qt::Key_ScrollLock,tr("ScrollLock"));
    listKeysToStrings.insert(Qt::Key_F1,tr("F1"));
    listKeysToStrings.insert(Qt::Key_F2,tr("F2"));
    listKeysToStrings.insert(Qt::Key_F3,tr("F3"));
    listKeysToStrings.insert(Qt::Key_F4,tr("F4"));
    listKeysToStrings.insert(Qt::Key_F5,tr("F5"));
    listKeysToStrings.insert(Qt::Key_F6,tr("F6"));
    listKeysToStrings.insert(Qt::Key_F7,tr("F7"));
    listKeysToStrings.insert(Qt::Key_F8,tr("F8"));
    listKeysToStrings.insert(Qt::Key_F9,tr("F9"));
    listKeysToStrings.insert(Qt::Key_F10,tr("F10"));
    listKeysToStrings.insert(Qt::Key_F11,tr("F11"));
    listKeysToStrings.insert(Qt::Key_F12,tr("F12"));
    listKeysToStrings.insert(Qt::Key_F13,tr("F13"));
    listKeysToStrings.insert(Qt::Key_F14,tr("F14"));
    listKeysToStrings.insert(Qt::Key_F15,tr("F15"));
    listKeysToStrings.insert(Qt::Key_F16,tr("F16"));
    listKeysToStrings.insert(Qt::Key_F17,tr("F17"));
    listKeysToStrings.insert(Qt::Key_F18,tr("F18"));
    listKeysToStrings.insert(Qt::Key_F19,tr("F19"));
    listKeysToStrings.insert(Qt::Key_F20,tr("F20"));
    listKeysToStrings.insert(Qt::Key_F21,tr("F21"));
    listKeysToStrings.insert(Qt::Key_F22,tr("F22"));
    listKeysToStrings.insert(Qt::Key_F23,tr("F23"));
    listKeysToStrings.insert(Qt::Key_F24,tr("F24"));
    listKeysToStrings.insert(Qt::Key_F25,tr("F25"));
    listKeysToStrings.insert(Qt::Key_F26,tr("F26"));
    listKeysToStrings.insert(Qt::Key_F27,tr("F27"));
    listKeysToStrings.insert(Qt::Key_F28,tr("F28"));
    listKeysToStrings.insert(Qt::Key_F29,tr("F29"));
    listKeysToStrings.insert(Qt::Key_F30,tr("F30"));
    listKeysToStrings.insert(Qt::Key_F31,tr("F31"));
    listKeysToStrings.insert(Qt::Key_F32,tr("F32"));
    listKeysToStrings.insert(Qt::Key_F33,tr("F33"));
    listKeysToStrings.insert(Qt::Key_F34,tr("F34"));
    listKeysToStrings.insert(Qt::Key_F35,tr("F35"));
    listKeysToStrings.insert(Qt::Key_Super_L,tr("Super_L"));
    listKeysToStrings.insert(Qt::Key_Super_R,tr("Super_R"));
    listKeysToStrings.insert(Qt::Key_Menu,tr("Menu"));
    listKeysToStrings.insert(Qt::Key_Hyper_L,tr("Hyper_L"));
    listKeysToStrings.insert(Qt::Key_Hyper_R,tr("Hyper_R"));
    listKeysToStrings.insert(Qt::Key_Help,tr("Help"));
    listKeysToStrings.insert(Qt::Key_Direction_L,tr("Direction_L"));
    listKeysToStrings.insert(Qt::Key_Direction_R,tr("Direction_R"));
    listKeysToStrings.insert(Qt::Key_Space,tr("Space"));
    listKeysToStrings.insert(Qt::Key_Any,tr("Any"));
    listKeysToStrings.insert(Qt::Key_Exclam,tr("Exclam"));
    listKeysToStrings.insert(Qt::Key_QuoteDbl,tr("QuoteDbl"));
    listKeysToStrings.insert(Qt::Key_NumberSign,tr("NumberSign"));
    listKeysToStrings.insert(Qt::Key_Dollar,tr("Dollar"));
    listKeysToStrings.insert(Qt::Key_Percent,tr("Percent"));
    listKeysToStrings.insert(Qt::Key_Ampersand,tr("Ampersand"));
    listKeysToStrings.insert(Qt::Key_Apostrophe,tr("Apostrophe"));
    listKeysToStrings.insert(Qt::Key_ParenLeft,tr("ParenLeft"));
    listKeysToStrings.insert(Qt::Key_ParenRight,tr("ParenRight"));
    listKeysToStrings.insert(Qt::Key_Asterisk,tr("Asterisk"));
    listKeysToStrings.insert(Qt::Key_Plus,tr("Plus"));
    listKeysToStrings.insert(Qt::Key_Comma,tr("Comma"));
    listKeysToStrings.insert(Qt::Key_Minus,tr("Minus"));
    listKeysToStrings.insert(Qt::Key_Period,tr("Period"));
    listKeysToStrings.insert(Qt::Key_Slash,tr("Slash"));
    listKeysToStrings.insert(Qt::Key_0,tr("0"));
    listKeysToStrings.insert(Qt::Key_1,tr("1"));
    listKeysToStrings.insert(Qt::Key_2,tr("2"));
    listKeysToStrings.insert(Qt::Key_3,tr("3"));
    listKeysToStrings.insert(Qt::Key_4,tr("4"));
    listKeysToStrings.insert(Qt::Key_5,tr("5"));
    listKeysToStrings.insert(Qt::Key_6,tr("6"));
    listKeysToStrings.insert(Qt::Key_7,tr("7"));
    listKeysToStrings.insert(Qt::Key_8,tr("8"));
    listKeysToStrings.insert(Qt::Key_9,tr("9"));
    listKeysToStrings.insert(Qt::Key_Colon,tr("Colon"));
    listKeysToStrings.insert(Qt::Key_Semicolon,tr("Semicolon"));
    listKeysToStrings.insert(Qt::Key_Less,tr("Less"));
    listKeysToStrings.insert(Qt::Key_Equal,tr("Equal"));
    listKeysToStrings.insert(Qt::Key_Greater,tr("Greater"));
    listKeysToStrings.insert(Qt::Key_Question,tr("Question"));
    listKeysToStrings.insert(Qt::Key_At,tr("At"));
    listKeysToStrings.insert(Qt::Key_A,tr("A"));
    listKeysToStrings.insert(Qt::Key_B,tr("B"));
    listKeysToStrings.insert(Qt::Key_C,tr("C"));
    listKeysToStrings.insert(Qt::Key_D,tr("D"));
    listKeysToStrings.insert(Qt::Key_E,tr("E"));
    listKeysToStrings.insert(Qt::Key_F,tr("F"));
    listKeysToStrings.insert(Qt::Key_G,tr("G"));
    listKeysToStrings.insert(Qt::Key_H,tr("H"));
    listKeysToStrings.insert(Qt::Key_I,tr("I"));
    listKeysToStrings.insert(Qt::Key_J,tr("J"));
    listKeysToStrings.insert(Qt::Key_K,tr("K"));
    listKeysToStrings.insert(Qt::Key_L,tr("L"));
    listKeysToStrings.insert(Qt::Key_M,tr("M"));
    listKeysToStrings.insert(Qt::Key_N,tr("N"));
    listKeysToStrings.insert(Qt::Key_O,tr("O"));
    listKeysToStrings.insert(Qt::Key_P,tr("P"));
    listKeysToStrings.insert(Qt::Key_Q,tr("Q"));
    listKeysToStrings.insert(Qt::Key_R,tr("R"));
    listKeysToStrings.insert(Qt::Key_S,tr("S"));
    listKeysToStrings.insert(Qt::Key_T,tr("T"));
    listKeysToStrings.insert(Qt::Key_U,tr("U"));
    listKeysToStrings.insert(Qt::Key_V,tr("V"));
    listKeysToStrings.insert(Qt::Key_W,tr("W"));
    listKeysToStrings.insert(Qt::Key_X,tr("X"));
    listKeysToStrings.insert(Qt::Key_Y,tr("Y"));
    listKeysToStrings.insert(Qt::Key_Z,tr("Z"));
    listKeysToStrings.insert(Qt::Key_BracketLeft,tr("BracketLeft"));
    listKeysToStrings.insert(Qt::Key_Backslash,tr("Backslash"));
    listKeysToStrings.insert(Qt::Key_BracketRight,tr("BracketRight"));
    listKeysToStrings.insert(Qt::Key_AsciiCircum,tr("AsciiCircum"));
    listKeysToStrings.insert(Qt::Key_Underscore,tr("Underscore"));
    listKeysToStrings.insert(Qt::Key_QuoteLeft,tr("QuoteLeft"));
    listKeysToStrings.insert(Qt::Key_BraceLeft,tr("BraceLeft"));
    listKeysToStrings.insert(Qt::Key_Bar,tr("Bar"));
    listKeysToStrings.insert(Qt::Key_BraceRight,tr("BraceRight"));
    listKeysToStrings.insert(Qt::Key_AsciiTilde,tr("AsciiTilde"));
    listKeysToStrings.insert(Qt::Key_nobreakspace,tr("nobreakspace"));
    listKeysToStrings.insert(Qt::Key_exclamdown,tr("exclamdown"));
    listKeysToStrings.insert(Qt::Key_cent,tr("cent"));
    listKeysToStrings.insert(Qt::Key_sterling,tr("sterling"));
    listKeysToStrings.insert(Qt::Key_currency,tr("currency"));
    listKeysToStrings.insert(Qt::Key_yen,tr("yen"));
    listKeysToStrings.insert(Qt::Key_brokenbar,tr("brokenbar"));
    listKeysToStrings.insert(Qt::Key_section,tr("section"));
    listKeysToStrings.insert(Qt::Key_diaeresis,tr("diaeresis"));
    listKeysToStrings.insert(Qt::Key_copyright,tr("copyright"));
    listKeysToStrings.insert(Qt::Key_ordfeminine,tr("ordfeminine"));
    listKeysToStrings.insert(Qt::Key_guillemotleft,tr("guillemotleft"));
    listKeysToStrings.insert(Qt::Key_notsign,tr("notsign"));
    listKeysToStrings.insert(Qt::Key_hyphen,tr("hyphen"));
    listKeysToStrings.insert(Qt::Key_registered,tr("registered"));
    listKeysToStrings.insert(Qt::Key_macron,tr("macron"));
    listKeysToStrings.insert(Qt::Key_degree,tr("degree"));
    listKeysToStrings.insert(Qt::Key_plusminus,tr("plusminus"));
    listKeysToStrings.insert(Qt::Key_twosuperior,tr("twosuperior"));
    listKeysToStrings.insert(Qt::Key_threesuperior,tr("threesuperior"));
    listKeysToStrings.insert(Qt::Key_acute,tr("acute"));
    listKeysToStrings.insert(Qt::Key_mu,tr("mu"));
    listKeysToStrings.insert(Qt::Key_paragraph,tr("paragraph"));
    listKeysToStrings.insert(Qt::Key_periodcentered,tr("periodcentered"));
    listKeysToStrings.insert(Qt::Key_cedilla,tr("cedilla"));
    listKeysToStrings.insert(Qt::Key_onesuperior,tr("onesuperior"));
    listKeysToStrings.insert(Qt::Key_masculine,tr("masculine"));
    listKeysToStrings.insert(Qt::Key_guillemotright,tr("guillemotright"));
    listKeysToStrings.insert(Qt::Key_onequarter,tr("onequarter"));
    listKeysToStrings.insert(Qt::Key_onehalf,tr("onehalf"));
    listKeysToStrings.insert(Qt::Key_threequarters,tr("threequarters"));
    listKeysToStrings.insert(Qt::Key_questiondown,tr("questiondown"));
    listKeysToStrings.insert(Qt::Key_Agrave,tr("Agrave"));
    listKeysToStrings.insert(Qt::Key_Aacute,tr("Aacute"));
    listKeysToStrings.insert(Qt::Key_Acircumflex,tr("Acircumflex"));
    listKeysToStrings.insert(Qt::Key_Atilde,tr("Atilde"));
    listKeysToStrings.insert(Qt::Key_Adiaeresis,tr("Adiaeresis"));
    listKeysToStrings.insert(Qt::Key_Aring,tr("Aring"));
    listKeysToStrings.insert(Qt::Key_AE,tr("AE"));
    listKeysToStrings.insert(Qt::Key_Ccedilla,tr("Ccedilla"));
    listKeysToStrings.insert(Qt::Key_Egrave,tr("Egrave"));
    listKeysToStrings.insert(Qt::Key_Eacute,tr("Eacute"));
    listKeysToStrings.insert(Qt::Key_Ecircumflex,tr("Ecircumflex"));
    listKeysToStrings.insert(Qt::Key_Ediaeresis,tr("Ediaeresis"));
    listKeysToStrings.insert(Qt::Key_Igrave,tr("Igrave"));
    listKeysToStrings.insert(Qt::Key_Iacute,tr("Iacute"));
    listKeysToStrings.insert(Qt::Key_Icircumflex,tr("Icircumflex"));
    listKeysToStrings.insert(Qt::Key_Idiaeresis,tr("Idiaeresis"));
    listKeysToStrings.insert(Qt::Key_ETH,tr("ETH"));
    listKeysToStrings.insert(Qt::Key_Ntilde,tr("Ntilde"));
    listKeysToStrings.insert(Qt::Key_Ograve,tr("Ograve"));
    listKeysToStrings.insert(Qt::Key_Oacute,tr("Oacute"));
    listKeysToStrings.insert(Qt::Key_Ocircumflex,tr("Ocircumflex"));
    listKeysToStrings.insert(Qt::Key_Otilde,tr("Otilde"));
    listKeysToStrings.insert(Qt::Key_Odiaeresis,tr("Odiaeresis"));
    listKeysToStrings.insert(Qt::Key_multiply,tr("multiply"));
    listKeysToStrings.insert(Qt::Key_Ooblique,tr("Ooblique"));
    listKeysToStrings.insert(Qt::Key_Ugrave,tr("Ugrave"));
    listKeysToStrings.insert(Qt::Key_Uacute,tr("Uacute"));
    listKeysToStrings.insert(Qt::Key_Ucircumflex,tr("Ucircumflex"));
    listKeysToStrings.insert(Qt::Key_Udiaeresis,tr("Udiaeresis"));
    listKeysToStrings.insert(Qt::Key_Yacute,tr("Yacute"));
    listKeysToStrings.insert(Qt::Key_THORN,tr("THORN"));
    listKeysToStrings.insert(Qt::Key_ssharp,tr("ssharp"));
    listKeysToStrings.insert(Qt::Key_division,tr("division"));
    listKeysToStrings.insert(Qt::Key_ydiaeresis,tr("ydiaeresis"));
    listKeysToStrings.insert(Qt::Key_Multi_key,tr("Multi_key"));
    listKeysToStrings.insert(Qt::Key_Codeinput,tr("Codeinput"));
    listKeysToStrings.insert(Qt::Key_SingleCandidate,tr("SingleCandidate"));
    listKeysToStrings.insert(Qt::Key_MultipleCandidate,tr("MultipleCandidate"));
    listKeysToStrings.insert(Qt::Key_PreviousCandidate,tr("PreviousCandidate"));
    listKeysToStrings.insert(Qt::Key_Mode_switch,tr("Mode_switch"));
    listKeysToStrings.insert(Qt::Key_Kanji,tr("Kanji"));
    listKeysToStrings.insert(Qt::Key_Muhenkan,tr("Muhenkan"));
    listKeysToStrings.insert(Qt::Key_Henkan,tr("Henkan"));
    listKeysToStrings.insert(Qt::Key_Romaji,tr("Romaji"));
    listKeysToStrings.insert(Qt::Key_Hiragana,tr("Hiragana"));
    listKeysToStrings.insert(Qt::Key_Katakana,tr("Katakana"));
    listKeysToStrings.insert(Qt::Key_Hiragana_Katakana,tr("Hiragana_Katakana"));
    listKeysToStrings.insert(Qt::Key_Zenkaku,tr("Zenkaku"));
    listKeysToStrings.insert(Qt::Key_Hankaku,tr("Hankaku"));
    listKeysToStrings.insert(Qt::Key_Zenkaku_Hankaku,tr("Zenkaku_Hankaku"));
    listKeysToStrings.insert(Qt::Key_Touroku,tr("Touroku"));
    listKeysToStrings.insert(Qt::Key_Massyo,tr("Massyo"));
    listKeysToStrings.insert(Qt::Key_Kana_Lock,tr("Kana_Lock"));
    listKeysToStrings.insert(Qt::Key_Kana_Shift,tr("Kana_Shift"));
    listKeysToStrings.insert(Qt::Key_Eisu_Shift,tr("Eisu_Shift"));
    listKeysToStrings.insert(Qt::Key_Eisu_toggle,tr("Eisu_toggle"));
    listKeysToStrings.insert(Qt::Key_Hangul,tr("Hangul"));
    listKeysToStrings.insert(Qt::Key_Hangul_Start,tr("Hangul_Start"));
    listKeysToStrings.insert(Qt::Key_Hangul_End,tr("Hangul_End"));
    listKeysToStrings.insert(Qt::Key_Hangul_Hanja,tr("Hangul_Hanja"));
    listKeysToStrings.insert(Qt::Key_Hangul_Jamo,tr("Hangul_Jamo"));
    listKeysToStrings.insert(Qt::Key_Hangul_Romaja,tr("Hangul_Romaja"));
    listKeysToStrings.insert(Qt::Key_Hangul_Jeonja,tr("Hangul_Jeonja"));
    listKeysToStrings.insert(Qt::Key_Hangul_Banja,tr("Hangul_Banja"));
    listKeysToStrings.insert(Qt::Key_Hangul_PreHanja,tr("Hangul_PreHanja"));
    listKeysToStrings.insert(Qt::Key_Hangul_PostHanja,tr("Hangul_PostHanja"));
    listKeysToStrings.insert(Qt::Key_Hangul_Special,tr("Hangul_Special"));
    listKeysToStrings.insert(Qt::Key_Dead_Grave,tr("Dead_Grave"));
    listKeysToStrings.insert(Qt::Key_Dead_Acute,tr("Dead_Acute"));
    listKeysToStrings.insert(Qt::Key_Dead_Circumflex,tr("Dead_Circumflex"));
    listKeysToStrings.insert(Qt::Key_Dead_Tilde,tr("Dead_Tilde"));
    listKeysToStrings.insert(Qt::Key_Dead_Macron,tr("Dead_Macron"));
    listKeysToStrings.insert(Qt::Key_Dead_Breve,tr("Dead_Breve"));
    listKeysToStrings.insert(Qt::Key_Dead_Abovedot,tr("Dead_Abovedot"));
    listKeysToStrings.insert(Qt::Key_Dead_Diaeresis,tr("Dead_Diaeresis"));
    listKeysToStrings.insert(Qt::Key_Dead_Abovering,tr("Dead_Abovering"));
    listKeysToStrings.insert(Qt::Key_Dead_Doubleacute,tr("Dead_Doubleacute"));
    listKeysToStrings.insert(Qt::Key_Dead_Caron,tr("Dead_Caron"));
    listKeysToStrings.insert(Qt::Key_Dead_Cedilla,tr("Dead_Cedilla"));
    listKeysToStrings.insert(Qt::Key_Dead_Ogonek,tr("Dead_Ogonek"));
    listKeysToStrings.insert(Qt::Key_Dead_Iota,tr("Dead_Iota"));
    listKeysToStrings.insert(Qt::Key_Dead_Voiced_Sound,tr("Dead_Voiced_Sound"));
    listKeysToStrings.insert(Qt::Key_Dead_Semivoiced_Sound,tr("Dead_Semivoiced_Sound"));
    listKeysToStrings.insert(Qt::Key_Dead_Belowdot,tr("Dead_Belowdot"));
    listKeysToStrings.insert(Qt::Key_Dead_Hook,tr("Dead_Hook"));
    listKeysToStrings.insert(Qt::Key_Dead_Horn,tr("Dead_Horn"));
    listKeysToStrings.insert(Qt::Key_Back,tr("Back"));
    listKeysToStrings.insert(Qt::Key_Forward,tr("Forward"));
    listKeysToStrings.insert(Qt::Key_Stop,tr("Stop"));
    listKeysToStrings.insert(Qt::Key_Refresh,tr("Refresh"));
    listKeysToStrings.insert(Qt::Key_VolumeDown,tr("VolumeDown"));
    listKeysToStrings.insert(Qt::Key_VolumeMute,tr("VolumeMute"));
    listKeysToStrings.insert(Qt::Key_VolumeUp,tr("VolumeUp"));
    listKeysToStrings.insert(Qt::Key_BassBoost,tr("BassBoost"));
    listKeysToStrings.insert(Qt::Key_BassUp,tr("BassUp"));
    listKeysToStrings.insert(Qt::Key_BassDown,tr("BassDown"));
    listKeysToStrings.insert(Qt::Key_TrebleUp,tr("TrebleUp"));
    listKeysToStrings.insert(Qt::Key_TrebleDown,tr("TrebleDown"));
    listKeysToStrings.insert(Qt::Key_MediaPlay,tr("MediaPlay"));
    listKeysToStrings.insert(Qt::Key_MediaStop,tr("MediaStop"));
    listKeysToStrings.insert(Qt::Key_MediaPrevious,tr("MediaPrevious"));
    listKeysToStrings.insert(Qt::Key_MediaNext,tr("MediaNext"));
    listKeysToStrings.insert(Qt::Key_MediaRecord,tr("MediaRecord"));
    listKeysToStrings.insert(Qt::Key_HomePage,tr("HomePage"));
    listKeysToStrings.insert(Qt::Key_Favorites,tr("Favorites"));
    listKeysToStrings.insert(Qt::Key_Search,tr("Search"));
    listKeysToStrings.insert(Qt::Key_Standby,tr("Standby"));
    listKeysToStrings.insert(Qt::Key_OpenUrl,tr("OpenUrl"));
    listKeysToStrings.insert(Qt::Key_LaunchMail,tr("LaunchMail"));
    listKeysToStrings.insert(Qt::Key_LaunchMedia,tr("LaunchMedia"));
    listKeysToStrings.insert(Qt::Key_Launch0,tr("Launch0"));
    listKeysToStrings.insert(Qt::Key_Launch1,tr("Launch1"));
    listKeysToStrings.insert(Qt::Key_Launch2,tr("Launch2"));
    listKeysToStrings.insert(Qt::Key_Launch3,tr("Launch3"));
    listKeysToStrings.insert(Qt::Key_Launch4,tr("Launch4"));
    listKeysToStrings.insert(Qt::Key_Launch5,tr("Launch5"));
    listKeysToStrings.insert(Qt::Key_Launch6,tr("Launch6"));
    listKeysToStrings.insert(Qt::Key_Launch7,tr("Launch7"));
    listKeysToStrings.insert(Qt::Key_Launch8,tr("Launch8"));
    listKeysToStrings.insert(Qt::Key_Launch9,tr("Launch9"));
    listKeysToStrings.insert(Qt::Key_LaunchA,tr("LaunchA"));
    listKeysToStrings.insert(Qt::Key_LaunchB,tr("LaunchB"));
    listKeysToStrings.insert(Qt::Key_LaunchC,tr("LaunchC"));
    listKeysToStrings.insert(Qt::Key_LaunchD,tr("LaunchD"));
    listKeysToStrings.insert(Qt::Key_LaunchE,tr("LaunchE"));
    listKeysToStrings.insert(Qt::Key_LaunchF,tr("LaunchF"));
    listKeysToStrings.insert(Qt::Key_MonBrightnessUp,tr("MonBrightnessUp"));
    listKeysToStrings.insert(Qt::Key_MonBrightnessDown,tr("MonBrightnessDown"));
    listKeysToStrings.insert(Qt::Key_KeyboardLightOnOff,tr("KeyboardLightOnOff"));
    listKeysToStrings.insert(Qt::Key_KeyboardBrightnessUp,tr("KeyboardBrightnessUp"));
    listKeysToStrings.insert(Qt::Key_KeyboardBrightnessDown,tr("KeyboardBrightnessDown"));
    listKeysToStrings.insert(Qt::Key_PowerOff,tr("PowerOff"));
    listKeysToStrings.insert(Qt::Key_WakeUp,tr("WakeUp"));
    listKeysToStrings.insert(Qt::Key_Eject,tr("Eject"));
    listKeysToStrings.insert(Qt::Key_ScreenSaver,tr("ScreenSaver"));
    listKeysToStrings.insert(Qt::Key_WWW,tr("WWW"));
    listKeysToStrings.insert(Qt::Key_Memo,tr("Memo"));
    listKeysToStrings.insert(Qt::Key_LightBulb,tr("LightBulb"));
    listKeysToStrings.insert(Qt::Key_Shop,tr("Shop"));
    listKeysToStrings.insert(Qt::Key_History,tr("History"));
    listKeysToStrings.insert(Qt::Key_AddFavorite,tr("AddFavorite"));
    listKeysToStrings.insert(Qt::Key_HotLinks,tr("HotLinks"));
    listKeysToStrings.insert(Qt::Key_BrightnessAdjust,tr("BrightnessAdjust"));
    listKeysToStrings.insert(Qt::Key_Finance,tr("Finance"));
    listKeysToStrings.insert(Qt::Key_Community,tr("Community"));
    listKeysToStrings.insert(Qt::Key_AudioRewind,tr("AudioRewind"));
    listKeysToStrings.insert(Qt::Key_BackForward,tr("BackForward"));
    listKeysToStrings.insert(Qt::Key_ApplicationLeft,tr("ApplicationLeft"));
    listKeysToStrings.insert(Qt::Key_ApplicationRight,tr("ApplicationRight"));
    listKeysToStrings.insert(Qt::Key_Book,tr("Book"));
    listKeysToStrings.insert(Qt::Key_CD,tr("CD"));
    listKeysToStrings.insert(Qt::Key_Calculator,tr("Calculator"));
    listKeysToStrings.insert(Qt::Key_ToDoList,tr("ToDoList"));
    listKeysToStrings.insert(Qt::Key_ClearGrab,tr("ClearGrab"));
    listKeysToStrings.insert(Qt::Key_Close,tr("Close"));
    listKeysToStrings.insert(Qt::Key_Copy,tr("Copy"));
    listKeysToStrings.insert(Qt::Key_Cut,tr("Cut"));
    listKeysToStrings.insert(Qt::Key_Display,tr("Display"));
    listKeysToStrings.insert(Qt::Key_DOS,tr("DOS"));
    listKeysToStrings.insert(Qt::Key_Documents,tr("Documents"));
    listKeysToStrings.insert(Qt::Key_Excel,tr("Excel"));
    listKeysToStrings.insert(Qt::Key_Explorer,tr("Explorer"));
    listKeysToStrings.insert(Qt::Key_Game,tr("Game"));
    listKeysToStrings.insert(Qt::Key_Go,tr("Go"));
    listKeysToStrings.insert(Qt::Key_iTouch,tr("iTouch"));
    listKeysToStrings.insert(Qt::Key_LogOff,tr("LogOff"));
    listKeysToStrings.insert(Qt::Key_Market,tr("Market"));
    listKeysToStrings.insert(Qt::Key_Meeting,tr("Meeting"));
    listKeysToStrings.insert(Qt::Key_MenuKB,tr("MenuKB"));
    listKeysToStrings.insert(Qt::Key_MenuPB,tr("MenuPB"));
    listKeysToStrings.insert(Qt::Key_MySites,tr("MySites"));
    listKeysToStrings.insert(Qt::Key_News,tr("News"));
    listKeysToStrings.insert(Qt::Key_OfficeHome,tr("OfficeHome"));
    listKeysToStrings.insert(Qt::Key_Option,tr("Option"));
    listKeysToStrings.insert(Qt::Key_Paste,tr("Paste"));
    listKeysToStrings.insert(Qt::Key_Phone,tr("Phone"));
    listKeysToStrings.insert(Qt::Key_Calendar,tr("Calendar"));
    listKeysToStrings.insert(Qt::Key_Reply,tr("Reply"));
    listKeysToStrings.insert(Qt::Key_Reload,tr("Reload"));
    listKeysToStrings.insert(Qt::Key_RotateWindows,tr("RotateWindows"));
    listKeysToStrings.insert(Qt::Key_RotationPB,tr("RotationPB"));
    listKeysToStrings.insert(Qt::Key_RotationKB,tr("RotationKB"));
    listKeysToStrings.insert(Qt::Key_Save,tr("Save"));
    listKeysToStrings.insert(Qt::Key_Send,tr("Send"));
    listKeysToStrings.insert(Qt::Key_Spell,tr("Spell"));
    listKeysToStrings.insert(Qt::Key_SplitScreen,tr("SplitScreen"));
    listKeysToStrings.insert(Qt::Key_Support,tr("Support"));
    listKeysToStrings.insert(Qt::Key_TaskPane,tr("TaskPane"));
    listKeysToStrings.insert(Qt::Key_Terminal,tr("Terminal"));
    listKeysToStrings.insert(Qt::Key_Tools,tr("Tools"));
    listKeysToStrings.insert(Qt::Key_Travel,tr("Travel"));
    listKeysToStrings.insert(Qt::Key_Video,tr("Video"));
    listKeysToStrings.insert(Qt::Key_Word,tr("Word"));
    listKeysToStrings.insert(Qt::Key_Xfer,tr("Xfer"));
    listKeysToStrings.insert(Qt::Key_ZoomIn,tr("ZoomIn"));
    listKeysToStrings.insert(Qt::Key_ZoomOut,tr("ZoomOut"));
    listKeysToStrings.insert(Qt::Key_Away,tr("Away"));
    listKeysToStrings.insert(Qt::Key_Messenger,tr("Messenger"));
    listKeysToStrings.insert(Qt::Key_WebCam,tr("WebCam"));
    listKeysToStrings.insert(Qt::Key_MailForward,tr("MailForward"));
    listKeysToStrings.insert(Qt::Key_Pictures,tr("Pictures"));
    listKeysToStrings.insert(Qt::Key_Music,tr("Music"));
    listKeysToStrings.insert(Qt::Key_Battery,tr("Battery"));
    listKeysToStrings.insert(Qt::Key_Bluetooth,tr("Bluetooth"));
    listKeysToStrings.insert(Qt::Key_WLAN,tr("WLAN"));
    listKeysToStrings.insert(Qt::Key_UWB,tr("UWB"));
    listKeysToStrings.insert(Qt::Key_AudioForward,tr("AudioForward"));
    listKeysToStrings.insert(Qt::Key_AudioRepeat,tr("AudioRepeat"));
    listKeysToStrings.insert(Qt::Key_AudioRandomPlay,tr("AudioRandomPlay"));
    listKeysToStrings.insert(Qt::Key_Subtitle,tr("Subtitle"));
    listKeysToStrings.insert(Qt::Key_AudioCycleTrack,tr("AudioCycleTrack"));
    listKeysToStrings.insert(Qt::Key_Time,tr("Time"));
    listKeysToStrings.insert(Qt::Key_Hibernate,tr("Hibernate"));
    listKeysToStrings.insert(Qt::Key_View,tr("View"));
    listKeysToStrings.insert(Qt::Key_TopMenu,tr("TopMenu"));
    listKeysToStrings.insert(Qt::Key_PowerDown,tr("PowerDown"));
    listKeysToStrings.insert(Qt::Key_Suspend,tr("Suspend"));
    listKeysToStrings.insert(Qt::Key_ContrastAdjust,tr("ContrastAdjust"));
    listKeysToStrings.insert(Qt::Key_MediaLast,tr("MediaLast"));
    listKeysToStrings.insert(Qt::Key_unknown,tr("unknown"));
    listKeysToStrings.insert(Qt::Key_Call,tr("Call"));
    listKeysToStrings.insert(Qt::Key_Context1,tr("Context1"));
    listKeysToStrings.insert(Qt::Key_Context2,tr("Context2"));
    listKeysToStrings.insert(Qt::Key_Context3,tr("Context3"));
    listKeysToStrings.insert(Qt::Key_Context4,tr("Context4"));
    listKeysToStrings.insert(Qt::Key_Flip,tr("Flip"));
    listKeysToStrings.insert(Qt::Key_Hangup,tr("Hangup"));
    listKeysToStrings.insert(Qt::Key_No,tr("No"));
    listKeysToStrings.insert(Qt::Key_Select,tr("Select"));
    listKeysToStrings.insert(Qt::Key_Yes,tr("Yes"));
    listKeysToStrings.insert(Qt::Key_Execute,tr("Execute"));
    listKeysToStrings.insert(Qt::Key_Printer,tr("Printer"));
    listKeysToStrings.insert(Qt::Key_Play,tr("Play"));
    listKeysToStrings.insert(Qt::Key_Sleep,tr("Sleep"));
    listKeysToStrings.insert(Qt::Key_Zoom,tr("Zoom"));
    listKeysToStrings.insert(Qt::Key_Cancel,tr("Cancel"));
}

