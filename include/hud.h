#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sstream.h>
#include <bn_string.h>
#include <bn_regular_bg_ptr.h>
#include <bn_rect_window.h>

#include "player.h"
#include "session_info.h"
namespace trog {
    class hud { 
        private:
            session_info& _sesh;

            bn::sprite_text_generator& _text_generator;  

            bn::vector<bn::sprite_ptr, 32> _score_text_sprites, 
                                           _mans_lv_text_sprites;


            bn::vector<bn::sprite_ptr, 10> _trogmeter_sprites;


            bn::regular_bg_ptr _burninatemeter;            
            bn::regular_bg_ptr _burninatemeter_invert;            
            bn::rect_window _burninatemeter_window;
            void set_all_visible(bool visible);
            bool _enabled;

        public:

            //todo this shouldn't be in hud; maybe throw it in a common functions class
            void set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, bool visible);

            hud(session_info &sesh, bn::sprite_text_generator& generator, unsigned short trogmeter_max);
            void update();
            void update_trogmeter(unsigned short trogmeter_value);
            void update_burninatemeter(unsigned int current_burninate_time, unsigned int total_burninate_time);

            void hide();
            void show();
    };
}