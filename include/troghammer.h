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
            // The amount of time until the troghammer spawns.
            unsigned short _waiting_time;

        public:
            troghammer(int xcor, int ycor, bool facingRight, int level);
            virtual void update() final;
            unsigned short get_waiting_time() {return _waiting_time;}
    };
}