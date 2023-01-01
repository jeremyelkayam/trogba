#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_brown.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"

#include "session_info.h"


namespace trog{

    class high_score_entry {
        private:
            bn::array<char, 10>_name;
            unsigned short _level;
            unsigned int _score;
        public:
            high_score_entry(bn::string<9> name, unsigned short level, unsigned short score);
            unsigned short get_level(){return _level;}
            unsigned int get_score(){return _score;}
            bn::string<9> get_name();
    };

    class hiscores_scene : public scene{ 
    private:

        bn::vector<high_score_entry, 9> _high_scores_table;

        session_info &_sesh;
        bn::sprite_text_generator& _text_generator;

        bn::vector<bn::sprite_ptr, 128> _text_sprites;
        bn::regular_bg_ptr _scroll;
        

    public:
        explicit hiscores_scene(session_info &sesh, bn::sprite_text_generator& text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
        void init_high_scores_table();
    };

}
