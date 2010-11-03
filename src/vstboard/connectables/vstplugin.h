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
        VstPlugin(int index, const ObjectInfo & info);
        ~VstPlugin();
        bool Open();
        bool Close();
        void Render();
        void Hide();
        int GetProcessingTime() {return 20;}
        long OnGetUniqueId() { return index; }
        long OnMasterCallback(long opcode, long index, long value, void *ptr, float opt, long currentReturnCode);
        void UpdateDisplay();
        void SetSleep(bool sleeping);
        void MidiMsgFromInput(long msg);
        QString GetParameterName(ConnectionInfo pinInfo);
        inline AEffect* GetPlugin() {return pEffect;}

        View::VstPluginWindow *editorWnd;

        static QList<VstPlugin*>listPlugins;
        static VstPlugin *pluginLoading;

        bool OpenEditor();
        void SetParentModelNode(QStandardItem* parent);
        void UpdateModelNode();
        void UpdateEditorNode();
        bool GetEditorVisible();

        static View::VstShellSelect *shellSelectView;

    protected:
        float sampleRate;
        long bufferSize;
        VstEvents *listEvnts;
        bool isShell;

        QMutex midiEventsMutex;
        QList<VstMidiEvent*>listVstMidiEvents;



    signals:
        void WindowSizeChange(int newWidth, int newHeight);

    public slots:
        void SetBufferSize(long size);
        void SetSampleRate(float rate=44100.0);
//        void ToggleEditor();
        void RaiseEditor();
        void EditorDestroyed();
        void EditIdle();
        void OnParameterChanged(ConnectionInfo pinInfo, float value);
        void CloseEditor();

        friend class View::VstPluginWindow;
    };

}

#endif // VSTPLUGIN_H
