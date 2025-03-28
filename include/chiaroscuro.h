#pragma once

#include "player.h"

namespace trog {
    class chiaroscuro : public player { 
        private:
            // bn::optional<bn::sprite_animate_action<7>> _flex;
            bn::sprite_animate_action<4> _walkcycle;

        public: 
            chiaroscuro(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);
            virtual void update() final;

            virtual void update_anim() final;

    };
}