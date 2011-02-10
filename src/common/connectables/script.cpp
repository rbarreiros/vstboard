#include "script.h"
#include "../mainhost.h"

using namespace Connectables;

Script::Script(MainHost *host, int index, const ObjectInfo &info) :
    Object(host,index,info)
{
//    QString prog="Programs.ChangeProg(2);";
//    debug2(<< "script:" << myHost->scriptEngine.evaluate(prog).toString())
}
