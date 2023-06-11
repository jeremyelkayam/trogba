#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "cottage.h"
#include "enums.h"
#include "session_info.h"
#include "bn_sprite_items_peasant.h"

namespace trog {
    class peasant : public entity { 
        private:
            const bn::fixed _maxdist;
            bn::fixed _currentdist;
            bn::fixed _speed;
            cottage &_home;


            uint8_t _time_waiting;

            uint8_t _time_dead;

            bn::fixed_point _direction;
            bool _onfire;
            bool _returning;
 
            bn::sprite_animate_action<2> _walkcycle;
            
        public:
            peasant(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed maxdist, cottage &home);
            virtual void update() final;

            void stomp();
            void burninate();
            bool dead(){return _time_dead > 0;}
            bool remove_from_map();
            bool onfire(){return _onfire;}
            void set_sprite_ablaze();
            void set_frame(short frame_number){_sprite.set_tiles(bn::sprite_items::peasant.tiles_item(), frame_number);}
            cottage &get_home(){return _home;}
    };
}