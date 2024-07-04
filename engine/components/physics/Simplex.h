#ifndef PHYSICS_SIMPLEX_H
#define PHYSICS_SIMPLEX_H

#include <list>

#include <linalg/Vec3D.h>

/**
 * @enum SimplexType
 * Enumerates the types of simplices based on their dimensions.
 */
enum class SimplexType {
    Zero,       ///< Represents a simplex with no dimensions.
    Point,      ///< Represents a 0-dimensional simplex (a point).
    Line,       ///< Represents a 1-dimensional simplex (a line segment).
    Triangle,   ///< Represents a 2-dimensional simplex (a triangle).
    Tetrahedron ///< Represents a 3-dimensional simplex (a tetrahedron).
};

/**
 * @struct SupportPoint
 * Represents a support point used in the GJK algorithm for collision detection.
 */
struct SupportPoint final {
    Vec3D p1;      ///< The first point in object A used to compute the support point.
    Vec3D p2;      ///< The second point in object B used to compute the support point.
    Vec3D support; ///< The support point in Minkowski space.
};

/**
 * @struct Simplex
 * Represents a simplex used in the GJK algorithm for collision detection.
 */
struct Simplex final {
private:
    std::list<SupportPoint> _points{}; ///< List of support points forming the simplex.

public:
    Simplex() = default;

    /**
     * Initializes a simplex with a list of support points.
     * @param list Initializer list of support points.
     */
    Simplex(std::initializer_list<SupportPoint> list) {
        for (const auto &v : list) {
            _points.push_back(v);
            if (_points.size() > 4) {
                _points.pop_front();
            }
        }
    }

    /**
     * Adds a support point to the front of the simplex.
     * @param point The support point to add.
     */
    void push_front(const SupportPoint &point) {
        _points.push_front(point);
        if (_points.size() > 4) {
            _points.pop_back();
        }
    }

    /**
     * Accesses a support point by index.
     * @param i The index of the support point.
     * @return The support point at the specified index.
     */
    SupportPoint operator[](unsigned i) const {
        auto it = _points.begin();
        for (unsigned k = 0; k < i; k++) {
            ++it;
        }
        return *it;

    }

    /**
     * Gets the number of points in the simplex.
     * @return The number of points.
     */
    [[nodiscard]] unsigned size() const { return _points.size(); }

    /**
     * Gets an iterator to the beginning of the points list.
     * @return An iterator to the beginning.
     */
    [[nodiscard]] auto begin() const { return _points.begin(); }

    /**
     * Gets an iterator to the end of the points list.
     * @return An iterator to the end.
     */
    [[nodiscard]] auto end() const { return _points.end(); }

    /**
     * Determines the type of the simplex based on its size.
     * @return The type of the simplex.
     */
    [[nodiscard]] SimplexType type() const { return static_cast<SimplexType>(_points.size()); }
};

#endif //PHYSICS_SIMPLEX_H
