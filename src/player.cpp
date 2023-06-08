#include <bn_keypad.h>
#include <bn_math.h>
#include "player.h"
#include "entity.h"
#include "bn_sprite_items_trogbody.h"
#include "bn_sprite_items_player_dead.h"

namespace trog {

player::player(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes) : 
        entity(xcor, ycor, 24, 34, bn::sprite_items::trogbody.create_sprite(xcor + BODY_XOFFSET, ycor + BODY_YOFFSET)),
        _speed(0.87),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 5, bn::sprite_items::trogbody.tiles_item(), 0, 0, 0, 0)),
        _trogmeter(0),
        _burninate_time(0),
        _time_dead(0),
        _breath(sesh),
        _sesh(sesh),
        _next_pos(xcor,ycor)
        {
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


    //todo maybe condense all the timers into one?
    // since they won't all be used at the same time
    if(iframes) {
        _iframes = 1;
    }else{
        _iframes = 0;
    }
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
            }else{
                _sprite.set_visible(true);
            }
            ++_iframes;
        }

        //once the iframes are over, return the sprite to normal
        if(_iframes > 120) {
            _sprite.set_visible(true);
            _iframes = 0;
        }
        update_next_pos();
        _sprite.set_x(_sprite.x() + ((_sprite.horizontal_flip() ? -1 : 1) * BODY_XOFFSET));
        _sprite.set_y(_sprite.y() + BODY_YOFFSET);        

        // #ifdef DEBUG
        //     //Insta-burninate by pressing b
        //     if(bn::keypad::b_pressed()){
        //         start_burninating();   
        //     }
        // #endif
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
}

bool player::invincible(){
    //if you're dead you're invincible. 
    // If you're burninating you're invincible.
    // If you have post-respawn invulnerability, you are also invincible.
    return _iframes || dead() || burninating();
}

void player::update_win_anim(){
    _sprite.set_horizontal_flip(false);
    _sprite.set_position(0, 0);
    _sprite.set_scale(2);
    _sprite.set_item(bn::sprite_items::trogbody);
    _sprite.put_above();
    _breath.set_visible(false);
}

}
