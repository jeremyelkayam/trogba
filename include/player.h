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

            bn::fixed_point _direction;
 
            bn::sprite_animate_action<4> _walkcycle;
            
            unsigned short _trogmeter;
            unsigned int _burninate_time;
            const unsigned short _trogmeter_max = TROG_TROGMETER_MAX;
            const int _burninate_length = TROG_BURNINATE_TIME; //temporarily this can be 10 seconds 
            unsigned short _time_dead;
            unsigned short _iframes;

            firebreath _breath;
            session_info &_sesh;

            //it's probably bad for this to have a reference to the cottage
            // vector, but I just need to get this working!!
            // TODO: Refactor trogdor to have direction
            bn::vector<cottage, 10> &_cottages;

            void move();
            void check_boundary_collision();

            bool invincible() {return _iframes;}
            void die(bn::sprite_item item);

            //todo: this doesn't need to be in this class
            bool any_dpad_input();

        public:
            player(session_info &sesh, bn::vector<cottage, 10> &cottages, bool iframes);
            virtual void update() final;
            bool burninating();
            void handle_cottage_collision(cottage &cottage);
            void handle_peasant_collision(peasant &peasant);
            void handle_knight_collision(knight &knight);
            void handle_arrow_collision(archer &archer);
            bool dead() {return _time_dead;}
            bool ready_to_respawn() {return _time_dead == TROG_RESPAWN_TIME;}

            unsigned short get_trogmeter(){return _trogmeter;}
            unsigned short get_burninating_time(){return _burninate_time;}

    };
}