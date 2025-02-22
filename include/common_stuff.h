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
#include "constants.h"
#include "sb_commentary.h"
#include "enums.h"
#include "achievements.h"

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

    class high_score_entry {
        private:
            bn::array<char, 10>_name;
            unsigned short _level;
            unsigned int _score;
        public:
            high_score_entry();
            high_score_entry(bn::string<9> name, unsigned short level, unsigned short score);
            unsigned short get_level() const {return _level;}
            unsigned int get_score() const {return _score;}
            bn::string<9> get_name() const ;
            void set_name_char(char name_char, int index){_name[index] = name_char;}
    };
    struct saved_session { 
        bool exists;
        uint8_t mans, level;
        unsigned short score;
        bool can_visit_treasure_hut, troghammer, can_lose_trogmeter;
        bn::array<bool, 6> cottage_burnination_status;
        troghammer_status thinfo;
        dragon current_dragon;
    };

    struct saved_options {
        bn::fixed sound_vol, music_vol, voice_vol;
        bool troghammer, decrement_trogmeter;
        uint8_t starting_lives;        
    };

    struct saved_stats {
        uint32_t play_time, points_earned, peasants_stomped;
        uint16_t cottages_entered,
            treasure_collected,
            times_sworded,
            times_arrowed,
            times_hammered,
            games_played,
            peasants_burned,
            cottages_burned,
            fastest_level;
        uint8_t highest_level;
    };

    struct saved_data {
        bn::array<char, 8> format_tag;
        //options
        saved_options options;

        bn::array<high_score_entry, 8> high_scores_table;

        saved_session session;

        dragon last_dragon_used;

        bn::array<long, 64> achievements;

        bn::array<bool, 13> unlocked_cutscenes;

        saved_stats stats;
    };

    class common_stuff { 
        public:
            bn::sprite_text_generator text_generator;
            bn::sprite_text_generator big_generator;
            bn::sprite_text_generator small_generator;
            bn::random rand;
            bn::vector<bn::pair<uint8_t, bn::string<32>>, 13> cutscene_levels;
            bn::array<char, 8> default_format_tag;

            achievements_mgr acm;

            sb_commentary commentary;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}
            void clear_saved_session();

            common_stuff();
            void set_autosave_text_visible(const bool &visible) {set_sprite_arr_visible(_autosave_text, visible);};

            void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);

            bool level_has_cutscene(const uint8_t &current_level) const;
            void unlock_cutscene_at_level(const uint8_t &current_level);

            const char* scene_type_to_string(const scene_type &type) const;

            bn::string<32> cutscene_name_for_level(const uint8_t &level) const;

            bn::fixed euclidean_dist(const bn::fixed_point &a, const bn::fixed_point &b) const;

            bool percent_chance(const bn::fixed &pct) {return commentary.percent_chance(pct);}

            bn::string<10> play_time();            
            
            /*

            * Formats a longer string into up to 3 separate strings, separated
            * automatically based on width.
            * 
            * Throws an error if the string is too big to fit into 3 lines (1 text box).
            */
           static bn::vector<bn::string<64>, 3> split_into_lines(const char *text);


        private:

            bn::vector<bn::sprite_ptr, 8> _autosave_text;
            bn::array<char, 8> str_to_format_tag(const char *str);

            void format_save();

    };







    //old save formats
    struct saved_session_v20 { 
        bool exists;
        uint8_t mans, level;
        unsigned short score;
        bool can_visit_treasure_hut, troghammer, can_lose_trogmeter;
        bn::array<bool, 6> cottage_burnination_status;
        troghammer_status thinfo;
    };
    struct saved_data_v20 {
        bn::array<char, 8> format_tag;
        //options
        saved_options options;

        bn::array<high_score_entry, 8> high_scores_table;

        saved_session_v20 session;

        bool cheat_unlocked; 
    };

}