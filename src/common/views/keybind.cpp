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
        kName->setEditable(false);
        listItems << kName;
        listItems << new QStandardItem( mapMainShortcuts.value( (MainShortcuts)n.value(i),"") );
        mainModel.invisibleRootItem()->appendRow(listItems);
    }
    return &mainModel;
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
            QStandardItem *kName = new QStandardItem( QString(n.key(j)) );
            kName->setEditable(false);
            listItems << kName;
            listItems << new QStandardItem( moveInput.valueToKey(mb.input) );
            QStandardItem *m = new QStandardItem();
            m->setData(static_cast<int>(mb.buttons),Qt::EditRole);
            listItems << m;

            QStandardItem *mod = new QStandardItem();
            mod->setData(static_cast<int>(mb.modifier),Qt::EditRole);
            listItems << mod;


//            QStandardItem *lmb = new QStandardItem();
//            lmb->setData(QVariant(),Qt::DisplayRole);
//            lmb->setCheckable(true);
//            if(mb.buttons & Qt::LeftButton)
//                lmb->setCheckState(Qt::Checked);
//            listItems << lmb;

//            QStandardItem *mmb = new QStandardItem();
//            mmb->setData(QVariant(),Qt::DisplayRole);
//            mmb->setCheckable(true);
//            if(mb.buttons & Qt::MiddleButton)
//                mmb->setCheckState(Qt::Checked);
//            listItems << mmb;

//            QStandardItem *rmb = new QStandardItem();
//            rmb->setCheckable(true);
//            if(mb.buttons & Qt::RightButton)
//                rmb->setCheckState(Qt::Checked);
//            listItems << rmb;

//            QStandardItem *mb4 = new QStandardItem();
//            mb4->setCheckable(true);
//            if(mb.buttons & Qt::XButton1)
//                mb4->setCheckState(Qt::Checked);
//            listItems << mb4;

//            QStandardItem *mb5 = new QStandardItem();
//            mb5->setCheckable(true);
//            if(mb.buttons & Qt::XButton2)
//                mb5->setCheckState(Qt::Checked);
//            listItems << mb5;

//            QStandardItem *shift = new QStandardItem();
//            shift->setCheckable(true);
//            if(mb.modifier & Qt::ShiftModifier)
//                shift->setCheckState(Qt::Checked);
//            listItems << shift;

//            QStandardItem *ctrl = new QStandardItem();
//            ctrl->setCheckable(true);
//            if(mb.modifier & Qt::ControlModifier)
//                ctrl->setCheckState(Qt::Checked);
//            listItems << ctrl;

//            QStandardItem *alt = new QStandardItem();
//            alt->setCheckable(true);
//            if(mb.modifier & Qt::AltModifier)
//                alt->setCheckState(Qt::Checked);
//            listItems << alt;
            mode->appendRow(listItems);
        }
        modesModel.appendRow(mode);
        ++i;
    }


    return &modesModel;
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
