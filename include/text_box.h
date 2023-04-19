#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_item.h>

namespace trog { 
    class text_box {
        private:
            bn::vector<bn::sprite_ptr, 16> _text_sprites;
            bn::regular_bg_item _box;

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text);
    }
}