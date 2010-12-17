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

#ifndef VSTPLUGIN_H
#define VSTPLUGIN_H

#include "object.h"
#include "../vst/ceffect.h"
#include "../views/vstshellselect.h"

namespace View {
    class VstPluginWindow;
}


namespace Connectables {

    class VstPlugin : public Object , public vst::CEffect
    {
    Q_OBJECT

    public:
        VstPlugin(MainHost *myHost,int index, const ObjectInfo & info);
        ~VstPlugin();
        bool Open();
        bool Close();
        void Render();
        void SetParkingIndex(const QModelIndex &parentIndex);
        int GetProcessingTime() {return 20;}
        long OnGetUniqueId() { return index; }
        long OnMasterCallback(long opcode, long index, long value, void *ptr, float opt, long currentReturnCode);
        void SetSleep(bool sleeping);
        void MidiMsgFromInput(long msg);
        QString GetParameterName(ConnectionInfo pinInfo);
        inline AEffect* GetPlugin() {return pEffect;}

        View::VstPluginWindow *editorWnd;

//        static QList<VstPlugin*>listPlugins;
        static QMap<AEffect*,VstPlugin*>mapPlugins;
        static VstPlugin *pluginLoading;

        void CreateEditorWindow();

        void SetParentModeIndex(const QModelIndex &parentIndex);
//        void UpdateModelNode();
//        void UpdateEditorNode();

        static View::VstShellSelect *shellSelectView;

        void SetContainerAttribs(const ObjectContainerAttribs &attr);
        void GetContainerAttribs(ObjectContainerAttribs &attr);
        Pin* CreatePin(const ConnectionInfo &info, quint16 nb);

    protected:
        float sampleRate;
        unsigned long bufferSize;
        VstEvents *listEvnts;
        bool isShell;

        QMutex midiEventsMutex;
        QList<VstMidiEvent*>listVstMidiEvents;
        QList<QVariant>listValues;

    signals:
        void WindowSizeChange(int newWidth, int newHeight);

    public slots:
        void SetBufferSize(unsigned long size);
        void SetSampleRate(float rate=44100.0);
        void RaiseEditor();
        void EditorDestroyed();
        void EditIdle();
        void OnParameterChanged(ConnectionInfo pinInfo, float value);
        void OnEditorVisibilityChanged(bool visible);

//        void TakeScreenshot();

        friend class View::VstPluginWindow;
    };

}

#endif // VSTPLUGIN_H
