#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include "scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "session_info.h"


namespace trog{

    class menu_option {
        private:
            bn::vector<bn::sprite_ptr, 8> _text_sprites;
        public:
            menu_option(const bn::fixed &x, const bn::fixed &y, const char *text, bn::sprite_text_generator& _text_generator);
            bn::fixed y(){return _text_sprites.at(0).y();}
    };
    class menu_scene : public scene{ 
    private:

        bn::vector<menu_option, 4> _menu_options;

        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 64> _menu_text_sprites;
        bn::sprite_ptr _cursor, _flames;
        bn::sprite_animate_action<4> _flames_anim;
        bn::sprite_scale_to_action _flames_scale;
        
        uint8_t _selected_option_index, _selection_anim_timer;

        session_info &_sesh;
        session_info _loaded_sesh;

        void setup_instructions();

    public:
        explicit menu_scene(session_info &sesh, bn::sprite_text_generator& text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };


}
