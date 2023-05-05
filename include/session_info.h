#pragma once
#include <bn_array.h>
#include "troghammer.h"

namespace trog
{
    class session_info {
        //TODO: Implement a saved_data class with all of our save data

        public:
            session_info();
            void score(unsigned int num_points);
            void die() {if(_level != 0) --_mans;}
            void set_killed_by_archer(bool archer){_killed_by_archer = archer;}

            void advance_level(){set_level(_level+1);}
            void reset();
            void secret_lives_boost() {_mans = 30;}

            void set_level(unsigned short level);

            unsigned short get_mans() {return _mans;}
            unsigned int get_score() {return _score;}
            unsigned short get_level() {return _level;}
            bool last_killed_by_archer() {return _killed_by_archer;}

            bool current_level_has_cutscene();
            bool is_valid_object();

            bn::array<char, 8> default_format_tag();

            //todo probably refactor this into a saved data class
            void clear_burnination_array();
            void set_cottage_burnination(unsigned short dex, bool status);
            bool load_cottage_burnination(unsigned short dex) {return _cottage_burnination_status[dex];}

            void visit_treasure_hut();
            bool can_visit_treasure_hut();

            //the troghammer NEVER spawns on the tutorial level (level 0).
            bool troghammer_enabled() {return _troghammer && _level != 0;}

            void set_troghammer_status (troghammer_status thinfo) { _thinfo = thinfo;}
            troghammer_status load_troghammer_status() {return _thinfo;}

            void reset_troghammer_status();

            void reset_score(){_score = 0;}

            bool can_lose_trogmeter(){return _can_lose_trogmeter; }
            
        private:
            bn::array<char, 8> _format_tag;
            unsigned short _mans;
            unsigned int _score;
            unsigned short _level;
            bool _killed_by_archer, _visited_treasure_hut, _troghammer, _can_lose_trogmeter;

            //TO BE USED WHEN SAVING/LOADING A FILE MID-LEVEL.
            bn::array<bool, 6> _cottage_burnination_status;
            troghammer_status _thinfo;
    };
}


