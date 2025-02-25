#include <bn_log.h>
#include <bn_keypad.h>
#include "achievements_scene.h"
#include "bn_sprite_items_lock_icon.h"
#include "bn_sprite_items_selector.h"
#include "bn_regular_bg_items_achievements_menu_bg.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_palette_items_font_yellow.h"

namespace trog {

// TODO, this crashes when you exit a game then return to it. WHY?
// I HAVE NO FUCKIN IDEA!!! 

achievements_scene::achievements_scene(common_stuff &common_stuff) : 
    _selected_icon(bn::sprite_items::lock_icon.create_sprite(-98,-59)),
    _selector(bn::sprite_items::selector.create_sprite(menu_top_left)),
    _selector_anim(bn::create_sprite_animate_action_forever(
        _selector, 10, bn::sprite_items::selector.tiles_item(), 
        1, 2, 2, 2, 1, 0, 0, 0)),
    _bg(bn::regular_bg_items::achievements_menu_bg.create_bg(0,0)),
    _common_stuff(common_stuff),
    _selected(0) 
{
    
    int z = 0;
    for(const achievement_rom_data &acd : acdata)
    {

        bn::sprite_ptr icon = 
            bn::sprite_items::lock_icon.create_sprite(
            menu_top_left.x() + spacing * (z % items_per_line),
            menu_top_left.y() + spacing * (z / items_per_line)
        );

        if(_common_stuff.acm.is_achieved(acd.tag))
        {
            icon.set_item(bn::sprite_items::achievements, z);
            icon.set_scale(0.5);    
        }

        _opts.emplace_back(acd, icon, z);
        ++z;
    }

    bn::sprite_ptr icon = 
        bn::sprite_items::achievements.create_sprite(
        menu_top_left.x() + spacing * (z % items_per_line),
        menu_top_left.y() + spacing * (z / items_per_line),
        11
    );
    icon.set_scale(0.5);
    _opts.emplace_back(acdata[0], icon, 11);



    update_info_box();

}



bn::optional<scene_type> achievements_scene::update()
{
    bn::optional<scene_type> result;

    _selector_anim.update();

    if(bn::keypad::up_pressed())
    {
        _selected -= items_per_line;
        if(_selected < 0)
        {
            _selected += _opts.size();
        }
    }
    else if(bn::keypad::down_pressed())
    {
        _selected += items_per_line;
    }
    else if(bn::keypad::left_pressed())
    {
        --_selected;
        if(_selected < 0)
        {
            _selected += items_per_line;
        }
    }
    else if(bn::keypad::right_pressed())
    {
        ++_selected;
    }

    if(_common_stuff.any_dpad_input())
    {
        update_info_box();
    }




    if(bn::keypad::b_pressed() || 
        (bn::keypad::a_pressed() &&
        _selected == _opts.size() - 1))
    {
        return scene_type::EXTRAS;
    }

    if(_selector_move)
    {
        _selector_move->update();
        if(_selector_move->done())
        {
            _selector_move.reset();
        }
    }

    return result;
}

void achievements_scene::update_info_box()
{
    
    _selected = _selected % _opts.size();

    ac_option & selected_option = _opts.at(_selected);

    //todo: bespoke logic for the back button
    bn::string<32> name(selected_option.data.name);
    bn::string<192> desc(selected_option.data.desc);

    _selector_move.emplace(_selector, 5, selected_option.icon.x(), 
        selected_option.icon.y());

    _text_sprites.clear();

    _common_stuff.small_generator.set_center_alignment();

    if(_selected == _opts.size() - 1)
    {
        //back button
        name = "Back";
        desc = "Press A to return to the extras menu.";
    }

    if(_selected == _opts.size() - 1 || 
        _common_stuff.acm.is_achieved(selected_option.data.tag))
    {    
        _common_stuff.small_generator.set_palette_item(
            bn::sprite_palette_items::font_yellow);

        _selected_icon.set_item(bn::sprite_items::achievements,
            selected_option.index);
    }
    else
    {
        _common_stuff.small_generator.set_palette_item(
            bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        name = "Locked";

        _selected_icon.set_item(bn::sprite_items::lock_icon);
    }

    _common_stuff.small_generator.generate(0, -72, 
        name, _text_sprites);

    bn::fixed yoffset = 0;
    for(const bn::string<64> &line : _common_stuff.split_into_lines(
        desc.c_str(), 185))
    {
        _common_stuff.small_generator.set_left_alignment();
        _common_stuff.small_generator.set_palette_item(WHITE_PALETTE);
        _common_stuff.small_generator.generate(-77, -60 + yoffset,
            line, _text_sprites);
        yoffset += 9;
    }



}

}