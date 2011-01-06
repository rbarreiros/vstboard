/******************************************************************************
#   Copyright 2006 Hermann Seib
#   Copyright 2010 Raphaël François
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

#include "cvsthost.h"                   /* private prototypes                */
#include "ceffect.h"
#include "../connectables/vstplugin.h"

using namespace vst;

/*===========================================================================*/
/* CVSTHost class members                                                    */
/*===========================================================================*/

CVSTHost * CVSTHost::pHost = NULL;      /* pointer to the one and only host  */

/*****************************************************************************/
/* CVSTHost : constructor                                                    */
/*****************************************************************************/

CVSTHost::CVSTHost()
{
    if (pHost)                              /* disallow more than one host!      */
        return;//throw((int)1);
    vstTimeInfo.samplePos = 0.0;
    vstTimeInfo.sampleRate = 44100.0;
    vstTimeInfo.nanoSeconds = 0.0;
    vstTimeInfo.ppqPos = 0.0;
    vstTimeInfo.tempo = 120.0;
    vstTimeInfo.barStartPos = 0.0;
    vstTimeInfo.cycleStartPos = 0.0;
    vstTimeInfo.cycleEndPos = 0.0;
    vstTimeInfo.timeSigNumerator = 4;
    vstTimeInfo.timeSigDenominator = 4;
    vstTimeInfo.smpteOffset = 0;
    vstTimeInfo.smpteFrameRate = 1;
    vstTimeInfo.samplesToNextClock = 0;
    vstTimeInfo.flags = 0;

    vstTimeInfo.flags |= kVstTransportChanged;//     = 1,		///< indicates that play, cycle or record state has changed
    vstTimeInfo.flags |= kVstTransportPlaying;//     = 1 << 1,	///< set if Host sequencer is currently playing
    vstTimeInfo.flags |= kVstTransportCycleActive;// = 1 << 2,	///< set if Host sequencer is in cycle mode
    //vstTimeInfo.flags |= kVstTransportRecording;//   = 1 << 3,	///< set if Host sequencer is in record mode
    //vstTimeInfo.flags |= kVstAutomationWriting;//    = 1 << 6,	///< set if automation write mode active (record parameter changes)
    vstTimeInfo.flags |= kVstAutomationReading;//    = 1 << 7,	///< set if automation read mode active (play parameter changes)
    vstTimeInfo.flags |= kVstNanosValid;//           = 1 << 8,	///< VstTimeInfo::nanoSeconds valid
    vstTimeInfo.flags |= kVstPpqPosValid;//          = 1 << 9,	///< VstTimeInfo::ppqPos valid
    vstTimeInfo.flags |= kVstTempoValid;//           = 1 << 10,	///< VstTimeInfo::tempo valid
    vstTimeInfo.flags |= kVstBarsValid;//            = 1 << 11,	///< VstTimeInfo::barStartPos valid
    vstTimeInfo.flags |= kVstCyclePosValid;//        = 1 << 12,	///< VstTimeInfo::cycleStartPos and VstTimeInfo::cycleEndPos valid
    vstTimeInfo.flags |= kVstTimeSigValid;//         = 1 << 13,	///< VstTimeInfo::timeSigNumerator and VstTimeInfo::timeSigDenominator valid
    vstTimeInfo.flags |= kVstSmpteValid;//           = 1 << 14,	///< VstTimeInfo::smpteOffset and VstTimeInfo::smpteFrameRate valid
    vstTimeInfo.flags |= kVstClockValid;


    loopLenght=4;
    pHost = this;                           /* install this instance as the one  */
}

/*****************************************************************************/
/* ~CVSTHost : destructor                                                    */
/*****************************************************************************/

CVSTHost::~CVSTHost()
{
    if (pHost == this)                      /* if we're the chosen one           */
        pHost = NULL;                         /* remove ourselves from pointer     */
}

void CVSTHost::SetTimeInfo(const VstTimeInfo *info) {
    if (!info)
        return;

    vstTimeInfo = *info;
}

void CVSTHost::UpdateTimeInfo(double timer, int addSamples, double sampleRate)
{
    vstTimeInfo.sampleRate = sampleRate;
    vstTimeInfo.nanoSeconds = timer * 1000000.0L;
    vstTimeInfo.samplePos += addSamples;

    //bar length in quarter notes
    int barLengthq = 4*vstTimeInfo.timeSigNumerator/vstTimeInfo.timeSigDenominator;


    vstTimeInfo.cycleEndPos = loopLenght*barLengthq;
    vstTimeInfo.cycleStartPos = 0;

    // we don't care for the mask in here
    static double fSmpteDiv[] =
    {
            24.f,
            25.f,
            24.f,
            30.f,
            29.97f,
            30.f
    };

    double dPos = vstTimeInfo.samplePos / vstTimeInfo.sampleRate;
    vstTimeInfo.ppqPos = dPos * vstTimeInfo.tempo / 60.L;
    /* offset in fractions of a second   */
    double dOffsetInSecond = dPos - floor(dPos);
    vstTimeInfo.smpteOffset = (long)(dOffsetInSecond * fSmpteDiv[vstTimeInfo.smpteFrameRate] * 80.L);

    if(vstTimeInfo.ppqPos > vstTimeInfo.cycleEndPos) {
        vstTimeInfo.samplePos = 0;
        vstTimeInfo.ppqPos = 0;
        vstTimeInfo.smpteOffset = 0;

    }

    //start of last bar
    vstTimeInfo.barStartPos = barLengthq*floor(vstTimeInfo.ppqPos/barLengthq);
}

/*****************************************************************************/
/* AudioMasterCallback : callback to be called by plugins                    */
/*****************************************************************************/
VstIntPtr VSTCALLBACK CVSTHost::AudioMasterCallback(AEffect *effect, VstInt32 opcode, VstInt32  index, VstInt32  value, void *ptr, float opt)
{
    long retValue = 0L;
    const char* str;

    switch(opcode) {
        case audioMasterVersion : //1
            return 2400L;

        case audioMasterWantMidi : //6
            retValue = true;
            break;

        case audioMasterGetTime : //7
            return (long)&pHost->vstTimeInfo;

        case audioMasterSetTime : //9
            {
                VstTimeInfo *ti = (VstTimeInfo*)ptr;

                if(value &= kVstTempoValid) {
                    pHost->vstTimeInfo.tempo = ti->tempo;
                }

                if(value &= kVstTimeSigValid) {
                    pHost->vstTimeInfo.timeSigNumerator = ti->timeSigNumerator;
                    pHost->vstTimeInfo.timeSigDenominator = ti->timeSigDenominator;
                }
            }

            return 0L;

        case audioMasterTempoAt : //10
            return 1000L*pHost->vstTimeInfo.tempo;

        case audioMasterGetCurrentProcessLevel : //23
            return 2L;

        case audioMasterGetVendorString : //32
            strcpy((char *)ptr,"CtrlBrk");
            return 1L;

        case audioMasterGetProductString : //33
            strcpy((char *)ptr,"VstBoard");
            return 1L;

        case audioMasterGetVendorVersion : //34
            return 0001L;

        case audioMasterCanDo : //37
            str = (const char*)ptr;

            //we take care of this list :
            if ((!strcmp(str, "sendVstEvents")) ||
                (!strcmp(str, "sendVstMidiEvent")) ||
                (!strcmp(str, "receiveVstEvents")) ||
                (!strcmp(str, "receiveVstMidiEvent")) ||
                (!strcmp(str, "sizeWindow")) ||
                (!strcmp(str, "sendVstMidiEventFlagIsRealtime")) ||
                (!strcmp(str, "reportConnectionChanges")) ||
                (!strcmp(str, "acceptIOChanges")) ||
                (!strcmp(str, "sendVstTimeInfo")) ||
                (!strcmp(str, "openFileSelector")) ||
                (!strcmp(str, "closeFileSelector")) ||
                (!strcmp(str, "supplyIdle")) ||
                (!strcmp(str, "receiveVstTimeInfo")) ||
                0)
                return 1L;

            //let our object see this one :
            if(strcmp(str, "shellCategory")) {

                //ignore the rest
                debug("host can't do '%s'",str)
                return 0L;
            }

#ifndef QT_NO_DEBUG
        //handled by the object
        case audioMasterAutomate : //0
        case audioMasterCurrentId : //2
        case audioMasterIdle : //3
        case audioMasterProcessEvents : //8
        case audioMasterGetNumAutomatableParameters : //11
        case audioMasterIOChanged : //13
        case audioMasterSizeWindow : //15
        case audioMasterUpdateDisplay : //42
        case audioMasterBeginEdit : //43
        case audioMasterEndEdit : //44
            break;

        default:
            debug(QString("vstcallback %1 not handled").arg(opcode).toAscii())
#endif
    }

    if(!effect) {
        debug("cvsthost : mastercallback from a null plugin ?")
        return 0L;
    }

    //send it to the connectable object
//    foreach(Connectables::VstPlugin *plug, Connectables::VstPlugin::listPlugins) {
//        if(plug->GetPlugin() == effect)
//            return plug->OnMasterCallback(opcode,index,value,ptr,opt,retValue);
//    }
    Connectables::VstPlugin *obj = Connectables::VstPlugin::mapPlugins.value(effect,0);
    if(obj)
        return obj->OnMasterCallback(opcode,index,value,ptr,opt,retValue);

    //unknown pointer, it must be the plugin currently loading
    if(Connectables::VstPlugin::pluginLoading) {
        debug("cvsthost : callback from the loading plugin")
        return Connectables::VstPlugin::pluginLoading->OnMasterCallback(opcode,index,value,ptr,opt,retValue);
    }

    debug("cvsthost : plugin not found")
    return 0L;

}
