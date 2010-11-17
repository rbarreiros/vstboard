//#include <QBuffer>
//#include "imagecollection.h"

//ImageCollection *ImageCollection::theImageCollection=0;

//void ImageCollection::Create(QObject *parent)
//{
//   if(!theImageCollection)
//        theImageCollection = new ImageCollection(parent);
//}

//ImageCollection *ImageCollection::Get()
//{
//    if(!theImageCollection)
//        return 0;

//    return theImageCollection;
//}

//ImageCollection::~ImageCollection()
//{
//    db.close();
//}

//ImageCollection::ImageCollection(QObject *parent) :
//        QObject(parent)
//{
//    if(!openDB())
//        return;
//    createDB();
//}

//bool ImageCollection::openDB()
//{

//    if( !QSqlDatabase::isDriverAvailable("QSQLITE") ) {
//        QMessageBox msgBox;
//        msgBox.setText(tr("sqlite driver not found"));
//        msgBox.exec();
//        return false;
//    }
//    db = QSqlDatabase::addDatabase("QSQLITE");

//#ifdef Q_OS_LINUX
//    // NOTE: We have to store database file into user home folder in Linux
//    QString path(QDir::home().path());
//    path.append(QDir::separator()).append("dbimg.db.sqlite");
//    path = QDir::toNativeSeparators(path);
//    db.setDatabaseName(path);
//#else
//    db.setDatabaseName("dbimg.db.sqlite");
//#endif

//    return db.open();
//}

//bool ImageCollection::createDB()
//{
//    QSqlQuery query;
//    query.exec("create table if not exists images (name varchar(30), bitmap blob)");
//    return true;
//}

//QSqlError ImageCollection::lastError()
//{
//    // If opening database has failed user can ask
//    // error description by QSqlError::text()
//    return db.lastError();
//}

//bool ImageCollection::deleteDB()
//{
//    // Close database
//    db.close();

//#ifdef Q_OS_LINUX
//    // NOTE: We have to store database file into user home folder in Linux
//    QString path(QDir::home().path());
//    path.append(QDir::separator()).append("dbimg.db.sqlite");
//    path = QDir::toNativeSeparators(path);
//    return QFile::remove(path);
//#else

//    // Remove created database binary file
//    return QFile::remove("dbimg.db.sqlite");
//#endif
//}

//bool ImageCollection::ImageExists(const QString &name)
//{
//    QSqlQuery query("select bitmap from images where name like '"+name+"'");
//    while (query.next()) {
//        return true;
//    }
//    return false;
//}

//QPixmap ImageCollection::GetImage(const QString &name)
//{
//    if(cachedImg.contains(name))
//        return cachedImg.value(name);

//    QSqlQuery query("select bitmap from images where name like '"+name+"'");
//    while (query.next()) {
//        QPixmap pix;
//        pix.loadFromData(query.value(0).toByteArray(), "PNG");
//        cachedImg.insert(name,pix);
//        return pix;
//    }
//    return QPixmap();
//}

//void ImageCollection::AddImage(const QString &name, const QPixmap &img)
//{
//    QByteArray bytes;
//    QBuffer buffer(&bytes);
//    buffer.open(QIODevice::WriteOnly);
//    img.save(&buffer, "PNG");

//    if(ImageExists(name)) {
//        debug("ImageCollection::AddImage already exists")
//        return;
//    }

//    QSqlQuery query;
//    query.prepare("insert into images (name,bitmap) values (:name,:img)");
//    query.bindValue(":name", name);
//    query.bindValue(":img", bytes);
//    query.exec();
//}
