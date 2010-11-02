/******************************************************************************
#   Copyright 2006, 2010 Hermann Seib, Raphaël François
#    Contact : ctrlbrk76@gmail.com
#
#    This file is part of VstBoard.
#
#    VstBoard is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    VstBoard is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with VstBoard.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

/*****************************************************************************/
/* CVSTHost.h: interface for the CVSTHost class.                             */
/*****************************************************************************/

/******************************************************************************
Copyright (C) 2006  Hermann Seib

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
******************************************************************************/

#if !defined(VSTHOST_H__INCLUDED_)
#define VSTHOST_H__INCLUDED_

#ifdef _MSC_VER
#pragma warning( disable: 4100 )
#endif

#include "../precomp.h"
#include "const.h"
#include "vstbank.h"


namespace vst
{

    /*****************************************************************************/
    /* CVSTHost class declaration                                                */
    /*****************************************************************************/

    class CVSTHost
    {
    friend class CEffect;
    public:
        CVSTHost();
        virtual ~CVSTHost();

        void UpdateTimeInfo(double timer, int addSamples, double sampleRate);

        int loopLenght;
         VstTimeInfo vstTimeInfo;

    protected:
        static CVSTHost * pHost;
        static VstIntPtr VSTCALLBACK AudioMasterCallback(AEffect *effect, VstInt32 opcode, VstInt32  index, VstInt32  value, void *ptr, float opt);
    };

}
#endif // !defined(VSTHOST_H__INCLUDED_)
