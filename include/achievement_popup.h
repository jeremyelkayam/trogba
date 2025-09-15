#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include "bn_sprite_items_achievements.h"

namespace trog { 
    class achievement_popup {
        private:
            bn::fixed _speed;
            bn::fixed_point _pos;

            unsigned int _timer;


            bn::vector<bn::sprite_ptr, 32> _text_sprites;
            bn::vector<bn::sprite_ptr, 5> _box_sprites;
            bn::sprite_ptr _icon_sprite;


            bn::vector<bn::sprite_ptr, 36> all_sprites();

            void set_position_relative(
                const bn::fixed_point &relative_pos);

        public:
            constexpr const static unsigned int MOVE_TIME = 20;
            constexpr const static unsigned int TOTAL_TIME = 210;
            constexpr const static unsigned int HEIGHT = 42;
            achievement_popup(const bn::fixed &sound_vol,
                const bn::string<32> &achievement_name,
                const int &index);

            void update();

            void set_position(const bn::fixed_point &pos);
            bn::fixed_point position() {return _pos;}
            bool moving() {return _speed != 0;}

            bool done() {return _timer >= TOTAL_TIME;}

            bn::fixed personal_speed();
            void set_speed(const bn::fixed &speed)
            {
                _speed = speed;
            }

    };
}