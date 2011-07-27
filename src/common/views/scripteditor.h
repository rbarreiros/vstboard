#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

//#include "precomp.h"

namespace Ui {
    class ScriptEditor;
}
namespace Connectables {
    class Script;
}
namespace View {

    class ScriptEditor : public QWidget
    {
        Q_OBJECT

    public:
        explicit ScriptEditor(QWidget *parent = 0);
        ~ScriptEditor();
        void SetScript(const QString &script);
        void SetObject(Connectables::Script *script);

    protected:
        void closeEvent ( QCloseEvent * event );

    signals:
        void Execute(const QString &str);
        void Hide();

    private slots:
        void on_btExecute_clicked();

    private:
        Ui::ScriptEditor *ui;
        Connectables::Script *object;
    };
}
#endif // SCRIPTEDITOR_H
