#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_vector.h>
#include "player.h"
#include "entity.h"
#include "bn_sprite_items_trogbody.h"
#include "bn_sprite_items_majesty.h"
#include "bn_sprite_items_player_dead.h"
#include "bn_sprite_items_beefy_arm.h"
#include "bn_sprite_items_trogtail.h"
#include "bn_sprite_items_trogfeet.h"

namespace trog {

player::player(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes) : 
        entity(xcor, ycor, 24, 34, bn::sprite_items::trogbody.create_sprite(xcor, ycor)),
        _beefy_arm(bn::sprite_items::beefy_arm.create_sprite(xcor, ycor)),
        _tail(bn::sprite_items::trogtail.create_sprite(xcor, ycor)),
        _feet(bn::sprite_items::trogfeet.create_sprite(xcor, ycor)),
        _speed(0.87),
        _majesty(bn::sprite_items::majesty.create_sprite(0,0)),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _feet, 6, bn::sprite_items::trogfeet.tiles_item(), 0, 1, 2, 3)),
        _trogmeter(0),
        _burninate_time(0),
        _time_dead(0),
        _majesty_flash_timer(0),
        _breath(sesh),
        _sesh(sesh),
        _next_pos(xcor,ycor)
        {
    _majesty.set_visible(false);
    _majesty.set_z_order(FRONT_ZORDER);
    _majesty.set_scale(2.3);
    _top_bound = -72;
    _sprite.set_z_order(FRONT_ZORDER);

    // LOGIC CRIBBED FROM Trogdor-Reburninated by Mips96
    _burninate_length = 360;
	if (_sesh.get_level() > 25) {
		_burninate_length /= 1.3;
	} else if (_sesh.get_level() > 20) {
		_burninate_length /= 1.2;
	} else if (_sesh.get_level() > 15) {
		_burninate_length /= 1.1;
	} else if (_sesh.get_level() > 10) {
		_burninate_length /= 1;
	} else if (_sesh.get_level() > 5) {
		_burninate_length /= 0.9;
	} else {
		_burninate_length /= 0.7;
	}
    update_sprites();    


    //todo maybe condense all the timers into one?
    // since they won't all be used at the same time
    if(iframes) {
        _iframes = 1;
    }else{
        _iframes = 0;
    }
}

void player::update_sprites(){
    bn::fixed_point body_offset(3, -6), arm_offset(-15, -2),
                    tail_offset(3, 14), feet_offset(0, tail_offset.y() + 6);
    if(_sprite.horizontal_flip()){
        body_offset.set_x(-body_offset.x());
        arm_offset.set_x(-arm_offset.x());
        tail_offset.set_x(-tail_offset.x());
        feet_offset.set_x(-feet_offset.x());
    } 

    if(_walkcycle.current_index() == 0 || _walkcycle.current_index() == 3){
        body_offset.set_y(body_offset.y() + 1);        
        arm_offset.set_y(arm_offset.y() + 1);        
        tail_offset.set_y(tail_offset.y() + 1);  
        _beefy_arm.set_tiles(bn::sprite_items::beefy_arm.tiles_item(), 1);      
    }else{
        _beefy_arm.set_tiles(bn::sprite_items::beefy_arm.tiles_item(), 0);      
    }

    _sprite.set_position(_pos + body_offset);
    _beefy_arm.set_position(_pos + arm_offset);
    _tail.set_position(_pos + tail_offset);
    _feet.set_position(_pos + feet_offset);
}

bool player::burninating(){
    return _burninate_time != 0 && !dead();
}

void player::update(){
    if(dead()){
        ++_time_dead;
    }
    // this cannot be an else if, because if the respawn method is called 
    // then trogdor will be alive again.
    if(!dead()){
        update_pos();
        entity::update();

        if(any_dpad_input()){
            _walkcycle.update();
        }

        if(_burninate_time > 0) { 
            _burninate_time--;
        }else if(_breath.enabled()){
            // if the breath is enabled AND our burninate timer has expired,
            // we need to end burnination
            _trogmeter = 0;
            _breath.disable();
        }
        update_firebreath();

        //update trogdor to show iframes
        if(_iframes){
            //if we're on an odd iframe, hide the sprite.
            //this creates a flicker effect
            bool odd = _iframes % 2;
            if(odd){
                _sprite.set_visible(false);
                _beefy_arm.set_visible(false);
                _tail.set_visible(false);
                _feet.set_visible(false);
            }else{
                _sprite.set_visible(true);
                _beefy_arm.set_visible(true);
                _tail.set_visible(true);
                _feet.set_visible(true);
            }
            ++_iframes;
        }

        //once the iframes are over, return the sprite to normal
        if(_iframes > 120) {
            _sprite.set_visible(true);
            _iframes = 0;
        }
        update_next_pos();
        update_sprites();
    }

}

bool player::any_dpad_input() {
    return (
        bn::keypad::up_held() ||
        bn::keypad::down_held() ||
        bn::keypad::left_held() ||
        bn::keypad::right_held()
    );
}

void player::update_next_pos(){
    _next_pos = _pos;

    if(bn::keypad::up_held()){
        _next_pos.set_y(_pos.y() - _speed);
    }
    if(bn::keypad::down_held()){
        _next_pos.set_y(_pos.y() + _speed);
    }
    if(bn::keypad::left_held()){
        set_horizontal_flip(true);
        _next_pos.set_x(_pos.x() - _speed);
    }
    if(bn::keypad::right_held()){
        set_horizontal_flip(false);
        _next_pos.set_x(_pos.x() + _speed);
    }

    // should check bounds while we r here
    check_boundary_collision();

}

void player::check_boundary_collision(){
    if(going_to_go_offscreen_x(_next_pos.x())){
        _next_pos.set_x(_pos.x());
    }
    if(going_to_go_offscreen_y(_next_pos.y())){
        _next_pos.set_y(_pos.y());
    }
}

void player::free_from_collisionbox(const bn::fixed_rect &box){
    //if you intersect a cottage that is unburninated, we need to warp you out
    BN_ASSERT(_hitbox.intersects(box));

    bn::fixed downdist,updist,leftdist,rightdist,min_dist;
    downdist = bn::abs(box.bottom() - _hitbox.top());
    updist = bn::abs(box.top() - _hitbox.bottom());


    leftdist = bn::abs(box.left() - _hitbox.right());
    rightdist = bn::abs(box.right() - _hitbox.left());

    // ah yes, 3 nested mins, very good coding jeremy
    min_dist = bn::min(bn::min(updist,downdist),bn::min(leftdist,rightdist));
    if(min_dist == downdist) {
        _next_pos.set_y(_pos.y() + downdist);
    }else if(min_dist == updist) {
        _next_pos.set_y(_pos.y() - updist);
    }else if(min_dist == leftdist) {
        _next_pos.set_x(_pos.x() - leftdist);
    }else if(min_dist == rightdist) {
        _next_pos.set_x(_pos.x() + rightdist);
    }else{
        BN_ASSERT(false, "error freeing player from cottage collision");
    }
}

void player::handle_wall_collision(const bn::fixed_rect &wall_hitbox){
    bool going_to_hit_wall_x = false;
    bool going_to_hit_wall_y = false;
    
    going_to_hit_wall_x = 
        going_to_hit_wall_x || going_to_collide_x(_next_pos.x(), wall_hitbox);

    going_to_hit_wall_y = 
        going_to_hit_wall_y || going_to_collide_y(_next_pos.y(), wall_hitbox);

    //separate clauses for x and y coords so that you can input a diagonal
    //but still move along the sides of the screen
    if(going_to_hit_wall_x){
        _next_pos.set_x(_pos.x());
    }
    if(going_to_hit_wall_y){
        _next_pos.set_y(_pos.y());
    }
}

bool player::handle_cottage_collision(cottage &cottage){
    const bn::fixed_rect &cottagebox = cottage.get_hitbox();
    if(!cottage.burninated()){
        handle_wall_collision(cottagebox);
    }    


    if(_hitbox.intersects(cottagebox) && !cottage.burninated()){
        //get him OUT of that box!!
        free_from_collisionbox(cottagebox);
    }
    if(burninating()){
        _breath.handle_cottage_collision(cottage);
    }
    return false;
}

void player::handle_peasant_collision(peasant &peasant){
    if(burninating()){
        _breath.handle_peasant_collision(peasant);
    }else if(!dead() && collides_with(peasant) && !peasant.dead()){
        peasant.stomp();
        _sesh.score(2);
        
        ++_trogmeter;
        if(_trogmeter == 10){
            start_burninating();
        }
    }

}

void player::start_burninating(){
    _burninate_time = _burninate_length;
    _breath.enable();
}

void player::handle_knight_collision(knight &knight){
    if(collides_with(knight) && !invincible()) { 
        die(1);
    }
}

void player::handle_arrow_collision(archer &archer){
    if(collides_with(archer) &&  !invincible()) { 
        die(0);
        archer.destroy_arrow();
    }
}

void player::die(short frame_no){
    _time_dead = 1;
    _sprite.set_item(bn::sprite_items::player_dead, frame_no);
    _sprite.set_y(_pos.y() + 10);
    _breath.disable();
    _beefy_arm.set_visible(false);
    _tail.set_visible(false);
    _feet.set_visible(false);
}

void player::update_firebreath(){
    short xoffset = 30;
    if(_sprite.horizontal_flip()){
        xoffset=-xoffset;
    }
    _breath.set_x(_pos.x() + xoffset);
    _breath.set_y(_pos.y() + -8);        
    _breath.update();
}

void player::set_visible(bool visible){
    entity::set_visible(visible);
    if(_breath.enabled()){
        _breath.set_visible(visible);
    }
}

void player::set_horizontal_flip(bool flip){
    entity::set_horizontal_flip(flip);
    _breath.set_horizontal_flip(flip);
    _beefy_arm.set_horizontal_flip(flip);
    _tail.set_horizontal_flip(flip);
    _feet.set_horizontal_flip(flip);
}

bool player::invincible(){
    //if you're dead you're invincible. 
    // If you're burninating you're invincible.
    // If you have post-respawn invulnerability, you are also invincible.
    return _iframes || dead() || burninating();
}

void player::setup_win_pose(const bn::fixed &x, const bn::fixed &y){
    _majesty_flash_timer++;
    _pos.set_x(x);
    _pos.set_y(y);
    set_horizontal_flip(false);
    _feet.set_tiles(bn::sprite_items::trogfeet.tiles_item(), 1);
    _beefy_arm.set_tiles(bn::sprite_items::beefy_arm.tiles_item(), 0);
    update_sprites();
    bn::vector<bn::sprite_ptr, 4> _sprites;
    _sprites.emplace_back(_sprite);
    _sprites.emplace_back(_beefy_arm);
    _sprites.emplace_back(_tail);
    _sprites.emplace_back(_feet);

    for(bn::sprite_ptr &sprite : _sprites) {
        sprite.set_position(sprite.x() * 2, sprite.y() * 2);
        sprite.set_scale(2);
        sprite.put_above();
    }
    _feet.set_y(_feet.y() + 2);
    _majesty.put_above();
    
    _majesty.set_visible(_majesty_flash_timer >= 15);
    _breath.set_visible(false);

    if(_majesty_flash_timer > 15 * 2){
        _majesty_flash_timer = 0;
    }

}


void player::flex(){
    _beefy_arm.set_tiles(bn::sprite_items::beefy_arm.tiles_item(), 0);
    _beefy_arm.set_vertical_flip(true);
    _beefy_arm.set_y(_beefy_arm.y() - 9 * _beefy_arm.vertical_scale());
}

}
