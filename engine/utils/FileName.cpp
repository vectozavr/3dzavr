//
// Created by Иван Ильин on 23/01/2024.
//

#include <iostream>

#include <utils/FileName.h>

FileName::FileName(const std::string& name) : _name(name), _path(name) {

}

bool FileName::contains(const FileName &nameTag) const {
    if(_name.find(nameTag.str()) != std::string::npos) {
        return true;
    }
    return false;
}

std::string FileName::parent_path() const {
    return _path.parent_path().string();
}

FileName::FileName(const std::string& path, const std::string& name) {
    _path = std::filesystem::path(path) / name;
    _name = _path.string();
}
