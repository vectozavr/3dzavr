//
// Created by Иван Ильин on 22/01/2024.
//

#include "Material.h"

bool MaterialTag::contains(const MaterialTag &nameTag) const {
    if(_name.find(nameTag.str()) != std::string::npos) {
        return true;
    }
    return false;
}