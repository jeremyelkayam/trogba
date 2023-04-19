#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>

namespace trog { 
    class text_box {
        private:
            bn::vector<bn::sprite_ptr, 16> _text_sprites;
            bn::regular_bg_ptr _box;

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text);
    };
}