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

#ifndef VSTPLUGINWINDOW_H
#define VSTPLUGINWINDOW_H

#include "../precomp.h"

namespace Ui {
    class VstPluginWindow;
}

namespace Connectables {
    class VstPlugin;
}

namespace View {


    class VstPluginWindow : public QFrame {
        Q_OBJECT
    public:
        VstPluginWindow(QWidget *parent = 0);
        ~VstPluginWindow();

        bool SetPlugin(Connectables::VstPlugin *plugin);
        WId GetWinId();
//        const QPixmap GetScreenshot();

        void LoadAttribs();
        void SaveAttribs();

    protected:
        void resizeEvent ( QResizeEvent * event );
        void showEvent ( QShowEvent * event );
        void closeEvent ( QCloseEvent * event );
        void hideEvent ( QHideEvent * event );
        Connectables::VstPlugin *plugin;

        QModelIndex modelIndex;

    private:
        Ui::VstPluginWindow *ui;

    signals:
        void Hide();

    public slots:
        void SetWindowSize(int newWidth, int newHeight);

    };
}
#endif // VSTPLUGINWINDOW_H
