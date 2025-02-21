#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include "common_stuff.h"

namespace trog { 
    class achievement_popup {
        private:
            bn::fixed _speed;
            bn::fixed_point _pos;

            unsigned int _timer;

            unsigned int MOVE_TIME = 20;
            unsigned int TOTAL_TIME = 400;
            unsigned int HEIGHT = 42;

            bn::vector<bn::sprite_ptr, 32> _text_sprites;
            bn::vector<bn::sprite_ptr, 3> _box_sprites;
            bn::sprite_ptr _icon_sprite;


            bn::vector<bn::sprite_ptr, 36> all_sprites();

            void set_position_relative(
                const bn::fixed_point &relative_pos);

        public:
            achievement_popup(
                common_stuff &cstuff,
                const bn::string<16> &achievement_name,
                const bn::sprite_item &achievement_icon);

            void update();

            void set_position(const bn::fixed_point &pos);

            bool done() {return _timer >= TOTAL_TIME;}

    };
}