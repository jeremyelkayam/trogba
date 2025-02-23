
#pragma once
#include <bn_string.h>
#include <bn_sprite_item.h>
#include <bn_unordered_map.h>
#include "bn_sprite_items_achievement_jhonka.h"

#include "achievement_popup.h"
#include "sram_data.h"

namespace trog {

    struct achievement_rom_data
    {
        unsigned int sram_index;
        bn::string<32> name;
        bn::string<128> desc;
        bool is_number;
        long threshold;
        bn::sprite_item icon;
    };

    class achievements_mgr
    {
        private:
            bn::unordered_map<bn::string<8>, 
                const achievement_rom_data, 64> _achievements;

            bn::sprite_text_generator &_generator;
            const bn::fixed &_sound_vol;

            
            bn::optional<achievement_popup> _popup;

            saved_data &_sram_data;

            void show_popup(bn::string<8> tag);

            int max_index(long threshold);

            static constexpr const char *string_test = "TEST!";

        public:
            achievements_mgr(saved_data &sram_data,
                bn::sprite_text_generator &generator,
                const bn::fixed &sound_vol);

            void update_achievement(bn::string<8> tag, const long &new_value);

            void update();

            bool is_achieved(bn::string<8> tag);



    };

}