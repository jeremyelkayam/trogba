// lock_icon.h

#pragma once
#include "bn_sprite_items_lock_icon.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class lock_icon : public entity { 
        protected:
            
        public:
            lock_icon(int xcor, int ycor)
                : entity(xcor, ycor, 0, 0, 
                    bn::sprite_items::lock_icon.create_sprite(
                        xcor, ycor)) {}
			virtual void update() final
                {entity::update();}
            virtual void update_anim() final
                {entity::update_anim();}
    };
}