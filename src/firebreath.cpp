#include <bn_math.h>
#include <bn_log.h>
#include "firebreath.h"
#include "bn_assert.h"
#include "sb_commentary.h"

namespace trog {

firebreath::firebreath(session_info &sesh, common_stuff &common_stuff) : 
        entity(bn::fixed(0), bn::fixed(0), bn::fixed(TROG_FIREBREATH_WIDTH), bn::fixed(TROG_FIREBREATH_HEIGHT), bn::sprite_items::firebreath.create_sprite(0, 0)),
        _burningflames(bn::create_sprite_animate_action_forever(
                    _sprite, 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3)),
        _sesh(sesh),
        _common_stuff(common_stuff) {
    _sprite.set_z_order(FRONT_ZORDER);
    disable();

}

void firebreath::update(){
    entity::update();
    _burningflames.update();
}

//due to the way the treasure hut works, an enabled firebreath can be 
// invisible (weird i know)
void firebreath::enable(){
    _sprite.set_visible(true);
    _enabled = true;
}

void firebreath::disable(){
    _sprite.set_visible(false);
    _enabled = false;
}

bool firebreath::enabled(){
    return _enabled;
}


// TODO add a burnable interface shared by these classes 
void firebreath::handle_cottage_collision(cottage &cottage){
    BN_ASSERT(enabled());
    bn::fixed_rect cottagebox = cottage.get_hitbox();
    if(_hitbox.intersects(cottagebox)){
        bool burninate_success = cottage.burninate();
        if(burninate_success){
            _sesh.score(TROG_COTTAGE_BREATHBURN_SCORE);
            _common_stuff.commentary.ignite_cottage(_sesh.get_dragon());
        }
    }
}

void firebreath::handle_peasant_collision(peasant &peasant){
    BN_ASSERT(enabled());
    bn::fixed_rect pbox = peasant.get_hitbox();
    if(_hitbox.intersects(pbox) && !peasant.dead() && !peasant.onfire()){
        peasant.burninate();        
        bn::sound_items::peasantscream.play(_common_stuff.savefile.options.sound_vol);
        _common_stuff.commentary.ignite_peasant(_sesh.get_dragon());
    }
}

}