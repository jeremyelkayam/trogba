#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>

namespace trog { 
    class tutorial_box {
        private:
            bn::vector<bn::sprite_ptr, 32> _text_sprites;
            bn::regular_bg_ptr _box;
            bn::fixed _speed;
            bn::sprite_text_generator _small_gen;

        public:
            tutorial_box(const char *text);
            tutorial_box();
            void set_text(const char *text);
            bool is_off_screen() {return _box.y() >= 103;}

            void move_on_screen() {_speed = -3;}
            void move_off_screen() {_speed = 3;}
            void update();

    };
}