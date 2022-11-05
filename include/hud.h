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
            bn::optional<player> _trogdor;
            int frames;

        public:
            hud(session_info &sesh, bn::sprite_text_generator& generator);
            void update();
    };
}