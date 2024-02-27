#ifndef STONEHELPER_H
#define STONEHELPER_H

#include <string>

enum Stone{
    RED,
    BLUE
};

class StoneHelper {
public:
    static char getName(Stone stone);
    /**
     * returns the name of the stone,
     * used for identification of the stone
     */
};

#endif  // STONEHELPER_H
