#pragma once

#include "player.h"

namespace trog {

    class shockwave {
        private:
            bn::vector<bn::sprite_ptr, 4> _sprites;
            bn::vector<bn::sprite_animate_action<17>, 4> _anims;

        public:
            shockwave(const bn::fixed &x, const bn::fixed &y);
            void update();
            void set_visible(const bool &visible);
            void set_position(const bn::fixed_point &pos);
            bool done() const {return _anims.at(0).done();}
            bool visible() const {return _sprites.at(0).visible();}
            void reset();
    };

    class sucks : public player { 
        private:
            bn::sprite_ptr _sweat;
            bn::sprite_animate_action<6> _walkcycle;
            bn::sprite_animate_action<8> _sweat_anim;
            unsigned int _stomp_timer;
            shockwave _shockwave;
            bn::optional<bn::sprite_palette_fade_to_action> _fade_action;
            static constexpr bn::fixed _hi = 0.6;
            static constexpr bn::fixed _lo = 0.3;
            unsigned int _oscillate_time;


            virtual void start_burninating();
            virtual void stop_burninating();

            void change_walkcycle(const bn::isprite_animate_action &walkcycle);

            virtual void die(const unsigned int &death_index);

            void reset_fade();

            bn::fixed attachments_y_offset() {return abs(_walkcycle.current_index() - (_walkcycle.graphics_indexes().size()) / 2) - 2;}

            void update_sweat_pos(bn::fixed yoffset);

        public: 
            sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, unsigned int initial_trogmeter=0);

            virtual void update() final;
            virtual void update_anim() final;
            void stomp();

            bool can_stomp() {return _stomp_timer == 0 && !burninating() && !dead();}
            unsigned int stomp_timer() {return _stomp_timer;}

            virtual bool can_move() override final {return _stomp_timer == 0 || _stomp_timer > 60;}

            virtual void demo_anim() override final {stomp();} 

            virtual void enable_breath() override final;
            virtual void disable_breath() override final;

            virtual void update_win_anim();
            bn::fixed_point foot_pos();

            virtual void set_visible(const bool &visible) override final;
    };
}