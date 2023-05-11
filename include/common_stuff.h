#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>

namespace trog {

    struct level_save { 
        bool exists;
        uint8_t mans, level;
        unsigned short score;
        bool visited_treasure_hut, troghammer, can_lose_trogmeter;
        bn::array<bool, 6> cottage_burnination_status;
        // troghammer_status thinfo;
    };

    struct saved_data {
        bn::array<char, 8> format_tag;
        level_save save;
        // bn::array<high_score_entry, 8> high_scores_table;
        
        //options
        bn::fixed sound_vol, music_vol, voice_vol;
        bool troghammer, decrement_trogmeter;
        uint8_t starting_lives;
    };

    class common_stuff { 
        public:
            bn::sprite_text_generator text_generator;
            bn::sprite_text_generator big_generator;
            bn::sprite_text_generator small_generator;
            bn::random rand;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}
            void clear_level_save();

            common_stuff();

    };
}