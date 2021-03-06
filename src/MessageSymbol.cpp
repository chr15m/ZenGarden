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

#include "MessageSymbol.h"
#include "StaticUtils.h"

MessageSymbol::MessageSymbol(char *initString) : MessageInputMessageOutputObject(1, 1, initString) {
  symbol = NULL;
  setSymbol("");
}

MessageSymbol::MessageSymbol(char *newSymbol, char *initString) : MessageInputMessageOutputObject(1, 1, initString) {
  symbol = NULL;
  setSymbol(newSymbol);
}

MessageSymbol::~MessageSymbol() {
  if (symbol == NULL) {
    free(symbol);
  }
}

void MessageSymbol::setSymbol(char *newSymbol) {
  if (symbol != NULL) {
    free(symbol);
  }
  symbol = StaticUtils::copyString(newSymbol);
}

void MessageSymbol::processMessage(int inletIndex, PdMessage *message) {
  if (inletIndex == 0) {
    MessageElement *messageElement = message->getElement(0);
    switch (messageElement->getType()) {
      case SYMBOL: {
        setSymbol(messageElement->getSymbol());
        // allow fallthrough
      }
      case BANG: {
        PdMessage *outgoingMessage = getNextOutgoingMessage(0);
        outgoingMessage->getElement(0)->setSymbol(symbol);
        outgoingMessage->setBlockIndex(message->getBlockIndex());
        break;
      }
      default: {
        break;
      }
    }
  }
}

PdMessage *MessageSymbol::newCanonicalMessage() {
  PdMessage *message = new PdMessage();
  message->addElement(new MessageElement());
  return message;
}
