#pragma once
#include <bn_string.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include "small_fonts.h"

namespace trog {
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
    )
    {
        bn::vector<bn::string<64>, 3>result;
        
        uint8_t line_width = 0;
        uint8_t line_num = 0;
        uint16_t line_start = 0;
        uint16_t line_end = 0;
        for(uint16_t pos = 0; text[pos] != '\0'; ++pos){
            char c = text[pos];
            line_width += char_widths[c - ' '];
            if(line_width > max_line_width){
                uint16_t last_space = pos;
                while(text[last_space] != ' '){
                    //Find the last space
                    last_space--;
                }
                line_end = last_space;
            }

            //this case handles the end of the string (\0)
            //pos + 1 here because the for loop won't process the end-of-string character
            if(text[pos + 1] == '\0') line_end = pos + 1;

            //this case lets us handle manual newlines (\n)
            if(text[pos] == '\n') line_end = pos;

            //if this variable is set, we know where the end of the line is
            if(line_end){
                //copy the current line, up to the last space, into the result vector
                bn::string<64> line;
                bn::ostringstream line_stream(line);
                for(uint16_t i = line_start; i < line_end; i++){
                    line_stream << text[i];
                }
                result.emplace_back(line);

                if(text[pos + 1] != '\0'){
                    pos = line_end + 1;
                }
                line_start = pos;

                //we are effectively un-setting these variables by zeroing them
                line_width = 0;
                line_end = 0;

                ++line_num;
            }

        }

        return result;
    }

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


    static bn::string<64> to_lower(const bn::string<64> &s)
    {
    bn::string<64> result;
    for(const char &c : s)
    {
        if('A' <= c && c <= 'Z')
        {
            result.push_back(c - ('Z' - 'z'));
        }
        else
        {
            result.push_back(c);
        }
    }
    return result;
    }   

    static bn::string<64> to_upper(const bn::string<64> &s)
    {
        bn::string<64> result;
        for(const char &c : s)
        {
            if('a' <= c && c <= 'z')
            {
                result.push_back(c + ('Z' - 'z'));
            }
            else
            {
                result.push_back(c);
            }
        }
        return result;
    }


}