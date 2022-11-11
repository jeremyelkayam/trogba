#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_player.h"
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "constants.h"
#include "firebreath.h"
#include "session_info.h"

namespace trog {
    class player : public entity { 
        private:
            const bn::fixed _speed;
 
            bn::sprite_animate_action<4> _walkcycle;
            
            unsigned short _trogmeter;
            unsigned int burninate_time;
            const unsigned short trogmeter_max = TROG_TROGMETER_MAX;
            const int burninate_length = TROG_BURNINATE_TIME; //temporarily this can be 10 seconds 

            firebreath breath;
            session_info &_sesh;

            void move();
            void check_boundary_collision();
        public:
            player(session_info &sesh);
            virtual void update() final;
            bool burninating();
            void check_cottage_collision(cottage &cottage);
            void check_peasant_collision(peasant &peasant);
            unsigned short get_trogmeter(){return _trogmeter;}
            unsigned short get_burninating_time(){return burninate_time;}

    };
}

// void debug_adjust(bn::regular_bg_ptr bg){
//     //debug adjustment -- move a background around with the dpad and log the coords
//     if(bn::keypad::left_held()){
//         bg.set_x(bg.x() - 1);
//         BN_LOG("xcor ", bg.x());
//     }
//     else if(bn::keypad::right_held()){
//         bg.set_x(bg.x() + 1);
//         BN_LOG("xcor ", bg.x());
//     }
//     if(bn::keypad::up_held()){
//         bg.set_y(bg.y() - 1);
//         BN_LOG("ycor", bg.y());
//     }
//     else if(bn::keypad::down_held()){
//         bg.set_y(bg.y() + 1);
//         BN_LOG("ycor", bg.y());
//     }
// }