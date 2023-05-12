#include <bn_blending.h>
#include "common_stuff.h"
#include "trogdor_variable_8x16_sprite_font.h"
#include "trogdor_variable_32x64_sprite_font.h"
#include "trogdor_variable_8x8_sprite_font.h"
namespace trog { 


common_stuff::common_stuff() : 
    text_generator(trog::variable_8x16_sprite_font),
    big_generator(trog::variable_32x64_sprite_font),
    small_generator(trog::variable_8x8_sprite_font) { 

    //DEFAULT format tag
    bn::array<char, 8> default_format_tag;
    bn::istring_base default_format_tag_istring(default_format_tag._data);
    bn::ostringstream default_format_tag_stream(default_format_tag_istring);
    default_format_tag_stream.append(TROG_FORMAT_TAG);

    bn::sram::read(savefile);
    //if there is no saved data, then we have to set it up.
    if(savefile.format_tag != default_format_tag){
        savefile.format_tag = default_format_tag;

        //as long as this flag is set to false,
        //the rest of the saved session can be garbage data
        savefile.session.exists = false;

        //default settings!
        savefile.sound_vol = 0.75;
        savefile.music_vol = 0.75;
        savefile.voice_vol = 0.75;
        savefile.troghammer = true;
        savefile.decrement_trogmeter = true;
        savefile.starting_lives = 3;
    }

    text_generator.set_left_alignment();
    text_generator.set_palette_item(WHITE_PALETTE);        
    text_generator.generate(-120, 75, "autosaved.", _autosave_text);
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


}
