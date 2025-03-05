#include "achievement_popup.h"
#include "constants.h"
#include "bn_sprite_items_achievement_box_narrow.h"
#include "small_fonts.h"
#include <bn_log.h>
#include <bn_sound_items.h>
#include "common_stuff.h"

namespace trog { 

achievement_popup::achievement_popup(const bn::fixed &sound_vol,
    const bn::string<32> &achievement_name,
    const int &index) : 
        _speed(0),
        _pos(40,101),
        _timer(0),
        _icon_sprite(bn::sprite_items::achievements.create_sprite(
            _pos.x() - 58, _pos.y(), index))
{
    bn::sound_items::bubble.play(sound_vol);
    for(int z = 0; z < _box_sprites.max_size(); ++z)
    {
        bn::fixed xoffset = 32 * (z - (_box_sprites.max_size() - 1) / 2);
        _box_sprites.push_back(
            bn::sprite_items::achievement_box_narrow.create_sprite(
            _pos.x() + xoffset, _pos.y(), z));
    }
    
    _icon_sprite.put_above();

    bn::sprite_text_generator yellow_generator = 
        bn::sprite_text_generator(small_font_yellow);
    yellow_generator.set_center_alignment();
    yellow_generator.generate(_pos.x() + 18, _pos.y() - 8, "Achievement unlocked",
        _text_sprites);

    bn::sprite_text_generator white_generator = 
    bn::sprite_text_generator(small_font_white);
    white_generator.set_center_alignment();
    

    bn::fixed yoffset = 0;
    bn::vector<bn::string<64>, 3> lines = common_stuff::split_into_lines(
        achievement_name.c_str(), 119);
    yoffset = -9 * lines.size() / 2;
    for(const bn::string<64> &line : lines)
    {
        white_generator.generate(_pos.x() + 18, _pos.y() + 12 + yoffset, 
            line, _text_sprites);
        yoffset += 9;
    }



    //yeah this sucks lol but what can ya do
    for(bn::sprite_ptr &spr : all_sprites())
    {
        spr.set_z_order(0);
        spr.set_bg_priority(0);
    }
}

void achievement_popup::update()
{

    set_position_relative(bn::fixed_point(0, _speed));
    
    ++_timer;
}

bn::vector<bn::sprite_ptr, 36> achievement_popup::all_sprites()
{
    bn::vector<bn::sprite_ptr, 36> result;
    for(bn::sprite_ptr &spr : _box_sprites)
    {
        result.emplace_back(spr);
    }
    for(bn::sprite_ptr &spr : _text_sprites)
    {
        result.emplace_back(spr);
    }
    result.emplace_back(_icon_sprite);
    return result;
}

void achievement_popup::set_position(const bn::fixed_point &pos)
{
    set_position_relative(_pos - pos);
}

void achievement_popup::set_position_relative(const bn::fixed_point &relative_pos)
{
    for(bn::sprite_ptr &spr : all_sprites())
    {
        spr.set_position(spr.position() + relative_pos);
    }
}

bn::fixed achievement_popup::personal_speed()
{
    if(_timer < MOVE_TIME)
    {
        return -(bn::fixed(HEIGHT) / MOVE_TIME);
    }
    else if(_timer >= TOTAL_TIME - MOVE_TIME)
    {
        return bn::fixed(HEIGHT) / MOVE_TIME;
    }
    else return 0;

}

}