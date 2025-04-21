#pragma once
#include <bn_fixed_point.h>
#include <bn_array.h>
#include "enums.h"

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

        bn::array<bool, 4> unlocked_dragons;
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