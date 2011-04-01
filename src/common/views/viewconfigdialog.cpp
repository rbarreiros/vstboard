#include "viewconfigdialog.h"
#include "ui_viewconfigdialog.h"

using namespace View;

ViewConfigDialog::ViewConfigDialog(MainHost *myHost, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewConfigDialog),
    myHost(myHost),
    currentGrp(ColorGroups::ND),
    currentCol(Colors::ND)
{
    ui->setupUi(this);

    conf = &myHost->mainWindow->viewConfig;

    ui->groupColor->setEnabled(false);

    int cpt=0;
    QMap<ColorGroups::Enum,ColorGroup*>::iterator i = conf->listColorGroups.begin();
    while(i!=conf->listColorGroups.end()) {
        ui->listPalettes->addItem( conf->GetColorGroupName(i.key()) );
        ui->listPalettes->item(cpt)->setData(Qt::UserRole+1,i.key());
        cpt++;
        ++i;
    }
}

ViewConfigDialog::~ViewConfigDialog()
{
    delete ui;
}

void ViewConfigDialog::UpdateSliders()
{
    QColor col = conf->GetColor( currentGrp, currentCol );
    ui->RedSlider->setValue(col.red());
    ui->GreenSlider->setValue(col.green());
    ui->BlueSlider->setValue(col.blue());
    ui->AlphaSlider->setValue(col.alpha());
}

void ViewConfigDialog::UpdateColor()
{
    if(currentGrp==ColorGroups::ND || currentCol==Colors::ND)
        return;

    QColor col(ui->RedSlider->value(), ui->GreenSlider->value(), ui->BlueSlider->value(), ui->AlphaSlider->value());
    conf->SetColor(currentGrp,currentCol, col);
}

void View::ViewConfigDialog::on_listPalettes_itemClicked(QListWidgetItem* item)
{
    currentGrp = (ColorGroups::Enum)item->data(Qt::UserRole+1).toInt();

    ui->listRoles->clear();

    ColorGroup *grp = conf->listColorGroups.value( currentGrp );
    if(!grp)
        return;

    bool colorExistsInGroup=false;
    int cpt=0;
    QMap<Colors::Enum,QColor>::iterator i = grp->listColors.begin();
    while(i!=grp->listColors.end()) {

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
        currentCol = Colors::ND;
        ui->groupColor->setEnabled(false);
    }
}

void View::ViewConfigDialog::on_listRoles_itemClicked(QListWidgetItem* item)
{
    currentCol = (Colors::Enum)item->data(Qt::UserRole+1).toInt();
    ui->groupColor->setEnabled(true);
    UpdateSliders();
}

void View::ViewConfigDialog::on_RedSlider_sliderMoved(int position)
{
    UpdateColor();
}

void View::ViewConfigDialog::on_GreenSlider_sliderMoved(int position)
{
    UpdateColor();
}

void View::ViewConfigDialog::on_BlueSlider_sliderMoved(int position)
{
    UpdateColor();
}

void View::ViewConfigDialog::on_AlphaSlider_sliderMoved(int position)
{
    UpdateColor();
}
