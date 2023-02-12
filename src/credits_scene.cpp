#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_version.h>
#include "credits_scene.h"
#include "bn_sprite_items_titlegraphic.h"
#include "constants.h"

namespace trog {

credits_scene::credits_scene(bn::sprite_text_generator& text_generator) : 
    _time(0),
    _text_generator(text_generator) {


}

void credits_scene::setup_credits(){
    int ycor = 100;

    for(int z = 0; z < 4 ; ++z){
        _title_sprites.push_back(bn::sprite_items::titlegraphic.create_sprite(TROG_TITLE_TEXT_X + 64*z, ycor, z));
    }

    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(WHITE_PALETTE);

    bn::string<64> butano_str;
    bn::ostringstream butano_string_stream(butano_str);
    butano_string_stream << "Created using Butano " << bn::version::major() << "." << bn::version::minor() << "." << bn::version::patch();

    bn::string<64> credits[] = {
        "The GBA Game",
        "",
        "programmed by Jeremy Elkayam",
        "",
        butano_str,
        "by Gustavo Valiente",
        "github.com/GValiente/butano",
        "",
        "Thanks to",
        "Mips96 + TrogdorReburninated",
        "for inspiration/code reference",
    };

    ycor += 25;

    for(bn::string<64> str : credits){ 
        _text_generator.generate(0, ycor,str, _text_sprites);

        ycor += 16;
    }

    ycor += 30;
    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(0, ycor,"THANKS OG TROGDOR TEAM!!", _text_sprites);   
    ycor += 30;

    _text_generator.set_palette_item(WHITE_PALETTE);    
    bn::string<64> original_game_credits[] = {
        "",
        "Designed by", 
        "Mike and Matt Chapman",
        "",
        "Original Flash ver.", 
        "programmed by Jonathan Howe",
        "",
        "HTML5 ver. programmed by",
        "Aeon Softworks",
        "",
        "Board game by ",
        "The Brothers Chaps",
        "and James Ernest"
    };

    for(bn::string<64> str : original_game_credits){ 
        _text_generator.generate(0, ycor,str, _text_sprites);
        ycor += 16;
    }
}



bn::optional<scene_type> credits_scene::update(){
    //have to do this because we construct the new scene before discarding the old one
    if(_time == 0) setup_credits();

    bn::optional<scene_type> result;

    bn::fixed speed = 0.5;
    ++_time;

    if(bn::keypad::a_held()){
        speed*=4;
    }

    for(bn::sprite_ptr &sprite : _title_sprites ){
        sprite.set_y(sprite.y() - speed);
    }

    for(bn::sprite_ptr &sprite : _text_sprites ){
        sprite.set_y(sprite.y() - speed);
    }

    if(_text_sprites.back().y() < -90){
        result = scene_type::TITLE;
    }

    return result;
}

}