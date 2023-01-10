// big_text.h
// Purpose: represent the big-ass text that shows up sometimes

#pragma once

#include "big_text.h"
#include "bn_sprite_animate_actions.h"

namespace trog {
    class blood_droplet {
        private:
            bn::sprite_ptr _droplet;
            bn::sprite_animate_action<5> _anim;
        public:
            void update();
            blood_droplet(bn::sprite_item item, int xcor, int ycor, int anim_index);
            void set_blending_enabled(bool enabled){_droplet.set_blending_enabled(enabled);}
    };
    class bloody_text : public big_text { 
        private:

            bn::vector<blood_droplet, 8> _droplets;
            
        public:
            bloody_text(bool bigger, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette);
			virtual void update() final;
            virtual void set_visible(bool visible) final;
            void set_blending_enabled(bool enabled);
    };

}