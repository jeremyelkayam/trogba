
#pragma once
#include <bn_string.h>
#include "common_stuff.h"


namespace trog {

    struct achievement
    {
        unsigned int sram_index;
        bn::string<32> name;
        bn::string<128> desc;
    };
}