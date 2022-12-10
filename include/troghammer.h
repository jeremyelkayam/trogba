// knight.h
// Purpose: encapsulate the TROGHAMMER who appears on later levels

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_random.h>
#include "bn_sprite_items_knight.h"
#include "knight.h"

namespace trog {
    class troghammer : public knight { 
        private:

        public:
            troghammer(int xcor, int ycor, bool facingRight);
    };
}