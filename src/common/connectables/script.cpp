#include "script.h"
#include "../mainhost.h"

using namespace Connectables;

Script::Script(MainHost *host, int index, const ObjectInfo &info) :
    Object(host,index,info)
{
    setObjectName( QString("objScript%1").arg(index) );
    objScriptName = objectName();
    objScriptName.append("sc");

    QScriptValue scriptObj = myHost->scriptEngine.newQObject(this);
    myHost->scriptEngine.globalObject().setProperty(objScriptName, scriptObj);
}

bool Script::Open()
{
    QString fileName = "C:\\Users\\CtrlBrk\\Documents\\vstboard.shadow.msvc\\bin\\debug\\script.qs";
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly)) {
        QMessageBox m(
            QMessageBox::Critical,
            tr("Script error"),
            tr("Can't open %1").arg(fileName)
        );
        m.exec();
        return false;
    }

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();

    QScriptSyntaxCheckResult chk = myHost->scriptEngine.checkSyntax(contents);
    if(chk.state()!=QScriptSyntaxCheckResult::Valid) {
        QMessageBox msg(
            QMessageBox::Critical,
            tr("Script syntax error"),
            tr("line %1\n%2").arg(chk.errorLineNumber()).arg(chk.errorMessage())
        );
        msg.exec();
        return false;
    }

    //contents.replace("obj.", objScriptName+".");
    contents = QString( "function %1class(t) { obj=t; %2 }  %1m = new %1class(%1);" ).arg(objScriptName).arg(contents);

    debug2(<<contents)
    QScriptValue result = myHost->scriptEngine.evaluate(contents, fileName);
    if(myHost->scriptEngine.hasUncaughtException()) {
        int line = myHost->scriptEngine.uncaughtExceptionLineNumber();
        QMessageBox msg(
            QMessageBox::Critical,
            tr("Script exception"),
            tr("line %1\n%2").arg(line).arg(result.toString())
        );
        msg.exec();
        return false;
    }

    myHost->scriptEngine.evaluate( objScriptName+"m.open();" );

    return true;
}

void Script::Render()
{
    myHost->scriptEngine.evaluate( objScriptName+"m.render();" );

}

void Script::alert(const QString &str)
{
    QMessageBox msg(
        QMessageBox::Information,
        objectName(),
        str
    );
    msg.exec();
}

Pin* Script::CreatePin(const ConnectionInfo &info)
{
    if(info.type == PinType::Parameter) {
        ParameterPin *newPin = 0;
        switch(info.direction) {
        case PinDirection::Input :
            newPin = new ParameterPinIn(this,info.pinNumber,0,true,QString("ParamIn%1").arg(info.pinNumber));
            break;
        case PinDirection::Output :
            newPin = new ParameterPinOut(this,info.pinNumber,0,true,QString("ParamOut%1").arg(info.pinNumber));
            break;
        }
        newPin->SetLimitsEnabled(false);
        return newPin;
    }
    return Object::CreatePin(info);
}
