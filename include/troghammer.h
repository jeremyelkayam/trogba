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
            unsigned short _waiting_time, _initial_waiting_time;
            static constexpr short daytime_path[240] {-56, -56, -57, -57, -57, -58, -58, -59, -59, -59, -60, -60, -60, -60, -61, -61, -61, -61, -61, -61, -61, -61, -61, -61, -61, -60, -60, -60, -60, -59, -59, -58, -58, -58, -57, -57, -57, -57, -57, -56, -56, -56, -56, -56, -55, -55, -55, -55, -55, -55, -55, -55, -55, -56, -56, -56, -56, -56, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -57, -56, -56, -55, -55, -55, -55, -55, -55, -54, -54, -54, -54, -54, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -53, -54, -54, -54, -54, -54, -54, -55, -55, -55, -55, -55, -56, -56, -56, -57, -57, -57, -58, -58, -58, -58, -58, -59, -59, -59, -60, -60, -60, -60, -61, -61, -61, -61, -61, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -62, -61, -61, -61, -61, -61, -61, -61, -60, -60, -60, -60, -60, -59, -59, -59, -59, -58, -58, -58, -57, -57, -56, -56, -56, -55, -55, -55, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -54, -55, -55, -55, -55, -55, -55, -56, -56, -56, -56, -56, -56, -57, -57, -57, -57, -57, -58, -58, -58, -58, -58, -58, -58};


            void change_direction();


        public:
            troghammer(const bn::fixed_point &pos, bool facingRight, int level);
            virtual void update() final;
            // unsigned short get_waiting_time() {return _waiting_time;}
            bool in_play(){return _waiting_time == 0;}

            bool awake_alert(){return _waiting_time == _initial_waiting_time * 2 / 3;}
            bool arrived_alert() {return _waiting_time == 1;}
            bool coming_alert(){return _waiting_time == _initial_waiting_time / 3;}
    };
}