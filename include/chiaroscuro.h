#pragma once

#include "player.h"

namespace trog {
    class chiaroscuro : public player { 
        private:
            // bn::optional<bn::sprite_animate_action<7>> _flex;
            bn::sprite_animate_action<6> _walkcycle;
                        
            bn::optional<bn::sprite_animate_action<18>> _roar;

        public: 
            chiaroscuro(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);
            virtual void update() override final;

            virtual void update_anim() override final;

            void change_walkcycle(const bn::isprite_animate_action &walkcycle);

            virtual void start_burninating() override;
            virtual void stop_burninating() override;

            virtual void demo_anim() override;

            virtual void update_firebreath() override;

            virtual void update_win_anim() override;


    };
}