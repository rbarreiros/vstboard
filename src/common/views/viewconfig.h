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

namespace View {

    class ColorGroup
    {
    public:
        QMap<Colors::Enum,QColor>listColors;
    };


    class ObjectView;
    class ViewConfig : public QObject
    {
    Q_OBJECT

    public:
        ViewConfig( QObject *parent=0 );

        void SetColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        QColor GetColor(ColorGroups::Enum groupId, Colors::Enum colorId);
        QString GetColorGroupName(ColorGroups::Enum groupId);
        QString GetColorName(Colors::Enum colorId);
        QPalette::ColorRole GetPaletteRoleFromColor(Colors::Enum colorId);
        QPalette GetPaletteFromColorGroup(ColorGroups::Enum groupId, const QPalette &oriPalette);
        void SetListGroups(QMap<ColorGroups::Enum,ColorGroup> newList);
        void SaveInRegistry(MainHost *myHost);
        void LoadFromRegistry(MainHost *myHost);


        void LoadPreset(const QString &presetName);
        inline const QString & GetPresetName() const {return currentPresetName;}

        inline QMap<ColorGroups::Enum,ColorGroup> * ViewConfig::GetCurrentPreset()
        {
            return &(*GetListOfPresets())[currentPresetName];
        }

        inline QMap<QString, QMap<ColorGroups::Enum,ColorGroup> > * ViewConfig::GetListOfPresets()
        {
            if(savedInSetupFile)
                return &listPresetsInSetup;
            else
                return &listPresets;
        }

        bool IsSavedInSetup() {return savedInSetupFile;}
        void SetSavedInSetup(bool inSetup) {savedInSetupFile=inSetup; }

        void AddPreset(QString &presetName);
        void RemovePreset(const QString &presetName);
        void RenamePreset(const QString &oldName, const QString &newName);
        void CopyPreset(QString &presetName);

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        static float KeyboardNumber(int key);

    protected:
        ///list of presets in registry
        QMap<QString, QMap<ColorGroups::Enum,ColorGroup> >listPresets;

        ///list of presets saved in setup file
        QMap<QString, QMap<ColorGroups::Enum,ColorGroup> >listPresetsInSetup;

        bool savedInSetupFile;
        void InitPresets();
        void AddColor(const QString &preset, ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        void UpdateAllWidgets();

        /// list of groups names
        QMap<ColorGroups::Enum,QString>colorGroupNames;

        /// list of colors names
        QMap<Colors::Enum,QString>colorsNames;

        QString currentPresetName;

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

QDataStream & operator<< (QDataStream & out, const View::ViewConfig& value);
QDataStream & operator>> (QDataStream & in, View::ViewConfig& value);


#endif // VIEWCONFIG_H
