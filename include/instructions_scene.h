#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"


namespace trog{

    class instructions_scene : public scene{ 
    private:
        bn::vector<bn::sprite_ptr, 4> _title_sprites;

        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 64> _instruction_text_sprites;
        bn::vector<bn::sprite_ptr, 64> _start_text_sprites;
        unsigned short _flashing_text_counter;

    public:
        explicit instructions_scene(bn::sprite_text_generator& a_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
