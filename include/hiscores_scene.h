#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"

#include "session_info.h"


namespace trog{

    class hiscores_scene : public scene{ 
    private:

        bn::array<char, 41> _selectable_letters;

        session_info &_sesh;
        common_stuff &_common_stuff;
        bn::sprite_text_generator _fw_gen, _vw_gen, _red_gen;
        const scene_type _last_scene;

        bn::vector<bn::sprite_ptr, 128> _text_sprites;
        bn::vector<bn::sprite_ptr, 8> _name_entry_sprites, _header_sprites;
        bn::sprite_ptr _cursor_sprite;
        bn::regular_bg_ptr _scroll;
        
        bn::array<high_score_entry, 8> &_high_scores_table;

        void draw_high_scores_table();
        void update_name_entry();
        void end_name_entry();
        void draw_name_entry();
        void set_selectable_chars_index_to_current_char_in_str();
        
        short _table_index, _string_index, _selectable_letters_index;
        uint8_t _blink_timer, _hold_down_timer;
        unsigned short _nothing_pressed_timer;

        bool _go_to_credits;


    public:
        explicit hiscores_scene(session_info &sesh, common_stuff &common_stuff, const scene_type &last_scene);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
