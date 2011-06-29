#ifndef HEAPCHECK_H
#define HEAPCHECK_H

#ifdef _DEBUG
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif


#endif // HEAPCHECK_H
