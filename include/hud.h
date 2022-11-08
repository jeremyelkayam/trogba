#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sstream.h>
#include <bn_string.h>
#include "player.h"
#include "session_info.h"

namespace trog {
    class hud { 
        private:
            session_info& _sesh;

            bn::sprite_text_generator& _text_generator;            
            bn::vector<bn::sprite_ptr, 32> _score_text_sprites;     
            bn::vector<bn::sprite_ptr, 32> _mans_lv_text_sprites;
            bn::vector<bn::sprite_ptr, 10> _trogmeter;

        public:
            hud(session_info &sesh, bn::sprite_text_generator& generator, unsigned short trogmeter_max);
            void update();
            void update_trogmeter(unsigned short trogmeter);
            void update_burninatemeter(unsigned int burninate_time);
    };
}