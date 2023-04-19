#include "text_box.h"
#include "bn_regular_bg_items_text_box.h"

namespace trog { 

text_box::text_box(bn::sprite_text_generator &text_generator, const char *text) : 
    _box(_countryside(bn::regular_bg_items::text_box.create_bg(0, 58))){

}

}