#ifndef __VIRUS_STYLES__H
#define __VIRUS_STYLES__H

#include "enviro.h"

using namespace enviro;

const json  PLAYER_STYLE = { 
                {"fill", "red"}, 
                {"stroke", "black"}
            },
            GOLD_STYLE = { 
                {"fill", "gold"}, 
                {"stroke", "black"}
            },
            GHOST_STYLE = { 
                {"fill", "lightgreen"}, 
                {"stroke", "black"}
            },
            EMPTY_STYLE = { 
                {"fill", "none"}, 
                {"stroke", "none"}
            };

#endif