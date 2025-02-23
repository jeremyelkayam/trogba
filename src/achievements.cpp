#include "achievements.h"
#include <bn_math.h>
#include <bn_sram.h>
#include "bn_sprite_items_achievement_goldpile.h"

namespace trog
{

achievements_mgr::achievements_mgr(saved_data &sram_data,
    bn::sprite_text_generator &generator,
    const bn::fixed &sound_vol) : 
    _generator(generator),
    _sound_vol(sound_vol),
    _sram_data(sram_data)
{
    

    int z = 0;
    for(const achievement_rom_data &acd : acdata)
    {
        _achievements.insert(acd.tag,achievement(
            z,
            acd.name,
            acd.desc,
            acd.is_number,
            acd.threshold,
            bn::sprite_items::achievement_jhonka
        ));
        ++z;
    }
}

void achievements_mgr::update_achievement(bn::string<8> tag, 
    const long &new_value)
{
    BN_ASSERT(_achievements.contains(tag), 
        "Invalid tag passed to update_achievement");

    const achievement &data = _achievements.at(tag);

    bool was_achieved = is_achieved(tag);

    if(data.is_number)
    {
        _sram_data.achievements[data.sram_index] = new_value;
    }
    else
    {
        //it's a boolean so new_value is actually a digit
        BN_ASSERT(new_value < max_index(data.threshold), 
            "Boolean position is out of range");
        _sram_data.achievements[data.sram_index] = 
            _sram_data.achievements[data.sram_index] | (1 << new_value);
    }


    if(!was_achieved && is_achieved(tag))
    {
        show_popup(tag);
        bn::sram::write(_sram_data);
    }
}

void achievements_mgr::update()
{
    if(_popup)
    {
        _popup->update();
        if(_popup->done())
        {
            _popup.reset();
        }
    }    
}

bool achievements_mgr::is_achieved(bn::string<8> tag)
{
    return _sram_data.achievements[_achievements.at(tag).sram_index] 
        >= _achievements.at(tag).threshold;
}

void achievements_mgr::show_popup(bn::string<8> tag)
{
    const achievement &data = _achievements.at(tag);

    _popup.emplace(_generator, _sound_vol, data.name, data.icon);

}

int achievements_mgr::max_index(long threshold)
{
    threshold += 1;
    BN_ASSERT(bn::power_of_two(threshold), 
        "threshold for boolean achievements must be a power of two minus one");

    int places_shifted = 0;
    while(threshold != 1)
    {
        threshold = threshold >> 1;
        ++places_shifted;
    }

    return places_shifted;

}

}