#pragma once
#include <bn_fixed_rect.h>
#include <bn_sprite_ptr.h>
#include <bn_math.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
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
        bool out_of_bounds();

        void set_visible(bool visible) {_sprite.set_visible(visible);}


        //sprite stuff
        void set_horizontal_flip(bool flip) { _sprite.set_horizontal_flip(flip);}
        virtual void update_anim();
        void set_frame(short frame);


        void move_to(short time, bn::fixed x, bn::fixed y);
        

    protected:
        explicit entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite);
        

        //todo: add const reference keyword where applicable
        bool going_to_go_offscreen_x(const bn::fixed_rect &new_hitbox);   
        bool going_to_go_offscreen_x(const bn::fixed &new_x);  
        
        bool going_to_go_offscreen_y(const bn::fixed_rect &new_hitbox);  
        bool going_to_go_offscreen_y(const bn::fixed &new_y);  

        bool going_to_collide_x(const bn::fixed &new_x, const bn::fixed_rect &box);
        bool going_to_collide_y(const bn::fixed &new_y, const bn::fixed_rect &box);

        bool going_to_go_offscreen(const bn::fixed &speed, const bn::fixed &direction); 

        bn::fixed_point unit_vector(bn::fixed angle){return bn::fixed_point(bn::degrees_cos(angle),bn::degrees_sin(angle));}

        bn::fixed_point _pos;
        bn::fixed_rect _hitbox;


        bn::sprite_ptr _sprite;
        bn::optional<bn::sprite_move_to_action> _move_action;

        short _top_bound; // top bound is Different for different classes
    };

}