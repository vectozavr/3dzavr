#include <iostream>

#include <utils/FilePath.h>

FilePath::FilePath(const std::string& path) : _path(path) {

}

bool FilePath::contains(const FilePath &nameTag) const {
    if(_path.string().find(nameTag.str()) != std::string::npos) {
        return true;
    }
    return false;
}

std::string FilePath::parentPath() const {
    return _path.parent_path().string();
}

std::string FilePath::fileName() const {
    return _path.filename().string();
}

FilePath::FilePath(const std::string& path, const std::string& name) {
    _path = std::filesystem::path(path) / name;
}

