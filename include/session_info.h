#pragma once
#include <bn_array.h>

namespace trog
{
    class session_info {
        //TODO: Implement a saved_data class with all of our save data

        public:
            session_info();
            void score(unsigned int num_points);
            void die() {--_mans;}

            void advance_level(){set_level(_level+1);}
            void reset();
            void secret_lives_boost() {_mans = 30;}

            void set_level(unsigned short level);

            unsigned short get_mans() {return _mans;}
            unsigned int get_score() {return _score;}
            unsigned short get_level() {return _level;}

            
        private:
            bn::array<char, 8> _format_tag;
            unsigned short _mans;
            unsigned int _score;
            unsigned short _level;

    };
}


