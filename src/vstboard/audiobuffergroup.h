/******************************************************************************
#    Copyright 2010 Raphaël François
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

#ifndef AUDIOBUFFERGROUP_H
#define AUDIOBUFFERGROUP_H

#include "precomp.h"
#include "audiobuffer.h"

class AudioBufferGroup
{
public:
        AudioBufferGroup(bool externalAllocation = false);
        ~AudioBufferGroup(void);
        bool AllocBuffers(int nbBuffers, long bufZise);
        inline AudioBuffer * GetBuffer (int cpt) const;
        void GetAllBuffers(float ** buf);
        long GetSize() const {return nSize;}
        int GetNbBuffers() const {return nCptBuffers;}
        AudioBuffer * AddBuffer();
        bool RemoveBuffer(int nb=1);
        void SetAllBuffers(float ** buff);
        bool SetSize(long size);
        bool SetNbBuffers(int nbBuff);
        bool ClearBuffer(int cpt);
        bool ClearBuffer(float *buff);
        void ClearAllBuffer();

protected:
        void DeleteAllBuffers();

        bool bExternalAllocation;

        AudioBuffer ** pBuffers;

        //number of used buffers
        int nCptBuffers;
        //usable size of buffers
        long nSize;

        //number of allocated buffers
        int nAllocatedCptBuffers;
};


#endif // AUDIOBUFFERGROUP_H
