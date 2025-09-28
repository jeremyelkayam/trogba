#include "wormdingler.h"
#include "dragon_data.h"
#include "bn_sprite_items_wormdingler.h"
#include "bn_sprite_items_wormdingler_tongue.h"
#include <bn_math.h>
#include <bn_keypad.h>
#include "common_functions.h"

namespace trog { 


wormdingler::wormdingler(bn::fixed xcor, bn::fixed ycor, 
    session_info &sesh, bool iframes, common_stuff &common_stuff, 
    uint8_t initial_trogmeter) : 
    player(xcor, ycor, WDATA.width, WDATA.height, WDATA.speed, 
        bn::fixed_point(31, 0), sesh, 
        iframes, bn::sprite_items::wormdingler, 5, common_stuff, 
        initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 3, 
            bn::sprite_items::wormdingler.tiles_item(), 1, 2, 3, 4, 3, 2, 
            1, 0)) 
{

}


void wormdingler::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action || 
        _update_anim_when_not_moving){
        _walkcycle.update();
    }
    update_firebreath();

    
    if(_extendo){
        _extendo->update();
    }
}

void wormdingler::kill_peasant(peasant &peasant)
{
    player::kill_peasant(peasant);
    if(_tongue)
    {
        _common_stuff.update_achievement("sub");
    }
}


void wormdingler::update(){

    player::update();

    if(!_tongue && !dead() && any_dpad_input()){
        _walkcycle.update();
    }
    
    if(burninating()){
        _tongue.reset();


    }
    else if(!_tongue && !dead() && bn::keypad::a_pressed())
    {
        _tongue.emplace(_pos, _sprite.horizontal_flip(), 
            _common_stuff.savefile.options.sound_vol);

        _sprite.set_tiles(bn::sprite_items::wormdingler.tiles_item(),
            _walkcycle.graphics_indexes().at(
                _walkcycle.current_index()) + 8);
    }
    if(_tongue)
    {

        _tongue->update();
        if(bn::keypad::a_released())
        {
            _tongue->retract();
        }

        if(_iframes)
        {
            _tongue->set_visible(_sprite.visible());
        }

        if(_tongue->done() || dead())
        {
            _tongue.reset();
            
            if(!dead())
            {
                _sprite.set_tiles(
                    bn::sprite_items::wormdingler.tiles_item(),
                    _walkcycle.graphics_indexes().at(
                        _walkcycle.current_index()));
            }
        }
    }
}

void wormdingler::demo_anim()
{
    _extendo = bn::create_sprite_animate_action_forever(
        _sprite, 2, bn::sprite_items::wormdingler.tiles_item(), 
        8, 13, 14, 15, 0, 16, 17, 18, 19, 18, 17, 16, 0, 15, 14, 13);
}

bool wormdingler::collides_with(const entity &e){
    return _hitbox.intersects(e.get_hitbox()) || 
        (_tongue && _tongue->get_hitbox().intersects(e.get_hitbox()));
}

tongue::tongue(bn::fixed_point pos, bool facing_left, 
    const bn::fixed & volume) : 
    entity(pos.x() + (facing_left ? -30 : 30), 
        pos.y() + 1, 
        13, 
        6, 
        bn::sprite_items::wormdingler_tongue.create_sprite(0, 0)),
    _retracting(false),
    _speed(1),
    _vol(volume)
{
    if(facing_left) _speed *= -1;
    _sprite.set_horizontal_flip(facing_left);
    _sprite.set_y(_pos.y());
    _sprite.set_x(_pos.x());
    _sprite.set_z_order(1);
    _sprite.put_below();

    _handle = bn::sound_items::hiss.play(_vol, 2, 0);
}

void tongue::update(){

    bn::fixed dx = 0.25;
    bn::fixed speed_cap = 5;
    if(_sprite.horizontal_flip()) dx *= -1;

    if(_retracting)
    {
        dx *= -2;
        speed_cap *= 2;
    }

    _speed += dx;

    if(_retracting && _handle && 
        ((_sprite.horizontal_flip() && _speed > 0) || 
        (!_sprite.horizontal_flip() && _speed < 0)))
    {
        _handle->stop();
        _handle.reset();
        bn::sound_items::hiss_rev.play(_vol, 2, 0);
    }

    if(_speed < -speed_cap) _speed = -speed_cap;
    else if(_speed > speed_cap) _speed = speed_cap;

    _sprite.set_x(_sprite.x() + _speed);

    bn::fixed end = _sprite.x() + 4;
    bn::fixed begin = _pos.x();

    bn::fixed width = bn::abs(end - begin);
    bn::fixed xcor = (end + begin) * bn::fixed(0.5);

    _hitbox.set_width(width);
    _hitbox.set_x(xcor);

    int width_rounded = (_hitbox.width() / 8).floor_integer();
    if(_sprite.horizontal_flip())
        width_rounded = (_hitbox.width() / 8).ceil_integer();

    if(width_rounded > (_tongue_sprites.size() - 2))
    {
        if(_tongue_sprites.size() > 12)
            retract();

        if(!_tongue_sprites.full())
        {
            bn::sprite_ptr new_sprite = 
                bn::sprite_items::wormdingler_tongue.create_sprite(0, 
                    _pos.y(), 1);
            new_sprite.set_z_order(1);
            new_sprite.put_below();
            _tongue_sprites.emplace_back(new_sprite); 
        }
        
    }
    else if(_retracting && width_rounded < 
        (_tongue_sprites.size() - 2))
    {
        _tongue_sprites.pop_back();
    }

    for(int z = 0; z < _tongue_sprites.size(); ++z)
    {
        _tongue_sprites.at(z).put_below();
        bn::fixed mult = -8;
        if(_sprite.horizontal_flip()) mult = 8;
        _tongue_sprites.at(z).set_x(_sprite.x() + (z + 1) * mult);
    }
}

void tongue::retract()
{
    _retracting = true;
}

bool tongue::done()
{
    return _retracting && (_sprite.horizontal_flip() ?
        _sprite.x() > _pos.x() :
        _sprite.x() < _pos.x());
}

void tongue::set_visible(const bool &visible)
{
    entity::set_visible(visible);
    for(bn::sprite_ptr &sprite : _tongue_sprites)
    {
        sprite.set_visible(visible);
    }
}

void wormdingler::update_firebreath()
{
    _breath_offset.set_x(32 - 2 * 
        abs(_walkcycle.current_index() - 
        (_walkcycle.graphics_indexes().size()) / 2) - 2);
    
    player::update_firebreath();
}

}