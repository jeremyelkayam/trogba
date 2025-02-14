#pragma once

#include "player.h"

namespace trog {
    class wormdingler : public player { 
        private:
            // bn::optional<bn::sprite_animate_action<7>> _flex;
            bn::sprite_animate_action<9> _walkcycle;

        public: 
            wormdingler(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);
            virtual void update() final;

            virtual void update_anim() final;

    };
}