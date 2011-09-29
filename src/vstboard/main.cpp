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
//#ifndef QT_NO_DEBUG
//    #include "vld.h"
//#endif
#include <QLibraryInfo>
#include <QTranslator>

#include "mainhosthost.h"
#include "mainwindowhost.h"
#include "connectables/objectinfo.h"

#ifndef QT_NO_DEBUG

    void myMessageOutput(QtMsgType type, const char *msg)
     {
        qInstallMsgHandler(0);
        qDebug(msg);
        qInstallMsgHandler(myMessageOutput);

         switch (type) {

         case QtWarningMsg:
             break;
         case QtCriticalMsg:
         case QtFatalMsg:

             abort();
             break;

         case QtDebugMsg:
             break;
         }
     }
#endif

int main(int argc, char *argv[])
{
    qRegisterMetaType<ConnectionInfo>("ConnectionInfo");
    qRegisterMetaType<ObjectInfo>("ObjectInfo");
    qRegisterMetaType<int>("ObjType::Enum");
    qRegisterMetaType<QVariant>("QVariant");
    qRegisterMetaType<AudioBuffer*>("AudioBuffer*");

    qRegisterMetaTypeStreamOperators<ObjectInfo>("ObjectInfo");

#ifndef QT_NO_DEBUG
    qInstallMsgHandler(myMessageOutput);
#endif

    QCoreApplication::setOrganizationName("CtrlBrk");
    QCoreApplication::setApplicationName("VstBoard");

    QApplication app(argc, argv);

#ifdef QT_NO_DEBUG
    QTranslator qtTranslator;
    if(qtTranslator.load("qt_" + QLocale::system().name(), ":/translations/"))
        app.installTranslator(&qtTranslator);

    QTranslator commonTranslator;
    if(commonTranslator.load("common_" + QLocale::system().name(), ":/translations/"))
        app.installTranslator(&commonTranslator);

    QTranslator myappTranslator;
    if(myappTranslator.load("vstboard_" + QLocale::system().name(), ":/translations/"))
        app.installTranslator(&myappTranslator);
#endif

    MainHostHost host;
    MainWindowHost w(&host);
    host.Open();
    w.readSettings();
    w.show();
    w.LoadDefaultFiles();

    app.exec();
    return 0;
}
