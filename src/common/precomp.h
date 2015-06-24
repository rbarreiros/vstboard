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

    #include <QtWidgets/QAction>
    #include <QtWidgets/QAbstractButton>
    #include <QtWidgets/QAbstractItemView>
    #include <QtAlgorithms>
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QButtonGroup>
    #include <QtWidgets/QCheckBox>
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QHeaderView>
    #include <QClipboard>
    #include <QtWidgets/QComboBox>
    #include <QtWidgets/QDialog>
    #include <QDir>
    #include <QDragEnterEvent>
    #include <QDropEvent>
    #include <QEvent>
    #include <QtWidgets/QFileDialog>
    #include <QtWidgets/QFileSystemModel>
    #include <QFocusEvent>
    #include <QFont>
    #include <QFontDatabase>
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
    #include <QLibrary>
    #include <QLibraryInfo>
    #include <QtWidgets/QLineEdit>
    #include <QtWidgets/QListView>
    #include <QtWidgets/QListWidgetItem>
    #include <QtWidgets/QMainWindow>
    #include <QMap>
    #include <qmath.h>
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QMessageBox>
    #include <QMimeData>
    #include <QMultiHash>
    #include <QMutex>
    #include <QtGui/QMouseEvent>
    #include <QObject>
    #include <QtGui/QPainter>
    #include <QtGui/QPen>
    #include <QtWidgets/QPushButton>
    #include <QReadWriteLock>
    #include <QtWidgets/QScrollBar>
    #include <QSemaphore>
    #include <QSettings>
    #include <QSharedData>
    #include <QSharedPointer>
    #include <QSignalMapper>
    #include <QtWidgets//QSpacerItem>
    #include <QtWidgets/QSplitter>
    #include <QStandardItemModel>
    #include <QStringList>
    #include <QtWidgets/QStyledItemDelegate>
    #include <QThread>
    #include <QTime>
    #include <QTimer>
    #include <QtWidgets/QToolButton>
    #include <QTranslator>
    #include <QtWidgets/QTreeWidget>
    #include <QtWidgets/QUndoCommand>
    #include <QtWidgets/QUndoStack>
    #include <QUrl>
    #include <QtCore/QVariant>
    #include <QtWidgets/QVBoxLayout>
    #include <QWaitCondition>
    #include <QtWidgets/QWidget>

    #ifdef SCRIPTENGINE
        #include <QtScript>
    #endif

    #ifdef QT_NO_DEBUG
        #define LOG(...)
    #else
        #include <QDebug>
#define LOG(...); {qDebug()<<__VA_ARGS__<<"("<<__FUNCTION__<<":"<< __LINE__<<" )";}
    #endif

#endif
//#endif // PRECOMP_H
