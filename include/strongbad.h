// strongbad.h
// Purpose: represent Strong Bad's whole self

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_strongbad.h"
#include "entity.h"
#include "session_info.h"

#include "constants.h"

namespace trog {
    class strongbad : public entity { 
        protected:
            bn::sprite_animate_action<2> _anim;
            bool _advance_anim;

        public:
            strongbad(int xcor, int ycor);
			virtual void update() final;
            virtual void update_anim() final;
            void talk();
            void start_animating() {_advance_anim = true;}
            void stop_animating() {_advance_anim = false;}
            void set_frame(short frame_number){_sprite.set_tiles(bn::sprite_items::strongbad.tiles_item(), frame_number);}

    };
}