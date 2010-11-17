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
