/******************************************************************************
#    Copyright 2010 Raphaël François
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
******************************************************************************/

#include "configdialog.h"
#include "ui_configdialog.h"
#include "../mainhost.h"

ConfigDialog::ConfigDialog(MainHost *myHost, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    myHost(myHost)
{
    ui->setupUi(this);
//    ui->treeShortcuts->setModel(MainConfig::Get()->GetShortcutsModel());
//    ui->treeShortcuts->setItemDelegateForColumn(0,new ShortcutDelegate(this));
//    ui->treeActions->setModel(MainConfig::Get()->GetAllActionsModel());

    QSettings settings;

    //vst path
    connect( ui->defaultVstPath, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onVstPathIndexChanged(int)));

    QString vstPath = settings.value("defaultVstPath").toString();
    if(vstPath.isEmpty()) {
        vstPath = "systemDefault";
    }

    ui->defaultVstPath->addItem(QIcon(":/img16x16/folder.png"), tr("System default"), "systemDefault");
    ui->defaultVstPath->addItem(QIcon(":/img16x16/folder.png"), tr("From last session"), "fromLastSession");
    ui->defaultVstPath->addItem(tr("Custom path"));
    if(vstPath == "systemDefault" || vstPath == "fromLastSessions")
        ui->defaultVstPath->setCurrentIndex( ui->defaultVstPath->findData( vstPath ) );
    else
        ui->defaultVstPath->setCurrentIndex( ui->defaultVstPath->findText( vstPath ) );


    //setup file
    connect( ui->defaultSetup, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onSetupIndexChanged(int)));

    QString setupFile = settings.value("defaultSetupFile").toString();
    if(setupFile.isEmpty()) {
        setupFile = "fromLastSession";
    }

    ui->defaultSetup->addItem(QIcon(":/img16x16/empty.png"), tr("Empty setup"), "empty");
    ui->defaultSetup->addItem(QIcon(":/img16x16/file_setup.png"), tr("From last session"), "fromLastSession");
    ui->defaultSetup->addItem(tr("Custom setup file"));
    foreach(const QString &str, settings.value("recentSetupFiles").toStringList()) {
        ui->defaultSetup->addItem(str);
    }
    if(setupFile == "empty" || setupFile == "fromLastSession") {
        ui->defaultSetup->setCurrentIndex( ui->defaultSetup->findData( setupFile ) );
    } else {
        int i = ui->defaultSetup->findText( setupFile );
        if(i == -1) {
            ui->defaultSetup->addItem(setupFile);
            i = ui->defaultSetup->findText( setupFile );
        }
        ui->defaultSetup->setCurrentIndex( i );
    }

    //project file
    connect( ui->defaultProject, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onProjectIndexChanged(int)));

    QString projectFile = settings.value("defaultProjectFile").toString();
    if(projectFile.isEmpty()) {
        projectFile = "fromLastSession";
    }

    ui->defaultProject->addItem(QIcon(":/img16x16/empty.png"), tr("Empty project"), "empty");
    ui->defaultProject->addItem(QIcon(":/img16x16/file_project.png"), tr("From last session"), "fromLastSession");
    ui->defaultProject->addItem(tr("Custom project file"));
    foreach(const QString &str, settings.value("recentProjectFiles").toStringList()) {
        ui->defaultProject->addItem(str);
    }
    if(projectFile == "empty" || projectFile == "fromLastSession") {
        ui->defaultProject->setCurrentIndex( ui->defaultProject->findData( projectFile ) );
    } else {
        int i = ui->defaultProject->findText( projectFile );
        if(i == -1) {
            ui->defaultProject->addItem(projectFile);
            i = ui->defaultProject->findText( projectFile );
        }
        ui->defaultProject->setCurrentIndex( i );
    }


    ui->sampleRate->addItem("44.1 kHz",44100);
    ui->sampleRate->addItem("48 kHz",48000);
    ui->sampleRate->addItem("88.2 kHz",88200);
    ui->sampleRate->addItem("96 kHz",96000);
    int index=ui->sampleRate->findData( myHost->GetSampleRate() );
    if(index==-1) {
        debug("ConfigDialog invalid sample rate")
        return;
    }
    ui->sampleRate->setCurrentIndex(index);


    ui->bufferSize->addItem("Auto",0);
    index = ui->bufferSize->findData( (unsigned int)myHost->GetBufferSize() );
    if(index==-1) {
        debug("ConfigDialog invalid buffer size")
        return;
    }
    ui->bufferSize->setCurrentIndex(index);

    ui->samplePrecision->addItem("16 bits",16);
    ui->samplePrecision->addItem("20 bits",20);
    ui->samplePrecision->addItem("32 bits",32);
    index=ui->samplePrecision->findData( 16 );
    if(index==-1) {
        debug("ConfigDialog invalid sample precision")
        return;
    }
    ui->samplePrecision->setCurrentIndex(index);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}


void ConfigDialog::onVstPathIndexChanged(int index)
{
    QString txt = ui->defaultVstPath->itemData(index).toString();
    if(txt == "systemDefault" || txt == "fromLastSession") {
        ui->defaultVstPath->setEditable(false);
    } else {
        ui->defaultVstPath->setEditable(true);
    }
}

void ConfigDialog::onSetupIndexChanged(int index)
{
    QString txt = ui->defaultSetup->itemData(index).toString();
    if( txt == "empty" || txt == "fromLastSession") {
        ui->defaultSetup->setEditable(false);
    } else {
        ui->defaultSetup->setEditable(true);
    }
}

void ConfigDialog::onProjectIndexChanged(int index)
{
    QString txt = ui->defaultProject->itemData(index).toString();
    if( txt == "empty" || txt == "fromLastSession") {
        ui->defaultProject->setEditable(false);
    } else {
        ui->defaultProject->setEditable(true);
    }
}


const QString ConfigDialog::defaultSetupFile()
{
    QSettings settings;
    QString file = settings.value("defaultSetupFile").toString();
    if(file.isEmpty()) {
        file = "fromLastSession";
    }

    if(file == "empty") {
        return "";
    }

    if(file == "fromLastSession") {
        file = settings.value("lastSetupFile").toString();
    }

    return file;
}

const QString ConfigDialog::defaultProjectFile()
{
    QSettings settings;
    QString file = settings.value("defaultProjectFile").toString();
    if(file.isEmpty()) {
        file = "fromLastSession";
    }

    if(file == "empty") {
        return "";
    }

    if(file == "fromLastSession") {
        file = settings.value("lastProjectFile").toString();
    }

    return file;
}

const QString ConfigDialog::defaultVstPath()
{
    QSettings settings;
    QString vstPath = settings.value("defaultVstPath").toString();

    if(vstPath == "fromLastSession") {
        vstPath = settings.value("lastVstPath").toString();
    }

    if(vstPath.isEmpty() || vstPath=="systemDefault") {
        QSettings vstSettings("HKEY_LOCAL_MACHINE\\Software\\VST", QSettings::NativeFormat);
        vstPath = vstSettings.value("VSTPluginsPath", QDir::homePath()).toString();
        vstPath.replace("\\","/");
    }

    return vstPath;
}

const float ConfigDialog::defaultSampleRate()
{
    QSettings settings;
    float rate = settings.value("sampleRate").toFloat();
    if(rate==.0f) {
        rate=44100.0;
    }

    return rate;
}

const int ConfigDialog::defaultBufferSize()
{
    QSettings settings;
    int size = settings.value("bufferSize").toInt();
    return size;
}

const int ConfigDialog::defaultSamplePrecision()
{
    QSettings settings;
    int bits = settings.value("samplePrecision").toInt();
    if(bits==0)
        bits=16;
    return bits;
}

void ConfigDialog::AddRecentSetupFile(const QString &file)
{
    QSettings settings;
    if(!file.isEmpty()) {
        QStringList lstFiles = settings.value("recentSetupFiles").toStringList();
        lstFiles.insert(0, file);
        lstFiles.removeDuplicates();
        while(lstFiles.size()>NB_RECENT_FILES) {
            lstFiles.removeLast();
        }
        settings.setValue("recentSetupFiles",lstFiles);
    }
    settings.setValue("lastSetupFile", file);
}

void ConfigDialog::AddRecentProjectFile(const QString &file)
{
    QSettings settings;
    if(!file.isEmpty()) {
        QStringList lstFiles = settings.value("recentProjectFiles").toStringList();
        lstFiles.insert(0, file);
        lstFiles.removeDuplicates();
        while(lstFiles.size()>NB_RECENT_FILES) {
            lstFiles.removeLast();
        }
        settings.setValue("recentProjectFiles",lstFiles);
    }
    settings.setValue("lastProjectFile", file);
}

void ConfigDialog::accept()
{

    QString vstDir = ui->defaultVstPath->itemData( ui->defaultVstPath->currentIndex() ).toString();
    if(vstDir != "systemDefault" && vstDir != "fromLastSession") {
        vstDir = ui->defaultVstPath->currentText();
        QDir dir;
        if(!dir.exists(vstDir)) {
            QMessageBox msg(QMessageBox::Critical,
                            tr("Error"),
                            tr("Vst path is not a valid directory"),
                            QMessageBox::Ok,
                            this);
            msg.exec();
            ui->defaultVstPath->setFocus();
            return;
        }
    }

    QString setupFile =  ui->defaultSetup->itemData( ui->defaultSetup->currentIndex() ).toString();
    if(setupFile != "empty" && setupFile != "fromLastSession") {
        setupFile = ui->defaultSetup->currentText();
        QFile file;
        if(!file.exists(setupFile) || !setupFile.endsWith(SETUP_FILE_EXTENSION)) {
            QMessageBox msg(QMessageBox::Critical,
                            tr("Error"),
                            tr("Setup file is not valid"),
                            QMessageBox::Ok,
                            this);
            msg.exec();
            ui->defaultSetup->setFocus();
            return;
        }
    }

    QString projectFile =  ui->defaultProject->itemData( ui->defaultProject->currentIndex() ).toString();
    if(projectFile != "empty" && projectFile != "fromLastSession") {
        projectFile = ui->defaultProject->currentText();
        QFile file;
        if(!file.exists(projectFile) || !projectFile.endsWith(PROJECT_FILE_EXTENSION)) {
            QMessageBox msg(QMessageBox::Critical,
                            tr("Error"),
                            tr("Project file is not valid"),
                            QMessageBox::Ok,
                            this);
            msg.exec();
            ui->defaultProject->setFocus();
            return;
        }
    }

    QSettings settings;

    float rate = ui->sampleRate->itemData(ui->sampleRate->currentIndex()).toFloat();
    settings.setValue("sampleRate", rate);
    myHost->SetSampleRate( rate );

    int buffer = ui->bufferSize->itemData(ui->bufferSize->currentIndex()).toInt();
    settings.setValue("bufferSize", buffer);

    int precision = ui->samplePrecision->itemData(ui->samplePrecision->currentIndex()).toInt();
    settings.setValue("samplePrecision",precision);

    //myHost->UpdateShortcutsFromModel();

    settings.setValue("defaultVstPath", vstDir );
    settings.setValue("defaultSetupFile", setupFile );
    settings.setValue("defaultProjectFile", projectFile );
    QDialog::accept();
}

void ConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ConfigDialog::on_browseVst_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Default Vst path"), "", tr("Directory (*.)"));

    if(fileName.isEmpty())
        return;

    if(ui->defaultVstPath->findText(fileName) == -1)
        ui->defaultVstPath->addItem(fileName);
    ui->defaultVstPath->setCurrentIndex( ui->defaultVstPath->findText(fileName) );
}

void ConfigDialog::on_browseSetup_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Default setup file"), "", tr("Setup file (*.%1)").arg(SETUP_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(SETUP_FILE_EXTENSION)) {
        QMessageBox msgBox;
        msgBox.setText(tr("Wrong file extension."));
        msgBox.exec();
        return;
    }

    AddRecentSetupFile(fileName);

    int i = ui->defaultSetup->findText(fileName);
    if(i == -1) {
        ui->defaultSetup->addItem(fileName);
        i = ui->defaultSetup->findText(fileName);
    }
    ui->defaultSetup->setCurrentIndex( i );
}

void ConfigDialog::on_browseProject_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Default setup file"), "", tr("Setup file (*.%1)").arg(PROJECT_FILE_EXTENSION));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(PROJECT_FILE_EXTENSION)) {
        QMessageBox msgBox;
        msgBox.setText(tr("Wrong file extension."));
        msgBox.exec();
        return;
    }

    AddRecentProjectFile(fileName);

    int i = ui->defaultProject->findText(fileName);
    if(i == -1) {
        ui->defaultProject->addItem(fileName);
        i = ui->defaultProject->findText(fileName);
    }
    ui->defaultProject->setCurrentIndex( i );
}
