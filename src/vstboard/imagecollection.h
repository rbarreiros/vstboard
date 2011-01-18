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

//#ifndef IMAGECOLLECTION_H
//#define IMAGECOLLECTION_H

//#include "precomp.h"
//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>


//class ImageCollection : public QObject
//{
//    Q_OBJECT
//public:
//    static void Create(QObject *parent=0);
//    static ImageCollection * Get();
//    virtual ~ImageCollection();

//    bool ImageExists(const QString &name);
//    QPixmap GetImage(const QString &name);
//    void AddImage(const QString &name, const QPixmap &img);

//private:
//    bool openDB();
//    bool deleteDB();
//    bool createDB();
//    QSqlError lastError();
//    QSqlDatabase db;
//    QMap<QString,QPixmap>cachedImg;

//    explicit ImageCollection(QObject *parent=0);
//    static ImageCollection *theImageCollection;
//};

//#endif // IMAGECOLLECTION_H
