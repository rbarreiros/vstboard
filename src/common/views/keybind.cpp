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

#include "keybind.h"
#include "precomp.h"

KeyBind::KeyBind(QObject *parent) :
    QObject(parent),
    currentMode("Cable")
{
    mapMainShortcuts.insert(openProject,         "Ctrl+O");
    mapMainShortcuts.insert(saveProject,         "Ctrl+S");
    mapMainShortcuts.insert(saveProjectAs,       "Ctrl+Shift+S");
    mapMainShortcuts.insert(newProject,          "Ctrl+N");
    mapMainShortcuts.insert(openSetup,           "Alt+O");
    mapMainShortcuts.insert(saveSetup,           "Alt+S");
    mapMainShortcuts.insert(saveSetupAs,         "Alt+Shift+S");
    mapMainShortcuts.insert(newSetup,            "Alt+N");
    mapMainShortcuts.insert(hostPanel,           "Ctrl+H");
    mapMainShortcuts.insert(projectPanel,        "Ctrl+R");
    mapMainShortcuts.insert(programPanel,        "Ctrl+P");
    mapMainShortcuts.insert(groupPanel,          "Ctrl+G");
    mapMainShortcuts.insert(defaultLayout,       "");
    mapMainShortcuts.insert(toolBar,             "Ctrl+Alt+O");
    mapMainShortcuts.insert(tools,               "Ctrl+Alt+T");
    mapMainShortcuts.insert(vstPlugins,          "Ctrl+Alt+V");
    mapMainShortcuts.insert(browser,             "Ctrl+Alt+B");
    mapMainShortcuts.insert(programs,            "Ctrl+Alt+P");
    mapMainShortcuts.insert(midiDevices,         "Ctrl+Alt+M");
    mapMainShortcuts.insert(audioDevices,        "Ctrl+Alt+A");
    mapMainShortcuts.insert(solverModel,         "Ctrl+Alt+S");
    mapMainShortcuts.insert(hostModel,           "Ctrl+Alt+H");
    mapMainShortcuts.insert(undoHistory,         "Ctrl+Alt+U");
    mapMainShortcuts.insert(configuration,       "Ctrl+Alt+C");
    mapMainShortcuts.insert(appearence,          "Ctrl+Alt+E");
    mapMainShortcuts.insert(undo,                "Ctrl+Z");
    mapMainShortcuts.insert(redo,                "Ctrl+Y");
    mapMainShortcuts.insert(refreashAudioDevices,"Ctrl+F5");
    mapMainShortcuts.insert(refreashMidiDevices, "Alt+F5");

    {
        QMap<MovesBindings, MoveBind>mapMv;
        {
            MoveBind b={mouseWheel, Qt::NoButton, Qt::NoModifier};
            mapMv.insert( zoom, b );
        }

        {
            MoveBind b={none, Qt::MiddleButton, Qt::NoModifier};
            mapMv.insert( zoomReset, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( moveView, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( moveObject, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( createCable, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::ControlModifier};
            mapMv.insert( changeValue, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( changeCursorValue, b );
        }
        mapModes.insert("Cable",mapMv);
    }

    {
        QMap<MovesBindings, MoveBind>mapMv;
        {
            MoveBind b={mouseWheel, Qt::NoButton, Qt::NoModifier};
            mapMv.insert( zoom, b );
        }

        {
            MoveBind b={none, Qt::MiddleButton, Qt::NoModifier};
            mapMv.insert( zoomReset, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( moveView, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( moveObject, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( createCable, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::ControlModifier};
            mapMv.insert( changeValue, b );
        }

        {
            MoveBind b={mouse, Qt::LeftButton, Qt::NoModifier};
            mapMv.insert( changeCursorValue, b );
        }
        mapModes.insert("Value",mapMv);
    }
}

QStandardItemModel * KeyBind::GetMainBindingModel()
{
    mainModel.clear();
    mainModel.setColumnCount(2);
    QMetaEnum n = metaObject()->enumerator( metaObject()->indexOfEnumerator("MainShortcuts") );
    for(int i=0; i<n.keyCount(); ++i) {
        QList<QStandardItem*>listItems;
        QStandardItem *kName = new QStandardItem( QString(n.key(i)) );
        kName->setData(n.value(i));
        kName->setEditable(false);
        listItems << kName;
        listItems << new QStandardItem( mapMainShortcuts.value( (MainShortcuts)n.value(i),"") );
        mainModel.invisibleRootItem()->appendRow(listItems);
    }
    return &mainModel;
}

void KeyBind::SetMainBindingModel(QStandardItemModel *model)
{
    mapMainShortcuts.clear();
    for(int i=0; i<model->rowCount(); ++i) {
        mapMainShortcuts.insert( (MainShortcuts)model->item(i,0)->data().toInt(), QKeySequence( model->item(i,1)->text() ).toString() );
    }
    emit BindingChanged();
}

QStandardItemModel * KeyBind::GetModesModel()
{
    modesModel.clear();
    QStringList titles;
    titles << tr("Function")
           << tr("Controller")
           << tr("Buttons")
           << tr("Modifiers");
    modesModel.setHorizontalHeaderLabels(titles);

    QMetaEnum n = metaObject()->enumerator( metaObject()->indexOfEnumerator("MovesBindings") );
    QMetaEnum moveInput =  metaObject()->enumerator( metaObject()->indexOfEnumerator("MoveInputs") );

    QMap<QString, QMap<MovesBindings, MoveBind> >::const_iterator i = mapModes.constBegin();
    while(i!=mapModes.constEnd()) {
        QStandardItem *mode = new QStandardItem(i.key());

        for(int j=0; j<n.keyCount(); ++j) {
            MoveBind mb = i.value().value((MovesBindings)n.value(j));
            QList<QStandardItem*>listItems;

            QStandardItem *keyItem = new QStandardItem( QString(n.key(j)) );
            keyItem->setData(static_cast<int>(n.value(j)));
            keyItem->setEditable(false);
            listItems << keyItem;

            QStandardItem *moveItem = new QStandardItem( moveInput.valueToKey(mb.input) );
            moveItem->setData(static_cast<int>(mb.input));
            listItems << moveItem;

            QStandardItem *buttonsItem = new QStandardItem();
            buttonsItem->setData(static_cast<int>(mb.buttons),Qt::EditRole);
            listItems << buttonsItem;

            QStandardItem *modifiersItem = new QStandardItem();
            modifiersItem->setData(static_cast<int>(mb.modifier),Qt::EditRole);
            listItems << modifiersItem;

            mode->appendRow(listItems);
        }
        modesModel.appendRow(mode);
        ++i;
    }
    return &modesModel;
}

void KeyBind::SetModesModel(QStandardItemModel *model)
{
    mapModes.clear();
    for(int mode=0; mode<model->rowCount(); ++mode) {
        QStandardItem *modeItem = model->item(mode);
        QMap<MovesBindings, MoveBind>mapMove;
        for(int move=0; move<modeItem->rowCount(); ++move) {
            MoveBind bind;

            int mv = modeItem->child(move,0)->data().toInt();

            int in = modeItem->child(move,1)->data().toInt();
            bind.input = static_cast<MoveInputs>(in);

            bind.buttons=0;
            int bt = modeItem->child(move,2)->data(Qt::EditRole).toInt();
            if(bt & Qt::LeftButton) bind.buttons |= Qt::LeftButton;
            if(bt & Qt::MiddleButton) bind.buttons |= Qt::MiddleButton;
            if(bt & Qt::RightButton) bind.buttons |= Qt::RightButton;
            if(bt & Qt::XButton1) bind.buttons |= Qt::XButton1;
            if(bt & Qt::XButton2) bind.buttons |= Qt::XButton2;

            bind.modifier=0;
            int mod = modeItem->child(move,3)->data(Qt::EditRole).toInt();
            if(mod & Qt::ShiftModifier) bind.modifier |= Qt::ShiftModifier;
            if(mod & Qt::ControlModifier) bind.modifier |= Qt::ControlModifier;
            if(mod & Qt::AltModifier) bind.modifier |= Qt::AltModifier;

            mapMove.insert( static_cast<MovesBindings>(mv), bind );
        }
        mapModes.insert(modeItem->text(),mapMove);
    }
    emit ModeChanged();
}

const QString KeyBind::GetMainShortcut(const MainShortcuts id) const
{
    return mapMainShortcuts.value(id,"");
}

const KeyBind::MoveBind KeyBind::GetMoveSortcuts(const MovesBindings id) const
{
    if(!mapModes.contains(currentMode)) {
        LOG("current mode not found");
        return MoveBind();
    }
    if(!mapModes.value(currentMode).contains(id)) {
        LOG("move id not found");
        return MoveBind();
    }
    return mapModes.value(currentMode).value(id);
}
