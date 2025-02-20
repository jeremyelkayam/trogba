#pragma once

#include "player.h"

namespace trog {
    class trogdor : public player { 
        private:
            bn::optional<bn::sprite_animate_action<7>> _flex;
            bn::sprite_animate_action<8> _walkcycle;

            virtual void die(const unsigned int &death_index);

        public: 
            trogdor(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);

            void pass_out();
            void thumb_it_up();
            void flex();
            virtual void update() final;

            virtual void update_anim() final;

            
            virtual void demo_anim() override final 
                {flex();} 

    };
}