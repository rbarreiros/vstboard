#ifndef VIEWCONFIG_H
#define VIEWCONFIG_H

#include "precomp.h"

namespace View {

    namespace Palettes {
        enum Enum {
            Object,
            VstPlugin,
            Bridge
        };
    }

    namespace ColorGroups {
        enum Enum {
            ND,
            VstPlugin,
            AudioPin,
            MidiPin,
            ParameterPin,
            Bridge,
            Object
        };
    }

    namespace Colors {
        enum Enum {
            ND,
            Background,
            Borders,
            Text,
            VuMeter
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
        QPalette * GetPalette(Palettes::Enum palId);
        int PalettesCount() {return listPalettes.size();}
        QString GetPaletteName(Palettes::Enum palId) const;
        void SetPaletteColor(Palettes::Enum palId, QPalette::ColorRole role, const QColor &color);

        void SetColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        QColor GetColor(ColorGroups::Enum groupId, Colors::Enum colorId);
        QString GetColorGroupName(ColorGroups::Enum groupId);
        QString GetColorName(Colors::Enum colorId);
        QMap<ColorGroups::Enum,ColorGroup*>listColorGroups;

    protected:
        void AddColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
        void AddPalette(Palettes::Enum palId, const QString &name, QPalette * palette);

        ///list of palettes
        QMap<Palettes::Enum,QPalette*>listPalettes;

        ///list of palettes names
        QMap<Palettes::Enum,QString>paletteNames;

        QMap<ColorGroups::Enum,QString>colorGroupNames;
        QMap<Colors::Enum,QString>colorsNames;


    signals:
        void PaletteChanged(Palettes::Enum palId);
        void ColorChanged(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color);
    };

}
#endif // VIEWCONFIG_H
