#include "scripteditor.h"
#include "ui_scripteditor.h"

using namespace View;

ScriptEditor::ScriptEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptEditor)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags(Qt::Tool);
    setWindowTitle(tr("Script editor"));
}

ScriptEditor::~ScriptEditor()
{
    delete ui;
}

void ScriptEditor::closeEvent( QCloseEvent * event )
{
    hide();
    emit Hide();
    event->ignore();
}

void ScriptEditor::SetScript(const QString &script)
{
    ui->plainTextEdit->setPlainText( script );
}

void ScriptEditor::on_btExecute_clicked()
{
    emit Execute( ui->plainTextEdit->toPlainText() );
}
