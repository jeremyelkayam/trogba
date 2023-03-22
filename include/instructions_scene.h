#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "session_info.h"
#include "big_text.h"


namespace trog{

    class instructions_scene : public scene{ 
    private:
        bn::vector<bn::keypad::key_type, 10> _secret_code;
        unsigned short _secret_code_index;

        bn::sprite_text_generator& _text_generator;
        bn::vector<bn::sprite_ptr, 64> _instruction_text_sprites;
        bn::vector<bn::sprite_ptr, 64> _start_text_sprites;
        
        unsigned short _flashing_text_counter;
        bool _level_select, _show_secret_hints, _show_credits, _continue_menu_visible, _continue_selected;
        session_info &_sesh;


        int _text_ycor;
        big_text _title;
        
        void setup_instructions();
        void setup_secret_hints();
        void setup_credits();
        void clear_text();
        void setup_continue_menu();
        void write_instruction(const char* str, const bn::sprite_palette_item &palette, int line_spacing);

    public:
        explicit instructions_scene(session_info &sesh, bn::sprite_text_generator& text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
