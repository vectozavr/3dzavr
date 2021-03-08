//
// Created by Иван Ильин on 08.03.2021.
//

#ifndef INC_3DZAVR_SIMPLEX_H
#define INC_3DZAVR_SIMPLEX_H

#include "../utils/Point4D.h"

struct Simplex {
private:
    std::array<Point4D, 4> m_points{};
    unsigned m_size;

public:
    Simplex() : m_points({0, 0,0,0}), m_size(0) {}

    Simplex& operator=(std::initializer_list<Point4D> list) {
        for (auto v = list.begin(); v != list.end(); v++) {
            m_points[std::distance(list.begin(), v)] = *v;
        }
        m_size = list.size();
        return *this;
    }

    void push_front(const Point4D& point) {
        m_points = { point, m_points[0], m_points[1], m_points[2] };
        m_size = std::min(m_size + 1, 4u);
    }

    Point4D& operator[](unsigned i) { return m_points[i]; }
    [[nodiscard]] unsigned size() const { return m_size; }

    auto begin() const { return m_points.begin(); }
    auto end()   const { return m_points.end() - (4 - m_size); }
};

#endif //INC_3DZAVR_SIMPLEX_H
