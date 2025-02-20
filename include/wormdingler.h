#pragma once

#include "player.h"
#include <bn_deque.h>

namespace trog {

class tongue : public entity {
    private:
        bn::vector<bn::sprite_ptr, 20> _tongue_sprites;

        bool _retracting;

        bn::fixed _speed;
        const bn::fixed & _vol;
        bn::optional<bn::sound_handle> _handle;
        
    public:
        tongue(bn::fixed_point pos, bool facing_left, 
            const bn::fixed &volume);


        virtual void update() override final;

        void retract();
        bool done();
        virtual void set_visible(const bool &visible) override final;
    };

    class wormdingler : public player { 
        private:
            bn::sprite_animate_action<9> _walkcycle;
            bn::optional<tongue> _tongue;

        public: 
            wormdingler(bn::fixed xcor, bn::fixed ycor, 
                session_info &sesh, bool iframes, 
                common_stuff &common_stuff, 
                uint8_t initial_trogmeter=0);
            virtual void update() override final;

            virtual void update_anim() override final;

            virtual bool collides_with(const entity &e)
                override final;

            virtual bool can_move() override final {return !_tongue;}


    };

}