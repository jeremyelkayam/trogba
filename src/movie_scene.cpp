#include <bn_keypad.h>
#include <bn_log.h>
#include "movie_scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_strongbad.h"
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_knight.h"
#include "bn_sprite_items_firebreath.h"
#include "bn_sprite_items_cottageleft.h"

namespace trog{

movie_scene::movie_scene(session_info &sesh, bn::sprite_text_generator &text_generator) : 
    
    _timer(0),
    _cutscene_length(270),
    _text_generator(text_generator),
    _sesh(sesh)
{
    if(_sesh.get_level() != 101) {
        bn::sound_items::intermish.play();
    }

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.set_center_alignment();


    switch(_sesh.get_level()) { 
        case 5:

            //this is the absolute worst

            write_text("stompin' good");

            _start_pos = bn::fixed_point(140, 0);
            _finish_pos = bn::fixed_point(-80, 0);

            _sprites.insert("trogdor", bn::sprite_items::player.create_sprite(_start_pos));
            _sprites.at("trogdor").set_horizontal_flip(true);
            _anim_actions.insert("trogdor anim", 
                bn::create_sprite_animate_action_forever(
                _sprites.at("trogdor"), 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3));
            _move_actions.insert("trogdor movement", 
                bn::sprite_move_to_action(_sprites.at("trogdor"), _cutscene_length / 2, _finish_pos));

            _sprites.insert("blueknight", bn::sprite_items::knight.create_sprite(_start_pos + bn::fixed_point(50, -20)));
            _sprites.at("blueknight").set_horizontal_flip(false);
            _anim_actions.insert("blueknight anim", 
                bn::create_sprite_animate_action_forever(
                    _sprites.at("blueknight"), 10, bn::sprite_items::knight.tiles_item(), 0, 3, 2, 3));
            _move_actions.insert("blueknight movement", 
                bn::sprite_move_to_action(_sprites.at("blueknight"), _cutscene_length / 2, _finish_pos + bn::fixed_point(50, -20)));

            _sprites.insert("redknight", bn::sprite_items::knight.create_sprite(_start_pos + bn::fixed_point(80, 10)));
            _sprites.at("redknight").set_horizontal_flip(false);
            _anim_actions.insert("redknight anim", 
                bn::create_sprite_animate_action_forever(
                    _sprites.at("redknight"), 10, bn::sprite_items::knight.tiles_item(), 0, 3, 2, 3));
            _move_actions.insert("redknight movement", 
                bn::sprite_move_to_action(_sprites.at("redknight"), _cutscene_length / 2, _finish_pos + bn::fixed_point(80, 10)));

            break;
        case 9:
            write_text("fry 'em up dan");
            
            break;
        case 13:
            write_text("parade of trogdors");
            for(int z = 0; z < 9; z++){

                // _sprites.put("trogdorz",bn::sprite_items::player.create_sprite(-140 - 30 * z, 0)),
                // _anim_actions.insert(0, bn::create_sprite_animate_action_forever(
                        // _sprites.at(z), 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3)),
                // _move_actions.emplace_back(bn::sprite_move_to_action(_sprites.at(z), 300, 400 - 30*z, 0));                
            }

            break;
        case 17:
            write_text("dancin' time");
            break;
        case 21:
            write_text("flex it, troggie");
            // _sprites.emplace_back(bn::sprite_items::player.create_sprite(0, 0));
            // _anim_actions.
            // _anim_actions.emplace_back(bn::create_sprite_animate_action_forever(
                        // _sprites.at(0), 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3));
            break;
        case 43:
            write_text("2 cottages");
            // _sprites.emplace_back(bn::sprite_items::cottageleft.create_sprite(30, 0));
            // _sprites.emplace_back(bn::sprite_items::cottageleft.create_sprite(-30, 0));
            // _sprites.at(1).set_horizontal_flip(true);
            break;
        default:
            BN_ERROR("Movie scene instantiated on invalid level: ", _sesh.get_level());
    }
}

void movie_scene::write_text(const char* str){
    _text_generator.generate(0, -60, str, _text_sprites);

}

bn::optional<scene_type> movie_scene::update(){
    _timer++;
    bn::optional<scene_type> result;

    //yeah i know this sucks but i hate cutscenes i just want to end this
    switch(_sesh.get_level()){
        case 5:
            
            update_lv5();
        default: //no special logic for this level
            break;
    }

    for(auto &it : _move_actions){
        if(!it.second.done()){
            it.second.update();
        }
    }
    for(auto &it : _anim_actions){
        if(!it.second.done()){
            it.second.update();
        }
    }

    if(cutscene_over()) {
        return scene_type::PLAY;
    }

    // move_background(_dead_trogdor);

    return result;
}

bool movie_scene::cutscene_over(){
    return _timer >= 300;
}

void movie_scene::update_lv5() {
    if(_move_actions.at("trogdor movement").done()){
        //make him go the other way 
        _sprites.at("trogdor").set_horizontal_flip(false);
        _move_actions.insert_or_assign("trogdor movement", 
            bn::sprite_move_to_action(_sprites.at("trogdor"), _cutscene_length / 2, _start_pos));

        _sprites.at("blueknight").set_horizontal_flip(true);
        _move_actions.insert_or_assign("blueknight movement", 
            bn::sprite_move_to_action(_sprites.at("blueknight"), _cutscene_length / 2,
                _start_pos + bn::fixed_point(50, -20)));   

        _sprites.at("redknight").set_horizontal_flip(true);
        _move_actions.insert_or_assign("redknight movement", 
            bn::sprite_move_to_action(_sprites.at("redknight"), _cutscene_length / 2,
                _start_pos + bn::fixed_point(80, 10))); 

        _sprites.insert("firebreath", bn::sprite_items::firebreath.create_sprite(0,0));
        _anim_actions.insert("firebreath", 
                bn::create_sprite_animate_action_forever(
                    _sprites.at("firebreath"), 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3));
    }

    if(_sprites.contains("firebreath")) { 
        BN_LOG("yo wtf");
        _sprites.at("firebreath").set_x(_sprites.at("trogdor").x() + TROG_FIREBREATH_XOFFSET);
        _sprites.at("firebreath").set_x(_sprites.at("trogdor").y() + TROG_FIREBREATH_YOFFSET);        
    }
}



// will likely come back to this later

// _character_sprite(bn::sprite_items::strongbad.create_sprite(130, 0)),
//     _sb_anim(bn::create_sprite_animate_action_forever(
//                 _character_sprite, 5, bn::sprite_items::strongbad.tiles_item(),
//                 0, 1 )),
//     _sb_move(_character_sprite, 180, 0, 0),
// void movie_scene::update_ending_cutscene(){
//     if(!_sb_move.done()){
//         _sb_anim.update();
//         _sb_move.update();
//     }else{
//         if(_timer == 0){
//             _sb_anim = bn::create_sprite_animate_action_forever(
//                     _character_sprite, 10, bn::sprite_items::strongbad.tiles_item(),
//                     2, 3 );            
//             _sb_anim.update();
//         }
//         if(_timer == 30){
//             bn::sound_items::cutscene_congrats.play(TROG_DEFAULT_VOLUME);
//             _text_generator.generate(0, -60, "congratulations.", _text_sprites);
//         }
//         if(_timer == 130){
//             _text_generator.generate(0, -48, "you got", _text_sprites);
//         }
//         if(_timer == 190){
//             _text_generator.generate(0, -36, "good score", _text_sprites);
//         }
//         if(_timer > 30 && _timer < 210){
//             _sb_anim.update();
//         }        

//         if(_timer == 260){
//             bn::sound_items::cutscene_credits.play(TROG_DEFAULT_VOLUME);
//             _text_sprites.clear();
//             _text_generator.generate(0, -60, "keep playing!", _text_sprites);
//             _character_sprite.set_x(30);
//             _character_sprite.set_tiles(bn::sprite_items::strongbad.tiles_item(), 3);
//             _character_sprite.set_rotation_angle(315); //bn::sprite_items::player.create_sprite(0, 0);
//             _character_sprite = bn::sprite_items::player.create_sprite(-30, 0);
//             _character_sprite.set_rotation_angle(45); //bn::sprite_items::player.create_sprite(0, 0);

//         }
//         _timer++;
//     }
// }

}