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

#ifndef VSTSHELLSELECT_H
#define VSTSHELLSELECT_H

#include "../precomp.h"


namespace Ui {
    class VstShellSelect;
}

namespace View {
    class VstShellSelect : public QWidget {
        Q_OBJECT
    public:
        VstShellSelect();
        ~VstShellSelect();

        void SetListPlugins(QString file, QMap<ulong,QString> &listPlugins);

    protected:
        void changeEvent(QEvent *e);
        QString vstDll;
        int containerId;
    private:
        Ui::VstShellSelect *ui;

private slots:
//    void on_listPlugins_doubleClicked(QModelIndex index);
    void on_buttonCancel_clicked();
    void on_buttonOk_clicked();
};
}
#endif // VSTSHELLSELECT_H
