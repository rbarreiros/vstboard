#ifndef SCRIPT_H
#define SCRIPT_H

#include "object.h"

namespace Connectables {

    class Script : public Object
    {
    Q_OBJECT
    public:
        Script(MainHost *host, int index, const ObjectInfo &info);
    };
}

#endif // SCRIPT_H
