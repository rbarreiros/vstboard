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

#ifndef PRECOMP_H
#define PRECOMP_H

#if defined __cplusplus

#define QT_USE_FAST_CONCATENATION
#define QT_USE_FAST_OPERATOR_PLUS

#define VST_FORCE_DEPRECATED 0

#ifdef WIN32
    #define NOMINMAX
    #include "Windows.h"
#endif

#include <string>
#include <math.h>
#include <algorithm>

#include <QAbstractItemView>
#include <QtAlgorithms>
#include <QtGui/QApplication>
#include <QDialog>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QFocusEvent>
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
#include <QLibrary>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QMultiHash>
#include <QMutex>
#include <QObject>
#include <QScrollBar>
#include <QSettings>
#include <QSharedData>
#include <QSharedPointer>
#include <QStandardItemModel>
#include <QStringList>
#include <QStyledItemDelegate>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QTreeWidget>
#include <QUrl>
#include <QWidget>

#ifdef QT_NO_DEBUG
    #define debug(...)
#else
    #define debug(...) {qDebug(__VA_ARGS__);}
#endif

//#include "porttime.h"
//#include "portmidi.h"
//#include "pmutil.h"
//#include "portaudio.h"

#endif
#endif // PRECOMP_H
