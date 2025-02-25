
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

    struct achievement_rom_data
    {
        const char *tag;
        const char *name;
        const char *desc;
        bool is_number;
        long threshold;
    };

    constexpr const int num_achievements = 2;
    constexpr const achievement_rom_data acdata[2] =
    {
        {
            "onehut", 
            "Jhonka's Riches",
            "Find a treasure hut", 
            false, 
            0b1
        },
        {
            "allhuts", 
            "Aztec Gold",
            "Find the treasure huts on all level layouts", 
            false, 
            0b111
        },
    };

    class achievements_mgr
    {
        private:

            bn::unordered_map<bn::string<8>, 
                const achievement, 64> _achievements;

            bn::sprite_text_generator &_generator;
            const bn::fixed &_sound_vol;

            
            bn::optional<achievement_popup> _popup;

            saved_data &_sram_data;

            void show_popup(bn::string<8> tag);

            int max_index(long threshold);

        public:
            achievements_mgr(saved_data &sram_data,
                bn::sprite_text_generator &generator,
                const bn::fixed &sound_vol);

            void update_achievement(bn::string<8> tag, const long &new_value);

            void update();

            bool is_achieved(bn::string<8> tag);



    };

}