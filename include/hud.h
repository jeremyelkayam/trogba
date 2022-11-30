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
                                           _mans_lv_text_sprites,
                                           _trogmeter;

            bn::regular_bg_ptr _burninatemeter;            
            bn::regular_bg_ptr _burninatemeter_invert;            
            bn::rect_window _burninatemeter_window;
            void set_all_visible(bool visible);
            void set_sprite_arr_visible(bn::vector<bn::sprite_ptr, 32> sprites, bool visible);
            
        public:
            hud(session_info &sesh, bn::sprite_text_generator& generator, unsigned short trogmeter_max);
            void update();
            void update_trogmeter(unsigned short trogmeter_value);
            void update_burninatemeter(unsigned int burninate_time);

            void hide();
            void show();
    };
}