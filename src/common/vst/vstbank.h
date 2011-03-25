/******************************************************************************
#   Copyright 2006 Hermann Seib
#   Copyright 2010 Rapha�l Fran�ois
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    under the terms of the GNU Lesser General Public License for more details.
#
#    You should have received a copy of the under the terms of the GNU Lesser General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef VSTBANK_H
#define VSTBANK_H

#include "../precomp.h"
#include "aeffeditor.h"

namespace vst
{
    const int MyVersion = 1;                /* highest known VST FX version      */

    /*===========================================================================*/
    /* structures for .fxb / .fxp files                                          */
    /*===========================================================================*/

    // These structures haven't been officially documented before VST SDK V2.2.
    // Since then, they have undergone quite "interesting" changes in the SDK,
    // which makes it quite a pain to maintain a link between them and VSTHost's
    // file-handling classes. That is simply too dumb.
    // From now on, I'll use my OWN structures to describe the contents of
    // .fxb / .fxp files. In C++ style, which is 10 times easier to read
    // and maintain. Of course, they'll match the latest available SDK.

    /*****************************************************************************/
    /* Constants used in the files - copied verbatim from vstfxstore.h           */
    /*****************************************************************************/

#define cMagic                  'CcnK'
#define fMagic                  'FxCk'
#define bankMagic               'FxBk'
#define chunkGlobalMagic	'FxCh'
#define chunkPresetMagic	'FPCh'
#define chunkBankMagic		'FBCh'

    /*****************************************************************************/
    /* SFxHeader : header for all chunks                                         */
    /*****************************************************************************/

    struct SFxHeader
    {
        VstInt32 chunkMagic;                  /* 'CcnK' in any case                */
        VstInt32 byteSize;                    /* size of this chunk, excluding     */
                                              /* chunkMagic & byteSize             */
    };

    /*****************************************************************************/
    /* SFxBase : base structure for all chunks                                   */
    /*****************************************************************************/

    struct SFxBase : public SFxHeader
    {
        VstInt32 fxMagic;                     /* for programs:                     */
        /*   'FxCk' (regular) or             */
        /*   'FPCh' (opaque chunk)           */
        /* for banks:                        */
        /*   'FxBk' (regular) or             */
        /*   'FBCh' (opaque chunk)           */
        VstInt32 version;                     /* format version                    */
        /* for programs: 1                   */
        /* for banks: 1 or 2                 */
        VstInt32 fxID;                        /* fx unique ID                      */
        VstInt32 fxVersion;                   /* fx version                        */
    };

    /*****************************************************************************/
    /* SFxProgramBase : base structure for all programs                          */
    /*****************************************************************************/

    struct SFxProgramBase : public SFxBase
    {
        VstInt32 numParams;                   /* number of parameters              */
        //std::string * prgName;                     /* program name (ASCIIZ)             */
        char prgName[28];
    };

    /*****************************************************************************/
    /* SFxProgram : a program in single-parameter style                          */
    /*****************************************************************************/

    struct SFxProgram : public SFxProgramBase
    {
        float params[1];                      /* in reality, a variable-sized array*/
        /* with numParams parameter values   */
    };

    /*****************************************************************************/
    /* SFxProgramChunk : a program in chunk style                                */
    /*****************************************************************************/

    struct SFxProgramChunk : public SFxProgramBase
    {
        VstInt32 size;                        /* size of program data in bytes     */
        char chunk[1];                        /* variable-sized opaque array       */
    };

    /*****************************************************************************/
    /* SFxBankBase : base structure for a bank of programs                       */
    /*****************************************************************************/

    struct SFxBankBase : public SFxBase
    {
        VstInt32 numPrograms;                 /* number of programs in bank        */
        VstInt32 currentProgram;              /* if version 2, current program,    */
        /* else irrelevant                   */
        char future[124];                     /* reserved (should be zero)         */
    };

    /*****************************************************************************/
    /* SFxBank : structure for a bank of programs consisting of parameters       */
    /*****************************************************************************/

    struct SFxBank : public SFxBankBase
    {
        SFxProgram programs[1];               /* variable number of programs       */
    };

    /*****************************************************************************/
    /* SFxBankChunk : structure for a bank of programs as an opaque chunk        */
    /*****************************************************************************/

    struct SFxBankChunk : public SFxBankBase
    {
        VstInt32 size;                        /* size of bank data in bytes        */
        char chunk[1];                        /* variable-sized opaque array       */
    };

#if !defined(VST_2_3_EXTENSIONS)
    struct VstSpeakerArrangement;
    struct VstPatchChunkInfo;
#endif

#if !defined(VST_2_1_EXTENSIONS)
    enum                                    /* V2.1 dispatcher opcodes           */
    {
        effEditKeyDown = effNumV2Opcodes,
        effEditKeyUp,
        effSetEditKnobMode,
        effGetMidiProgramName,
        effGetCurrentMidiProgram,
        effGetMidiProgramCategory,
        effHasMidiProgramsChanged,
        effGetMidiKeyName,
        effBeginSetProgram,
        effEndSetProgram,
        effNumV2_1Opcodes
    };
#endif

#if !defined(VST_2_3_EXTENSIONS)
    enum                                    /* V2.3 dispatcher opcodes           */
    {
        effGetSpeakerArrangement = effNumV2_1Opcodes,
        effShellGetNextPlugin,
        effStartProcess,
        effStopProcess,
        effSetTotalSampleToProcess,
        effSetPanLaw,
        effBeginLoadBank,
        effBeginLoadProgram,
        effNumV2_3Opcodes
    };
#endif

#if !defined(VST_2_4_EXTENSIONS)
    enum                                    /* V2.4 dispatcher opcodes           */
    {
        effSetProcessPrecision = effNumV2_3Opcodes,
        effGetNumMidiInputChannels,
        effGetNumMidiOutputChannels,
        effNumV2_4Opcodes
    };
    enum                                    /* V2.4 flags                        */
    {
        effFlagsCanDoubleReplacing = 1 << 12,
    };
    enum VstMidiEventFlags                  /* V2.4 MIDI Event flags             */
    {
        kVstMidiEventIsRealtime = 1 << 0
                              };
    enum VstAutomationStates                /* V2.4 automation state definitions */
    {
        kVstAutomationUnsupported = 0,
        kVstAutomationOff,
        kVstAutomationRead,
        kVstAutomationWrite,
        kVstAutomationReadWrite
    };
#endif

    //#if defined(VST_2_4_EXTENSIONS)
    //#include "aeffeditor.h"
    //#else
    //#include "AEffEditor.hpp"
    //#endif



    /*****************************************************************************/
    /* CFxBase : base class for FX Bank / Program Files                          */
    /*****************************************************************************/

    class CFxBase
    {
    public:
        VstPatchChunkInfo *GetPatchChunkInfo() {return &patchChunkInfo;}

    protected:
        static bool NeedsBSwap;
        VstPatchChunkInfo patchChunkInfo;

    protected:
        static void SwapBytes(float &f);
        static void SwapBytes(long &l);
#if defined(VST_2_4_EXTENSIONS)
        static void SwapBytes(VstInt32 &vi);
#endif

    };

    /*****************************************************************************/
    /* CFxBank : class for an Fx Bank file                                       */
    /*****************************************************************************/

    class CFxBank : public CFxBase
    {
    public:
        CFxBank(std::string *pszFile = 0);
        CFxBank(int nPrograms, int nParams);
        CFxBank(int nChunkSize);
        CFxBank(CFxBank const &org) { DoCopy(org); }
        virtual ~CFxBank();
        CFxBank & operator=(CFxBank const &org) { return DoCopy(org); }
    public:
        bool SetSize(int nPrograms, int nParams);
        bool SetSize(int nChunkSize);
        bool LoadBank(std::string *pszFile);
        bool SaveBank(std::string *pszFile);
        void Unload();
        bool IsLoaded() { return !!bBank; }
        bool IsChunk() { return bChunk; }

        // access functions
    public:
        long GetVersion() { if (!bBank) return 0; return ((SFxBase*)bBank)->version; }
        long GetFxID() { if (!bBank) return 0; return ((SFxBase*)bBank)->fxID; }
        void SetFxID(long id) { if (bBank) ((SFxBase*)bBank)->fxID = id; if (!bChunk) for (int i = GetNumPrograms() -1; i >= 0; i--) GetProgram(i)->fxID = id; }
        long GetFxVersion() { if (!bBank) return 0; return ((SFxBase*)bBank)->fxVersion; }
        void SetFxVersion(long v) { if (bBank) ((SFxBase*)bBank)->fxVersion = v; if (!bChunk) for (int i = GetNumPrograms() -1; i >= 0; i--) GetProgram(i)->fxVersion = v; }
        long GetNumPrograms() { if (!bBank) return 0; return ((SFxBankBase*)bBank)->numPrograms; }
        long GetNumParams() { if (bChunk) return 0; return GetProgram(0)->numParams; }
        long GetChunkSize() { if (!bChunk) return 0; return ((SFxBankChunk *)bBank)->size; }
        void *GetChunk() { if (!bChunk) return 0; return ((SFxBankChunk *)bBank)->chunk; }
        bool SetChunk(void *chunk) { if (!bChunk) return false; memcpy(((SFxBankChunk *)bBank)->chunk, chunk, ((SFxBankChunk *)bBank)->size); return true; }

        SFxProgram * GetProgram(int nProgNum);

        char * GetProgramName(int nProgram);
        void SetProgramName(int nProgram, char *name);
        float GetProgParm(int nProgram, int nParm);
        bool SetProgParm(int nProgram, int nParm, float val = 0.0);

    protected:
        std::string szFileName;
        unsigned char * bBank;
        int nBankLen;
        bool bChunk;

    protected:
        void Init();
        CFxBank & DoCopy(CFxBank const &org);
    };

    /*****************************************************************************/
    /* CFxProgram : class for an Fx Program file                                 */
    /*****************************************************************************/

    // not really designed yet...
    class CFxProgram : public CFxBase
    {
    };

}

#endif // VSTBANK_H
