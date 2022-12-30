#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include "scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "session_info.h"


namespace trog{

    class menu_scene : public scene{ 
    private:

        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 64> _menu_text_sprites;
        bn::sprite_ptr _cursor;
        
        unsigned short _selected_option_index;
        session_info &_sesh;
        session_info _loaded_sesh;

        void setup_instructions();

    public:
        explicit menu_scene(session_info &sesh, bn::sprite_text_generator& text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
