#include <bn_log.h>
#include <bn_display.h>
#include "constants.h"
#include "entity.h"


namespace trog {

entity::entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite) : 
        _pos(xcor, ycor),
        _starting_pos(_pos),
        _hitbox(xcor, ycor, width, height),
        _sprite(sprite),
        _top_bound(TROG_COUNTRYSIDE_TOP_BOUND),
        _return_to_starting_point(false),
        _keep_jumping(false),
        _jump_timer(0),
        _jump_time(0),
        _jump_height(0) {
    BN_LOG("entity created at", xcor, ", ", ycor);
}

void entity::move_to(short time, bn::fixed x, bn::fixed y){
    _move_action = bn::sprite_move_to_action(_sprite, time, x, y);
}
void entity::move_to_and_back(short time, bn::fixed x, bn::fixed y){
    _starting_pos = _pos;
    _move_action = bn::sprite_move_to_action(_sprite, time/2, x, y);
    _return_to_starting_point = true;
}

void entity::update(){
    _hitbox.set_position(_pos);
    _sprite.set_position(_pos);
}

void entity::update_anim(){
    if(_move_action && !_move_action->done()){
        _move_action->update();
    }
    if(_move_action && _move_action->done() && _return_to_starting_point){
        set_horizontal_flip(!_sprite.horizontal_flip());
        _move_action = bn::sprite_move_to_action(_sprite, _move_action->duration_updates(), 
            _starting_pos.x(), _starting_pos.y());;
    }
    if(_flip_action){
        _flip_action->update();
    }
    if(_jump_timer){
        _jump_timer++;
        if(_jump_timer == _jump_time){
            _sprite.set_y(_sprite.y() + _jump_height);
        }else if(_jump_timer == _jump_time * 2){
            _sprite.set_y(_sprite.y() - _jump_height);
            if(_keep_jumping) _jump_timer = 1;
        }
    }
}

void entity::flip_every(unsigned short frames){
    _flip_action = bn::sprite_horizontal_flip_toggle_action(_sprite, frames);
}

void entity::jump(short time, bn::fixed height, bool repeating){
    _jump_timer = 1;
    _jump_time = time;
    _jump_height = height;
    _keep_jumping = repeating;
}


bool entity::collides_with(entity &e){
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


}