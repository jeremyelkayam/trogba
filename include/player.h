#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
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

            bn::sprite_ptr _majesty;

            bn::fixed_point _direction;
 
            bn::sprite_animate_action<4> _walkcycle;
            bn::optional<bn::sprite_animate_action<7>> _flex;
            
            unsigned short _trogmeter;
            unsigned int _burninate_time;
            const unsigned short _trogmeter_max = TROG_TROGMETER_MAX;
            const int _burninate_length = TROG_BURNINATE_TIME; //temporarily this can be 10 seconds 
            unsigned short _time_dead;
            unsigned short _iframes;
            unsigned short _majesty_flash_timer;

            firebreath _breath;
            session_info &_sesh;

            //it's probably bad for this to have a reference to the cottage
            // vector, but I just need to get this working!!
            // TODO: Refactor trogdor to have direction
            // bn::vector<cottage, 10> &_cottages;

            void check_boundary_collision();

            bool invincible();
            void die(short frame_no);

            //todo: this doesn't need to be in this class
            bool any_dpad_input();

            void free_from_collisionbox(const bn::fixed_rect &box);

            void update_next_pos();
            void update_pos() {_pos = _next_pos;}

            void start_burninating();
            void update_firebreath();

            bn::fixed_point _next_pos;


        public:
            player(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes);
            virtual void update() final;

            bool burninating();
            bool handle_cottage_collision(cottage &cottage);
            void handle_peasant_collision(peasant &peasant);
            void handle_knight_collision(knight &knight);
            void handle_arrow_collision(archer &archer);
            void handle_wall_collision(const bn::fixed_rect &wall_hitbox);
            
            bool dead() {return _time_dead;}
            bool ready_to_respawn() {return _time_dead == TROG_RESPAWN_TIME;}

            unsigned short get_trogmeter(){return _trogmeter;}
            unsigned short get_burninating_time(){return _burninate_time;}
            void set_visible(bool visible);
            void enable_breath(){_breath.enable();}

            void update_win_anim();

            void pass_out();
            void thumb_it_up();
            void flex();


            virtual void update_anim() final;


    };
}