/*
 *  Copyright 2009 Reality Jockey, Ltd.
 *                 info@rjdj.me
 *                 http://rjdj.me/
 * 
 *  This file is part of ZenGarden.
 *
 *  ZenGarden is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ZenGarden is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with ZenGarden.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "DspInletOutlet.h"

DspInletOutlet::DspInletOutlet(int blockSize, char *initString) :
    DspInputDspOutputObject(1, 1, blockSize, initString) {
  // nothing to do
}

DspInletOutlet::~DspInletOutlet() {
  // nothing to do
}

void DspInletOutlet::processDspToIndex(int newBlockIndex) {
  memcpy(localDspBufferAtOutlet[0], localDspBufferAtInlet[0], numBytesInBlock);
}
