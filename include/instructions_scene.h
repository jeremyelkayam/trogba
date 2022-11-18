#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "bn_regular_bg_items_titlegraphic.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"


namespace trog{

    class instructions_scene : public scene{ 
    private:
        bn::regular_bg_ptr title;
        bn::sprite_text_generator& text_generator;
        bn::vector<bn::sprite_ptr, 64> instruction_text_sprites;
        bn::vector<bn::sprite_ptr, 64> start_text_sprites;
        unsigned short flashing_text_counter;
        const unsigned short visible_text_time = 40;
        const unsigned short invisible_text_time = 10;

    public:
        explicit instructions_scene(bn::sprite_text_generator& a_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
