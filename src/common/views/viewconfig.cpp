#include "precomp.h"
#include "viewconfig.h"
#include "objectview.h"
#include "mainhost.h"

using namespace View;

/*!
  \class View::ViewConfig
  \brief manage the application appearance
  */

/*!
  Constructor
  */
ViewConfig::ViewConfig(QObject *parent) :
    QObject(parent),
    savedInSetupFile(false)
{
    colorGroupNames.insert( ColorGroups::ND, tr("-undefined-") );
    colorGroupNames.insert( ColorGroups::VstPlugin, tr("Vst Plugin") );
    colorGroupNames.insert( ColorGroups::AudioPin, tr("Audio Pin") );
    colorGroupNames.insert( ColorGroups::MidiPin, tr("Midi Pin") );
    colorGroupNames.insert( ColorGroups::ParameterPin, tr("Parameter Pin") );
    colorGroupNames.insert( ColorGroups::Bridge, tr("Bridge") );
    colorGroupNames.insert( ColorGroups::Object, tr("Object") );
    colorGroupNames.insert( ColorGroups::Windows, tr("Windows") );

    colorsNames.insert( Colors::ND, tr("-undefined-") );
    colorsNames.insert( Colors::Background, tr("Background") );
    colorsNames.insert( Colors::Borders, tr("Borders") );
    colorsNames.insert( Colors::Text, tr("Text") );
    colorsNames.insert( Colors::VuMeter, tr("Vu-meter") );
    colorsNames.insert( Colors::Button, tr("Button") );
    colorsNames.insert( Colors::ButtonText, tr("Button Text") );
    colorsNames.insert( Colors::Window, tr("Window") );
    colorsNames.insert( Colors::WindowText, tr("Window Text") );
    colorsNames.insert( Colors::Base, tr("Base") );
    colorsNames.insert( Colors::AlternateBase, tr("Alternate Base") );
    colorsNames.insert( Colors::ToolTipBase, tr("ToolTip Base") );
    colorsNames.insert( Colors::ToolTipText, tr("ToolTip Text") );
    colorsNames.insert( Colors::BrightText, tr("BrightText") );
    colorsNames.insert( Colors::HighlightBackground, tr("Highlight Background") );


    AddColor(ColorGroups::VstPlugin,Colors::Background,QColor(255,255,128,128));
    AddColor(ColorGroups::VstPlugin,Colors::Text,QColor(0,0,0,255));
    AddColor(ColorGroups::VstPlugin,Colors::HighlightBackground,QColor(255,255,200,128));

    AddColor(ColorGroups::AudioPin,Colors::Background,QColor(200,170,160,255));
    AddColor(ColorGroups::AudioPin,Colors::VuMeter,QColor(210,210,100,255));
    AddColor(ColorGroups::AudioPin,Colors::HighlightBackground,QColor(255,255,255,128));

    AddColor(ColorGroups::MidiPin,Colors::Background,QColor(190,230,230,255));
    AddColor(ColorGroups::MidiPin,Colors::VuMeter,QColor(210,210,100,255));
    AddColor(ColorGroups::MidiPin,Colors::HighlightBackground,QColor(255,255,255,128));

    AddColor(ColorGroups::ParameterPin,Colors::Background,QColor(160,185,200,255));
    AddColor(ColorGroups::ParameterPin,Colors::VuMeter,QColor(210,210,100,255));
    AddColor(ColorGroups::ParameterPin,Colors::HighlightBackground,QColor(255,255,255,128));

    AddColor(ColorGroups::Bridge,Colors::Background,QColor(160,170,160,255));
    AddColor(ColorGroups::Bridge,Colors::Borders,QColor(0,0,0,255));
    AddColor(ColorGroups::Bridge,Colors::HighlightBackground,QColor(200,255,200,128));

    AddColor(ColorGroups::Object,Colors::Background,QColor(120,160,185,128));
    AddColor(ColorGroups::Object,Colors::Borders,QColor(0,0,0,255));
    AddColor(ColorGroups::Object,Colors::Text,QColor(0,0,0,255));

    AddColor(ColorGroups::Windows,Colors::Text,QColor(0,0,0,255));
    AddColor(ColorGroups::Windows,Colors::Window,QColor(175,165,135,255));
    AddColor(ColorGroups::Windows,Colors::WindowText,QColor(0,0,0,255));
    AddColor(ColorGroups::Windows,Colors::Button,QColor(175,165,135,255));
    AddColor(ColorGroups::Windows,Colors::ButtonText,QColor(0,0,0,255));
    AddColor(ColorGroups::Windows,Colors::Base,QColor(190,190,190,255));
    AddColor(ColorGroups::Windows,Colors::AlternateBase,QColor(100,100,100,255));
    AddColor(ColorGroups::Windows,Colors::ToolTipBase,QColor(200,200,0,255));
    AddColor(ColorGroups::Windows,Colors::ToolTipText,QColor(0,0,0,255));
    AddColor(ColorGroups::Windows,Colors::BrightText,QColor(50,0,0,255));
}

/*!
  Destructor
  */
ViewConfig::~ViewConfig()
{

}

/*!
  Update all colors with a new list, used by the dialog when changes are discarded
  \param newList list of groups
  */
void ViewConfig::SetListGroups(QMap<ColorGroups::Enum,ColorGroup> newList)
{
    listColorGroups = newList;
    UpdateAllWidgets();
}

/*!
  Sends all colors to all widgets
  */
void ViewConfig::UpdateAllWidgets()
{
    QMap<ColorGroups::Enum,ColorGroup>::iterator i = listColorGroups.begin();
    while( i!=listColorGroups.end() ) {
        ColorGroup grp = i.value();
        QMap<Colors::Enum,QColor>::iterator j = grp.listColors.begin();
        while( j!=grp.listColors.end() ) {
            emit ColorChanged( i.key(), j.key(), j.value() );
            ++j;
        }
        ++i;
    }
}

/*!
  Get the name of a group, used by the dialog
  \param groupId the group Id
  \return group name
  */
QString ViewConfig::GetColorGroupName(ColorGroups::Enum groupId)
{
    if(!colorGroupNames.contains(groupId))
        return "-unknown-";
    return colorGroupNames.value(groupId);
}

/*!
  Get the name of a color, used by the dialog
  \param colorId the color Id
  \return color name
  */
QString ViewConfig::GetColorName(Colors::Enum colorId)
{
    if(!colorsNames.contains(colorId))
        return "-unknown-";
    return colorsNames.value(colorId);
}

/*!
  Get the ColorRole corresponding to a color Id
  \param colorId the color Id
  \return palette role
  */
QPalette::ColorRole ViewConfig::GetPaletteRoleFromColor(Colors::Enum colorId)
{
    switch(colorId) {
        case Colors::Window:
            return QPalette::Window;
        case Colors::Text:
            return QPalette::Text;
        case Colors::Button:
            return QPalette::Button;
        case Colors::ButtonText:
            return QPalette::ButtonText;
        case Colors::WindowText:
            return QPalette::WindowText;
        case Colors::Base:
            return QPalette::Base;
        case Colors::AlternateBase:
            return QPalette::AlternateBase;
        case Colors::ToolTipBase:
            return QPalette::ToolTipBase;
        case Colors::ToolTipText:
            return QPalette::ToolTipText;
        case Colors::BrightText:
            return QPalette::BrightText;
        default:
            return QPalette::Window;
    }

}

/*!
  Update the given palette with the group colors
  \param groupId the group Id
  \param oriPalette the palette to modify
  \return modified palette
  */
QPalette ViewConfig::GetPaletteFromColorGroup(ColorGroups::Enum groupId, const QPalette &oriPalette)
{
    if(!listColorGroups.contains(groupId))
        return oriPalette;

    ColorGroup grp = listColorGroups.value(groupId);
    QPalette pal(oriPalette);

    QMap<Colors::Enum,QColor>::iterator i = grp.listColors.begin();
    while(i != grp.listColors.end()) {
        pal.setColor( GetPaletteRoleFromColor( i.key() ), i.value() );
        ++i;
    }
    return pal;
}

/*!
  Add a color to a group
  \param groupId group Id
  \param colorId color Id
  \param color the color
  */
void ViewConfig::AddColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    listColorGroups[groupId].listColors[colorId]=color;
    emit ColorChanged(groupId,colorId,color);
}

/*!
  Modifiy a color, don't create it if not found
  \param groupId group Id
  \param colorId color Id
  \param color the new color
  */
void ViewConfig::SetColor(ColorGroups::Enum groupId, Colors::Enum colorId, const QColor &color)
{
    if( !listColorGroups.contains(groupId) || !listColorGroups.value(groupId).listColors.contains(colorId) )
        return;

    listColorGroups[groupId].listColors[colorId]=color;
    emit ColorChanged(groupId,colorId,color);
}

/*!
  Get a color
  \param groupId group Id
  \param colorId color Id
  \return the color or a default color if not found
  */
QColor ViewConfig::GetColor(ColorGroups::Enum groupId, Colors::Enum colorId)
{
    if(!listColorGroups.contains(groupId))
        return QColor();

    return listColorGroups[groupId].listColors[colorId];
}

void ViewConfig::SaveInRegistry(MainHost *myHost)
{
    QByteArray tmpStream;
    QDataStream tmp( &tmpStream , QIODevice::ReadWrite);
    tmp << *this;
    QVariant lVar=QVariant::fromValue(tmpStream);
    myHost->SetSetting("Colors",lVar);
}

void ViewConfig::LoadFromRegistry(MainHost *myHost)
{
    QVariant lVar = myHost->GetSetting("Colors", QVariant::Invalid);

    if( !lVar.isValid() ) {
        SaveInRegistry(myHost);
        lVar = myHost->GetSetting("Colors");
    }

    QByteArray ba( lVar.value<QByteArray>() );
    QDataStream tmp( &ba , QIODevice::ReadWrite);
    tmp >> *this;
    UpdateAllWidgets();
    savedInSetupFile=false;
}

/*!
  Put in a stream
  \param[in] out a QDataStream
  \return the stream
  */
QDataStream & ViewConfig::toStream(QDataStream & out) const
{
    out << (quint16)listColorGroups.count();

    QMap<ColorGroups::Enum,ColorGroup>::const_iterator i = listColorGroups.constBegin();
    while(i!=listColorGroups.constEnd()) {
        out << (quint8)i.key();
        out << (quint8)i.value().listColors.count();

        QMap<Colors::Enum,QColor>::const_iterator j = i.value().listColors.begin();
        while(j!=i.value().listColors.end()) {
            out << (quint8)j.key();
            out << (quint8)j.value().red();
            out << (quint8)j.value().green();
            out << (quint8)j.value().blue();
            out << (quint8)j.value().alpha();
            ++j;
        }
        ++i;
    }

    return out;
}

/*!
  Load from a stream
  \param[in] in a QDataStream
  \return the stream
  */
QDataStream & ViewConfig::fromStream(QDataStream & in)
{
    quint16 nbGrp;
    in >> nbGrp;

    for(int i=0; i<nbGrp; i++) {
        quint8 grpId;
        in >> grpId;
        quint8 nbCol;
        in >> nbCol;
        for(int j=0; j<nbCol; j++) {
            quint8 colId;
            in >> colId;

            quint8 red;
            in >> red;
            quint8 green;
            in >> green;
            quint8 blue;
            in >> blue;
            quint8 alpha;
            in >> alpha;

            listColorGroups[(ColorGroups::Enum)grpId].listColors[(Colors::Enum)colId] = QColor( red,green,blue,alpha );
        }
    }
    UpdateAllWidgets();
    return in;
}

QDataStream & operator<< (QDataStream & out, const View::ViewConfig& value)
{
    return value.toStream(out);
}

QDataStream & operator>> (QDataStream & in, View::ViewConfig& value)
{
    return value.fromStream(in);
}
