#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "session_info.h"


namespace trog{

    class instructions_scene : public scene{ 
    private:
        bn::vector<bn::sprite_ptr, 4> _title_sprites;

        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 64> _instruction_text_sprites;
        bn::vector<bn::sprite_ptr, 64> _start_text_sprites;
        
        unsigned short _flashing_text_counter;
        bool _level_select;
        session_info &_sesh;
        

    public:
        explicit instructions_scene(session_info &sesh, bn::sprite_text_generator& text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
