#include <bn_log.h>
#include <bn_display.h>
#include "constants.h"
#include "entity.h"
#include "bn_sprite_items_wmg_dustcloud.h"


namespace trog {

entity::entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite) : 
        _pos(xcor, ycor),
        _starting_pos(_pos),
        _hitbox(xcor, ycor, width, height),
        _sprite(sprite),
        _top_bound(TROG_COUNTRYSIDE_TOP_BOUND),
        _return_to_starting_point(false),
        _drop(false),
        _update_anim_when_not_moving(false),
        _keep_jumping(false),
        _jump_timer(0),
        _jump_time(0),
        _jump_height(0) {
}

void entity::move_to(const short &time, const bn::fixed &x, const bn::fixed &y){
    _move_action.reset(new bn::sprite_move_to_action(_sprite, time, x, y));
}

void entity::move_from(const short &time, const bn::fixed &x, const bn::fixed &y){
    _sprite.set_position(x, y);
    move_to(time, _pos.x(), _pos.y());
}

void entity::move_to_and_back(const short &time, const bn::fixed &x, const bn::fixed &y){
    _starting_pos = _pos;
    _move_action.reset(new bn::sprite_move_to_action(_sprite, time/2, x, y));
    _return_to_starting_point = true;
}

void entity::update(){
    _hitbox.set_position(_pos);
    _sprite.set_position(_pos + _offset);
}

void entity::update_anim(){
    if(_move_action && !_move_action->done()){
        _move_action->update();
    }
    if(_move_action && _move_action->done() && _return_to_starting_point){
        set_horizontal_flip(!_sprite.horizontal_flip());
        _move_action.reset(new bn::sprite_move_to_action(_sprite, _move_action->duration_updates(), 
            _starting_pos.x(), _starting_pos.y()));
    }

    //drop clause
    //this is fucking garbage and i'm ashamed of what i have done
    if(_move_action && _move_action->done() && _drop){
        //once it's dropped, start the dust cloud animation
        if(!_dust_cloud){
            _dust_cloud = bn::sprite_items::wmg_dustcloud.create_sprite(_pos.x(), _hitbox.bottom() + 2);
            _dust_anim.reset(new bn::sprite_animate_action(bn::create_sprite_animate_action_once(*_dust_cloud.get(), 2, 
                bn::sprite_items::wmg_dustcloud.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7)));
        }

        // once we finish squishing the sprite down, unsquish it back up.
        if(_vst_action->done()) {
            _drop = false;
            _vst_action.reset(new bn::sprite_vertical_scale_to_action(_sprite, 15, 1));
        }
    }

    if(_dust_anim){
        _dust_anim->update();
        if(_dust_anim->done()){
            _dust_anim.reset();
            _dust_cloud.reset();
        }
    }

    //squish clause
    if(_hst_action && !_hst_action->done())
        _hst_action->update();
    if(_vst_action && !_vst_action->done()){
        if(!_drop || (_move_action && _move_action->done())){
            _vst_action->update();
            _sprite.set_y(_pos.y() + (1 - _sprite.vertical_scale()) * (_hitbox.height() * 1.25));
        }
    }

    if(_flip_action){
        _flip_action->update();
    }
    if(_jump_timer){
        _jump_timer++;
        if(_jump_timer == _jump_time){
            //up is negative so jump height should be subtracted
            _sprite.set_y(_sprite.y() - _jump_height);
        }else if(_jump_timer == _jump_time * 1.5){
            _sprite.set_y(_sprite.y() + _jump_height);
            if(_keep_jumping) _jump_timer = 1;
        }
    }
    if(_move_by_action){
        BN_LOG("updating move by action");
        _move_by_action->update();
    }
    if(_scale_action && !_scale_action->done()){
        _scale_action->update();
    }
    if(_gst_action && !_gst_action->done()){
        _gst_action->update();
    }
}

void entity::flip_every(const uint8_t &frames){
    _flip_action.reset(new bn::sprite_horizontal_flip_toggle_action(_sprite, frames));
}

void entity::jump(const short &time, const bn::fixed &height, const bool &repeating){
    _jump_timer = 1;
    _jump_time = time;
    _jump_height = height;
    _keep_jumping = repeating;
}

void entity::set_grayscale(const bn::fixed &intensity){
    bn::sprite_palette_ptr palette = _sprite.palette();
    palette.set_grayscale_intensity(intensity);
}


bool entity::collides_with(const entity &e){
    return _hitbox.intersects(e.get_hitbox());
}

bool entity::out_of_bounds(){
    return !_hitbox.intersects(bn::fixed_rect(0, 0, bn::display::width(), bn::display::height()));
}


//collision shit
bool entity::going_to_go_offscreen_x(const bn::fixed_rect &new_hitbox){
    bool oob_left = new_hitbox.left() < TROG_COUNTRYSIDE_LEFT_BOUND; 
    bool oob_right = new_hitbox.right() > TROG_COUNTRYSIDE_RIGHT_BOUND;
    return (oob_left || oob_right);
}

bool entity::going_to_go_offscreen_y(const bn::fixed_rect &new_hitbox){
    bool oob_top = new_hitbox.top() < _top_bound;
    bool oob_bottom =  new_hitbox.bottom() > TROG_COUNTRYSIDE_BOTTOM_BOUND;
    return (oob_top || oob_bottom);
}


// overloads
bool entity::going_to_go_offscreen_x(const bn::fixed &new_x){
    bn::fixed_point new_pos(new_x, _hitbox.y());
    return going_to_go_offscreen_x(bn::fixed_rect(new_pos, _hitbox.dimensions()));
}

bool entity::going_to_go_offscreen_y(const bn::fixed &new_y){
    bn::fixed_point new_pos(_hitbox.x(), new_y);
    return going_to_go_offscreen_y(bn::fixed_rect(new_pos, _hitbox.dimensions()));
}

bool entity::going_to_go_offscreen(const bn::fixed &speed, const bn::fixed &direction){
    bn::fixed_rect new_hitbox = bn::fixed_rect(_hitbox.position() + (unit_vector(direction).multiplication(speed)), _hitbox.dimensions());
    return going_to_go_offscreen_x(new_hitbox) || going_to_go_offscreen_y(new_hitbox);
}

bool entity::going_to_collide_x(const bn::fixed &new_x, const bn::fixed_rect &box){
    bn::fixed_rect new_hitbox=_hitbox;
    new_hitbox.set_x(new_x);

    return box.intersects(new_hitbox);
}

bool entity::going_to_collide_y(const bn::fixed &new_y, const bn::fixed_rect &box){
    bn::fixed_rect new_hitbox=_hitbox;
    new_hitbox.set_y(new_y);

    return box.intersects(new_hitbox);
}

void entity::move_by(const bn::fixed &x, const bn::fixed &y){
    BN_LOG("help me");
    _move_by_action.reset(new bn::sprite_move_by_action(_sprite, x, y));
}

void entity::squish(const short &time){
    _sprite.set_vertical_scale(0.8);
    _sprite.set_horizontal_scale(1.2);
    _vst_action.reset(new bn::sprite_vertical_scale_to_action(_sprite, time, 1));
    _hst_action.reset(new bn::sprite_horizontal_scale_to_action(_sprite, time, 1));
}

void entity::grayscale_to(const int &duration_updates, const bn::fixed &final_intensity){
    _gst_action.reset(new bn::sprite_palette_grayscale_to_action(_sprite.palette(), duration_updates, final_intensity));
}

void entity::drop(){
    _sprite.set_y(_pos.y() - 160);
    move_to(60, _pos.x(), _pos.y());
    _vst_action.reset(new bn::sprite_vertical_scale_to_action(_sprite, 15, 0.6));
    _drop = true;
}

void entity::scale_to(const int &duration_updates, const bn::fixed &scale_factor){
    _scale_action.reset(new bn::sprite_scale_to_action(_sprite, duration_updates, scale_factor));
}

}