#pragma once

#include "matrix.hpp"
#include <raylib.h>
#include <vector>

namespace ssu {
struct Path {
        std::vector<Vec3> vertices;
        Color color;
        float thickness;

        Path(std::vector<Vec3> vertices, Color color, float thickness)
            : vertices(vertices),
              color(color),
              thickness(thickness) {}
};

struct Model {
        std::vector<Path> paths;
        Mat4 modelM;

        Model() = default;
        Model(std::vector<Path> paths, Mat4 mat)
            : paths(paths),
              modelM(mat) {}
};
} // namespace ssu
