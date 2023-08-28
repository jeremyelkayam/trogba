#pragma once

#include "player.h"

namespace trog {
    class sucks : public player { 
        private:
            bn::sprite_animate_action<6> _walkcycle;
            unsigned short _stomp_timer;
            bn::sprite_ptr _shockwave;
            bn::sprite_animate_action<16> _shockwave_anim;
            bn::optional<bn::sprite_palette_fade_loop_action> _fade_action;
            uint8_t _prev_index;


            virtual void start_burninating();
            virtual void stop_burninating();

            void change_walkcycle(const bn::isprite_animate_action &walkcycle);

        public: 
            sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);

            virtual void update() final;
            virtual void update_anim() final;
            void stomp();

            bool can_stomp() {return _stomp_timer == 0 && !burninating() && !dead();}
            unsigned short stomp_timer() {return _stomp_timer;}

            virtual bool can_move() override final {return _stomp_timer == 0 || _stomp_timer > 60;}


            virtual void update_win_anim();
            bn::fixed_point foot_pos();
    };
}