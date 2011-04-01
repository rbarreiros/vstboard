#include "precomp.h"
#include "viewconfig.h"
#include "objectview.h"

using namespace View;

/*!
  \class ViewConfig
  \brief manage the view configuration
  */

/*!
  Constructor
  */
ViewConfig::ViewConfig(QObject *parent) :
    QObject(parent)
{
    colorGroupNames.insert( ColorGroups::ND, tr("-undefined-") );
    colorGroupNames.insert( ColorGroups::VstPlugin, tr("Vst Plugin") );
    colorGroupNames.insert( ColorGroups::AudioPin, tr("Audio Pin") );
    colorGroupNames.insert( ColorGroups::MidiPin, tr("Midi Pin") );
    colorGroupNames.insert( ColorGroups::ParameterPin, tr("Parameter Pin") );
    colorGroupNames.insert( ColorGroups::Bridge, tr("Bridge") );
    colorGroupNames.insert( ColorGroups::Object, tr("Object") );

    colorsNames.insert( Colors::ND, tr("-undefined-") );
    colorsNames.insert( Colors::Background, tr("Background") );
    colorsNames.insert( Colors::Borders, tr("Borders") );
    colorsNames.insert( Colors::Text, tr("Text") );
    colorsNames.insert( Colors::VuMeter, tr("Vu-meter") );


    AddColor(ColorGroups::VstPlugin,Colors::Background,QColor(255,255,128,128));
    AddColor(ColorGroups::VstPlugin,Colors::Text,QColor(0,0,0,255));

    AddColor(ColorGroups::AudioPin,Colors::Background,QColor(255,128,128,128));
    AddColor(ColorGroups::AudioPin,Colors::VuMeter,QColor(128,128,128,128));

    AddColor(ColorGroups::MidiPin,Colors::Background,QColor(128,255,128,128));
    AddColor(ColorGroups::MidiPin,Colors::VuMeter,QColor(128,128,128,128));

    AddColor(ColorGroups::ParameterPin,Colors::Background,QColor(128,128,255,128));
    AddColor(ColorGroups::ParameterPin,Colors::VuMeter,QColor(128,128,128,128));

    AddColor(ColorGroups::Bridge,Colors::Background,QColor(0,0,0,0));
    AddColor(ColorGroups::Bridge,Colors::Borders,QColor(0,0,0,255));

    AddColor(ColorGroups::Object,Colors::Background,QColor(128,255,255,128));
    AddColor(ColorGroups::Object,Colors::Borders,QColor(0,0,0,255));
    AddColor(ColorGroups::Object,Colors::Text,QColor(0,0,0,255));


    QPalette *pal = new QPalette( qApp->palette() );
    pal->setColor(QPalette::Window, QColor(10,60,10,100));
    AddPalette(Palettes::Object, QObject::tr("Default objects"), pal);

    pal = new QPalette( qApp->palette() );
    pal->setColor(QPalette::Window, QColor(10,10,60,100));
    AddPalette(Palettes::VstPlugin, QObject::tr("VST plugins"), pal);

    pal = new QPalette( qApp->palette() );
    pal->setColor(QPalette::Window, QColor(0,0,0,0));
    AddPalette(Palettes::Bridge, QObject::tr("Bridges"), pal);
}

/*!
  Destructor
  */
ViewConfig::~ViewConfig()
{
//    QMap<Palettes::Enum,QPalette*>::iterator i = listPalettes.end();
//    while(i!=listPalettes.begin()) {
//        delete i.value();
//        --i;
//    }
}

/*!
  Get a palette by id
  \param pal palette id
  \return pointer to the QPalette
  */
QPalette * ViewConfig::GetPalette(Palettes::Enum palId)
{
    if(!listPalettes.contains(palId)) {
        debug2(<<"ViewConfig::GetPalette"<<palId<<"not found")
        return &qApp->palette();
    }

    return listPalettes.value(palId);
}

/*!
  Get the palette name
  \param pal palette id
  \return the palette name
  */
QString ViewConfig::GetPaletteName(Palettes::Enum palId) const
{
    if(!listPalettes.contains(palId)) {
        debug2(<<"ViewConfig::GetPaletteName"<<palId<<"not found")
        return "ND";
    }
    return paletteNames.value(palId);
}

/*!
  Update a color, notify registered objects
  \param pal palette id
  \param role the role
  \param color the color
  */
void ViewConfig::SetPaletteColor(Palettes::Enum palId, QPalette::ColorRole role, const QColor &color)
{
    listPalettes.value(palId)->setColor(role, color);
    emit PaletteChanged(palId);
}

/*!
  Add a palette to the list
  \param pal palette id
  \param name palette name
  \param palette the palette
  */
void ViewConfig::AddPalette(Palettes::Enum palId, const QString &name, QPalette * palette)
{
    listPalettes.insert( palId, palette );
    paletteNames.insert( palId, name );
}

QString ViewConfig::GetColorGroupName(ColorGroups::Enum groupId)
{
    if(!colorGroupNames.contains(groupId))
        return "-unknown-";
    return colorGroupNames.value(groupId);
}

QString ViewConfig::GetColorName(Colors::Enum colorId)
{
    if(!colorsNames.contains(colorId))
        return "-unknown-";
    return colorsNames.value(colorId);
}

void ViewConfig::AddColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    ColorGroup *grp = 0;
    if(!listColorGroups.contains(groupId)) {
        grp = new ColorGroup();
        listColorGroups.insert(groupId, grp );
    } else {
        grp = listColorGroups.value(groupId);
    }

    grp->listColors.insert(colorId,color);
    emit ColorChanged(groupId,colorId,color);
}


void ViewConfig::SetColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if( !listColorGroups.contains(groupId) || !listColorGroups.value(groupId)->listColors.contains(colorId) )
        return;

    listColorGroups.value(groupId)->listColors.insert(colorId,color);
    emit ColorChanged(groupId,colorId,color);
}

QColor ViewConfig::GetColor(ColorGroups::Enum groupId, Colors::Enum colorId)
{
    ColorGroup *grp = listColorGroups.value(groupId);
    if(!grp)
        return QColor();

    return grp->listColors.value(colorId);
}
