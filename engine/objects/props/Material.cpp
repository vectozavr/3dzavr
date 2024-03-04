#include "Material.h"

bool MaterialTag::contains(const MaterialTag &nameTag) const {
    if(_name.find(nameTag.str()) != std::string::npos) {
        return true;
    }
    return false;
}