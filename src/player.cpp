#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>
#include "player.h"
#include "entity.h"
#include "bn_sprite_items_majesty.h"
#include "bn_sprite_items_player.h"

namespace trog {

player::player(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes) : 
        entity(xcor, ycor, TROG_PLAYER_WIDTH, TROG_PLAYER_HEIGHT, bn::sprite_items::player.create_sprite(xcor, ycor)),
        _speed(TROG_PLAYER_SPEED),
        _majesty(bn::sprite_items::majesty.create_sprite(0,0)),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3)),
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
    _top_bound = TROG_COUNTRYSIDE_PLAYER_TOP_BOUND;
    _sprite.set_z_order(FRONT_ZORDER);

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
        if(_iframes > TROG_RESPAWN_INV_TIME) {
            BN_LOG("end invincibility");
            _sprite.set_visible(true);
            _iframes = 0;
        }
        update_next_pos();

        #ifdef DEBUG
            //Insta-burninate by pressing b
            if(bn::keypad::b_pressed()){
                start_burninating();   
            }
        #endif
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
        _sprite.set_horizontal_flip(true);
        _breath.set_horizontal_flip(true);
        _next_pos.set_x(_pos.x() - _speed);
    }
    if(bn::keypad::right_held()){
        _sprite.set_horizontal_flip(false);
        _breath.set_horizontal_flip(false);
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
    BN_LOG("down distance ", downdist);
    updist = bn::abs(box.top() - _hitbox.bottom());
    BN_LOG("up distance ", updist);


    leftdist = bn::abs(box.left() - _hitbox.right());
    BN_LOG("left distance ", leftdist);
    rightdist = bn::abs(box.right() - _hitbox.left());
    BN_LOG("right distance ", rightdist);

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
    if(!cottage.burninated() && !cottage.has_treasure()){
        handle_wall_collision(cottagebox);
    }    


    if(_hitbox.intersects(cottagebox) && !cottage.burninated()){
        if(cottage.has_treasure()){
            return true;
        }else{
            //get him OUT of that box!!
            free_from_collisionbox(cottagebox);
        }
    }
    if(burninating()){
        _breath.handle_cottage_collision(cottage);
    }
    return false;
}

void player::handle_peasant_collision(peasant &peasant){
    if(!dead() && collides_with(peasant) && !peasant.dead()){
        BN_LOG("stomped.");
        peasant.stomp();
        _sesh.score(TROG_PEASANT_STOMP_SCORE);
        
        ++_trogmeter;
        if(_trogmeter == _trogmeter_max){
            start_burninating();
        }
    }

    if(burninating()){
        _breath.handle_peasant_collision(peasant);
    }
}

void player::start_burninating(){
    _burninate_time = _burninate_length;
    _breath.enable();

    bn::sound_items::burninate.play(TROG_DEFAULT_VOLUME);
}

void player::handle_knight_collision(knight &knight){
    if(collides_with(knight) && !invincible()) { 
        die(6);
    }
}

void player::handle_arrow_collision(archer &archer){
    if(collides_with(archer) &&  !invincible()) { 
        die(4);
        archer.destroy_arrow();
    }
}

void player::die(short frame_no){
    bn::sound_items::death.play(TROG_DEFAULT_VOLUME);
    _time_dead = 1;
    _sprite.set_tiles(bn::sprite_items::player.tiles_item(), frame_no);
    _breath.disable();
}

void player::pass_out(){
    _sprite.set_tiles(bn::sprite_items::player.tiles_item(), 12);
}

void player::thumb_it_up(){
    _sprite.set_tiles(bn::sprite_items::player.tiles_item(), 11);
}

void player::flex(){
    _flex = bn::create_sprite_animate_action_forever(
                _sprite, 5, bn::sprite_items::player.tiles_item(), 
                7, 8, 9, 10, 10, 9, 8);
}
void player::update_anim(){
    entity::update_anim();
    if(_move_action && !_move_action->done()){
        _walkcycle.update();
    }
    if(_flex){
        _flex->update();
    }
    update_firebreath();

}

void player::update_firebreath(){
    short xoffset = TROG_FIREBREATH_XOFFSET;
    if(_sprite.horizontal_flip()){
        xoffset=-xoffset;
    }
    _breath.set_x(_sprite.position().x() + xoffset);
    _breath.set_y(_sprite.position().y() + TROG_FIREBREATH_YOFFSET);        
    _breath.update();
}

void player::set_visible(bool visible){
    entity::set_visible(visible);
    if(_breath.enabled()){
        _breath.set_visible(visible);
    }
}

bool player::invincible(){
    //if you're dead you're invincible. 
    // If you're burninating you're invincible.
    // If you have post-respawn invulnerability, you are also invincible.
    return _iframes || dead() || burninating();
}

void player::update_win_anim(){
    _majesty_flash_timer++;

    
    _sprite.set_horizontal_flip(false);
    _sprite.set_position(0, 0);
    _sprite.set_scale(2);
    _sprite.set_item(bn::sprite_items::player);
    _sprite.put_above();
    _majesty.put_above();
    
    _majesty.set_visible(_majesty_flash_timer < TROG_MAJESTY_FLASH_INTERVAL);
    _breath.set_visible(false);

    if(_majesty_flash_timer > TROG_MAJESTY_FLASH_INTERVAL * 2){
        _majesty_flash_timer = 0;
    }

}


}
