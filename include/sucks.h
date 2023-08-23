#pragma once

#include "player.h"

namespace trog {
    class sucks : public player { 
        private:
            bn::sprite_animate_action<6> _walkcycle;
            unsigned short _stomp_timer;
            bn::sprite_ptr _shockwave, _arms;
            bn::sprite_animate_action<16> _shockwave_anim;

        public: 
            sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter=0);

            virtual void update() final;
            virtual void update_anim() final;
            void stomp();

            bool can_stomp() {return _stomp_timer == 0 && !burninating() && !dead();}
            unsigned short stomp_timer() {return _stomp_timer;}

            virtual bool can_move() override final {return _stomp_timer == 0 || _stomp_timer > 60;}

            bn::fixed_point foot_pos();

            virtual void set_visible(const bool &visible) override {entity::set_visible(visible); _arms.set_visible(visible);}
            virtual void set_horizontal_flip(const bool &flip) override {entity::set_horizontal_flip(flip); _arms.set_horizontal_flip(flip);}

    };
}