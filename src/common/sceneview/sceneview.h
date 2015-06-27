/**************************************************************************
#    Copyright 2010-2011 Rapha�l Fran�ois
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

#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4100 )
#endif

//#include "precomp.h"
#include "connectables/connectioninfo.h"
#include "views/maingraphicsview.h"

namespace Connectables {
    class ObjectFactory;
}
class MainHost;
namespace View {
    class MainContainerView;
    class ViewConfig;
    class SceneView : public QAbstractItemView
    {
        Q_OBJECT
    public:
        explicit SceneView(MainHost *myHost,Connectables::ObjectFactory *objFactory, MainGraphicsView *viewHost, MainGraphicsView *viewProject, MainGraphicsView *viewProgram, MainGraphicsView *viewGroup,QWidget *parent = 0);

        QRect visualRect(const QModelIndex &index) const {
	  Q_UNUSED(index);
	  return QRect();
	}
	
        void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) {
	  Q_UNUSED(index);
	  Q_UNUSED(hint);
	}
	
        QModelIndex indexAt(const QPoint &point) const {
	  Q_UNUSED(point);
	  return QModelIndex();
	}

        void SetParkings(QWidget *progPark, QWidget *groupPark);

        MainGraphicsView *viewHost;
        MainGraphicsView *viewProject;
        MainGraphicsView *viewProgram;
        MainGraphicsView *viewGroup;

    protected:

        //we need parent objects to avoid a bug in qgraphicssene
        QGraphicsRectItem *rootObjHost;
        QGraphicsRectItem *rootObjProject;
        QGraphicsRectItem *rootObjProgram;
        QGraphicsRectItem *rootObjInsert;

        QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) {
	  Q_UNUSED(cursorAction);
	  Q_UNUSED(modifiers);
	  return QModelIndex();
	}
	
        int horizontalOffset() const {return 0;}
        int verticalOffset() const {return 0;}

	bool isIndexHidden(const QModelIndex &index) const {
	  Q_UNUSED(index);
	  return false;
	}
	
        void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) {
	  Q_UNUSED(rect);
	  Q_UNUSED(command);
	}
	
        QRegion visualRegionForSelection(const QItemSelection &selection) const {
	  Q_UNUSED(selection);
	  return QRegion();
	}

        QModelIndex traverseTroughIndexes ( QModelIndex index );

        QHash<QPersistentModelIndex,QObject*>hashItems;
        QMap<ConnectionInfo,QPersistentModelIndex>mapConnectionInfo;

        QGraphicsScene *sceneHost;
        QGraphicsScene *sceneProject;
        QGraphicsScene *sceneProgram;
        QGraphicsScene *sceneGroup;

        MainContainerView *hostContainerView;
        MainContainerView *projectContainerView;
        MainContainerView *programContainerView;
        MainContainerView *groupContainerView;

        QWidget *progParking;
        QWidget *groupParking;

        QTimer *timerFalloff;
        Connectables::ObjectFactory *objFactory;
        MainHost *myHost;

    signals:
//        void hostShown(bool shown);
//        void projectShown(bool shown);
//        void programShown(bool shown);
//        void insertShown(bool shown);

    public slots:
        void ConnectPins(const ConnectionInfo &pinOut, const ConnectionInfo &pinIn);
        void RemoveCablesFromPin(const ConnectionInfo &pin);
        void RemovePin(const ConnectionInfo &pin);
        void ToggleHostView(bool show);
        void ToggleProjectView(bool show);
        void ToggleProgramView(bool show);
        void ToggleInsertView(bool show);

    protected slots:
        void dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight  );
        void rowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end  );
        void rowsInserted ( const QModelIndex & parent, int start, int end  );
        void graphicObjectRemoved ( QObject* obj);
    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // SCENEVIEW_H
