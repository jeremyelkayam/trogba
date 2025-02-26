#include "achievement_popup.h"
#include "constants.h"
#include "bn_sprite_items_achievement_box_narrow.h"
#include "small_fonts.h"
#include <bn_log.h>
#include <bn_sound_items.h>

namespace trog { 

achievement_popup::achievement_popup(const bn::fixed &sound_vol,
    const bn::string<16> &achievement_name,
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
        bn::fixed xoffset = 64 * (z - (_box_sprites.max_size() - 1) / 2);
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
    white_generator.generate(_pos.x() + 18, _pos.y() + 8, achievement_name,
        _text_sprites);
    
    _speed = -bn::fixed(HEIGHT) / MOVE_TIME;

    for(bn::sprite_ptr &spr : all_sprites())
    {
        spr.set_z_order(0);
    }
}

void achievement_popup::update()
{
    if(_timer == MOVE_TIME)
    {
        _speed = 0;
    }
    else if(_timer == TOTAL_TIME - MOVE_TIME)
    {
        _speed = bn::fixed(HEIGHT) / MOVE_TIME;
    }

    set_position_relative(bn::fixed_point(0, _speed));
    
    ++_timer;
}

bn::vector<bn::sprite_ptr, 36> achievement_popup::all_sprites()
{
    bn::vector<bn::sprite_ptr, 36> result;
    for(bn::sprite_ptr &spr : _text_sprites)
    {
        result.emplace_back(spr);
    }
    for(bn::sprite_ptr &spr : _box_sprites)
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

}