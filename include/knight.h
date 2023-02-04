// knight.h
// Purpose: represent the knight hazards present on every level.
// Rotation logic adapted from the Trogdor Reburninated Knight class by Mips96

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_random.h>
#include "bn_sprite_items_knight.h"
#include "entity.h"
#include "session_info.h"

#include "constants.h"

namespace trog {
    class knight : public entity { 
        protected:
            
		    unsigned short _timer;
			unsigned short _rotation;
            bn::fixed _speed;

            short _cycletime;

        	bn::random _random; 
            bn::sprite_animate_action<4> _walkcycle;

			void change_direction();
            void update_anim() override final;
            
        public:
            knight(bn::fixed xcor, bn::fixed ycor, bool facingRight);
			virtual void update();
            void animate_faster() {_walkcycle.set_wait_updates(_walkcycle.wait_updates() / 2);}
			// void update_home(short knight_increment);
    };
}