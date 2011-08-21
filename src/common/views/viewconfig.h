#ifndef VIEWCONFIG_H
#define VIEWCONFIG_H

//#include "precomp.h"

class MainHost;

namespace ColorGroups {
    enum Enum {
        ND,
        Window,
        Panel,
        Bridge,
        Object,
        VstPlugin,
        AudioPin,
        MidiPin,
        ParameterPin,
        Cursor,
        Programs
    };
}

namespace Colors {
    enum Enum {
        ND,
        Window,
        Base,
        Background,
        HighlightBackground,
        VuMeter,
        WindowText,
        Text,
        Lines,
        Button,
        ButtonText
//            AlternateBase,
//            ToolTipBase,
//            ToolTipText,
//            BrightText,
    };
}

namespace EditMode {
    enum Enum {
        Cable,
        Value
    };
}

#define viewConfigPreset QMap<ColorGroups::Enum, QMap<Colors::Enum, QColor> >
#define viewConfigPresetList QMap<QString, QMap<ColorGroups::Enum, QMap<Colors::Enum, QColor> > >


namespace View {

//    class ColorGroup
//    {
//    public:
//        QMap<Colors::Enum,QColor>listColors;

//    };


    class ObjectView;
    class ViewConfig : public QObject
    {
    Q_OBJECT

    public:
        ViewConfig( MainHost *myHost,  QObject *parent=0 );

        void SetColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        QColor GetColor(ColorGroups::Enum groupId, Colors::Enum colorId);
        QString GetColorGroupName(ColorGroups::Enum groupId);
        QString GetColorName(Colors::Enum colorId);
        QPalette::ColorRole GetPaletteRoleFromColor(Colors::Enum colorId);
        QPalette GetPaletteFromColorGroup(ColorGroups::Enum groupId, const QPalette &oriPalette);
        void SetListGroups(viewConfigPreset newList);

        void SaveToFile( QDataStream & out );
        void LoadFromFile( QDataStream & in );
        void SaveInRegistry();
        void LoadFromRegistry();


        void LoadPreset(const QString &presetName);
        inline const QString & GetPresetName() const {return currentPresetName;}

        inline viewConfigPreset * ViewConfig::GetCurrentPreset()
        {
            return &(*GetListOfPresets())[currentPresetName];
        }

        inline viewConfigPresetList * ViewConfig::GetListOfPresets()
        {
            if(savedInSetupFile)
                return &listPresetsInSetup;
            else
                return &listPresets;
        }

        bool IsSavedInSetup() {return savedInSetupFile;}
        void SetSavedInSetup(bool inSetup) {if(savedInSetupFile==inSetup) return; savedInSetupFile=inSetup; currentPresetName="Default";}

        void AddPreset(QString &presetName);
        void RemovePreset(const QString &presetName);
        void RenamePreset(const QString &oldName, QString &newName);
        void CopyPreset(const QString &presetName, QString &newName=QString(""));

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        static float KeyboardNumber(int key);

        void SetEditMode(const EditMode::Enum mode) {currentEditMode=mode;}
        EditMode::Enum EditMode() {return currentEditMode;}

    protected:
        ///list of presets in registry
        viewConfigPresetList listPresets;

        ///list of presets saved in setup file
        viewConfigPresetList listPresetsInSetup;

        bool savedInSetupFile;
        void InitPresets();
        void AddColor(const QString &preset, ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        void UpdateAllWidgets();

        /// list of groups names
        QMap<ColorGroups::Enum,QString>colorGroupNames;

        /// list of colors names
        QMap<Colors::Enum,QString>colorsNames;

        QString currentPresetName;

        MainHost *myHost;

        EditMode::Enum currentEditMode;

    signals:
        /*!
            emited when a color changed
            \param groupId group Id
            \param colorId color Id
            \param color the new color
        */
        void ColorChanged(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);

        void NewSetupLoaded();
    };

}


#endif // VIEWCONFIG_H
