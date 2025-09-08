
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
 
namespace trog
{

class dialog
{
    public:
        dialog(const char *text);

    private:
        bn::vector<bn::sprite_ptr, 16> _text_sprites;
    
};
    
}
