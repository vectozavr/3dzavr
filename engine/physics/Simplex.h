//
// Created by Иван Ильин on 08.03.2021.
//

#ifndef ENGINE_SIMPLEX_H
#define ENGINE_SIMPLEX_H

#include <list>

#include "../math/Vec3D.h"

enum class SimplexType {
    Zero,
    Point,
    Line,
    Triangle,
    Tetrahedron
};

struct Simplex final {
private:
    std::list<Vec3D> _points{};

public:
    Simplex() = default;

    Simplex(std::initializer_list<Vec3D> list) {
        for (const auto &v : list) {
            _points.push_back(v);
            if (_points.size() > 4) {
                _points.pop_front();
            }
        }
    }

    void push_front(const Vec3D &point) {
        _points.push_front(point);
        if (_points.size() > 4) {
            _points.pop_back();
        }
    }

    Vec3D operator[](unsigned i) const {
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

#endif //INC_3DZAVR_SIMPLEX_H
