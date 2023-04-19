#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>

namespace trog { 
    class text_box {
        private:
            bn::vector<bn::sprite_ptr, 32> _text_sprites;
            bn::regular_bg_ptr _box;

            /*
             * Formats a longer string into up to 3 separate strings, separated
             * automatically based on width.
             * 
             * Throws an error if the string is too big to fit into 3 lines (1 text box).
             */
            bn::vector<bn::string<64>, 3> split_into_lines(const char *text);

        public:
            text_box(bn::sprite_text_generator &text_generator, const char *text);
    };
}