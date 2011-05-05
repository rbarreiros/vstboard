#include "script.h"
#include "../mainhost.h"
#include "../mainwindow.h"

using namespace Connectables;

Script::Script(MainHost *host, int index, const ObjectInfo &info) :
    Object(host,index,info),
    editorWnd(0)
{
    setObjectName( QString("objScript%1").arg(index) );
    objScriptName = objectName();
    objScriptName.append("sc");

    QScriptValue scriptObj = myHost->scriptEngine.newQObject(this);
    myHost->scriptEngine.globalObject().setProperty(objScriptName, scriptObj);

    //editor pin
    listEditorVisible << "hide";
    listEditorVisible << "show";
    listParameterPinIn->AddPin(FixedPinNumber::editorVisible);


//    QString fileName = "C:\\Users\\CtrlBrk\\Documents\\vstboard.shadow.32\\bin\\debug\\script.qs";
//    QFile scriptFile(fileName);
//    if (!scriptFile.open(QIODevice::ReadOnly)) {
//        QMessageBox m(
//            QMessageBox::Critical,
//            tr("Script error"),
//            tr("Can't open %1").arg(fileName)
//        );
//        m.exec();
//    }

//    QTextStream stream(&scriptFile);
//    scriptText = stream.readAll();
//    scriptFile.close();

    connect(this, SIGNAL(_dspMsg(QString,QString)),
            this, SLOT(DspMsg(QString,QString)),
            Qt::QueuedConnection);

    editorWnd = new View::ScriptEditor(myHost->mainWindow);
    connect(editorWnd, SIGNAL(Execute(QString)),
            this,SLOT(ReplaceScript(QString)));
    connect(editorWnd, SIGNAL(Hide()),
            this, SLOT(OnHideEditor()));
    connect(editorWnd, SIGNAL(destroyed()),
            this, SLOT(EditorDestroyed()));
}



bool Script::Open()
{
    static_cast<ParameterPinIn*>(listParameterPinIn->listPins.value(FixedPinNumber::editorVisible))->SetAlwaysVisible(true);

//    if(scriptText.isEmpty()) {
//        scriptText = "\
//this.open = function() {\n\
//    obj.listParameterPinIn.nbPins=1;\n\
//    obj.listParameterPinOut.nbPins=1;\n\
//    obj.listAudioPinIn.nbPins=1;\n\
//    obj.listAudioPinOut.nbPins=1;\n\
//}\n\
//\n\
//this.render = function() {\n\
//\n\
//}";
//    }

    if(scriptText.isEmpty()) {
        scriptText = "\
({open: function() {\n\
    this.listParameterPinIn.nbPins=1;\n\
    this.listParameterPinOut.nbPins=1;\n\
    this.listAudioPinIn.nbPins=1;\n\
    this.listAudioPinOut.nbPins=1;\n\
},\n\
\n\
render: function() {\n\
\n\
})}";
    }

    mutexScript.lock();

    QScriptSyntaxCheckResult chk = myHost->scriptEngine.checkSyntax(scriptText);
    if(chk.state()!=QScriptSyntaxCheckResult::Valid) {
        comiledScript="";
        mutexScript.unlock();

        QMessageBox msg(
            QMessageBox::Critical,
            tr("Script syntax error"),
            tr("line %1\n%2").arg(chk.errorLineNumber()).arg(chk.errorMessage())
        );
        msg.exec();
        return false;
    }

//    comiledScript = QString( "function %1class(t) { obj=t; %2 }  %1m = new %1class(%1);" ).arg(objScriptName).arg(scriptText);
//    QScriptValue result = myHost->scriptEngine.evaluate(comiledScript);

//    if(myHost->scriptEngine.hasUncaughtException()) {
//        comiledScript="";
//        mutexScript.unlock();

//        int line = myHost->scriptEngine.uncaughtExceptionLineNumber();
//        QMessageBox msg(
//            QMessageBox::Critical,
//            tr("Script exception"),
//            tr("line %1\n%2").arg(line).arg(result.toString())
//        );
//        msg.exec();

//        return false;
//    }

//    myHost->scriptEngine.evaluate( objScriptName+"m.open();" );

    QScriptValue objScript = myHost->scriptEngine.evaluate(scriptText);
    openScript = objScript.property("open");
    renderScript = objScript.property("render");

    QScriptValue result = openScript.call(object);

    mutexScript.unlock();

    return true;
}

Script::~Script()
{
    Close();
}

bool Script::Close()
{
    if(editorWnd) {
        editorWnd->close();
        editorWnd->deleteLater();
        editorWnd=0;
    }
    return true;
}

void Script::Render()
{
    mutexScript.lock();

    foreach(Pin *pin, listAudioPinIn->listPins) {
        static_cast<AudioPinIn*>(pin)->GetBuffer()->ConsumeStack();
        static_cast<AudioPinIn*>(pin)->NewRenderLoop();
    }
    foreach(Pin *pin, listAudioPinOut->listPins) {
        static_cast<AudioPinOut*>(pin)->NewRenderLoop();
    }

    QScriptValue result = renderScript.call(object);

//    if(!comiledScript.isEmpty()) {
//        QScriptValue result = myHost->scriptEngine.evaluate( objScriptName+"m.render();" );
//        if(myHost->scriptEngine.hasUncaughtException()) {
//            comiledScript="";

//            int line = myHost->scriptEngine.uncaughtExceptionLineNumber();
//            emit _dspMsg(
//                tr("Script exception"),
//                tr("line %1\n%2").arg(line).arg(result.toString())
//            );
//        }
//    }

    foreach(Pin *pin, listAudioPinOut->listPins) {
        static_cast<AudioPinOut*>(pin)->GetBuffer()->ConsumeStack();
        static_cast<AudioPinOut*>(pin)->SendAudioBuffer();
    }

    mutexScript.unlock();
}

void Script::alert(const QString &str)
{
    emit _dspMsg(objectName(),str);
}

void Script::DspMsg(const QString &title, const QString &str)
{
    QMessageBox msg(
        QMessageBox::Information,
        title,
        str
    );
    msg.exec();
}

void Script::ReplaceScript(const QString &str)
{
    scriptText = str;
    OnProgramDirty();
    Open();
}

Pin* Script::CreatePin(const ConnectionInfo &info)
{
    Pin *newPin = Object::CreatePin(info);
    if(newPin)
        return newPin;

    ParameterPin *pin=0;

    if(info.type == PinType::Parameter) {
        switch(info.direction) {
            case PinDirection::Input :
                if(info.pinNumber == FixedPinNumber::editorVisible) {
                    ParameterPin *newPin = new ParameterPinIn(this,FixedPinNumber::editorVisible,"hide",&listEditorVisible,false,tr("Editor"));
                    newPin->SetLimitsEnabled(false);
                    return newPin;
                } else {
                    pin = new ParameterPinIn(this,info.pinNumber,0,true,QString("ParamIn%1").arg(info.pinNumber));
                    pin->SetAlwaysVisible(true);
                    return pin;
                }
            case PinDirection::Output :
                pin = new ParameterPinOut(this,info.pinNumber,0,true,QString("ParamOut%1").arg(info.pinNumber));
                pin->SetAlwaysVisible(true);
                return pin;
        }
    }

    return 0;
}

void Script::OnShowEditor()
{
    if(!editorWnd || editorWnd->isVisible())
        return;

    editorWnd->SetScript(scriptText);
    editorWnd->show();
}

void Script::OnHideEditor()
{
    if(!editorWnd || !editorWnd->isVisible())
        return;
    editorWnd->close();
}

void Script::OnEditorClosed()
{
    ToggleEditor(false);
}

void Script::EditorDestroyed()
{
    editorWnd=0;
    ToggleEditor(false);
}

void Script::SaveProgram()
{
    if(!currentProgram || !currentProgram->isDirty)
        return;

    Object::SaveProgram();

    currentProgram->listOtherValues.insert(0, scriptText);
}

void Script::LoadProgram(int prog)
{
    Object::LoadProgram(prog);
    if(!currentProgram)
        return;

    scriptText = currentProgram->listOtherValues.value(0,"").toString();
    Open();

    if(editorWnd && editorWnd->isVisible())
        editorWnd->SetScript(scriptText);
}
