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

#ifndef VSTSHELLSELECT_H
#define VSTSHELLSELECT_H

#include "precomp.h"
#include "connectables/objectinfo.h"

namespace Ui {
    class VstShellSelect;
}
namespace Connectables {
    class VstPlugin;
}
class ComAddObject;
class MainHost;
namespace View {
    class VstShellSelect : public QWidget {
        Q_OBJECT
    public:
        VstShellSelect(MainHost *myHost, Connectables::VstPlugin *plugin);
        ~VstShellSelect();
        static ComAddObject* command;

    private:
        ObjectInfo info;
        Ui::VstShellSelect *ui;
        MainHost *myHost;

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();
    };
}
#endif // VSTSHELLSELECT_H
