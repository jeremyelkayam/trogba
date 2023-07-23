#include <bn_blending.h>
#include "common_stuff.h"
#include "trogdor_variable_8x16_sprite_font.h"
#include "trogdor_variable_32x64_sprite_font.h"
#include "trogdor_variable_8x8_sprite_font.h"
namespace trog { 

common_stuff::common_stuff() : 
    text_generator(variable_8x16_sprite_font),
    big_generator(variable_32x64_sprite_font),
    small_generator(variable_8x8_sprite_font),
    commentary(savefile.options.voice_vol, rand) { 

    _cutscene_levels.emplace_back(5);
    _cutscene_levels.emplace_back(9);
    _cutscene_levels.emplace_back(13);
    _cutscene_levels.emplace_back(17);
    _cutscene_levels.emplace_back(21);
    _cutscene_levels.emplace_back(25);
    _cutscene_levels.emplace_back(31);
    _cutscene_levels.emplace_back(35);
    _cutscene_levels.emplace_back(39);
    _cutscene_levels.emplace_back(43);
    _cutscene_levels.emplace_back(47);
    _cutscene_levels.emplace_back(51);
    _cutscene_levels.emplace_back(101);
    
    bn::array<bool, 13> base_unlocked_cutscenes;
    for(bool &unlocked : base_unlocked_cutscenes){
        unlocked = false;
    }

    //DEFAULT format tag
    bn::array<char, 8> default_format_tag = str_to_format_tag(TROG_FORMAT_TAG);

    //2.0 format tag

    bn::sram::read(savefile);
    //if the format tag is invalid, then we have to set it up.
    if(savefile.format_tag == str_to_format_tag("trog2.0")){
        //CONVERT TO 3.0 SAVE FORMAT.
        saved_data_v20 old_save;
        bn::sram::read(old_save);
        savefile.format_tag = default_format_tag;
        savefile.options = old_save.options;
        savefile.high_scores_table = old_save.high_scores_table;
        savefile.cheat_unlocked = old_save.cheat_unlocked;

        //session variables
        savefile.session.exists = old_save.session.exists;
        savefile.session.mans = old_save.session.mans;
        savefile.session.level = old_save.session.level;
        savefile.session.score = old_save.session.score;
        savefile.session.can_visit_treasure_hut = old_save.session.can_visit_treasure_hut;
        savefile.session.troghammer = old_save.session.troghammer;
        savefile.session.can_lose_trogmeter = old_save.session.can_lose_trogmeter;
        savefile.session.cottage_burnination_status = old_save.session.cottage_burnination_status;
        //new session parameter
        savefile.session.current_dragon = dragon::TROGDOR;

        savefile.unlocked_cutscenes = base_unlocked_cutscenes;

        uint8_t max_level_reached = savefile.session.level;
        for(const high_score_entry &entry : savefile.high_scores_table) {
            if(entry.get_level() > max_level_reached) 
                max_level_reached = entry.get_level();
        }
        for(uint8_t i = 0; i < savefile.unlocked_cutscenes.size(); i++){
            //if we have reached a level higher than this cutscene level, we should unlock it
            savefile.unlocked_cutscenes[i] = max_level_reached > _cutscene_levels.at(i);
        }

        bn::sram::write(savefile);

        
    }else if(savefile.format_tag != default_format_tag){
        savefile.format_tag = default_format_tag;

        //as long as this flag is set to false,
        //the rest of the saved session can be garbage data
        savefile.session.exists = false;

        //default settings!
        savefile.options.sound_vol = 0.75;
        savefile.options.music_vol = 0.75;
        savefile.options.voice_vol = 0.75;
        savefile.options.troghammer = true;
        savefile.options.decrement_trogmeter = true;
        savefile.options.starting_lives = 3;

        savefile.high_scores_table.fill(high_score_entry("", 0, 0));

        savefile.cheat_unlocked = false;

        savefile.unlocked_cutscenes = base_unlocked_cutscenes;
    }

    small_generator.set_left_alignment();
    small_generator.set_palette_item(WHITE_PALETTE);        
    small_generator.generate(-120, 77, "autosaved.", _autosave_text);
    bn::blending::set_transparency_alpha(0.5);
    for(bn::sprite_ptr &sprite : _autosave_text) { 
        sprite.set_blending_enabled(true);
    }
    set_autosave_text_visible(false);
}

void common_stuff::set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, const bool &visible) {
    for(bn::sprite_ptr &sprite : sprites) {
        sprite.set_visible(visible);
        sprite.put_above();
    }
}

bn::array<char, 8> common_stuff::str_to_format_tag(const char *str){
    bn::array<char, 8> result;
    bn::istring_base result_istring(result._data);
    bn::ostringstream result_stream(result_istring);
    result_stream.append(str);
    return result;
}

high_score_entry::high_score_entry(bn::string<9> name, unsigned short level, unsigned short score) : 
    _level(level),
    _score(score) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append(name);
}

high_score_entry::high_score_entry() : 
    _level(0),
    _score(0) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append("DUMMY");
}

bn::string<9> high_score_entry::get_name() const {
    bn::string<9>result;
    //go until you start hitting null chars
    for(int z=0; _name[z] != 0; ++z){
        result.append(_name[z]);
    }

    return result;
}

bool common_stuff::level_has_cutscene(const uint8_t &current_level) const {
    for(const uint8_t &lv : _cutscene_levels) {
        if(lv == current_level) return true;
    }
    return false;
}

void common_stuff::unlock_cutscene_at_level(const uint8_t &current_level) {
    for(int i = 0; i < _cutscene_levels.size(); ++i) {
        if(_cutscene_levels[i] == current_level){
            savefile.unlocked_cutscenes[i] = true;
        }
    }
}

const char* common_stuff::scene_type_to_string(const scene_type &type) const{
    switch(type){
        case scene_type::BONUS:
            return "bonus";
        case scene_type::CREDITS:
            return "credits";
        case scene_type::CUTSCENE_VIEWER:
            return "cutscene viewer";
        case scene_type::DEVS:
            return "devs";
        case scene_type::DRAGON_SELECT:
            return "dragon select";
        case scene_type::FIREYRAGE:
            return "firey rage";
        case scene_type::HISCORES:
            return "hi scores";
        case scene_type::LEVELBEAT:
            return "level beat";
        case scene_type::LOGO:
            return "logo";
        case scene_type::LOSE:
            return "game over";
        case scene_type::MENU:
            return "main menu";
        case scene_type::MOVIE:
            return "movie";
        case scene_type::OPTIONS:
            return "options";
        case scene_type::PLAY:
            return "play";
        case scene_type::TITLE:
            return "title";
        default:
            return "unknown scene";
    }
}

}
