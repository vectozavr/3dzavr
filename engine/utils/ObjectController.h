#ifndef UTILS_OBJECTCONTROLLER_H
#define UTILS_OBJECTCONTROLLER_H

#include "components/TransformMatrix.h"
#include <io/Keyboard.h>
#include <io/Mouse.h>

class ObjectController {
private:
    std::shared_ptr<TransformMatrix> _object;

public:
    ObjectController(std::shared_ptr<TransformMatrix> transformMatrix);

    void update();
};

#endif //UTILS_OBJECTCONTROLLER_H
