/******************************************************************************
#    Copyright 2010 Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "precomp.h"

#include <QLibraryInfo>
#include <QTranslator>

#include "mainwindow.h"
#include "mainconfig.h"
#include "shortcutkeys.h"
#include "pathsolver.h"
#include "connectables/objectinfo.h"

#ifndef QT_NO_DEBUG

#include "debugmemreport.h"


    void myMessageOutput(QtMsgType type, const char *msg)
     {
        qInstallMsgHandler(0);
        qDebug(msg);
        qInstallMsgHandler(myMessageOutput);

         switch (type) {

         case QtWarningMsg:
#ifdef _MSC_VER
                __asm int 3
#else
                __asm("int3");
#endif
            break;

         case QtCriticalMsg:
         case QtFatalMsg:
#ifdef _MSC_VER
                __asm int 3
#else
                __asm("int3");
#endif
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
    qRegisterMetaType<ShortcutKeys>("ShortcutKeys");
    qRegisterMetaType<int>("ObjType::Enum");
    qRegisterMetaType<orderedNodes>("orderedNodes");
    qRegisterMetaType<QVariant>("QVariant");

    qRegisterMetaTypeStreamOperators<ObjectInfo>("ObjectInfo");

#ifndef QT_NO_DEBUG
    qInstallMsgHandler(myMessageOutput);
#endif

    {
        QCoreApplication::setOrganizationName("CtrlBrk");
//        QCoreApplication::setOrganizationDomain("vstboard.ctrlbrk.com.fake");
        QCoreApplication::setApplicationName("VstBoard");

        QApplication app(argc, argv);

        QTranslator qtTranslator;
        qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);

        QTranslator myappTranslator;
        myappTranslator.load("vstboard_" + QLocale::system().name());
        app.installTranslator(&myappTranslator);

        MainConfig::Get(&app);
        MainWindow *w = MainWindow::Get();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
        app.exec();
    }
/*
#ifdef WIN32
#ifndef QT_NO_DEBUG
    fdumpLeaks();
#endif
#endif
*/
    return 0;
}
