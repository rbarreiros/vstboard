/**************************************************************************
#    Copyright 2010-2011 Raphaël François
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
**************************************************************************/
#include "heap.h"


#include "viewconfigdialog.h"
#include "ui_viewconfigdialog.h"

using namespace View;

/*!
  \class View::ViewConfigDialog
  \brief modify the application appearance
  */

/*!
  Constructor
  \param myHost pointer to the MainHost
  \param parent pointer to a parent widget
  */
ViewConfigDialog::ViewConfigDialog(MainHost *myHost, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewConfigDialog),
    myHost(myHost),
    currentGrp(ColorGroups::ND),
    currentCol(Colors::ND),
    modified(false),
    updateInProgress(false)
{
    ui->setupUi(this);

    conf = &myHost->mainWindow->viewConfig;

    DisableSliders();

    int cpt=0;

    backupColors = conf->listColorGroups;
    backupSaveInSetup = conf->savedInSetupFile;

    QMap<ColorGroups::Enum,ColorGroup>::iterator i = conf->listColorGroups.begin();
    while(i!=conf->listColorGroups.end()) {
        ui->listPalettes->addItem( conf->GetColorGroupName(i.key()) );
        ui->listPalettes->item(cpt)->setData(Qt::UserRole+1,i.key());
        cpt++;
        ++i;
    }
    ui->checkSavedInSetupFile->setChecked( conf->savedInSetupFile );

    connect(ui->picker,SIGNAL(colorSelected(QColor)),
            this,SLOT(onPickerColorSelected(QColor)));
}

/*!
  Destructor
  */
ViewConfigDialog::~ViewConfigDialog()
{
    delete ui;
}

/*!
  Called when Ok is clicked
  */
void ViewConfigDialog::accept()
{
    if(!ui->checkSavedInSetupFile->isChecked() && modified) {
        conf->SaveInRegistry(myHost);
    }
    QDialog::accept();
}

/*!
  Called when Cancel is clicked. Asks the user if he wants to discard changes
  */
void ViewConfigDialog::reject()
{

    if(modified) {
//        QMessageBox msg(QMessageBox::Question, tr("VstBoard Appearance"), tr("Discard changes ?"), QMessageBox::Ok | QMessageBox::Cancel, this);
//        if(msg.exec() == QMessageBox::Cancel)
//            return;

        conf->SetListGroups( backupColors );
        conf->savedInSetupFile = backupSaveInSetup;
    }

    QDialog::reject();
}

/*!
  Update color sliders position according to the selected color
  */
void ViewConfigDialog::GetColorFromConf()
{
    if(!ui->groupColor->isEnabled())
        return;

    currentColor = conf->GetColor( currentGrp, currentCol );

    updateInProgress=true;
    UpdateSliders();
    ui->AlphaSpinBox->setValue(currentColor.alpha());
    ui->HueSpinBox->setValue(currentColor.hue());
    ui->picker->setMainColor(currentColor);
    updateInProgress=false;
}

void ViewConfigDialog::UpdateSliders()
{
    ui->RedSpinBox->setValue(currentColor.red());
    ui->GreenSpinBox->setValue(currentColor.green());
    ui->BlueSpinBox->setValue(currentColor.blue());
}

/*!
  Disable sliders, when no color selected
  */
void ViewConfigDialog::DisableSliders()
{
    currentCol = Colors::ND;
    ui->groupColor->setEnabled(false);
    ui->RedSlider->setValue(0);
    ui->GreenSlider->setValue(0);
    ui->BlueSlider->setValue(0);
    ui->AlphaSlider->setValue(0);
}

/*!
  Send the modified color to the config
  */
void ViewConfigDialog::SaveColor()
{
    if(currentGrp==ColorGroups::ND || currentCol==Colors::ND)
        return;
    modified=true;
    conf->SetColor(currentGrp,currentCol, currentColor);
}

/*!
  A group was clicked, update the list of colors
  \param item the selected group item
  */
void View::ViewConfigDialog::on_listPalettes_itemClicked(QListWidgetItem* item)
{
    currentGrp = (ColorGroups::Enum)item->data(Qt::UserRole+1).toInt();

    ui->listRoles->clear();

    if(!conf->listColorGroups.contains(currentGrp))
        return;

    ColorGroup grp = conf->listColorGroups.value( currentGrp );

    bool colorExistsInGroup=false;
    int cpt=0;
    QMap<Colors::Enum,QColor>::iterator i = grp.listColors.begin();
    while(i!=grp.listColors.end()) {

        ui->listRoles->addItem( conf->GetColorName( i.key() ) );
        ui->listRoles->item(cpt)->setData(Qt::UserRole+1,i.key());

        if(i.key() == currentCol) {
            colorExistsInGroup=true;
            ui->listRoles->setCurrentRow(cpt);
        }

        cpt++;
        ++i;
    }

    if(colorExistsInGroup) {
        ui->groupColor->setEnabled(true);
        GetColorFromConf();
    } else {
        DisableSliders();
    }
}

/*!
  The selection changed, update the color sliders
  \param item the selected color item
  */
void View::ViewConfigDialog::on_listRoles_itemClicked(QListWidgetItem* item)
{
    currentCol = (Colors::Enum)item->data(Qt::UserRole+1).toInt();
    ui->groupColor->setEnabled(true);
    GetColorFromConf();
}

/*!
  SaveInSetup was clicked, load the colors from the registry or the setup file
  */
void View::ViewConfigDialog::on_checkSavedInSetupFile_clicked(bool checked)
{
    modified=true;

    conf->savedInSetupFile=checked;

    if(checked) {
        if( backupSetupColors.count()!=0 )
        conf->SetListGroups( backupSetupColors );
    } else {
        backupSetupColors = conf->listColorGroups;
        conf->LoadFromRegistry(myHost);
    }
    GetColorFromConf();
}


void View::ViewConfigDialog::on_RedSpinBox_valueChanged(int value)
{
    if(!StartUpdate())
        return;
    ui->picker->setRed(value);
    ui->HueSpinBox->setValue( ui->picker->selectedColor().hue() );
    EndUpdate();
}

void View::ViewConfigDialog::on_GreenSpinBox_valueChanged(int value)
{
    if(!StartUpdate())
        return;
    ui->picker->setGreen(value);
    ui->HueSpinBox->setValue( ui->picker->selectedColor().hue() );
    EndUpdate();
}

void View::ViewConfigDialog::on_BlueSpinBox_valueChanged(int value)
{
    if(!StartUpdate())
        return;
    ui->picker->setBlue(value);
    ui->HueSpinBox->setValue( ui->picker->selectedColor().hue() );
    EndUpdate();
}

void View::ViewConfigDialog::on_AlphaSpinBox_valueChanged(int value)
{
    if(!StartUpdate())
        return;
    ui->picker->setAlpha(value);
    EndUpdate();
}

void View::ViewConfigDialog::on_HueSpinBox_valueChanged(int value)
{
    if(!StartUpdate())
        return;
    ui->picker->setHue(value);
    currentColor=ui->picker->selectedColor();
    UpdateSliders();
    EndUpdate();
}

void ViewConfigDialog::onPickerColorSelected(const QColor &color)
{
    StartUpdate();
    currentColor=color;
    UpdateSliders();
    EndUpdate();
}

bool ViewConfigDialog::StartUpdate()
{
    if(updateInProgress)
        return false;
    updateInProgress=true;
    return true;
}

void ViewConfigDialog::EndUpdate()
{
    currentColor=ui->picker->selectedColor();
    SaveColor();
    updateInProgress=false;
}
