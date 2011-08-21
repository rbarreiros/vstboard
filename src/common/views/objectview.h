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

#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "../precomp.h"
#include "../globals.h"
#include "../connectables/object.h"
#include "listpinsview.h"
//#include "textbutton.h"
#include "../mainhost.h"
#include "../mainwindow.h"

class MainHost;
namespace View {

    class ConnectablePinView;
    class PinView;
    class ObjectView : public QGraphicsWidget
    {
    Q_OBJECT
    public:
        explicit ObjectView(MainHost *myHost, QAbstractItemModel *model, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
        virtual ~ObjectView();

        virtual void SetModelIndex(QPersistentModelIndex index);
        virtual void UpdateModelIndex();
        void Shrink();

        /// list of audio inputs
        ListPinsView *listAudioIn;

        /// list of audio outputs
        ListPinsView *listAudioOut;

        /// list of midi inputs
        ListPinsView *listMidiIn;

        /// list of midi outputs
        ListPinsView *listMidiOut;

        /// list of parameters pins inputs
        ListPinsView *listParametersIn;

        /// list of parameters pins outputs
        ListPinsView *listParametersOut;

        /// list of bridges pins
        ListPinsView *listBridge;

        ViewConfig *config;

        void SetEditorPin(ConnectablePinView *pin, float value);
        void SetLearnPin(ConnectablePinView *pin, float value);

    protected:
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        void resizeEvent ( QGraphicsSceneResizeEvent * event );
        virtual void closeEvent ( QCloseEvent * event );
        virtual void focusInEvent ( QFocusEvent * event );
        virtual void focusOutEvent ( QFocusEvent * event );
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

        void SetErrorMessage(const QString & msg);
        void UpdateTitle();

        /// the title text
        QGraphicsSimpleTextItem *titleText;

        /// the object border
        QGraphicsRectItem *border;

        /// the selected object border
        QGraphicsRectItem *selectBorder;

        /// error message icon
        QGraphicsPixmapItem *errorMessage;

        /// objects layout
        QGraphicsGridLayout *layout;

        /// main model
        QAbstractItemModel *model;

        /// objects model index
        QPersistentModelIndex objIndex;

        /// the remove action
        QAction *actRemove;
        /// the remove+bridge action
        QAction *actRemoveBridge;
        /// show editor
        QAction *actShowEditor;
        //switch learn mode
        QAction *actLearnSwitch;

        /// true if a shrink is already in progress
        bool shrinkAsked;

        /// pointer to the MainHost
        MainHost *myHost;

        bool highlighted;

        ConnectablePinView *editorPin;
        ConnectablePinView *learnPin;

    private slots:
        void SwitchEditor(bool show);
        void SwitchLearnMode(bool on);

    public slots:
        void ShrinkNow();
        virtual void UpdateColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        virtual void HighlightStart() {}
        virtual void HighlightStop() {}
        void RemoveWithBridge();
        void ToggleEditor();

    friend class PinView;
    };
}

#endif // OBJECTVIEW_H
