#include <bn_log.h>
#include <bn_keypad.h>
#include "achievements_scene.h"
#include "bn_sprite_items_lock_icon.h"
#include "bn_sprite_items_selector.h"
#include "bn_regular_bg_items_achievements_menu_bg.h"

namespace trog {

achievements_scene::achievements_scene(common_stuff &common_stuff) : 
    _selected_icon(bn::sprite_items::lock_icon.create_sprite(-98,-59)),
    _selector(bn::sprite_items::selector.create_sprite(0,0)),
    _bg(bn::regular_bg_items::achievements_menu_bg.create_bg(0,0)),
    _common_stuff(common_stuff),
    _selected(0) 
{
    

}



bn::optional<scene_type> achievements_scene::update()
{

    bn::optional<scene_type> result;

    if(bn::keypad::b_pressed())
    {
        return scene_type::EXTRAS;
    }

    return result;
}

}