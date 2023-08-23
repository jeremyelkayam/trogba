#pragma once
#include <bn_fixed_rect.h>
#include <bn_sprite_ptr.h>
#include <bn_math.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_palette_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_unique_ptr.h>
#define FRONT_ZORDER 0
#define MID_ZORDER 1
#define BACK_ZORDER 2
#define FURTHEST_BACK_ZORDER 3

namespace trog{

    class entity{ 
    public:
        virtual ~entity() = default;
        virtual void update();
        virtual bn::fixed_rect get_hitbox() const { return _hitbox; }
        bool collides_with(const entity &e);
        bn::fixed get_x() { return _pos.x();}
        bn::fixed get_y() { return _pos.y();}
        bn::fixed_point get_pos() { return _pos; }
        bn::fixed sprite_x() { return _sprite.x();}
        bn::fixed sprite_y() { return _sprite.y();}
        void set_x(const bn::fixed &x) { _pos.set_x(x); _sprite.set_x(x);}
        void set_y(const bn::fixed &y) { _pos.set_y(y); _sprite.set_y(y);}
        void set_y_offset(const bn::fixed &y) {_offset.set_y(y); _sprite.set_y(_pos.y() + y);}
        bool out_of_bounds();
        void set_rotation_angle(const short &angle){_sprite.set_rotation_angle(angle);}

        virtual void set_visible(const bool &visible) {_sprite.set_visible(visible);}

        //sprite stuff
        virtual void set_horizontal_flip(const bool &flip) { _sprite.set_horizontal_flip(flip);}
        virtual void update_anim();
        void flip_every(const uint8_t &frames);

        void move_to(const short &time, const bn::fixed &x, const bn::fixed &y);
        void move_from(const short &time, const bn::fixed &x, const bn::fixed &y);        
        void move_to_and_back(const short &time, const bn::fixed &x, const bn::fixed &y);
        void move_by(const bn::fixed &x, const bn::fixed &y);
        void update_anim_action_when_not_moving(const bool &update) 
            {_update_anim_when_not_moving = update;}
        void jump(const short &time, const bn::fixed &height, const bool &repeating);
        void squish(const short &time);

        void grayscale_to(const int &duration_updates, const bn::fixed &final_intensity);

        void set_grayscale(const bn::fixed &intensity);

        void set_blending_enabled(const bool &enabled) 
            {_sprite.set_blending_enabled(enabled);}

        void drop();

        void scale_to(const int &duration_updates, const bn::fixed &scale_factor);
        void set_scale(const bn::fixed &scale) {_sprite.set_scale(scale);}

    protected:
        explicit entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite);
        

        bool going_to_go_offscreen_x(const bn::fixed_rect &new_hitbox);   
        bool going_to_go_offscreen_x(const bn::fixed &new_x);  
        
        bool going_to_go_offscreen_y(const bn::fixed_rect &new_hitbox);  
        bool going_to_go_offscreen_y(const bn::fixed &new_y);  

        bool going_to_collide_x(const bn::fixed &new_x, const bn::fixed_rect &box);
        bool going_to_collide_y(const bn::fixed &new_y, const bn::fixed_rect &box);

        bool going_to_go_offscreen(const bn::fixed &speed, const bn::fixed &direction); 

        bn::fixed_point unit_vector(bn::fixed angle)
            {return bn::fixed_point(bn::degrees_cos(angle),bn::degrees_sin(angle));}

        bn::fixed_point _pos, _starting_pos, _offset;
        bn::fixed_rect _hitbox;


        bn::sprite_ptr _sprite;
        bn::unique_ptr<bn::sprite_move_to_action> _move_action;
        bn::unique_ptr<bn::sprite_horizontal_flip_toggle_action> _flip_action;
        bn::unique_ptr<bn::sprite_move_by_action> _move_by_action;
        bn::unique_ptr<bn::sprite_scale_to_action> _scale_action;
        bn::unique_ptr<bn::sprite_vertical_scale_to_action> _vst_action;
        bn::unique_ptr<bn::sprite_horizontal_scale_to_action> _hst_action;
        bn::unique_ptr<bn::sprite_palette_grayscale_to_action> _gst_action;

        //for dropping only
        bn::optional<bn::sprite_ptr> _dust_cloud;
        bn::unique_ptr<bn::sprite_animate_action<8>> _dust_anim;

        short _top_bound; // top bound is Different for different classes
        bool _return_to_starting_point;//challenge again
        bool _drop;
        bool _update_anim_when_not_moving;        
        bool _keep_jumping;
        short _jump_timer, _jump_time;
        bn::fixed _jump_height;
    };

}