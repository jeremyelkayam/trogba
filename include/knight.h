// knight.h
// Purpose: represent the knight hazards present on every level.
// Adapted from the Trogdor Reburninated Knight class by Mips96

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_knight.h"
#include "entity.h"
#include "session_info.h"

#include "constants.h"

namespace trog {
// class Knight {
// 	public:
// 		
// 		inline void updateCollision();
// 		void updateHome(Sint8);
// 		void updateFrameStateAndMove();
// };
    class knight : public entity { 
        private:
            
		    unsigned short _frameState;
		    bool _moving;      // used in toggleKnightMotion()
		    int _half_src_w;
		    int _half_src_h;
		    short _direction;
		    int _home_x;    // the parent (x,y) coordinates
		    int _home_y;    // the parent (x,y) coordinates
		    int _offset_x;  // the offset relative to home
		    int _offset_y;  // the offset relative to home

 
            bn::sprite_animate_action<4> _walkcycle;
        public:
            knight(int xcor, int ycor, short dir, bool facingRight);
			virtual void update() final;
			void update_home(short knight_increment);
    };
}