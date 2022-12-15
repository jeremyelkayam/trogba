#pragma once


namespace trog
{
    class session_info {

        public:
            session_info();
            void score(unsigned int num_points);
            void die(){--_mans;}
            void advance_level(){++_level;}
            void reset();
            void secret_lives_boost() {_mans = 30;}

            void set_level(unsigned short level){_level=level;}

            unsigned short get_mans() {return _mans;}
            unsigned int get_score() {return _score;}
            unsigned short get_level() {return _level;}

            bool current_level_has_cutscene();
            
        private:
            unsigned short _mans;
            unsigned int _score;
            unsigned short _level;
            // bn::unordered_set<int, 16> _cutscene_levels;
    };
}


