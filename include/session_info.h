#pragma once
#include <bn_array.h>
#include "hud.h"

namespace trog
{
    class session_info {

        public:
            session_info(hud& hud);
            void score(unsigned int num_points);
            void die() {--_mans; _hud.update_mans_lv(_mans, _level);}
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

            void visit_treasure_hut();
            bool can_visit_treasure_hut();
            bool troghammer_enabled() {return _troghammer;}
            
        private:
            unsigned short _mans;
            unsigned int _score;
            unsigned short _level;
            bool _killed_by_archer, _visited_treasure_hut, _troghammer;

            hud &_hud;

    };
}


