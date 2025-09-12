#include <bn_music_items.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_jingle.h>

#include "char_unlock_scene.h"
#include "sb_commentary.h"

#include "bn_sprite_items_nose_smoke.h"
#include "bn_sprite_items_cottagefire.h"
#include "bn_sprite_items_a_button_prompt.h"
#include "serif_fonts.h"
#include "level_data.h"
#include "dragon_data.h"
#include "create_dragon.h"
#include "common_functions.h"


namespace trog {

char_unlock_scene::char_unlock_scene(session_info &sesh, 
    common_stuff &common_stuff, const scene_type &next_scene, const dragon &dtype) : 
        _happy_dragon(bn::regular_bg_items::trogsmile.create_bg(8, 61)),
        _a_button(bn::sprite_items::a_button_prompt.create_sprite(113,71)),
        _a_button_anim(bn::create_sprite_animate_action_forever(
            _a_button, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1)),
        _dtype(dtype),
        _sesh(sesh),
        _common_stuff(common_stuff),
        _next_scene(next_scene) {
    
    _a_button.set_visible(false);

    bn::sprite_text_generator serif_white(serif_font_white);
    bn::sprite_text_generator serif_yellow(serif_font_yellow);
    bn::sprite_text_generator serif_red(serif_font_red);
    
    serif_white.set_center_alignment();
    serif_yellow.set_center_alignment();
    serif_red.set_center_alignment();

    // common_stuff.commentary.level_win_scene(_sesh.get_dragon());


    if(sesh.get_level() != 0 && sesh.get_level() != 100){
        save();
    }

    const dragon_data &dd = dragons[(int)dtype]; 

    _happy_dragon.set_item(dd.smile);
    
    dd.jingle.play(common_stuff.savefile.options.sound_vol, false);

    
    bn::fixed xcor = 70;
    bn::fixed ycor = -20;

    bn::fixed yoffset = ycor + 35;


    bn::string<16> name(dd.name);

    if(dtype == dragon::SUCKS)
    {
        serif_white.generate(xcor, yoffset,
            "THE", _text_sprites);
            yoffset += 13;
        name = name + "!";
    }


    //todo: this sucks and should be refactored... 
    for(const bn::string<64> &line : split_into_lines(
        name.c_str(), 90, serif_font_character_widths))
    {
        serif_yellow.generate(xcor, yoffset,
            line, _text_sprites);
        yoffset += 13;
    }
    yoffset += 3;

    for(const bn::string<64> &line : split_into_lines(
        dd.join_phrase, 90, serif_font_character_widths))
    {
        serif_white.generate(xcor, yoffset,
            line, _text_sprites);
        yoffset += 13;
    }

    bn::sprite_palette_ptr flashing_palette = 
        bn::sprite_palette_ptr::create_new(serif_red.palette_item());

    _hueshift.emplace(flashing_palette, 30, 1);
    
    serif_red.generate(0, -74, "NEW CHARACTER UNLOCKED", _text_sprites);

    
    player_entity.reset(create_player(dtype,xcor,ycor,sesh, false, 
        common_stuff, 0));

    player_entity->set_horizontal_flip(true);
    player_entity->demo_anim();
}

void char_unlock_scene::save(){

    _common_stuff.save();

    _common_stuff.set_autosave_text_visible(true);
}

bn::optional<scene_type> char_unlock_scene::update(){
    bn::optional<scene_type> result;

    if(_a_button.visible())
    {
        _a_button_anim.update();
    }

    if(!bn::jingle::playing())
    {
        _a_button.set_visible(true);
        //you can skip this scene by pressing A.
        if(bn::keypad::a_pressed()){

            result = _next_scene;

            _common_stuff.set_autosave_text_visible(false);
            if(_dtype == dragon::TROGDOR)
            {
                _common_stuff.dialog.reset(new trog::dialog("This version of Trogdor has many different playable dragons, each with their own unique traits and abilities. Try to get them all!"));
            }
        }

    }

    player_entity->update_anim();

    _hueshift->update();


    return result;
}

}