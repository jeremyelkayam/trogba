#include "achievements.h"
#include <bn_math.h>
#include <bn_sram.h>
#include <bn_log.h>
#include <bn_sprite_tiles.h>
#include <bn_keypad.h>
#include "constants.h"
#include "achievement_rom_data.h"

namespace trog
{

achievements_mgr::achievements_mgr(saved_data &sram_data,
    const bn::fixed &sound_vol) : 
    _sound_vol(sound_vol),
    _sram_data(sram_data)
{
    int z = 0;
    for(const ext::achievement_rom_data &acd : ext::acdata)
    {
        _achievements.insert(acd.tag,achievement(
            z,
            acd.name,
            acd.is_number,
            acd.threshold
        ));
        ++z;
    }
}

bool achievements_mgr::update_achievement(const bn::string<8> &tag, 
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
        enqueue_popup(tag);
        bn::sram::write(_sram_data);
        return true;
    }
    return false;
}

int achievements_mgr::total_unlocked() const
{
    int total = 0;
    for(bn::pair<bn::string<8>, const achievement> ac : _achievements)
    {
        if(is_achieved(ac.first))
        {
            ++total;
        }
    }
    return total;
}

void achievements_mgr::update()
{
    bn::fixed total_speed = 0;
    for( int z = _popups.size() - 1; z >= 0; --z)
    {
        achievement_popup &p = _popups.at(z);
        total_speed += p.personal_speed();
        p.set_speed(total_speed);
        p.update();
    }
    bn::erase_if(_popups, popup_done);


    while(!_popup_queue.empty() && 
        bn::sprite_tiles::available_tiles_count() > 300 && 
        !_popups.full())
    {
        const achievement &data = _achievements.at(_popup_queue.front());

        _popups.emplace_back(_sound_vol, data.name, data.sram_index);

        _popup_queue.pop_front();
    }

    
    #ifdef DEBUG
        if(bn::keypad::r_pressed())
        {
            enqueue_popup("night");
        }
    #endif
}

bool achievements_mgr::is_achieved(const bn::string<8> &tag) const
{
    return _sram_data.achievements[_achievements.at(tag).sram_index] 
        >= _achievements.at(tag).threshold;
}

void achievements_mgr::enqueue_popup(const bn::string<8> &tag)
{
    _popup_queue.push_back(tag);
}

int achievements_mgr::max_index(long threshold) const
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