#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "enums.h"
#include "session_info.h"
#include "bn_sprite_items_peasant.h"

namespace trog {
    class peasant : public entity { 
        private:
            const bn::fixed _maxdist;
            bn::fixed _currentdist;
            bn::fixed _speed;

            const unsigned short _waittime;
            unsigned short _time_waiting;

            const unsigned short _despawn_delay;
            unsigned short _time_dead;

            bn::fixed_point _direction;
            bool _onfire;
            bool _returning;
 
            bn::sprite_animate_action<2> _walkcycle;
            
        public:
            peasant(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed maxdist, direction direction);
            virtual void update() final;

            void stomp();
            void burninate();
            bool dead(){return _time_dead > 0;}
            bool remove_from_map();
            bool onfire(){return _onfire;}
            virtual void update_anim() final;
            void set_sprite_ablaze();
            void set_frame(short frame_number){_sprite.set_tiles(bn::sprite_items::peasant.tiles_item(), frame_number);}

    };
}