#pragma once
#include <bn_fixed_rect.h>
#include <bn_sprite_ptr.h>
#include <bn_math.h>
#define FRONT_ZORDER 0
#define MID_ZORDER 1
#define BACK_ZORDER 2


namespace trog{

    class entity{ 
    public:
        virtual ~entity() = default;
        virtual void update();
        virtual bn::fixed_rect get_hitbox() { return _hitbox; }
        bool collides_with(entity &e);
        bn::fixed get_x() { return _pos.x();}
        bn::fixed get_y() { return _pos.y();}
        void set_x(const bn::fixed &x) { _pos.set_x(x);}
        void set_y(const bn::fixed &y) { _pos.set_y(y);}
        void set_horizontal_flip(bool flip) { _sprite.set_horizontal_flip(flip);}
        bool out_of_bounds();
        
        

    protected:
        entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite);
        

        //todo: add const reference keyword where applicable
        bool going_to_go_offscreen_x(const bn::fixed_rect &new_hitbox);   
        bool going_to_go_offscreen_x(const bn::fixed &new_x);  
        
        bool going_to_go_offscreen_y(const bn::fixed_rect &new_hitbox);  
        bool going_to_go_offscreen_y(const bn::fixed &new_y);  


        bool going_to_go_offscreen(const bn::fixed &speed, const bn::fixed &direction); 

        bn::fixed_point unit_vector(bn::fixed angle){return bn::fixed_point(bn::degrees_cos(angle),bn::degrees_sin(angle));}

        bn::fixed_point _pos;
        bn::fixed_rect _hitbox;
        bn::sprite_ptr _sprite;
    };

}