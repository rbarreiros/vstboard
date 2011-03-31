#include "vstbankbase.h"


using namespace vst;


/*===========================================================================*/
/* CFxBase class members                                                     */
/*===========================================================================*/

static char szChnk[] = "CcnK";          /* set up swapping flag              */
static long lChnk = 'CcnK';
bool CFxBase::NeedsBSwap = !!memcmp(szChnk, &lChnk, 4);

/*****************************************************************************/
/* SwapBytes : swaps bytes for big/little-endian difference                  */
/*****************************************************************************/

void CFxBase::SwapBytes(long &l)
{
    unsigned char *b = (unsigned char *)&l;
    long intermediate =  ((long)b[0] << 24) |
                         ((long)b[1] << 16) |
                         ((long)b[2] << 8) |
                         (long)b[3];
    l = intermediate;
}

void CFxBase::SwapBytes(float &f)
{
    long *pl = (long *)&f;
    SwapBytes(*pl);
}

#if defined(VST_2_4_EXTENSIONS)
void CFxBase::SwapBytes(VstInt32 &vi)
{
    unsigned char *b = (unsigned char *)&vi;
    VstInt32 intermediate =  ((VstInt32)b[0] << 24) |
                             ((VstInt32)b[1] << 16) |
                             ((VstInt32)b[2] << 8) |
                             (VstInt32)b[3];
    vi = intermediate;
}
#endif

