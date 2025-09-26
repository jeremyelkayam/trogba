#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_version.h>
#include <bn_sound_items.h>
#include "credits_scene.h"
#include "bn_regular_bg_items_titlegraphic.h"
#include "constants.h"
#include "small_fonts.h"
#include "serif_fonts.h"

namespace trog {

credits_scene::credits_scene(common_stuff &common_stuff, const scene_type &last_scene) : 
    _last_scene(last_scene),
    _small_white_gen(small_font_white),
    _common_stuff(common_stuff) {
    _small_white_gen.set_z_order(1);

    setup_credits();
}

void credits_scene::setup_credits(){
    int ycor = 100;

    //todo: redraw this graphic. I already remade the logo in high res 
    // so i can just downscale it
    _titlegraphic = bn::regular_bg_items::titlegraphic.create_bg_optional(0, ycor);


    bn::string<64> butano_str;
    bn::ostringstream butano_string_stream(butano_str);
    butano_string_stream << "Created using Butano " << bn::version::major() << "." << bn::version::minor() << "." << bn::version::patch();


    bn::string<64> credits[] = {
        "The GBA Game",
        bn::string<64>("version ") + TROG_SEMANTIC_VERSION_NUMBER,
        "created by Arkbuilder Interactive",
        "",
        "Programming",
        "by Jeremy Elkayam",
        "",
        "New graphics & sound",
        "by Jeremy Elkayam",
        "",
        "New design elements",
        "by Jeremy and Michael Elkayam",
        "",
        "New jingles",
        "by Dot Maetrix",
        "",
        "",
        "Dust effects by WeenterMakesGames",
        "weentermakesgames.itch.io",
        "",
        "",
        butano_str,
        "by Gustavo Valiente",
        "github.com/GValiente/butano",
        "",
        "",
        "Thanks to:",
        "Mips96 and Trogdor Reburninated",
        "for inspiration/code reference",
        "github.com/Mips96/Trogdor-Reburninated",
        "",
        "Mira & Michael Elkayam",
        "for design input and bug testing"
    };

    ycor += 25;

    for(bn::string<64> str : credits){ 
        credit_line line;
        line.str = str;
        line.ycor = ycor;
        _credits.push_back(line);

        ycor += 14;
    }

    ycor += 30;

    credit_line og_team_line;
    bn::sprite_text_generator serif_gen(serif_font_red); 
    serif_gen.set_center_alignment();
    serif_gen.generate(0, ycor,"THANKS OG TROGDOR TEAM!!", og_team_line.sprites);   
    og_team_line.ycor = ycor;
    _credits.emplace_back(og_team_line);
    ycor += 30;
 
    bn::string<64> original_game_credits[] = {
        "",
        "Designed by", 
        "Mike and Matt Chapman",
        "videlectrix.itch.io",
        "",
        "Original Flash ver. programmed by", 
        "Jonathan Howe",
        "",
        "HTML5 ver. programmed by",
        "Aeon Softworks",
        "",
        "Board game by",
        "The Brothers Chaps and James Ernest"
    };

    for(bn::string<64> str : original_game_credits){ 
        credit_line line;
        line.str = str;
        line.ycor = ycor;
        _credits.push_back(line);

        ycor += 14;
    }

    _small_white_gen.set_center_alignment();

    bn::sound_items::cutscene_credits.play(_common_stuff.savefile.options.music_vol);
}



bn::optional<scene_type> credits_scene::update(){
    //have to do this because we construct the new scene before discarding the old one
    //and if we do that, then we risk taking up too much ram

    bn::optional<scene_type> result;

    bn::fixed speed = 0.5;

    if(bn::keypad::a_held()){
        speed*=4;
    }

     
    if(_titlegraphic){
        _titlegraphic->set_y(_titlegraphic->y() - speed);
        if(_titlegraphic->y() < -100){
            _titlegraphic.reset();
        }
    }


    for(credit_line &line : _credits ){
        line.ycor = line.ycor - speed;

        if(line.ycor < -100){
            if(!line.sprites.empty())
                line.sprites.clear();
        }else if(line.ycor < 90 && line.sprites.empty()
                && line.str != bn::string<64>("")){
            BN_LOG("drawing line: ", line.str);
            BN_LOG("at ycor: ", line.ycor);
            _small_white_gen.generate(0, line.ycor,line.str, line.sprites);    
        }
        //todo refactor this into a method of the class
        for(bn::sprite_ptr &sprite : line.sprites) {
            sprite.set_y(line.ycor);
        }

    }

    if(_credits.back().ycor < -90){
        
        _credits.clear();
        _titlegraphic.reset();


        if(_last_scene == scene_type::EXTRAS)
        {
            result = scene_type::EXTRAS;
        }
        else
        {
            result = scene_type::TITLE;
        }
    }

    return result;
}

}