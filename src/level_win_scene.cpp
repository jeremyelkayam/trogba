#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_sram.h>

#include "level_win_scene.h"
#include "sb_commentary.h"

#include "bn_regular_bg_items_trogsmile.h"
#include "bn_regular_bg_items_sucksmile.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_nose_smoke.h"
#include "bn_sprite_items_cottagefire.h"
#include "bn_sprite_items_a_button_prompt.h"


namespace trog {

level_win_scene::level_win_scene(session_info &sesh, common_stuff &common_stuff) : 
        _happy_dragon(bn::regular_bg_items::trogsmile.create_bg(8, 61)),
        _nose_smoke(bn::sprite_items::nose_smoke.create_sprite(40, -35)),
        _flames(bn::sprite_items::cottagefire.create_sprite(115, -78)),
        _a_button(bn::sprite_items::a_button_prompt.create_sprite(113,71)),
        _smoke_anim(bn::create_sprite_animate_action_once(
                    _nose_smoke, 3, bn::sprite_items::nose_smoke.tiles_item(),
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30)),
        _burningflames(bn::create_sprite_animate_action_once(
                    _flames, 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3)),
        _a_button_anim(bn::create_sprite_animate_action_forever(
            _a_button, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1)),
        _sesh(sesh),
        _common_stuff(common_stuff), 
        _timer(0) {

    _flames.set_visible(false);
    _flames.put_above();
    _flames.set_scale(0.7);
    _a_button.set_visible(false);

    bn::sound_items::burninate.play(common_stuff.savefile.options.sound_vol);

    common_stuff.text_generator.set_center_alignment();
    common_stuff.text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    common_stuff.text_generator.generate(73, 5, "nice work!", _nicework_text_sprites);

    bn::string<7> line2 = "BEATEN!";
    //3% chance that the game misspells it lol
    if(common_stuff.rand.get_int(33) == 0){
        line2 = "BEATED!";
    }
    

    common_stuff.text_generator.generate(70, 25, "LEVEL", _levelbeated_text_sprites);
    common_stuff.text_generator.generate(70, 40, line2, _levelbeated_text_sprites);
    common_stuff.commentary.level_win_scene();

    if(sesh.get_level() != 0 && sesh.get_level() != 100){
        save();
    }
    switch(_sesh.get_dragon()){
        case dragon::TROGDOR:
            _happy_dragon.set_item(bn::regular_bg_items::trogsmile);
        break;
        case dragon::SUCKS:
            _happy_dragon.set_item(bn::regular_bg_items::sucksmile);
        break;
        default:
            BN_ERROR("Invalid dragon type found in session info");
        break;
    }

}

void level_win_scene::save(){
    _common_stuff.savefile.session = _sesh.export_save();

    // the level doesn't technically advance until later in the animation
    //  so we should increment it here if it hasn't happened yet
    if(_timer < 40) _common_stuff.savefile.session.level++;

    _common_stuff.save();


    //todo: maybe improve the look of this ... 

    _common_stuff.set_autosave_text_visible(true);
}

bn::optional<scene_type> level_win_scene::update(){
    ++_timer;
    bn::optional<scene_type> result;

    _a_button_anim.update();
    if(_sesh.get_level() == 0 && !_text_box){
        _sesh.reset_score();
        _text_box = text_box(_common_stuff.small_generator, "There are 100 levels in the game. Try to beat them all while aiming for a high score!");
    }

    if(30 < _timer && !_burningflames.done()){
        _flames.set_visible(true);
        _burningflames.update();
    }
    if(_timer == 40) { 
        _sesh.advance_level();
    }
    if(_burningflames.done()){
        _flames.set_visible(false);
    }

    if(!_smoke_anim.done()){
        _smoke_anim.update();
    }else{
        _nose_smoke.set_visible(false);
        _a_button.set_visible(true);
    }

    //you can skip this scene by pressing A.
    if(bn::keypad::a_pressed()){
        //if you skip the scene before the level up animation 
        //we will just advance it for you
        if(_timer < 40) _sesh.advance_level();

        if(_sesh.current_level_has_cutscene()) {
            result = scene_type::MOVIE;
        }else{
            result = scene_type::PLAY;
        }

        _common_stuff.set_autosave_text_visible(false);
    }

    return result;
}

}