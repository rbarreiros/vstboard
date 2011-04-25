#ifndef SCRIPT_H
#define SCRIPT_H

#include "object.h"

namespace Connectables {

    class Script : public Object
    {
    Q_OBJECT

    public:
        Script(MainHost *host, int index, const ObjectInfo &info);
        bool Open();
        void Render();
        Pin* CreatePin(const ConnectionInfo &info);

    protected:
        QString objScriptName;

    public slots:
        void alert(const QString &str);
    };
}

#endif // SCRIPT_H
