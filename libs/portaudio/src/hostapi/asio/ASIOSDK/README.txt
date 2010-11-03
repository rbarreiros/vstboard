we only need the 'common' and 'host' folders found in the ASIO SDK

|- asio
|  |- ASIOSDK
|  |  |- common
|  |  |- host
|  |  |  |- mac
|  |  |  |- pc

  
Macintosh ASIO SDK Bug Patch (from the portaudio documentation) 
===============================================================
(just in case, VstBoard is not ported on Mac "yet")

There is a bug in the ASIO SDK that causes the Macintosh version to
often fail during initialization. Below is a patch that you can apply.

In codefragments.cpp replace getFrontProcessDirectory function with
the following one (GetFrontProcess replaced by GetCurrentProcess).


bool CodeFragments::getFrontProcessDirectory(void *specs)
{
        FSSpec *fss = (FSSpec *)specs;
        ProcessInfoRec pif;
        ProcessSerialNumber psn;

        memset(&psn,0,(long)sizeof(ProcessSerialNumber));
        //  if(GetFrontProcess(&psn) == noErr)  // wrong !!!
        if(GetCurrentProcess(&psn) == noErr)  // correct !!!
        {
                pif.processName = 0;
                pif.processAppSpec = fss;
                pif.processInfoLength = sizeof(ProcessInfoRec);
                if(GetProcessInformation(&psn, &pif) == noErr)
                                return true;
        }
        return false;
}
