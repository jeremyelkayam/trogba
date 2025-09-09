#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_string.h>
#include <bn_unordered_map.h>
#include <bn_optional.h>
#include <bn_keypad.h>
#include <bn_deque.h>
#include "constants.h"
#include "sb_commentary.h"
#include "achievements.h"
#include "sram_data.h"
#include "small_fonts.h"
#include "dialog.h"


namespace trog {

    class common_stuff { 
        public:
            bn::sprite_text_generator big_generator;
            bn::random rand;
            bn::vector<bn::pair<uint8_t, bn::string<32>>, 20> cutscene_levels;
            bn::array<char, 8> default_format_tag;
            
            bn::unique_ptr<trog::dialog> dialog;


            bn::deque<dragon, 4> newly_unlocked;


            sb_commentary commentary;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}
            void clear_saved_session();

            common_stuff();
            void set_autosave_text_visible(const bool &visible) {set_sprite_arr_visible(_autosave_text, visible);};

            void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);

            bool level_has_cutscene(const uint8_t &current_level) const;
            void unlock_cutscene_at_level(const uint8_t &current_level);
            void unlock_character(const dragon &dragon);
            void unlock_random_character();

            const char* scene_type_to_string(const scene_type &type) const;

            bn::string<32> cutscene_name_for_level(const uint8_t &level) const;

            bn::fixed euclidean_dist(const bn::fixed_point &a, const bn::fixed_point &b) const;

            bool percent_chance(const bn::fixed &pct) {return commentary.percent_chance(pct);}

            bn::string<10> play_time();            

            void update_achievement(const bn::string<8> &tag, 
                const long &new_value=0);
            
            bool is_achieved(const bn::string<8> &tag) const
                {return _acm.is_achieved(tag);}

            void update();

            bn::vector<dragon, NUM_DRAGONS> available_dragons();

        private:
        
            achievements_mgr _acm;

            bn::vector<bn::sprite_ptr, 8> _autosave_text;
            bn::array<char, 8> str_to_format_tag(const char *str);

            void format_save();

    };

}