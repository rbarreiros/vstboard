#include "heap.h"

#ifndef QT_NO_DEBUG
    _CrtMemState *checkPt=0;

    void HeapCheckpoint() {

        if(checkPt) {
            _CrtMemDumpAllObjectsSince( checkPt );

    //        _CrtMemState checkPt2,checkPt3;
    //        _CrtMemCheckpoint( &checkPt2 );
    //        _CrtMemDifference(&checkPt3,checkPt,&checkPt2);
    //        _CrtMemDumpStatistics( &checkPt3 );
    //        _CrtCheckMemory( );
            delete checkPt;
        }
        checkPt = new _CrtMemState;
        _CrtMemCheckpoint( checkPt );
    }
#endif


