//
// Created by Ilya on 30.05.2023.
//

#include "StoneHelper.h"

char StoneHelper::getName(Stone stone) {
    if (stone == Stone::RED) {
        return 'R';
    }
    return 'B';
}