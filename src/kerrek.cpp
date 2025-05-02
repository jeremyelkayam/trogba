#include "kerrek.h"

namespace trog { 

kerrek::kerrek(bn::fixed xcor, bn::fixed ycor) : 
        entity(xcor, ycor, 10, 20, bn::sprite_items::kerrek.create_sprite(xcor, ycor)),
        _reveal_roast(bn::create_sprite_animate_action_once(
                    _sprite, 5, bn::sprite_items::kerrek_smoked.tiles_item(), 0, 1, 2)),
        _drop_timer(0),
        _squish_timer(0) {
    _sprite.set_z_order(MID_ZORDER);
    
}

void kerrek::update_anim(){
    if(_squish_timer){
        ++_squish_timer;
        if(_squish_timer == 10){
            _squish_timer = 0;
            _sprite.set_vertical_scale(1);
            _sprite.set_y(_sprite.y() - 5);            
        }
    }else if(_burninate){

        if(_burninate->done()){
            _sprite.set_palette(bn::sprite_items::kerrek_smoked.palette_item());
            if(!_reveal_roast.done()){
                _reveal_roast.update();
            }else{
                ++_drop_timer;
                if(_drop_timer == 1){
                    _sprite.set_y(_sprite.y() - 1);
                }else if(_drop_timer == 5){
                    _sprite.set_y(_sprite.y() - 1);
                }else if(_drop_timer == 9){
                    _sprite.set_y(_sprite.y() - 1);
                }else if(_drop_timer == 13){
                    _sprite.set_y(_sprite.y() + 1);
                }else if(_drop_timer == 17){
                    _sprite.set_y(_sprite.y() + 1);
                }else if(_drop_timer == 21){
                    _sprite.set_y(_sprite.y() + 3);
                }else if(_drop_timer == 25){
                    _sprite.set_y(_sprite.y() + 5);
                    _sprite.set_rotation_angle(10);
                }else if(_drop_timer == 29){
                    _sprite.set_y(_sprite.y() + 10);
                    _sprite.set_rotation_angle(350);
                }else if(_drop_timer == 33){
                    _sprite.set_y(_sprite.y() + 10);
                    _sprite.set_rotation_angle(10);
                }else if(_drop_timer == 37){
                    _sprite.set_rotation_angle(350);
                }else if(_drop_timer == 41){
                    _sprite.set_rotation_angle(0);
                }else if(_drop_timer == 45){
                    _sprite.set_y(_sprite.y() + 3);
                }
            }
        }else{
            _burninate->update();
        }
    }
}

void kerrek::update(){

}

void kerrek::burninate(){
    _burninate = bn::create_sprite_animate_action_once(
                    _sprite, 5, bn::sprite_items::kerrek.tiles_item(), 0, 1, 2, 3, 4, 5, 6);
    _sprite.set_vertical_scale(0.9);
    _sprite.set_y(_sprite.y() + 5);
    _squish_timer = 1;
}

}
