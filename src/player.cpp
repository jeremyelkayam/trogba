#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>
#include "player.h"
#include "entity.h"
#include "bn_sprite_items_majesty.h"
#include "sb_commentary.h"

namespace trog {
player::player(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::fixed speed, bn::fixed_point breath_offset, session_info &sesh, bool iframes, bn::sprite_item spritem, unsigned int  walk_cycle_frames, common_stuff &common_stuff, unsigned int  initial_trogmeter) : 
        entity(xcor, ycor, width, height, spritem.create_sprite(xcor, ycor)),
        _spritem(spritem),
        _speed(speed),
        _breath_offset(breath_offset),
        _majesty(bn::sprite_items::majesty.create_sprite(0,0)),
        _trogmeter(initial_trogmeter),
        _burninate_time(0),
        _time_dead(0),

        //todo maybe condense all the timers into one?
        // since they won't all be used at the same time        
        _iframes(iframes ? TROG_RESPAWN_INV_TIME : 0),
        _majesty_flash_timer(0),
        _walk_cycle_frames(walk_cycle_frames),
        _breath(sesh, common_stuff),
        _sesh(sesh),
        _common_stuff(common_stuff),
        _next_pos(xcor,ycor)
        {
    if(_trogmeter > 0){
        BN_ASSERT(sesh.get_level() == 0, "Initial trogmeter value can only be set in the tutorial");
        BN_ASSERT(_trogmeter < _trogmeter_max, "Invalid value entered for trogmeter value: ", _trogmeter, " (greater than trogmeter max value of ", _trogmeter_max, ")");
    }
    
    _majesty.set_visible(false);
    _majesty.set_z_order(FRONT_ZORDER);
    _majesty.set_scale(2.3);
    _top_bound = TROG_COUNTRYSIDE_PLAYER_TOP_BOUND;
    _sprite.set_z_order(FRONT_ZORDER);

    // LOGIC CRIBBED FROM Trogdor-Reburninated by Mips96
    _burninate_length = TROG_BURNINATE_TIME;
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

        if(_burninate_time > 0) { 
            _burninate_time--;
        }
        if(_burninate_time == 0 && _breath.enabled()){
            //after the timer runs out, we must stop burninating
            stop_burninating();
        }

        //update trogdor to show iframes
        if(_iframes){
            //if we're on an odd iframe, hide the sprite.
            //this creates a flicker effect
            bool odd = _iframes % 2;
            _sprite.set_visible(odd);
            --_iframes;
        }
        if(can_move()){
            update_next_pos();
        }
        update_firebreath();

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
    if(burninating()){
        _breath.handle_peasant_collision(peasant);
    }else if(!dead() && collides_with(peasant) && !peasant.dead()){
        kill_peasant(peasant);
    }
}

void player::kill_peasant(peasant &peasant)
{
    peasant.squish();

    bn::sound_items::stomp.play(_common_stuff.savefile.options.sound_vol);
    _common_stuff.savefile.stats.peasants_stomped++;
    
    _sesh.score(TROG_PEASANT_STOMP_SCORE);
    
    ++_trogmeter;
    if(_trogmeter == _trogmeter_max){
        start_burninating();
    }else{
        //only say a stomp quote if you didn't just start burning
        _common_stuff.commentary.stomp_peasant(_sesh.get_dragon());
    }
}

void player::start_burninating(){
    _burninate_time = _burninate_length;
    enable_breath();
    bn::sound_items::burninate.play(_common_stuff.savefile.options.sound_vol);
}

void player::stop_burninating(){
    _trogmeter = 0;
    disable_breath();
}

void player::handle_knight_collision(knight &knight){
    if(!knight.frozen() && collides_with(knight) && !invincible()) { 
        die(2);
    }
}

void player::handle_troghammer_collision(troghammer &troghammer){
    if(!troghammer.frozen() && collides_with(troghammer) && !invincible()) { 
        die(1);
    }
}

void player::handle_arrow_collision(archer &archer){
    if(collides_with(archer) &&  !invincible()) { 
        die(0);
        archer.destroy_arrow();
    }
}

void player::die(const unsigned int &death_index){
    bn::sound_items::death.play(_common_stuff.savefile.options.sound_vol);
    _time_dead = 1;
    _breath.disable();
    _sprite.set_tiles(_spritem.tiles_item(), death_index + _walk_cycle_frames);
}


void player::update_firebreath(){
    bn::fixed xoffset = _breath_offset.x();
    if(_sprite.horizontal_flip()){
        xoffset=-xoffset;
    }
    _breath.set_x(_sprite.position().x() + xoffset);
    _breath.set_y(_sprite.position().y() + _breath_offset.y());        
    _breath.update();
}

void player::set_visible(const bool &visible){
    entity::set_visible(visible);
    if(_breath.enabled()){
        _breath.set_visible(visible);
    }
}

void player::set_horizontal_flip(const bool &flip){
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
    _majesty_flash_timer++;

    //reset the sprite if the player is dead.
    if(dead()){
        _sprite.set_tiles(_spritem.tiles_item(), 0);
    }
    
    _sprite.set_horizontal_flip(false);
    _sprite.set_position(0, 0);
    _sprite.set_scale(2);
    bn::sprite_palette_ptr pal = _sprite.palette();
    pal.set_fade_intensity(0);

    _sprite.put_above();
    _majesty.put_above();
    
    _majesty.set_visible(_majesty_flash_timer < TROG_MAJESTY_FLASH_INTERVAL);
    _breath.set_visible(false);

    if(_majesty_flash_timer > TROG_MAJESTY_FLASH_INTERVAL * 2){
        _majesty_flash_timer = 0;
    }
}

}
