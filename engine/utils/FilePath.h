//
// Created by Иван Ильин on 23/01/2024.
//

#ifndef UTILS_FILEPATH_H
#define UTILS_FILEPATH_H

#include <filesystem>

class FilePath final {
private:
    std::filesystem::path _path;
public:
    explicit FilePath(const std::string& name = "");
    FilePath (const std::string& path, const std::string& name);

    [[nodiscard]] std::string str() const { return _path.string(); }

    [[nodiscard]] bool empty() const { return _path.empty(); }

    bool operator==(const FilePath &name) const { return _path == name._path; }
    bool operator!=(const FilePath &name) const { return _path != name._path; }
    bool operator<(const FilePath &name) const { return _path < name._path; }

    [[nodiscard]] bool contains(const FilePath& tag) const;

    [[nodiscard]] std::string parentPath() const;
    [[nodiscard]] std::string fileName() const;
};

#endif //UTILS_FILEPATH_H
