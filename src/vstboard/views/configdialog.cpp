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

#include "configdialog.h"
#include "ui_configdialog.h"
#include "../mainhost.h"

ConfigDialog::ConfigDialog(MainHost *myHost, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    myHost(myHost)
{
    ui->setupUi(this);

//vst path
    ui->defaultVstPath->addItem(QIcon(":/img16x16/folder.png"), tr("System default"), "systemDefault");
    ui->defaultVstPath->addItem(QIcon(":/img16x16/folder.png"), tr("From last session"), "fromLastSession");
    ui->defaultVstPath->addItem(tr("Custom path"));

    QString vstPath = myHost->GetSetting("defaultVstPath", "systemDefault").toString();
    if(vstPath == "systemDefault" || vstPath == "fromLastSession")
        ui->defaultVstPath->setCurrentIndex( ui->defaultVstPath->findData( vstPath ) );
    else
        ui->defaultVstPath->setCurrentIndex( ui->defaultVstPath->findText( vstPath ) );


    connect( ui->defaultVstPath, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onVstPathIndexChanged(int)));

//setup file
    ui->defaultSetup->addItem(QIcon(":/img16x16/empty.png"), tr("Empty setup"), "empty");
    ui->defaultSetup->addItem(QIcon(":/img16x16/file_setup.png"), tr("From last session"), "fromLastSession");
    ui->defaultSetup->addItem(tr("Custom setup file"));
    foreach(const QString &str, myHost->GetSetting("recentSetupFiles").toStringList()) {
        ui->defaultSetup->addItem(str);
    }

    QString setupFile = myHost->GetSetting("defaultSetupFile","fromLastSession").toString();
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

    connect( ui->defaultSetup, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onSetupIndexChanged(int)));


//project file
    ui->defaultProject->addItem(QIcon(":/img16x16/empty.png"), tr("Empty project"), "empty");
    ui->defaultProject->addItem(QIcon(":/img16x16/file_project.png"), tr("From last session"), "fromLastSession");
    ui->defaultProject->addItem(tr("Custom project file"));
    foreach(const QString &str, myHost->GetSetting("recentProjectFiles").toStringList()) {
        ui->defaultProject->addItem(str);
    }

    QString projectFile = myHost->GetSetting("defaultProjectFile","fromLastSession").toString();
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

    connect( ui->defaultProject, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onProjectIndexChanged(int)));

    int index=0;
    ui->bufferSize->setVisible(false);
    ui->labelbufferSize->setVisible(false);

#ifdef VST_PLUGIN
    //ui->groupSampleFormat->setVisible(false);
    ui->sampleRate->setVisible(false);
    ui->labelsampleRate->setVisible(false);

#else
//sample rate

    ui->sampleRate->addItem("44.1 kHz",44100);
    ui->sampleRate->addItem("48 kHz",48000);
    ui->sampleRate->addItem("88.2 kHz",88200);
    ui->sampleRate->addItem("96 kHz",96000);

    index=ui->sampleRate->findData( myHost->GetSampleRate() );
    if(index==-1) {
        debug("ConfigDialog invalid sample rate")
        return;
    }
    ui->sampleRate->setCurrentIndex(index);

//buffer size
//    ui->bufferSize->addItem("Auto",0);
//    index = ui->bufferSize->findData( (unsigned int)myHost->GetBufferSize() );
//    if(index==-1) {
//        debug("ConfigDialog invalid buffer size")
//        index=0;
//    }
//    ui->bufferSize->setCurrentIndex(index);
#endif
//sample precision
    ui->samplePrecision->addItem("32 bits",false);
    ui->samplePrecision->addItem("64 bits",true);
    index=ui->samplePrecision->findData( defaultDoublePrecision(myHost) );
    if(index==-1) {
        debug("ConfigDialog invalid sample precision")
        index=0;
    }
    ui->samplePrecision->setCurrentIndex(index);


//on unsaved setup
    ui->onUnsavedSetup->addItem(tr("Always ask"),Autosave::prompt);
    ui->onUnsavedSetup->addItem(tr("Save changes"),Autosave::save);
    ui->onUnsavedSetup->addItem(tr("Discard changes"),Autosave::discard);

    int unsavedSetup = myHost->GetSetting("onUnsavedSetup",Autosave::prompt).toInt();
    ui->onUnsavedSetup->setCurrentIndex( ui->onUnsavedSetup->findData(unsavedSetup) );

//on unsaved project
    ui->onUnsavedProject->addItem(tr("Always ask"),Autosave::prompt);
    ui->onUnsavedProject->addItem(tr("Save changes"),Autosave::save);
    ui->onUnsavedProject->addItem(tr("Discard changes"),Autosave::discard);

    int unsavedProject = myHost->GetSetting("onUnsavedProject",Autosave::prompt).toInt();
    ui->onUnsavedProject->setCurrentIndex( ui->onUnsavedProject->findData(unsavedProject) );
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


const QString ConfigDialog::defaultSetupFile(MainHost *myHost)
{
    QString file = myHost->GetSetting("defaultSetupFile","fromLastSession").toString();
    if(file == "empty") {
        return "";
    }
    if(file == "fromLastSession") {
        file = myHost->GetSetting("lastSetupFile","").toString();
    }
    return file;
}

const QString ConfigDialog::defaultProjectFile(MainHost *myHost)
{
    QString file = myHost->GetSetting("defaultProjectFile","fromLastSession").toString();
    if(file == "empty") {
        return "";
    }
    if(file == "fromLastSession") {
        file = myHost->GetSetting("lastProjectFile","").toString();
    }
    return file;
}

const QString ConfigDialog::defaultVstPath(MainHost *myHost)
{
    QString vstPath = myHost->GetSetting("defaultVstPath").toString();

    if(vstPath == "fromLastSession") {
        vstPath = myHost->GetSetting("lastVstPath").toString();
    }

    if(vstPath.isEmpty() || vstPath=="systemDefault") {
        QSettings vstSettings("HKEY_LOCAL_MACHINE\\Software\\VST", QSettings::NativeFormat);
        vstPath = vstSettings.value("VSTPluginsPath", QDir::homePath()).toString();
        vstPath.replace("\\","/");
    }

    return vstPath;
}

const float ConfigDialog::defaultSampleRate(MainHost *myHost)
{
    return myHost->GetSetting("sampleRate",44100.0).toFloat();
}

const int ConfigDialog::defaultBufferSize(MainHost *myHost)
{
    return myHost->GetSetting("bufferSize").toInt();
}

const bool ConfigDialog::defaultDoublePrecision(MainHost *myHost)
{
    return myHost->GetSetting("doublePrecision",true).toBool();
}

void ConfigDialog::AddRecentSetupFile(const QString &file,MainHost *myHost)
{
    if(!file.isEmpty()) {
        QStringList lstFiles = myHost->GetSetting("recentSetupFiles").toStringList();
        lstFiles.insert(0, file);
        lstFiles.removeDuplicates();
        while(lstFiles.size()>NB_RECENT_FILES) {
            lstFiles.removeLast();
        }
        myHost->SetSetting("recentSetupFiles",lstFiles);
    }
    myHost->SetSetting("lastSetupFile", file);
}

void ConfigDialog::AddRecentProjectFile(const QString &file,MainHost *myHost)
{
    if(!file.isEmpty()) {
        QStringList lstFiles = myHost->GetSetting("recentProjectFiles").toStringList();
        lstFiles.insert(0, file);
        lstFiles.removeDuplicates();
        while(lstFiles.size()>NB_RECENT_FILES) {
            lstFiles.removeLast();
        }
        myHost->SetSetting("recentProjectFiles",lstFiles);
    }
    myHost->SetSetting("lastProjectFile", file);
}

void ConfigDialog::accept()
{
    bool needRestart=false;

//vst directory
    QString vstDir = ui->defaultVstPath->itemData( ui->defaultVstPath->currentIndex() ).toString();
    if(vstDir != "systemDefault" && vstDir != "fromLastSession") {
        vstDir = ui->defaultVstPath->currentText();
        QDir dir;
        if(vstDir.isEmpty() || !dir.exists(vstDir)) {
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

//setup file
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

//project file
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

//default files
    myHost->SetSetting("defaultVstPath", vstDir );
    myHost->SetSetting("defaultSetupFile", setupFile );
    myHost->SetSetting("defaultProjectFile", projectFile );

#ifndef VST_PLUGIN
//sample format
    float rate = ui->sampleRate->itemData(ui->sampleRate->currentIndex()).toFloat();
    if(rate!=defaultSampleRate(myHost)) {
        myHost->SetSampleRate( rate );
    }
    myHost->SetSetting("sampleRate", rate);

//    int buffer = ui->bufferSize->itemData(ui->bufferSize->currentIndex()).toInt();
//    settings.setValue("bufferSize", buffer);
#endif
    bool precision = ui->samplePrecision->itemData(ui->samplePrecision->currentIndex()).toBool();
    if(defaultDoublePrecision(myHost)!=precision)
        needRestart=true;
    myHost->SetSetting("doublePrecision",precision);

//on unsaved changes
    myHost->SetSetting("onUnsavedSetup", ui->onUnsavedSetup->itemData( ui->onUnsavedSetup->currentIndex() ).toInt() );
    myHost->SetSetting("onUnsavedProject", ui->onUnsavedProject->itemData( ui->onUnsavedProject->currentIndex() ).toInt() );

    QDialog::accept();

    if(needRestart) {
        QMessageBox msg;
        msg.setText(tr("You must restart VstBoard for the changes to take effect"));
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }
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

    AddRecentSetupFile(fileName,myHost);

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

    AddRecentProjectFile(fileName,myHost);

    int i = ui->defaultProject->findText(fileName);
    if(i == -1) {
        ui->defaultProject->addItem(fileName);
        i = ui->defaultProject->findText(fileName);
    }
    ui->defaultProject->setCurrentIndex( i );
}
