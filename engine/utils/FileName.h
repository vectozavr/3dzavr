//
// Created by Иван Ильин on 23/01/2024.
//

#ifndef INC_3DZAVR_TEST_FILENAME_H
#define INC_3DZAVR_TEST_FILENAME_H

#include <filesystem>

class FileName final {
private:
    std::string _name;
    std::filesystem::path _path;
public:
    explicit FileName(const std::string& name = "");
    FileName (const std::string& path, const std::string& name);

    [[nodiscard]] std::string str() const { return _name; }

    [[nodiscard]] bool empty() const { return _name.empty(); }

    bool operator==(const FileName &name) const { return _name == name._name; }
    bool operator!=(const FileName &name) const { return _name != name._name; }
    bool operator<(const FileName &name) const { return _name < name._name; }

    [[nodiscard]] bool contains(const FileName& tag) const;

    [[nodiscard]] std::string parent_path() const;

};

#endif //INC_3DZAVR_TEST_FILENAME_H
