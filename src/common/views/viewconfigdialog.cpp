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
    modified(false)
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
void ViewConfigDialog::UpdateSliders()
{
    if(!ui->groupColor->isEnabled())
        return;

    QColor col = conf->GetColor( currentGrp, currentCol );
    ui->RedSlider->setValue(col.red());
    ui->GreenSlider->setValue(col.green());
    ui->BlueSlider->setValue(col.blue());
    ui->AlphaSlider->setValue(col.alpha());
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
void ViewConfigDialog::UpdateColor(int r,int v,int b, int a)
{
    if(currentGrp==ColorGroups::ND || currentCol==Colors::ND)
        return;

    QColor col(r,v,b,a);
    conf->SetColor(currentGrp,currentCol, col);
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
        UpdateSliders();
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
    UpdateSliders();
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
    UpdateSliders();
}


void View::ViewConfigDialog::on_RedSpinBox_valueChanged(int value)
{
    modified=true;
    UpdateColor(value, ui->GreenSlider->value(), ui->BlueSlider->value(), ui->AlphaSlider->value());
}

void View::ViewConfigDialog::on_GreenSpinBox_valueChanged(int value)
{
    modified=true;
    UpdateColor(ui->RedSlider->value(), value, ui->BlueSlider->value(), ui->AlphaSlider->value());
}

void View::ViewConfigDialog::on_BlueSpinBox_valueChanged(int value)
{
    modified=true;
    UpdateColor(ui->RedSlider->value(), ui->GreenSlider->value(), value, ui->AlphaSlider->value());
}

void View::ViewConfigDialog::on_AlphaSpinBox_valueChanged(int value)
{
    modified=true;
    UpdateColor(ui->RedSlider->value(), ui->GreenSlider->value(), ui->BlueSlider->value(), value);
}
