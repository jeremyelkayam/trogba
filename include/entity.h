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
        virtual void update();
        bn::fixed_rect get_hitbox() { return _hitbox; }
        bn::fixed get_x() { return _pos.x();}
        bn::fixed get_y() { return _pos.y();}
    protected:
        entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite);
        bn::fixed_point _pos;
        bn::fixed_rect _hitbox;
        bn::sprite_ptr _sprite;
    };

}