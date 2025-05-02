#include <bn_keypad.h>
#include <bn_sram.h>
#include "movie_scene.h"
#include "sb_commentary.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_strongbad.h"
#include "bn_sprite_items_knight.h"
#include "bn_sprite_items_firebreath.h"
#include "serif_fonts.h"
#include "create_dragon.h"
#include "dragon_data.h"


//this class is terrible! But it's the best I've got 
namespace trog{

movie_scene::movie_scene(session_info &sesh, common_stuff &common_stuff, const scene_type &last_scene) : 
    _serif_white(serif_font_white),
    _last_scene(last_scene),    
    _timer(0),
    _cutscene_length(270),
    _common_stuff(common_stuff),
    _sesh(sesh),
    _dummy_cottage(-240, -240, direction::DOWN, false, false, common_stuff)
{
    _serif_white.set_center_alignment();
    if(_sesh.get_level() != 101) {
        bn::sound_items::intermish.play(_common_stuff.savefile.options.music_vol);
        write_text(_common_stuff.cutscene_name_for_level(_sesh.get_level()));
    }

    if(_sesh.get_level() == 5){
        player* mydragon = create_player(_sesh.get_dragon(),
            140,0,_sesh,false,_common_stuff);
        mydragon->set_horizontal_flip(true);
        mydragon->move_to_and_back(_cutscene_length, -50, 0);
        _cutscene_objects.emplace_back(mydragon);
                
        knight* redknight = new knight(190,-5,false,_common_stuff.rand);
        redknight->move_to_and_back(_cutscene_length, 0, -5);
        _cutscene_objects.emplace_back(redknight);

        knight* blueknight = new knight(220, 5,false,_common_stuff.rand);
        blueknight->move_to_and_back(_cutscene_length, 30, 5);
        _cutscene_objects.emplace_back(blueknight);

        peasant* p = new peasant(-60, 0, 0, 0, _dummy_cottage);
        p->update();
        _cutscene_objects.emplace_back(p);

    }else if(_sesh.get_level() == 9){
        //todo: make the iframes param optional on trogdor.
        // and maybe more optional params in the future
        player* mytrogdor = create_player(_sesh.get_dragon(),
            20,0,_sesh,false,_common_stuff);
        mytrogdor->set_horizontal_flip(true);
        _cutscene_objects.emplace_back(mytrogdor);

    }else if(_sesh.get_level() == 13){
        //create a parade of 9 of the available dragons
        bn::vector<dragon, NUM_DRAGONS> unlocked = 
            common_stuff.available_dragons();
        if(unlocked.size() > 1)
        {
            write_text("parade of dragons");
        }
        

        bn::fixed floor_y = dragons[0].height / 2;
        for(int z = 0; z < 9; z++){
            dragon dtype = unlocked.at(z % unlocked.size());
            bn::fixed ycor = floor_y - (dragons[(int)dtype].height / 2);

            _cutscene_objects.emplace_back(
                create_player(dtype, 140 + 50 * z, 
                    ycor, sesh, false,_common_stuff));
            _cutscene_objects.at(z)->set_horizontal_flip(true);                
            _cutscene_objects.at(z)->move_to(_cutscene_length, -500 + 50*z, ycor);                
        }
    }else if(_sesh.get_level() == 17){
        _common_stuff.update_achievement("twees", 0);

        for(int z=0; z < 10; z++){
            int xcor = 100 - (z/2)*13;
            int ycor = -12 + (z/2)*2;
            peasant *p = new peasant(z % 2 == 0 ? xcor : -xcor, ycor, 0, 0, _dummy_cottage);
            p->set_sprite_ablaze();
            p->update_anim_action_when_not_moving(true);
            _cutscene_objects.emplace_back(p);
        }
        for(int z=0; z < 2; z++){
            int xcor = 30;
            int ycor = -2;
            knight *k = new knight(z == 0 ? xcor : -xcor, ycor, z == 0,_common_stuff.rand);
            k->update_anim_action_when_not_moving(true);
            k->animate_faster();
            k->animate_faster();
            _cutscene_objects.emplace_back(k);
        }
        player *p = create_player(_sesh.get_dragon(), 0, 0, sesh, false,_common_stuff);
        p->flip_every(15);
        _cutscene_objects.emplace_back(p);

    }else if(_sesh.get_level() == 21){

        switch(sesh.get_dragon())
        {
            case dragon::SUCKS:
                write_text("stomp it out, sucka");
                break;
            case dragon::CHIAROSCURO:
                write_text("chiaro scream!!");
                break;
            case dragon::WORMDINGLER:
                write_text("what is wrong with that thing");
                break;
            default:
                break;
        }

        player* mytrogdor = create_player(_sesh.get_dragon(), 0,0,_sesh,false,_common_stuff);
        mytrogdor->demo_anim();
        _cutscene_objects.emplace_back(mytrogdor);
    }else if(_sesh.get_level() == 25){
        player* mytrogdor = create_player(_sesh.get_dragon(), 
            80,0,_sesh,false,_common_stuff);
        mytrogdor->set_horizontal_flip(true);
        _cutscene_objects.emplace_back(mytrogdor);

        for(int z = 0; z < 7; ++z){
            peasant* p = new peasant(45 - 20*z, 0, 0, 0, _dummy_cottage);
            _cutscene_objects.emplace_back(p);
        }

    }else if(_sesh.get_level() == 31){
        for(int z = 0; z < 2 ; ++z){
            int sign = z == 0 ? 1 : -1;
            peasant *p = new peasant(130 * sign, 0, 0, 0, _dummy_cottage);
            p->move_to(_cutscene_length / 3, 20 * sign, 0);
            p->update_anim_action_when_not_moving(true);
            _cutscene_objects.emplace_back(p);
        }
    }else if(_sesh.get_level() == 35){
        
        bn::vector<dragon, NUM_DRAGONS> unlocked = 
            common_stuff.available_dragons();


        player *first_dragon = create_player(_sesh.get_dragon(),
        -140, 0, sesh, false,_common_stuff);
        first_dragon->move_by(1, 0);
        _cutscene_objects.emplace_back(first_dragon);

        dragon dtype = _sesh.get_dragon();

        if(unlocked.size() > 1)
        {
            //DELETE the current dragon from our vector
            for(auto it = unlocked.begin(); it != unlocked.end(); ++it)
            {
                if(*it == _sesh.get_dragon())
                {
                    unlocked.erase(it);
                }
            }
            dtype = unlocked.at(common_stuff.rand.get_int(unlocked.size()));

            bn::string<64> title = "go ";
            title += dragons[(int)_sesh.get_dragon()].name;
            title += "!";
            write_text(common_stuff::to_lower(title));
        }
        player *second_dragon = create_player(dtype,
            -140, 30, sesh, false,_common_stuff);
        second_dragon->move_by(1.5, 0);
        _cutscene_objects.emplace_back(second_dragon);
        
        for(int z = 0; z < 7 ; ++z){
            peasant *p = new peasant(-60 + 20*z, -25 + (13*z) % 5, 0, 0, _dummy_cottage);
            p->jump(10 + z*2, 5, true);
            _cutscene_objects.emplace_back(p);
        }
    }else if(_sesh.get_level() == 39){
        for(int z = 0; z < 2 ; ++z){
            peasant *p = new peasant(40 + 20*z, 0, 0, 0, _dummy_cottage);  
            p->move_to(_cutscene_length / 2, -60 + 20*z, 0);      
            _cutscene_objects.emplace_back(p);
        }
        heart *h = new heart(50, -10);
        h->move_to(_cutscene_length / 2, -50, -10);          
        _cutscene_objects.emplace_back(h); 


    }else if(_sesh.get_level() == 43){
        _cutscene_objects.emplace_back(new cottage(30, 0, direction::LEFT, false, false, _common_stuff));
        _cutscene_objects.emplace_back(new cottage(-30, 0, direction::RIGHT, false, false, _common_stuff));

    }else if(_sesh.get_level() == 47){
        knight *k = new knight(-80, 0, false, _common_stuff.rand);
        k->update_anim_action_when_not_moving(false);
        _cutscene_objects.emplace_back(k);
        archer *arch = new archer(-5, true, _common_stuff);
        arch->set_x(130);
        arch->move_to(20, 110, -5);
        arch->set_bow_drawn(true);
        _cutscene_objects.emplace_back(arch);
    }else if(_sesh.get_level() == 51){
        _cutscene_objects.emplace_back(create_player(
            _sesh.get_dragon(),-25,10,_sesh,false,_common_stuff));
        _cutscene_objects.emplace_back(new kerrek(25, 0));   

        _cutscene_length *= 1.75; 
    }else if(_sesh.get_level() == 101){
        // CREDITS

        //once you beat the game, you can no longer restore your save data
        common_stuff.savefile.session.exists = false;
        common_stuff.save();

        strongbad *s = new strongbad(135, 0);
        s->move_to(120, 0, 0);
        _cutscene_objects.emplace_back(s);
        _common_stuff.commentary.im_in_this_game();

        _cutscene_length = 1350;
    }else BN_ERROR("Provided level does not have an associated cutscene: ",
        _sesh.get_level());
}

void movie_scene::write_text(bn::string<32> str){
    _text_sprites.clear();
    
    _serif_white.generate(0, -60, str, _text_sprites);
}

bn::optional<scene_type> movie_scene::update(){
    bn::optional<scene_type> result;

    for(bn::unique_ptr<entity> &e : _cutscene_objects){
        e->update_anim();
    }

    if(_sesh.get_level() == 5 ){
        if(_timer == _cutscene_length / 2){
            //stompin good
            ((player *) _cutscene_objects.at(0).get())->enable_breath();
            ((peasant *) _cutscene_objects.at(3).get())->squish();        
        }if(_cutscene_objects.at(0)->sprite_x() > 120){
            ((player *) _cutscene_objects.at(0).get())->disable_breath();
        }
    }
    if(_sesh.get_level() == 9){
        //fryemup dan
        for(int z = 0; z < 4 ; ++z){
            if(_timer == 30*z){
                peasant* p = new peasant(-130, 0, 0, 0, _dummy_cottage);
                p->move_to(_cutscene_length / 4, -10, 0);
                _cutscene_objects.emplace_back(p);
            }
            if(_timer == _cutscene_length / 4 + 5 + 30*z){
                ((player *) _cutscene_objects.at(0).get())->enable_breath();
            }
            if(_timer == _cutscene_length / 4 + 10 + 30*z){
                ((player *) _cutscene_objects.at(0).get())->disable_breath();
                peasant *p = (peasant *) _cutscene_objects.at(1 + z).get();
                p->set_sprite_ablaze();
                p->move_to(_cutscene_length / 4, -10, 100);
            }

        }
    }
    if(_sesh.get_level() == 25){
        // peasant dominoes
        if(_timer == 60){
            ((player *) _cutscene_objects.at(0).get())->enable_breath();
        }
        if(_timer == 70){
            ((player *) _cutscene_objects.at(0).get())->disable_breath();
        }
        for(int z = 0; z < 8; ++z){
            if(_timer == 70 + 5*z){
                if(z != 0) ((peasant *) _cutscene_objects.at(z).get())->squish();
                if(z != 7) ((peasant *) _cutscene_objects.at(z+1).get())->set_sprite_ablaze();                
            }
        }
        if(_timer == 140){
            ((player *) _cutscene_objects.at(0).get())->jump(10, 5, true);
        }
    }
    if(_sesh.get_level() == 31){
        // trogdor incognito
        if(_timer == _cutscene_length / 3 + 30){
            //peasant breathes fire 
            firebreath *f = new firebreath(_sesh, _common_stuff);
            f->set_x(-5);
            f->set_y(-10);
            f->enable();
            f->update();
            _cutscene_objects.emplace_back(f);
        }
        if(_timer == _cutscene_length / 3 + 40){
            ((firebreath *) _cutscene_objects.at(2).get())->disable();
            _cutscene_objects.at(0)->move_to(_cutscene_length / 4, 130, 0);
        }
        if(_timer == _cutscene_length / 2 + 60){
            _cutscene_objects.at(1)->jump(10, 3, true);

        }
    }

    if(_sesh.get_level() == 35){
        // trogdors race. looks ok, could improve
        if(_timer % 40 == 0){
            int _faster_player = (_timer / 40) % 2;
            _cutscene_objects.at(_faster_player)->move_by(1.5,0);
            _cutscene_objects.at(!_faster_player)->move_by(1,0);
        }
    }
    if(_sesh.get_level() == 39){
        //forbidden love
        if(_timer == _cutscene_length / 4){
            knight *k = new knight(140, 0, false, _common_stuff.rand);
            //todo make him pause
            k->move_to(_cutscene_length / 4, -80, 0);
            _cutscene_objects.emplace_back(k);
        }else if(_timer == _cutscene_length / 2){
            _cutscene_objects.at(3)->set_horizontal_flip(true);
        }else if(_timer == _cutscene_length / 2 + 20){
            _cutscene_objects.at(0)->move_to(_cutscene_length / 4, 160, 0);
            _cutscene_objects.at(3)->move_to(_cutscene_length / 4, 140, 0);
        }else if(_timer == 3 * _cutscene_length / 4){
            ((heart *) _cutscene_objects.at(2).get())->dissipate();
        }
    }

    if(_sesh.get_level() == 47){
        _cutscene_objects.at(1)->update_anim();

        // he needs to fire his bow more quickly

        if((_timer - 20) % 40 == 0){
            if(_cutscene_objects.size() > 2 && _timer / 40 < 5){
                ((arrow *) _cutscene_objects.back().get())->plink();
                _cutscene_objects.at(0).get()->squish(10);
            }

            if(_timer / 40 < 4){
                ((archer *) _cutscene_objects.at(1).get())->set_bow_drawn(true);
                arrow *a = new arrow(bn::fixed_point(120, -5), true);
                a->update();
                _cutscene_objects.emplace_back(a);
            }
            
        }else if((_timer - 20) % 40 == 20){
            if(_timer / 40 < 5){
                ((archer *) _cutscene_objects.at(1).get())->set_bow_drawn(false);
                ((arrow *) _cutscene_objects.back().get())->move_to(20, -80, -5);
            }
        }
        
        if(_timer == _cutscene_length - 60){
            _cutscene_objects.at(1).get()->move_to(20, 130, -5);
        }else if(_timer == _cutscene_length - 40){
            _cutscene_objects.at(0).get()->set_horizontal_flip(true);
        }
    }

    if(_sesh.get_level() == 51){
        // kerrek cutscene
        if(_timer == 20){
            ((player *) _cutscene_objects.at(0).get())->enable_breath();
        }else if(_timer == 30){
            ((kerrek *) _cutscene_objects.at(1).get())->burninate();
        }else if(_timer == 50){
            ((player *) _cutscene_objects.at(0).get())->disable_breath();
        }else if(_timer == _cutscene_length / 2 + 30){
            if(_sesh.get_dragon() == dragon::TROGDOR || 
                _sesh.get_dragon() == dragon::SUCKS)
            {
                _common_stuff.commentary.roast_kerrek();
            }
        }
    }

    if(_sesh.get_level() == 101){
        strongbad *sbad = ((strongbad *) _cutscene_objects.at(0).get());
        short credits_start_time = 560, credits_interval = 70;
        if(_timer == 120){
            sbad->talk();
            sbad->stop_animating();
        }else if(_timer == 180){
            sbad->start_animating();
            bn::sound_items::cutscene_congrats.play(_common_stuff.savefile.options.sound_vol);
            write_text("congratulations.");
        }else if(_timer == 250){
            sbad->stop_animating();
        }else if(_timer == 280){
            sbad->start_animating();
            _serif_white.generate(0, -48, "you got", _text_sprites);
        }else if(_timer == 320){
            sbad->stop_animating();
        }else if(_timer == 340){
            sbad->start_animating();
            _serif_white.generate(0, -36, "good score", _text_sprites);
        }else if(_timer == 380){
            _common_stuff.commentary.i_sound_realistic();
        }else if(_timer == 390){
            sbad->stop_animating();
        }else if(_timer == credits_start_time){ // previously 400
            sbad->set_visible(false);
            bn::sound_items::cutscene_credits.play(_common_stuff.savefile.options.music_vol);
            write_text("cast");
        }else if(_timer == credits_start_time + credits_interval){
            dragon dtype = _sesh.get_dragon();
            bn::string<64> name = 
                common_stuff::to_lower(
                    dragons[(int)dtype].name);
            if(dtype == dragon::CHIAROSCURO ||
                dtype == dragon::SUCKS)
            {
                name = "the " + name + " dragon";
            }
            write_text(name);
            _cutscene_objects.emplace_back(
                create_player(dtype, 0, 0, _sesh, 
                false,_common_stuff));
        }else if(_timer == credits_start_time + credits_interval*2){
            _cutscene_objects.at(1)->set_visible(false);
            write_text("perez");
            _cutscene_objects.emplace_back(new peasant(0, 0, 0, 0, _dummy_cottage));
        }else if(_timer == credits_start_time + credits_interval*3){
            _cutscene_objects.at(2)->set_visible(false);
            _text_sprites.clear();
            write_text("hackworth");
            peasant *hackworth = new peasant(0, 0, 0, 0, _dummy_cottage);
            hackworth->set_frame(1);
            _cutscene_objects.emplace_back(hackworth);
        }else if(_timer == credits_start_time + credits_interval*4){
            _cutscene_objects.at(3)->set_visible(false);
            write_text("'the steve'");
            peasant *the_steve = new peasant(0, 0, 0, 0, _dummy_cottage);
            the_steve->set_sprite_ablaze();
            _cutscene_objects.emplace_back(the_steve);
        }else if(_timer == credits_start_time + credits_interval*5){
            _cutscene_objects.at(4)->set_visible(false);
            write_text("the blue knight");
            _cutscene_objects.emplace_back(new knight(0, 0, true, _common_stuff.rand));
        }else if(_timer == credits_start_time + credits_interval*6){
            _cutscene_objects.at(5)->set_visible(false);
            write_text("the red knight");
            _cutscene_objects.emplace_back(new knight(0, 0, false, _common_stuff.rand));
        }else if(_timer == credits_start_time + credits_interval*7){
            _cutscene_objects.at(6)->set_visible(false);
            write_text("the conjoined");
            _serif_white.generate(0, -48, "archers", _text_sprites);
            for(int i = 0; i < 2 ; ++i){
                archer *arch = new archer(0, i, _common_stuff);
                arch->set_x(10 - 20*i);
                _cutscene_objects.emplace_back(arch);
            }
        }else if(_timer == credits_start_time + credits_interval*8){
            _cutscene_objects.at(7)->set_visible(false);
            _cutscene_objects.at(8)->set_visible(false);
            write_text("and WorldlyWise");
            _serif_white.generate(0, -48, "as The Kerrek", _text_sprites);
            _cutscene_objects.emplace_back(new kerrek(0, 0));
        }else if(_timer == credits_start_time + credits_interval*9 + 25){
            _cutscene_objects.at(9)->set_visible(false);
            write_text("keep playing!");
            sbad->set_visible(true);
            sbad->set_x(30);
            sbad->set_rotation_angle(330);
            sbad->set_frame(3);
            player *mytrogdor = ((player *) _cutscene_objects.at(1).get());
            mytrogdor->set_visible(true);
            mytrogdor->set_x(-35);
            mytrogdor->set_y(-5);
            mytrogdor->set_rotation_angle(30);

            _common_stuff.update_achievement("win");
            _common_stuff.unlock_character(dragon::WORMDINGLER);
            if(_sesh.get_score() > 0 && !_sesh.troghammer_enabled() && 
                !_sesh.can_lose_trogmeter() && _sesh.get_dragon() == dragon::TROGDOR)
            {
                _common_stuff.update_achievement("classic");
            }
        }
    }

    if(cutscene_over()) {
        if(_last_scene == scene_type::CUTSCENE_VIEWER){
            result = scene_type::CUTSCENE_VIEWER;
        }else if(_sesh.get_level() == 101){
            result = scene_type::HISCORES;
        }else{
            result = scene_type::PLAY;
        }
    }

    _timer++;
    return result;
}

bool movie_scene::cutscene_over(){
    return _timer >= _cutscene_length;
}

}