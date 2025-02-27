// knight.h
// Purpose: represent the knight hazards present on every level.
// Rotation logic adapted from the Trogdor Reburninated Knight class by Mips96

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_random.h>
#include "bn_sprite_items_knight.h"
#include "freezable.h"

#include "constants.h"

namespace trog {
    class knight : public freezable { 
        protected:

            knight(bn::fixed xcor, bn::fixed ycor, bool facingRight, bn::random &random, bn::fixed speed, uint8_t cycletime);

            
		    uint8_t _walkcycle_timer;
			unsigned short _rotation;
            
            const bn::fixed _speed;
            const uint8_t _cycletime;

        	bn::random &_random; 
            bn::sprite_animate_action<4> _walkcycle;
            bn::optional<bn::fixed_point> _alert_dist;

			void change_direction();
            
        public:
            knight(bn::fixed xcor, bn::fixed ycor, bool facingRight, bn::random &random) : 
                knight(xcor, ycor, facingRight, random, 
                TROG_KNIGHT_SPEED, TROG_KNIGHT_MOVE_CYCLE_TIME) {}
            
			virtual void update() override;
            void animate_faster() {_walkcycle.set_wait_updates(_walkcycle.wait_updates() / 2);}
			// void update_home(short knight_increment);
            void update_anim() override final;
            virtual void freeze() override {_freeze_timer = _cycletime;}
            void update_alert_direction(bn::fixed_point pos);
    };
}