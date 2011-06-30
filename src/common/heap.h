#ifndef HEAPCHECK_H
#define HEAPCHECK_H

#ifdef DEBUGMEM
    #include <crtdbg.h>
    //#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
    #define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
    void HeapCheckpoint();
#else
    #define DEBUG_CLIENTBLOCK
#endif

#endif
