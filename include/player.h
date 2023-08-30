#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_unique_ptr.h>
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "constants.h"
#include "firebreath.h"
#include "session_info.h"
#include "knight.h"
#include "troghammer.h"
#include "archer.h"

namespace trog {
    class player : public entity { 
        protected:
            bn::sprite_item _spritem;
            const bn::fixed _speed;
            bn::fixed_point _breath_offset;

            bn::sprite_ptr _majesty;

            bn::fixed_point _direction;
 
            uint8_t _trogmeter;
            unsigned short _burninate_time;
            const uint8_t _trogmeter_max = TROG_TROGMETER_MAX;
            unsigned short _burninate_length; 
            uint8_t _time_dead;
            uint8_t _iframes;
            uint8_t _majesty_flash_timer;
            const uint8_t _walk_cycle_frames;

            firebreath _breath;
            session_info &_sesh;
            common_stuff &_common_stuff;

            void check_boundary_collision();

            bool invincible();
            virtual void die(const uint8_t &death_index);

            //todo: this doesn't need to be in this class
            bool any_dpad_input();

            void free_from_collisionbox(const bn::fixed_rect &box);

            void update_next_pos();
            void update_pos() {_pos = _next_pos;}

            virtual void start_burninating();
            virtual void stop_burninating();
            void update_firebreath();

            bn::fixed_point _next_pos;

            virtual bool can_move() {return true;}
            
        public:
            player(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::fixed speed, bn::fixed_point breath_offset, session_info &sesh, bool iframes, bn::sprite_item spritem, uint8_t walk_cycle_frames, common_stuff &common_stuff, uint8_t initial_trogmeter=0);
            virtual void update();

            bool burninating();
            
            //refactor these into a burnable interface...
            bool handle_cottage_collision(cottage &cottage);
            void handle_peasant_collision(peasant &peasant);

            //todo: refactor these into an interface maybe
            void handle_knight_collision(knight &knight);
            void handle_troghammer_collision(troghammer &troghammer);
            void handle_arrow_collision(archer &archer);


            void handle_wall_collision(const bn::fixed_rect &wall_hitbox);
            
            bool dead() {return _time_dead;}
            bool ready_to_respawn() {return _time_dead == TROG_RESPAWN_TIME;}

            unsigned short get_trogmeter(){return _trogmeter;}
            unsigned short get_burninating_time(){return _burninate_time;}
            unsigned short get_burninating_length(){return _burninate_length;}
            virtual void set_visible(const bool &visible) override;
            void enable_breath(){_breath.enable(); _breath.set_horizontal_flip(_sprite.horizontal_flip());}
            void disable_breath(){_breath.disable();}
            virtual void set_horizontal_flip(const bool &horizontal_flip);

            void drop_trogmeter(){if(_trogmeter > 0) _trogmeter--;}

            virtual void update_win_anim();


    };
}