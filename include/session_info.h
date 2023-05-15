#pragma once
#include <bn_array.h>
#include "troghammer.h"
#include "common_stuff.h"

namespace trog
{
    class session_info {

        public:
            session_info(common_stuff &common_stuff);
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

            void import_save();
            saved_session export_save();

            //the troghammer NEVER spawns on the tutorial level (level 0).
            bool troghammer_enabled() {return _troghammer && _level != 0;}

            void reset_score(){_score = 0;}

            bool can_lose_trogmeter(){return _can_lose_trogmeter; }
            
        private:
            unsigned short _mans;
            unsigned int _score;
            unsigned short _level;
            bool _killed_by_archer, _troghammer, _can_lose_trogmeter;
            common_stuff &_common_stuff;
    };
}


