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

#include "ceffect.h"
#include "cvsthost.h"


using namespace vst;

/*===========================================================================*/
/* CEffect class members                                                     */
/*===========================================================================*/

/*****************************************************************************/
/* CEffect : constructor                                                     */
/*****************************************************************************/

CEffect::CEffect()
{
    pEffect = NULL;
    bEditOpen = false;
    bNeedIdle = false;
    bInEditIdle = false;
    bWantMidi = false;
    bInSetProgram = false;
    pMasterEffect = NULL;
    sName.clear();
}

/*****************************************************************************/
/* ~CEffect : destructor                                                     */
/*****************************************************************************/

CEffect::~CEffect()
{
    Unload();
}

/*****************************************************************************/
/* Load : loads the effect module                                            */
/*****************************************************************************/

bool CEffect::Load(const QString &name)
{
    pluginLib = new QLibrary(name);
    if(!pluginLib->load())
        return false;

    vstPluginFuncPtr entryPoint = (vstPluginFuncPtr)pluginLib->resolve("VSTPluginMain");
    if(!entryPoint)
        entryPoint = (vstPluginFuncPtr)pluginLib->resolve("main");
    if(!entryPoint)
        return false;

    try
    {
        pEffect = entryPoint(CVSTHost::AudioMasterCallback);
    }
    catch(...)
    {
        pEffect = NULL;
    }

    if(!pEffect)
        return false;

    if (pEffect->magic != kEffectMagic) {
        pEffect = NULL;
        return false;
    }
    sName = name;
    return true;
}

/*****************************************************************************/
/* Unload : unloads effect module                                            */
/*****************************************************************************/

bool CEffect::Unload()
{
    EffClose();
    pEffect = NULL;
    pluginLib->unload();
    pluginLib->deleteLater();
    return true;
}

/*****************************************************************************/
/* LoadBank : loads a .fxb file ... IF it's for this effect                  */
/*****************************************************************************/

bool CEffect::LoadBank(std::string *name)
{

    try
    {
        CFxBank fx(name);                     /* load the bank                     */
        if (!fx.IsLoaded())                  /* if error loading                  */
            throw (int)1;
    }
    catch(...)                              /* if any error occured              */
    {
        return false;                         /* return NOT!!!                     */
    }

    return true;                            /* pass back OK                      */
}

/*****************************************************************************/
/* SaveBank : saves current sound bank to a .fxb file                        */
/*****************************************************************************/

bool CEffect::SaveBank(std::string * name)
{
    return false;                           /* return error for now              */
}

/*****************************************************************************/
/* EffDispatch : calls an effect's dispatcher                                */
/*****************************************************************************/

long CEffect::EffDispatch(VstInt32 opCode, VstInt32 index, VstInt32 value, void *ptr, float opt)
{
    if (!pEffect)
        return 0;

    long disp=0L;
        disp = pEffect->dispatcher(pEffect, opCode, index, value, ptr, opt);

    return disp;
}

/*****************************************************************************/
/* EffProcess : calles an effect's process() function                        */
/*****************************************************************************/

void CEffect::EffProcess(float **inputs, float **outputs, long sampleframes)
{
    pEffect->process(pEffect, inputs, outputs, sampleframes);
}

/*****************************************************************************/
/* EffProcessReplacing : calls an effect's processReplacing() function       */
/*****************************************************************************/

void CEffect::EffProcessReplacing(float **inputs, float **outputs, long sampleframes)
{
    pEffect->processReplacing(pEffect, inputs, outputs, sampleframes);
 }

/*****************************************************************************/
/* EffProcessDoubleReplacing : calls an effect's processDoubleReplacing() f. */
/*****************************************************************************/

void CEffect::EffProcessDoubleReplacing(double **inputs, double **outputs, long sampleFrames)
{
#if defined(VST_2_4_EXTENSIONS)
    pEffect->processDoubleReplacing(pEffect, inputs, outputs, sampleFrames);
#endif
}

/*****************************************************************************/
/* EffSetParameter : calls an effect's setParameter() function               */
/*****************************************************************************/

void CEffect::EffSetParameter(long index, float parameter)
{
        pEffect->setParameter(pEffect, index, parameter);
}

/*****************************************************************************/
/* EffGetParameter : calls an effect's getParameter() function               */
/*****************************************************************************/

float CEffect::EffGetParameter(long index)
{
    return pEffect->getParameter(pEffect, index);
}

long CEffect::OnMasterCallback(long opcode, long index, long value, void *ptr, float opt,long currentReturnCode)
{
    switch(opcode) {
        case audioMasterWantMidi : //6
            bWantMidi=true;
            return true;
    }

    return currentReturnCode;
}

QString CEffect::EffGetParamName(long index)
{
    char txt[32] ={0};
    EffDispatch(effGetParamName, index, 0, txt);
    return QString::fromAscii(txt);

}
