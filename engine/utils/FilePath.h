#ifndef UTILS_FILEPATH_H
#define UTILS_FILEPATH_H

#include <filesystem>

/**
 * @class FilePath
 * @brief Encapsulates file path operations.
 *
 * Provides a wrapper around std::filesystem::path to simplify file path operations, including
 * construction, comparison, and information extraction such as parent path and file name.
 */
class FilePath final {
private:
    std::filesystem::path _path; ///< The underlying filesystem path.

public:
    /**
     * @brief Constructs a FilePath object with an optional file name.
     * @param name The file name to initialize the path with. If empty, creates an empty path.
     */
    explicit FilePath(const std::string& name = "");

    /**
     * @brief Constructs a FilePath object from a directory path and file name.
     * @param path The directory path.
     * @param name The file name.
     */
    FilePath (const std::string& path, const std::string& name);

    /**
     * @brief Returns the file path as a string.
     * @return The file path.
     */
    [[nodiscard]] std::string str() const { return _path.string(); }

    /**
     * @brief Checks if the file path is empty.
     * @return True if the path is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const { return _path.empty(); }

    /**
     * @brief Compares two FilePath objects for equality.
     * @param name The FilePath object to compare with.
     * @return True if the paths are equal, false otherwise.
     */
    bool operator==(const FilePath &name) const { return _path == name._path; }

    /**
     * @brief Compares two FilePath objects for inequality.
     * @param name The FilePath object to compare with.
     * @return True if the paths are not equal, false otherwise.
     */
    bool operator!=(const FilePath &name) const { return _path != name._path; }

    /**
     * @brief Compares two FilePath objects to determine ordering.
     * @param name The FilePath object to compare with.
     * @return True if this path is less than the given path, false otherwise.
     */
    bool operator<(const FilePath &name) const { return _path < name._path; }

    /**
     * @brief Checks if the file path contains the specified sub path (tag).
     * @param tag The FilePath object to check for containment.
     * @return True if this path contains the specified tag, false otherwise.
     */
    [[nodiscard]] bool contains(const FilePath& tag) const;

    /**
     * @brief Returns the parent directory path as a string.
     * @return The parent directory path.
     */
    [[nodiscard]] std::string parentPath() const;

    /**
    * @brief Returns the file name (without the path) as a string.
    * @return The file name.
    */
    [[nodiscard]] std::string fileName() const;
};

#endif //UTILS_FILEPATH_H
