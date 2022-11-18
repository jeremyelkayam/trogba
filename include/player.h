#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_trogdor_arrowed.h"
#include "bn_sprite_items_trogdor_sworded.h"
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "constants.h"
#include "firebreath.h"
#include "session_info.h"
#include "knight.h"
#include "archer.h"

namespace trog {
    class player : public entity { 
        private:
            const bn::fixed _speed;
 
            bn::sprite_animate_action<4> _walkcycle;
            
            unsigned short _trogmeter;
            unsigned int _burninate_time;
            const unsigned short _trogmeter_max = TROG_TROGMETER_MAX;
            const int _burninate_length = TROG_BURNINATE_TIME; //temporarily this can be 10 seconds 
            unsigned short _time_dead;
            unsigned short _iframes;

            firebreath _breath;
            session_info &_sesh;

            void move();
            void check_boundary_collision();
            void respawn();

            // 
            bool invincible() {return _iframes;}
            bool dead() {return _time_dead;}
            void die(bn::sprite_item item);

        public:
            player(session_info &sesh);
            virtual void update() final;
            bool burninating();
            void handle_cottage_collision(cottage &cottage);
            void handle_peasant_collision(peasant &peasant);
            void handle_knight_collision(knight &knight);
            void handle_arrow_collision(archer &archer);

            unsigned short get_trogmeter(){return _trogmeter;}
            unsigned short get_burninating_time(){return _burninate_time;}

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