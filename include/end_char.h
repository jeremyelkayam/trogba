// end_char.h
// Purpose: represent the character in the ending cutscene

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "session_info.h"

#include "constants.h"

namespace trog {
    class end_char : public entity { 
        protected:
            bn::sprite_animate_action<2> _anim;
            bool _advance_anim;
            const bn::sprite_item _item;

        public:
            end_char(int xcor, int ycor, const bn::sprite_item item);
			virtual void update() final;
            virtual void update_anim() final;
            void talk();
            void start_animating() {_advance_anim = true;}
            void stop_animating() {_advance_anim = false;}
            void set_frame(short frame_number)
                {_sprite.set_tiles(_item.tiles_item(), frame_number);}

    };
}