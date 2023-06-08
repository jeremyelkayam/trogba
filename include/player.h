#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "firebreath.h"
#include "session_info.h"
#include "knight.h"
#include "archer.h"

namespace trog {
    class player : public entity { 
        private:
            bn::sprite_ptr _beefy_arm, _tail, _feet;
            const bn::fixed _speed;

            bn::fixed_point _direction;
 
            bn::sprite_animate_action<4> _walkcycle;
            
            uint8_t _trogmeter;
            unsigned short _burninate_time;
            unsigned short _burninate_length; //temporarily this can be 10 seconds 
            uint8_t _time_dead;
            uint8_t _iframes;

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
            bool ready_to_respawn() {return _time_dead == 120;}

            unsigned short get_trogmeter(){return _trogmeter;}
            unsigned short get_burninating_time(){return _burninate_time;}
            unsigned short get_burninating_length(){return _burninate_length;}
            void set_visible(bool visible);
            void enable_breath(){_breath.enable(); _breath.set_horizontal_flip(_sprite.horizontal_flip());}
            void disable_breath(){_breath.disable();}
            void set_horizontal_flip(bool horizontal_flip);

            void update_win_anim();
            void update_sprites();
    };
}