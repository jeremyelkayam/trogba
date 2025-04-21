#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_random.h>
#include <bn_sram.h>
#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include <bn_string.h>
#include <bn_unordered_map.h>
#include <bn_optional.h>
#include <bn_keypad.h>
#include "constants.h"
#include "sb_commentary.h"
#include "achievements.h"
#include "sram_data.h"
#include "small_fonts.h"

namespace trog {

    class common_stuff { 
        public:
            bn::sprite_text_generator big_generator;
            bn::random rand;
            bn::vector<bn::pair<uint8_t, bn::string<32>>, 13> cutscene_levels;
            bn::array<char, 8> default_format_tag;

            achievements_mgr acm;

            sb_commentary commentary;

            saved_data savefile;
            void save() {bn::sram::write(savefile);}
            void clear_saved_session();

            common_stuff();
            void set_autosave_text_visible(const bool &visible) {set_sprite_arr_visible(_autosave_text, visible);};

            void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible);

            bool level_has_cutscene(const uint8_t &current_level) const;
            void unlock_cutscene_at_level(const uint8_t &current_level);

            const char* scene_type_to_string(const scene_type &type) const;

            bn::string<32> cutscene_name_for_level(const uint8_t &level) const;

            bn::fixed euclidean_dist(const bn::fixed_point &a, const bn::fixed_point &b) const;

            bool percent_chance(const bn::fixed &pct) {return commentary.percent_chance(pct);}

            bn::string<10> play_time();            

            /*

            * Formats a longer string into up to 3 separate strings, separated
            * automatically based on width.
            * 
            * Throws an error if the string is too big to fit into 3 lines (1 text box).
            */
            static bn::vector<bn::string<64>, 3> split_into_lines(
                const char *text, 
                const uint8_t max_line_width = 220,
                const int8_t *char_widths = small_font_character_widths
            );

            static bool any_dpad_input() {
                return bn::keypad::right_pressed() ||
                    bn::keypad::left_pressed() ||
                    bn::keypad::up_pressed() ||
                    bn::keypad::down_pressed();
            }

        //Given two scale factors and the current coordinate,
        // and the center of zooming, calculate the new position
        // of the zoomed coordinate.
        // Useful for zooming in/out of entire screens of sprites
            static bn::fixed scale_coord(const bn::fixed &old_scale, 
                const bn::fixed &new_scale, 
                const bn::fixed &old_coord, 
                const bn::fixed &center_coord)
            {
                return (new_scale / old_scale) * 
                    (old_coord - center_coord) + center_coord;
            }


        private:

            bn::vector<bn::sprite_ptr, 8> _autosave_text;
            bn::array<char, 8> str_to_format_tag(const char *str);

            void format_save();

    };

}