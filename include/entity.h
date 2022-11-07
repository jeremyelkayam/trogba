#pragma once
#include <bn_fixed_rect.h>
#include <bn_sprite_ptr.h>
#define FRONT_ZORDER 0
#define MID_ZORDER 1
#define BACK_ZORDER 2


namespace trog{

    class entity{ 
    public:
        virtual ~entity() = default;
        virtual void update() = 0;
        bn::fixed_rect get_hitbox() { return _hitbox; }; 
    protected:
        entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite);
        bn::fixed_point _pos;
        bn::fixed_rect _hitbox;
        bn::sprite_ptr _sprite;
    };

}