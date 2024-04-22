#ifndef PHYSICS_SIMPLEX_H
#define PHYSICS_SIMPLEX_H

#include <list>

#include "linalg/Vec3D.h"

enum class SimplexType {
    Zero,
    Point,
    Line,
    Triangle,
    Tetrahedron
};

struct SupportPoint final {
    Vec3D p1;
    Vec3D p2;
    Vec3D support;
};

struct Simplex final {
private:
    std::list<SupportPoint> _points{};

public:
    Simplex() = default;

    Simplex(std::initializer_list<SupportPoint> list) {
        for (const auto &v : list) {
            _points.push_back(v);
            if (_points.size() > 4) {
                _points.pop_front();
            }
        }
    }

    void push_front(const SupportPoint &point) {
        _points.push_front(point);
        if (_points.size() > 4) {
            _points.pop_back();
        }
    }

    SupportPoint operator[](unsigned i) const {
        auto it = _points.begin();
        for (unsigned k = 0; k < i; k++) {
            ++it;
        }
        return *it;

    }

    [[nodiscard]] unsigned size() const { return _points.size(); }

    [[nodiscard]] auto begin() const { return _points.begin(); }

    [[nodiscard]] auto end() const { return _points.end(); }

    [[nodiscard]] SimplexType type() const { return static_cast<SimplexType>(_points.size()); }
};

#endif //PHYSICS_SIMPLEX_H
