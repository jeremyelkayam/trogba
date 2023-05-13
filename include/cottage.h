#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "constants.h"
#include "common_stuff.h"
#include "enums.h"

namespace trog {
    class cottage : public entity { 
        private:
            direction _direction;
            bool _has_treasure;

            bn::optional<bn::sprite_ptr> _flames;
            bn::optional<bn::sprite_animate_action<4>> _burningflames;
            int _time_burning;
            common_stuff &_common_stuff;
        public:
            cottage(bn::fixed xcor, bn::fixed ycor, direction direction, bool has_treasure, bool burninated, common_stuff &common_stuff);
            virtual void update() final;
            bool burninate();
            bool burninated() {return _time_burning > (TROG_COTTAGEFIRE_TIME / 2);}
            direction get_direction() {return _direction;}
            bool has_treasure();
            void visit() {BN_ASSERT(_has_treasure); _has_treasure = false;}
            void set_visible(bool visible);
    };
}