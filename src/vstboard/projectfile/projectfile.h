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

#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "../precomp.h"
//#include "../connectables/connectioninfo.h"


//namespace Connectables {
//    class Object;
//}

namespace Project {
    class Program;
    class ProjectFile : public QObject
    {
    Q_OBJECT
    public:
        explicit ProjectFile(QObject *parent = 0);

      //  Program* GetProgram(int program);

     //   QStandardItemModel programModel;
      //  QStandardItem *ItemFromProgId(int progId);

      //  QMap<int,Program*>listPrograms;

        static ProjectFile *theProjectFile;

    protected:
      //  void BuildModel();
        QString fileName;

    signals:


    public slots:
        void SaveToFile(QString filePath);
        bool LoadFromFile(QString filePath);

    };

}
#endif // PROJECTFILE_H

