#pragma once
#include <bn_fixed_rect.h>
#include <bn_sprite_ptr.h>


namespace trog{

    class entity{ 
    public:
        virtual ~entity() = default;
        virtual void update() = 0;
    protected:
        entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite);
        bn::fixed_point _pos;
        bn::fixed_rect _hitbox;
        bn::sprite_ptr _sprite;
    };

}