#pragma once
#include <bn_array.h>

namespace trog
{
    class session_info {

        public:
            session_info();
            void score(unsigned int num_points);
            void die() {--_mans;}
            void set_killed_by_archer(bool archer){_killed_by_archer = archer;}

            void advance_level(){++_level;}
            void reset();
            void secret_lives_boost() {_mans = 30;}

            void set_level(unsigned short level){_level=level;}

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
            
        private:
            bn::array<char, 8> _format_tag;
            unsigned short _mans;
            unsigned int _score;
            unsigned short _level;
            bool _killed_by_archer;

            //TO BE USED WHEN SAVING/LOADING A FILE MID-LEVEL.
            bn::array<bool, 6> _cottage_burnination_status;
    };
}


