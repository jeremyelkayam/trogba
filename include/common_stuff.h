#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include "constants.h"

namespace trog {

    enum class troghammer_state
    {
        ALERT,
        AWAKE,
        COMING,
        ARRIVED,
        UNALERTED
    };

    struct troghammer_status{
        troghammer_state current_state;
        unsigned short timer;
        bn::fixed_point pos;
    };

    struct saved_session { 
        bool exists;
        uint8_t mans, level;
        unsigned short score;
        bool can_visit_treasure_hut, troghammer, can_lose_trogmeter;
        bn::array<bool, 6> cottage_burnination_status;
        troghammer_status thinfo;
    };

    struct saved_data {
        bn::array<char, 8> format_tag;
        //options
        bn::fixed sound_vol, music_vol, voice_vol;
        bool troghammer, decrement_trogmeter;
        uint8_t starting_lives;

        // bn::array<high_score_entry, 8> high_scores_table;

        saved_session session;
    };

    class common_stuff { 
        public:
            bn::sprite_text_generator text_generator;
            bn::sprite_text_generator big_generator;
            bn::sprite_text_generator small_generator;
            bn::random rand;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}
            void clear_saved_session();

            common_stuff();
            void set_autosave_text_visible(const bool &visible) {set_sprite_arr_visible(_autosave_text, visible);};

            void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);


        private:


            bn::vector<bn::sprite_ptr, 8> _autosave_text;

    };
}