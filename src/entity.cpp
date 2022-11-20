#include <bn_log.h>
#include <bn_display.h>
#include "constants.h"
#include "entity.h"


namespace trog {

entity::entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite) : 
        _pos(xcor, ycor),
        _hitbox(xcor, ycor, width, height),
        _sprite(sprite){
    BN_LOG("entity created at", xcor, ", ", ycor);
}

void entity::update(){
    _hitbox.set_position(_pos);
    _sprite.set_position(_pos);
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
    bool oob_top = new_hitbox.top() < TROG_COUNTRYSIDE_TOP_BOUND;
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

bool entity::going_to_collide_x(const bn::fixed &new_x, entity &e){
    bn::fixed_rect new_hitbox=_hitbox;
    new_hitbox.set_x(new_x);

    return e.get_hitbox().intersects(new_hitbox);
}

bool entity::going_to_collide_y(const bn::fixed &new_y, entity &e){
    bn::fixed_rect new_hitbox=_hitbox;
    new_hitbox.set_y(new_y);

    return e.get_hitbox().intersects(new_hitbox);
}

void entity::hide(){
    _sprite.set_visible(false);
}
void entity::show(){
    _sprite.set_visible(true);
}

}