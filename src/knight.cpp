#include <bn_log.h>
#include "knight.h"

namespace trog { 

knight::knight(int xcor, int ycor, short dir, bool facingRight) :
    entity(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
    _frameState(0),
	_moving(true),
	_half_src_w(TROG_KNIGHT_WIDTH / 2),
	_half_src_h(TROG_KNIGHT_HEIGHT / 2 + 4),
	_direction(dir),
	_home_x(xcor),
	_home_y(ycor),
	_offset_x(0),
	_offset_y(0),
    //todo: this is incomplete; fix it plz
    _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 20, bn::sprite_items::knight.tiles_item(), 0, 3, 2, 3))
{
    	_sprite.set_horizontal_flip(facingRight);
        // updateCollision();
}

void knight::update(){
    entity::update();
    _walkcycle.update();
    // not sure what this knight increment thing is but ok
    update_home(1);


    _frameState++;
	if (_frameState > 60) {
		_frameState -= 60;
	}

	if (_frameState <= 30) {
		_offset_x = _frameState * 34 / 30;
	} else {
		_offset_x = 68 - (_frameState * 34 / 30);
	}
	_offset_y = -_offset_x;
	if (!_sprite.horizontal_flip()) {
		_offset_x *= -1;
	}

	_pos.set_x(_home_x + _offset_x - _half_src_w);
	_pos.set_y(_home_y + _offset_y - _half_src_h);
	
    // this is probably wrong but basically we need to offset the hitbox based on sword position
    if (_sprite.horizontal_flip()) {
		_hitbox.set_position(_pos.x() + 2, _pos.y());
	} else {
		_hitbox.set_position(_pos.x() - 2, _pos.y());
	}

}

void knight::update_home(short knight_increment) {

    //in the real game, the knights have special boundaries. Maybe look into that
	if (_home_x < TROG_KNIGHT_LEFT_BOUND) {
		_direction = rand() % 6;
		_home_x = TROG_KNIGHT_LEFT_BOUND + 1;
	} else if (_home_x > TROG_KNIGHT_RIGHT_BOUND) {
		_direction = rand() % 6;
		_home_x = TROG_KNIGHT_RIGHT_BOUND - 1;
	}
	if (_home_y < TROG_KNIGHT_TOP_BOUND + 50) {
		_direction = rand() % 6;
		_home_y = TROG_KNIGHT_TOP_BOUND + 51;
	} else if (_home_y > TROG_KNIGHT_BOTTOM_BOUND) {
		_direction = rand() % 6;
		_home_y = TROG_KNIGHT_BOTTOM_BOUND - 1;
	}
	switch (_direction) {
		case 0:
			_home_x += knight_increment;
			break;
		case 1:
		case 2:
			_home_y += knight_increment;
			break;
		case 3:
		case 4:
			_home_x -= knight_increment;
			break;
		case 5:
			_home_y -= knight_increment;
			break;
		default:
			break;
	}
}

}