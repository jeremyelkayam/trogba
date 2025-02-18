#pragma once

#include "player.h"
#include <bn_deque.h>

namespace trog {

class tongue : public entity {
    private:
        bn::deque<bn::sprite_ptr, 16> _tongue_sprites;

        bool _retracting;
        
    public:
        tongue(bn::fixed_point pos, bool facing_left);

        virtual void update() final;

        void retract();
    };

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