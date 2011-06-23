#ifndef VIEWCONFIG_H
#define VIEWCONFIG_H

#include "precomp.h"

class MainHost;

namespace View {

    namespace ColorGroups {
        enum Enum {
            ND,
            VstPlugin,
            AudioPin,
            MidiPin,
            ParameterPin,
            Bridge,
            Object,
            Windows,
            Panel
        };
    }

    namespace Colors {
        enum Enum {
            ND,
            Background,
            Borders,
            Text,
            VuMeter,
            Window,
            WindowText,
            Button,
            ButtonText,
            Base,
            AlternateBase,
            ToolTipBase,
            ToolTipText,
            BrightText,
            HighlightBackground
        };
    }

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
        ~ViewConfig();

        void SetColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        QColor GetColor(ColorGroups::Enum groupId, Colors::Enum colorId);
        QString GetColorGroupName(ColorGroups::Enum groupId);
        QString GetColorName(Colors::Enum colorId);
        QPalette::ColorRole GetPaletteRoleFromColor(Colors::Enum colorId);
        QPalette GetPaletteFromColorGroup(ColorGroups::Enum groupId, const QPalette &oriPalette);
        void SetListGroups(QMap<ColorGroups::Enum,ColorGroup> newList);
        void SaveInRegistry(MainHost *myHost);
        void LoadFromRegistry(MainHost *myHost);

        /// list of color groups
        QMap<ColorGroups::Enum,ColorGroup>listColorGroups;

        QDataStream & toStream (QDataStream &) const;
        QDataStream & fromStream (QDataStream &);

        bool savedInSetupFile;

    protected:
        void AddColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        void UpdateAllWidgets();

        /// list of groups names
        QMap<ColorGroups::Enum,QString>colorGroupNames;

        /// list of colors names
        QMap<Colors::Enum,QString>colorsNames;

    signals:
        /*!
            emited when a color changed
            \param groupId group Id
            \param colorId color Id
            \param color the new color
        */
        void ColorChanged(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
    };

}

QDataStream & operator<< (QDataStream & out, const View::ViewConfig& value);
QDataStream & operator>> (QDataStream & in, View::ViewConfig& value);


#endif // VIEWCONFIG_H
