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

            bn::sprite_ptr _majesty;
            
            bn::fixed_point _direction;
 
            bn::sprite_animate_action<4> _walkcycle;
            
            uint8_t _trogmeter;
            unsigned short _burninate_time;
            unsigned short _burninate_length;
            uint8_t _time_dead;
            uint8_t _iframes;
            uint8_t _majesty_flash_timer;

            firebreath _breath;
            session_info &_sesh;

            void check_boundary_collision();

            bool invincible();
            void die(short frame_no);

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

            void setup_win_pose();
            void update_sprites();
            void flex();
    };
}