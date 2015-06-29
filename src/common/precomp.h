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
//#ifndef PRECOMP_H
//#define PRECOMP_H

#if defined __cplusplus

    #define QT_USE_FAST_CONCATENATION
    #define QT_USE_FAST_OPERATOR_PLUS

    #define VST_FORCE_DEPRECATED 0

    #ifdef WIN32
        #undef NOMINMAX
        #define NOMINMAX
        #include "Windows.h"
    #endif

    #include <string>
    #include <cmath>
    #include <algorithm>

    #ifdef __APPLE__
    #include <QAction>
    #include <QAbstractButton>
    #include <QAbstractItemView>
    #include <QApplication>
    #include <QButtonGroup>
    #include <QCheckBox>
    #include <QHBoxLayout>
    #include <QHeaderView>
    #include <QComboBox>
    #include <QDialog>
    #include <QFileDialog>
    #include <QFileSystemModel>
    #include <QFrame>
    #include <QGraphicsBlurEffect>
    #include <QGraphicsGridLayout>
    #include <QGraphicsLinearLayout>
    #include <QGraphicsLineItem>
    #include <QGraphicsRectItem>
    #include <QGraphicsScene>
    #include <QGraphicsSceneDragDropEvent>
    #include <QGraphicsSceneMouseEvent>
    #include <QGraphicsTextItem>
    #include <QGraphicsView>
    #include <QGraphicsWidget>
    #include <QHBoxLayout>
    #include <QLabel>
    #include <QLineEdit>
    #include <QListView>
    #include <QListWidgetItem>
    #include <QMainWindow>
    #include <QMenu>
    #include <QMessageBox>
    #include <QPushButton>
    #include <QScrollBar>
    #include <QSpacerItem>
    #include <QSplitter>
    #include <QStyledItemDelegate>
    #include <QToolButton>
    #include <QTreeWidget>
    #include <QUndoCommand>
    #include <QUndoStack>
    #include <QVBoxLayout>
    #include <QWidget>
    #else
    #include <QtWidgets/Action>
    #include <QtWidgets/QAbstractButton>
    #include <QtWidgets/QAbstractItemView>
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QButtonGroup>
    #include <QtWidgets/QCheckBox>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QHeaderView>
    #include <QtWidgets/QComboBox>
    #include <QtWidgets/QDialog>
    #include <QtWidgets/QFileDialog>
    #include <QtWidgets/QFileSystemModel>
    #include <QtWidgets/QFrame>
    #include <QtWidgets/QGraphicsBlurEffect>
    #include <QtWidgets/QGraphicsGridLayout>
    #include <QtWidgets/QGraphicsLinearLayout>
    #include <QtWidgets/QGraphicsLineItem>
    #include <QtWidgets/QGraphicsRectItem>
    #include <QtWidgets/QGraphicsScene>
    #include <QtWidgets/QGraphicsSceneDragDropEvent>
    #include <QtWidgets/QGraphicsSceneMouseEvent>
    #include <QtWidgets/QGraphicsTextItem>
    #include <QtWidgets/QGraphicsView>
    #include <QtWidgets/QGraphicsWidget>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QLineEdit>
    #include <QtWidgets/QListView>
    #include <QtWidgets/QListWidgetItem>
    #include <QtWidgets/QMainWindow>
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QMessageBox>
    #include <QtWidgets/QPushButton>
    #include <QtWidgets/QScrollBar>
    #include <QtWidgets//QSpacerItem>
    #include <QtWidgets/QSplitter>
    #include <QtWidgets/QStyledItemDelegate>
    #include <QtWidgets/QToolButton>
    #include <QtWidgets/QTreeWidget>
    #include <QtWidgets/QUndoCommand>
    #include <QtWidgets/QUndoStack>
    #include <QtWidgets/QVBoxLayout>
    #include <QtWidgets/QWidget>
    #endif
    #include <QtAlgorithms>
    #include <QClipboard>
    #include <QDir>
    #include <QDragEnterEvent>
    #include <QDropEvent>
    #include <QEvent>
    #include <QFocusEvent>
    #include <QFont>
    #include <QFontDatabase>
    #include <QLibrary>
    #include <QLibraryInfo>
    #include <QMap>
    #include <qmath.h>
    #include <QMimeData>
    #include <QMultiHash>
    #include <QMutex>
    #include <QtGui/QMouseEvent>
    #include <QObject>
    #include <QtGui/QPainter>
    #include <QtGui/QPen>
    #include <QReadWriteLock>
    #include <QSemaphore>
    #include <QSettings>
    #include <QSharedData>
    #include <QSharedPointer>
    #include <QSignalMapper>
    #include <QStandardItemModel>
    #include <QStringList>
    #include <QThread>
    #include <QTime>
    #include <QTimer>
    #include <QTranslator>
    #include <QUrl>
    #include <QtCore/QVariant>
    #include <QWaitCondition>

    #ifdef SCRIPTENGINE
        #include <QtScript>
    #endif

    #ifdef QT_NO_DEBUG
        #define LOG(...);
    #else
        #include <QDebug>
        #define LOG(...); {qDebug()<<__VA_ARGS__<<"("<<__FUNCTION__<<":"<< __LINE__<<" )";}
    #endif

#endif
//#endif // PRECOMP_H
