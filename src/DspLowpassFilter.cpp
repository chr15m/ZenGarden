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

#include <math.h>
#include <stdlib.h>
#include "DspLowpassFilter.h"

DspLowpassFilter::DspLowpassFilter(int blockSize, int sampleRate, char *initString) :
    DspMessageInputDspOutputObject(2, 1, blockSize, initString) {
  this->sampleRate = (float) sampleRate;
  calculateFilterCoefficients((float) (sampleRate/2)); // initialise the filter completely open
  tap_0 = 0.0f;
}

DspLowpassFilter::DspLowpassFilter(float cutoffFrequency, int blockSize, int sampleRate, char *initString) : 
    DspMessageInputDspOutputObject(2, 1, blockSize, initString) {
  this->sampleRate = (float) sampleRate;
  calculateFilterCoefficients(cutoffFrequency);
  tap_0 = 0.0f;
}

DspLowpassFilter::~DspLowpassFilter() {
  // nothing to do
}

void DspLowpassFilter::calculateFilterCoefficients(float cutoffFrequency) {
  if (cutoffFrequency < 0.0f) {
    cutoffFrequency = 0.0f;
  }
  alpha = cutoffFrequency * 2.0f * M_PI / sampleRate;
  if (alpha > 1.0f) {
    alpha = 1.0f;
  }
  beta = 1.0f - alpha;
}

inline void DspLowpassFilter::processMessage(int inletIndex, PdMessage *message) {
  switch (inletIndex) {
    case 0: {
      MessageElement *messageElement = message->getElement(0);
      if (messageElement != NULL && messageElement->getType() == SYMBOL) {
        if (strcmp(messageElement->getSymbol(), "clear") == 0) {
          processDspToIndex(message->getBlockIndex());
          tap_0 = 0.0f;
        }
      }
      break;
    }
    case 1: {
      MessageElement *messageElement = message->getElement(0);
      if (messageElement != NULL && messageElement->getType() == FLOAT) {
        processDspToIndex(message->getBlockIndex());
        calculateFilterCoefficients(messageElement->getFloat());
      }
      break;
    }
    default: {
      break;
    }
  }
}

inline void DspLowpassFilter::processDspToIndex(int newBlockIndex) {
  // DspLowpassFilter only supports signalPresedence == DSP_MESSAGE
  if (signalPresedence == DSP_MESSAGE) {
    float *inputBuffer = localDspBufferAtInlet[0]; 
    float *outputBuffer = localDspBufferAtOutlet[0];
    outputBuffer[blockIndexOfLastMessage] = 
        (alpha * inputBuffer[blockIndexOfLastMessage]) + (beta * tap_0);
    for (int i = blockIndexOfLastMessage + 1; i < newBlockIndex; i++) {
      outputBuffer[i] = alpha * inputBuffer[i] + beta * outputBuffer[i-1];
    }
    tap_0 = outputBuffer[newBlockIndex-1];
  }
  blockIndexOfLastMessage = newBlockIndex;
}
