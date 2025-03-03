// question_mark.h

#pragma once
#include "bn_sprite_items_question_mark.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class question_mark : public entity { 
        protected:
            bn::sprite_animate_action<20> _anim;
            
        public:
            question_mark(int xcor, int ycor)
                : entity(xcor, ycor, 0, 0, 
                    bn::sprite_items::question_mark.create_sprite(
                        xcor, ycor)),
                _anim(bn::create_sprite_animate_action_forever(
                    _sprite, 3,
                    bn::sprite_items::question_mark.tiles_item(),
                    1, 1, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 1, 1,
                    0, 0, 0, 0
                )){}
			virtual void update() final
                {entity::update();}
            virtual void update_anim() final
                {entity::update_anim(); _anim.update();}
    };
}