#include <bn_blending.h>
#include <bn_math.h>
#include "common_stuff.h"
#include "trogdor_variable_32x64_sprite_font.h"
namespace trog { 

common_stuff::common_stuff() : 
    big_generator(variable_32x64_sprite_font),
    commentary(savefile.options.voice_vol, rand),
    _acm(savefile, savefile.options.sound_vol){ 

    cutscene_levels.emplace_back(5, "stompin' good");
    cutscene_levels.emplace_back(9, "fry 'em up dan.");
    cutscene_levels.emplace_back(13, "parade of dragons");
    cutscene_levels.emplace_back(17, "dancin' time");
    cutscene_levels.emplace_back(21, "flex it, troggie");
    cutscene_levels.emplace_back(25, "peasant dominoes");
    cutscene_levels.emplace_back(31, "trogdor incognito");
    cutscene_levels.emplace_back(35, "the race");
    cutscene_levels.emplace_back(39, "forbidden peasant love");
    cutscene_levels.emplace_back(43, "2 cottages");
    cutscene_levels.emplace_back(47, "a funny joke");
    cutscene_levels.emplace_back(51, "smote that kerrek!");

    //NEW ONES...
    cutscene_levels.emplace_back(55, "checkitout checkitout");
    cutscene_levels.emplace_back(59, "stompin' not so good");
    cutscene_levels.emplace_back(63, "stack 'em to the...");
    cutscene_levels.emplace_back(67, "i'll upset your balance!");
    cutscene_levels.emplace_back(71, "checkitout 2");

    //old one
    cutscene_levels.emplace_back(101, "ending");


    bn::array<bool, 20> base_unlocked_cutscenes;
    base_unlocked_cutscenes.fill(false);

    bn::array<bool, 4> base_unlocked_dragons;
    base_unlocked_dragons.fill(false);
    base_unlocked_dragons[0] = true;

    //DEFAULT format tag
    default_format_tag = str_to_format_tag(TROG_FORMAT_TAG);
    bn::sram::read(savefile);

    //2.0 format tag

    //if the format tag is invalid, then we have to set it up.
    if(savefile.format_tag != default_format_tag){
        format_save();

        //CONVERT TO 3.0 SAVE FORMAT IF THERE IS NO SAVE.
        if(savefile.format_tag == str_to_format_tag("trog2.0")){
            saved_data_v20 old_save;
            bn::sram::read(old_save);
            savefile.options = old_save.options;
            savefile.high_scores_table = old_save.high_scores_table;

            
            savefile.achievements.fill(0);

            if(old_save.cheat_unlocked)
            {
                _acm.update_achievement("konami", 0);                
            }

            //session variables
            savefile.session.exists = old_save.session.exists;
            savefile.session.mans = old_save.session.mans;
            savefile.session.level = old_save.session.level;
            savefile.session.score = old_save.session.score;
            savefile.session.can_visit_treasure_hut = old_save.session.can_visit_treasure_hut;
            savefile.session.troghammer = old_save.session.troghammer;
            savefile.session.can_lose_trogmeter = old_save.session.can_lose_trogmeter;
            savefile.session.cottage_burnination_status = old_save.session.cottage_burnination_status;
            savefile.session.thinfo = old_save.session.thinfo;
            //new session parameters
            savefile.session.current_dragon = dragon::TROGDOR;

            //new elements of saved_data
            savefile.last_dragon_used = dragon::TROGDOR;

            savefile.unlocked_cutscenes = base_unlocked_cutscenes;
            
            //3.1 save ONLY
            savefile.unlocked_dragons = base_unlocked_dragons;

            uint8_t max_level_reached = savefile.session.level;
            for(const high_score_entry &entry : savefile.high_scores_table) {
                if(entry.get_level() > max_level_reached) 
                    max_level_reached = entry.get_level();
            }
            for(uint8_t i = 0; i < savefile.unlocked_cutscenes.size(); i++){
                //if we have reached a level higher than this cutscene level, we should unlock it
                savefile.unlocked_cutscenes[i] = max_level_reached > cutscene_levels.at(i).first;
            }        
        }

        bn::sram::write(savefile);
    }

    bn::sprite_text_generator autosave_gen(small_font_white);       
    autosave_gen.generate(-120, 77, "autosaved.", _autosave_text);

    //todo: if more transparency alpha becomes a thing (e.g. menu
    // transitions) then we will need to move this
    // into its own function where the text gets constructed
    // and destructed when needed
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

//N.B. : yes, binary search WOULD be faster here. However, this is running on an array
// of 20 elements and only runs once per level on the completion screen. 
// I think linear search will be fine for our purposes. 
bool common_stuff::level_has_cutscene(const uint8_t &current_level) const {
    for(const bn::pair<uint8_t, bn::string<64>> entry : cutscene_levels) {
        if(entry.first == current_level) return true;
    }
    return false;
}

void common_stuff::unlock_cutscene_at_level(const uint8_t &current_level) {
    for(int i = 0; i < cutscene_levels.size(); ++i) {
        if(cutscene_levels.at(i).first == current_level){
            savefile.unlocked_cutscenes[i] = true;
        }
    }
}

void common_stuff::unlock_character(const dragon &dragon) {
    if(!savefile.unlocked_dragons[(int)dragon])
    {
        savefile.unlocked_dragons[(int)dragon] = true;
        newly_unlocked.push_back(dragon);
        save();
    }
}

void common_stuff::update_achievement(const bn::string<8> &tag, 
    const long &new_value)
{
    bool newly_achieved = _acm.update_achievement(tag, new_value);
    if(newly_achieved)
    {
        int num_achievements = _acm.total_unlocked();
        int num_unlocked_chars = available_dragons().size();


        if(num_achievements % 6 == 0 && 
            num_achievements / 6 >= num_unlocked_chars )
        {
            unlock_random_character();
        }
    }
    
}

//todo: maybe binary search would be better for this lol
bn::string<32> common_stuff::cutscene_name_for_level(const uint8_t &level) const{
    for(const bn::pair<uint8_t, bn::string<64>> entry : cutscene_levels) {
        if(entry.first == level) return entry.second;
    }
    BN_ERROR("invalid level passed into common_stuff.cutscene_name_for_level");
    return bn::string<32>();
}

bn::fixed common_stuff::euclidean_dist(const bn::fixed_point &a, const bn::fixed_point &b) const {
    bn::fixed xdist = a.x() - b.x();
    bn::fixed ydist = a.y() - b.y();
    return bn::sqrt(xdist * xdist + ydist * ydist);
}

bn::string<10> common_stuff::play_time(){
    bn::string<10> result;
    bn::ostringstream play_time_stream(result);
    uint32_t seconds = (savefile.stats.play_time / 60);
    uint32_t minutes = (seconds / 60);
    uint16_t hours = (minutes / 60);
    seconds = seconds % 60;
    minutes = minutes % 60;

    if(hours > 999) hours = 999;
    play_time_stream << hours << ":";
    if(minutes < 10) play_time_stream << 0;
    play_time_stream << minutes;
    if(seconds < 10) play_time_stream << 0;
    play_time_stream << ":" << seconds;
    return result;
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

void common_stuff::format_save(){
        savefile.format_tag = default_format_tag;

        //default settings!
        savefile.options.sound_vol = 0.75;
        savefile.options.music_vol = 0.75;
        savefile.options.voice_vol = 0.75;
        savefile.options.troghammer = true;
        savefile.options.decrement_trogmeter = true;
        savefile.options.starting_lives = 3;

        savefile.high_scores_table.fill(high_score_entry("", 0, 0));

        savefile.session.exists = false;

        savefile.session.mans = 3;
        savefile.session.level = 1;
        savefile.session.score = 0;
        savefile.session.can_visit_treasure_hut = false;
        savefile.session.troghammer = false;
        savefile.session.can_lose_trogmeter = false;
        savefile.session.cottage_burnination_status.fill(false);
        savefile.session.thinfo.current_state = trog::troghammer_state::UNALERTED;
        savefile.session.thinfo.timer = 1800;
        savefile.session.thinfo.pos = TROG_VOIDTOWER_POS_DAY;

        savefile.session.current_dragon = dragon::TROGDOR;
        savefile.last_dragon_used = dragon::TROGDOR;

        savefile.achievements.fill(0);

        savefile.unlocked_cutscenes.fill(false);
        savefile.unlocked_dragons.fill(false);

        savefile.stats.play_time = 0;
        savefile.stats.points_earned = 0;
        savefile.stats.peasants_stomped = 0;
        savefile.stats.cottages_entered = 0;
        savefile.stats.treasure_collected = 0;
        savefile.stats.times_sworded = 0;
        savefile.stats.times_arrowed = 0;
        savefile.stats.times_hammered = 0;
        savefile.stats.games_played = 0;
        savefile.stats.peasants_burned = 0;
        savefile.stats.cottages_burned = 0;
        savefile.stats.fastest_level = -1;
        savefile.stats.highest_level = 1;
}



bn::vector<bn::string<64>, 3> common_stuff::split_into_lines(const char *text,
    const uint8_t max_line_width, const int8_t *char_widths){
    bn::vector<bn::string<64>, 3>result;
    
    uint8_t line_width = 0;
    uint8_t line_num = 0;
    uint16_t line_start = 0;
    uint16_t line_end = 0;
    for(uint16_t pos = 0; text[pos] != '\0'; ++pos){
        char c = text[pos];
        line_width += char_widths[c - ' '];
        if(line_width > max_line_width){
            uint16_t last_space = pos;
            while(text[last_space] != ' '){
                //Find the last space
                last_space--;
            }
            line_end = last_space;
        }

        //this case handles the end of the string (\0)
        //pos + 1 here because the for loop won't process the end-of-string character
        if(text[pos + 1] == '\0') line_end = pos + 1;

        //this case lets us handle manual newlines (\n)
        if(text[pos] == '\n') line_end = pos;

        //if this variable is set, we know where the end of the line is
        if(line_end){
            //copy the current line, up to the last space, into the result vector
            bn::string<64> line;
            bn::ostringstream line_stream(line);
            for(uint16_t i = line_start; i < line_end; i++){
                line_stream << text[i];
            }
            result.emplace_back(line);

            if(text[pos + 1] != '\0'){
                pos = line_end + 1;
            }
            line_start = pos;

            //we are effectively un-setting these variables by zeroing them
            line_width = 0;
            line_end = 0;

            ++line_num;
        }

    }

    return result;
}

void common_stuff::update()
{
    // Burn a random number every frame.
    // This makes it less likely to get the same random numbers every time you play
    rand.update();
    
    //Advance our commentary timer so that we don't commentate over ourself...
    commentary.update();

    //Update the achievements manager.
    _acm.update();
    
    //increment our total play time by 1 frame... 
    savefile.stats.play_time++;

    //if our play time is above a certain amount just unlock the character...
    if(savefile.stats.play_time % 108000 == 107999)
    {
        unlock_random_character();
    }
}

void common_stuff::unlock_random_character()
{
    bn::vector<dragon, NUM_DRAGONS> locked_dragons;
    for(int z = 0; z < NUM_DRAGONS; ++z)
    {
        if(!savefile.unlocked_dragons[z])
        {
            locked_dragons.emplace_back((dragon)z);
        }
    }

    if(locked_dragons.size() > 0)
    {
        unlock_character(locked_dragons.at(rand.get_int(0, locked_dragons.size())));
    }
}

bn::vector<dragon, NUM_DRAGONS> common_stuff::available_dragons()
{
    bn::vector<dragon, NUM_DRAGONS> result;
    for(int z = 0; z < NUM_DRAGONS; ++z)
    {
        if(savefile.unlocked_dragons[z])
        {
            result.emplace_back((dragon)z);
        }
    }
    return result;
}

bn::string<64> common_stuff::to_lower(const bn::string<64> &s)
{
    bn::string<64> result;
    for(const char &c : s)
    {
        if('A' <= c && c <= 'Z')
        {
            result.push_back(c - ('Z' - 'z'));
        }
        else
        {
            result.push_back(c);
        }
    }
    return result;
}

bn::string<64> common_stuff::to_upper(const bn::string<64> &s)
{
    bn::string<64> result;
    for(const char &c : s)
    {
        if('a' <= c && c <= 'z')
        {
            result.push_back(c + ('Z' - 'z'));
        }
        else
        {
            result.push_back(c);
        }
    }
    return result;
}

}
