#include "achievements.h"
#include <bn_math.h>

namespace trog
{

achievements_mgr::achievements_mgr(bn::array<long, 64> &sram_data,
    bn::sprite_text_generator &generator,
    const bn::fixed &sound_vol) : 
    _generator(generator),
    _sound_vol(sound_vol),
    _sram_data(sram_data)
{
    _achievements.insert("onehut", achievement_rom_data(
        1,
        "Jhonka's Riches",
        "Find a treasure hut",
        false,
        1,
        bn::sprite_items::achievement_jhonka
    ));
}

void achievements_mgr::update_achievement(bn::string<8> tag, 
    const long &new_value)
{
    BN_ASSERT(_achievements.contains(tag), 
        "Invalid tag passed to update_achievement");

    const achievement_rom_data &data = _achievements.at(tag);

    bool was_achieved = is_achieved(tag);

    if(data.is_number)
    {
        _sram_data[data.sram_index] = new_value;
    }
    else
    {
        //it's a boolean so new_value is actually a digit
        BN_ASSERT(new_value <= max_index(data.threshold));
        _sram_data[data.sram_index] = 
            _sram_data[data.sram_index] | (1 << new_value);
    }


    if(!was_achieved && is_achieved(tag))
    {
        show_popup(tag);
    }

    //TODO Save the progress after updating...
    // This will require moving SRAM Data into a new file
    // Common stuff will pass down the full sram data 
    // into this class which will reference it
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
    return _sram_data[_achievements.at(tag).sram_index] 
        >= _achievements.at(tag).threshold;
}

void achievements_mgr::show_popup(bn::string<8> tag)
{
    const achievement_rom_data &data = _achievements.at(tag);

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