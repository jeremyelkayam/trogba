
#pragma once
#include <bn_string.h>
#include <bn_sprite_item.h>
#include <bn_unordered_map.h>

#include "achievement_popup.h"
#include "sram_data.h"

namespace trog {

    struct achievement
    {
        unsigned int sram_index;
        bn::string<32> name;
        bool is_number;
        long threshold;
    };

    class achievements_mgr
    {
        private:

            bn::unordered_map<bn::string<8>, 
                const achievement, 64> _achievements;

            const bn::fixed &_sound_vol;

            
            bn::vector<achievement_popup, 4> _popups;

            saved_data &_sram_data;

            void show_popup(bn::string<8> tag);

            int max_index(long threshold);

            static bool popup_done(achievement_popup &p) 
                {return p.done();}


        public:
            achievements_mgr(saved_data &sram_data,
                const bn::fixed &sound_vol);

            void update_achievement(bn::string<8> tag, 
                const long &new_value=0);

            void update();

            bool is_achieved(bn::string<8> tag);



    };

}