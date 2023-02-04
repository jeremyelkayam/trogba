#pragma once
#include <bn_array.h>

namespace trog
{
    class saved_data {
        //TODO: Implement a saved_data class with all of our save data
 
        public:

            saved_data();

            bool is_valid_object();

            bn::array<char, 8> default_format_tag();

            //todo probably refactor this into a saved data class
            void clear_burnination_array();
            void set_cottage_burnination(unsigned short dex, bool status);
            bool load_cottage_burnination(unsigned short dex) {return _cottage_burnination_status[dex];}

            bool troghammer_enabled() {return _troghammer;}
            
        private:
            bn::array<char, 8> _format_tag;
            unsigned int _score;
            unsigned short _mans;
            unsigned short _level;
            
            bool _visited_treasure_hut, _troghammer;

            //TO BE USED WHEN SAVING/LOADING A FILE MID-LEVEL.
            bn::array<bool, 6> _cottage_burnination_status;
            
    };
}


