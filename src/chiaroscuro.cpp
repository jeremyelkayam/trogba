#include "chiaroscuro.h"
#include "bn_sprite_items_chiaroscuro.h"

#define NORM_WLKCL bn::create_sprite_animate_action_forever(_sprite, 8, bn::sprite_items::chiaroscuro.tiles_item(), 0, 1, 2, 3, 4, 1)
#define BURN_WLKCL bn::create_sprite_animate_action_forever(_sprite, 8, bn::sprite_items::chiaroscuro.tiles_item(), 5, 6, 7, 8, 9, 6)


namespace trog { 

chiaroscuro::chiaroscuro(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, TROG_TROGDOR_WIDTH, TROG_TROGDOR_HEIGHT, TROG_TROGDOR_SPEED, 
        bn::fixed_point(17, -12), sesh, 
        iframes, bn::sprite_items::chiaroscuro, 15, common_stuff, initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 8, 
            bn::sprite_items::chiaroscuro.tiles_item(), 0, 1, 2, 3, 4, 1)) {
    //TODO: Mess with bg priority on the player sprite to make it appear above the background.
}


void chiaroscuro::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action || _update_anim_when_not_moving){
        _walkcycle.update();
    }
    update_firebreath();

    if(_roar){
        _roar->update();
    }
}


//todo: this would be a lot easier to integrate into the player class
// if the walkcycle were part of it
// we can refactor walkcycle into the player class... 
void chiaroscuro::change_walkcycle(const bn::isprite_animate_action &walkcycle){
    int dex = _walkcycle.current_index();
    _walkcycle = walkcycle;
    do{
        //we need to update the walkcycle at least once 
        //otherwise if you don't move, he will stay on a non-burninating frame
        _walkcycle.update();
    }while(_walkcycle.current_index() != dex);
}

void chiaroscuro::update(){
    if(!dead()){
        _walkcycle.update();
    }
    player::update();
}

void chiaroscuro::start_burninating(){
    player::start_burninating();
    change_walkcycle(BURN_WLKCL);
}

void chiaroscuro::stop_burninating(){
    player::stop_burninating();
    change_walkcycle(NORM_WLKCL);
}

void chiaroscuro::demo_anim()
{
    _roar = 
        bn::create_sprite_animate_action_forever(_sprite, 4, 
            bn::sprite_items::chiaroscuro.tiles_item(), 10, 11, 12, 13, 14, 11);
}

void chiaroscuro::update_win_anim(){
    player::update_win_anim();
    change_walkcycle(NORM_WLKCL);
}

void chiaroscuro::update_firebreath()
{

    //8: -12
    //7, 9: -11
    // other: -10

    unsigned int index = _walkcycle.current_index();
    if(index == 4)
    {
        _breath_offset.set_y(-12);
    }
    else if(index == 3 || index == 5)
    {
        _breath_offset.set_y(-11);
    }
    else
    {
        _breath_offset.set_y(-10);
    }

    BN_LOG("graphics index ", index);

    BN_LOG("offset ", _breath_offset.y());

    player::update_firebreath();
}

}