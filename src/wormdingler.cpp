#include "wormdingler.h"
#include "bn_sprite_items_wormdingler.h"
#include "bn_sprite_items_wormdingler_tongue.h"
#include <bn_math.h>
#include <bn_keypad.h>

namespace trog { 


wormdingler::wormdingler(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, TROG_WORM_WIDTH, TROG_WORM_HEIGHT, TROG_WORM_SPEED, 
        bn::fixed_point(31, 0), sesh, 
        iframes, bn::sprite_items::wormdingler, 5, common_stuff, initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 3, 
            bn::sprite_items::wormdingler.tiles_item(), 1, 2, 3, 4, 3, 2, 1, 0)) {

}


void wormdingler::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action || _update_anim_when_not_moving){
        _walkcycle.update();
    }
    update_firebreath();
}

void wormdingler::update(){

    if(!_tongue)
    {
        player::update();
    }

    if(!_tongue && !dead() && any_dpad_input()){
        _walkcycle.update();
    }
    
    if(burninating()){
        _breath_offset.set_x(32 - 2 * 
            abs(_walkcycle.current_index() - 
                (_walkcycle.graphics_indexes().size()) / 2) - 2);

        BN_LOG(_walkcycle.current_index());
    }

    if(!_iframes && !_tongue && !dead() && bn::keypad::a_pressed())
    {
        _tongue.emplace(_pos, _sprite.horizontal_flip());

        _sprite.set_tiles(bn::sprite_items::wormdingler.tiles_item(),
            _walkcycle.graphics_indexes().at(_walkcycle.current_index()) + 8);
    }
    if(_tongue)
    {
        _tongue->update();
        if(bn::keypad::a_released())
        {
            _tongue->retract();
        }
        if(_tongue->done() || dead())
        {
            _tongue.reset();
            
            if(!dead())
            {
                _sprite.set_tiles(bn::sprite_items::wormdingler.tiles_item(),
                    _walkcycle.graphics_indexes().at(_walkcycle.current_index()));
            }
        }
    }
}

bool wormdingler::collides_with(const entity &e){
    return _hitbox.intersects(e.get_hitbox()) || 
        (_tongue && _tongue->get_hitbox().intersects(e.get_hitbox()));
}

tongue::tongue(bn::fixed_point pos, bool facing_left) : 
    entity(pos.x() + (facing_left ? -30 : 30), 
        pos.y() + 1, 
        13, 
        3, 
        bn::sprite_items::wormdingler_tongue.create_sprite(0, 0)),
    _retracting(false),
    _speed(1)
{
    if(facing_left) _speed *= -1;
    _sprite.set_horizontal_flip(facing_left);
    _sprite.set_y(_pos.y());
    _sprite.set_x(_pos.x());
}

void tongue::update(){

    bn::fixed dx = 0.25;
    if(_sprite.horizontal_flip()) dx *= -1;

    if(_retracting) dx *= -1;

    _speed += dx;

    // if(!_retracting)
    // {
        //bound our speed;
        if(_speed < -5) _speed = -5;
        else if(_speed > 5) _speed = 5;
    // }

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
            _tongue_sprites.emplace_back(
                bn::sprite_items::wormdingler_tongue.create_sprite(0, _pos.y(), 1));
        
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

}