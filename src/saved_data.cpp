#include "saved_data.h"
#include "constants.h"

namespace trog { 


saved_data::saved_data() {

}

bool saved_data::is_valid_object(){

    return _format_tag == default_format_tag();
}

bn::array<char, 8> saved_data::default_format_tag(){
    bn::array<char, 8> default_format_tag;
    bn::istring_base default_format_tag_istring(default_format_tag._data);
    bn::ostringstream default_format_tag_stream(default_format_tag_istring);
    default_format_tag_stream.append(TROG_FORMAT_TAG);
    return default_format_tag;    
}

void saved_data::set_cottage_burnination(unsigned short dex, bool status){
    _cottage_burnination_status[dex] = status;
}

void saved_data::clear_burnination_array(){
    for(int z = 0; z < 6; ++z){
        _cottage_burnination_status[z] = false;
    }
}

}
