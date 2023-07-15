#pragma once

#include "player.h"

namespace trog {
    class sucks : public player { 
        private:
            bn::sprite_animate_action<6> _walkcycle;
            uint8_t _stomp_timer;

        public: 
            sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);

            virtual void update() final;
            virtual void update_anim() final;
            void stomp();

    };
}