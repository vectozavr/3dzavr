#ifndef UTILS_OBJECTCONTROLLER_H
#define UTILS_OBJECTCONTROLLER_H

#include "components/TransformMatrix.h"
#include <io/Keyboard.h>
#include <io/Mouse.h>

/**
 * @class ObjectController
 * @brief Manages the transformation of an object in a 3D scene.
 *
 * This class is responsible for controlling the transformation (e.g., position, rotation, scale) of an object within a 3D environment.
 * It utilizes a shared pointer to a TransformMatrix, which encapsulates the transformation state of the object. The update method
 * can be overridden or extended to implement custom behavior for object transformation based on input or other conditions.
 *
 * @private_section
 * _object A shared pointer to a TransformMatrix that represents the transformation state of the controlled object.
 *
 * @public_section
 * ObjectController(std::shared_ptr<TransformMatrix> transformMatrix) Constructor that initializes the controller with a TransformMatrix.
 * void update() Updates the transformation state of the object. This method should be called regularly (e.g., every frame) to apply transformations.
 */
class ObjectController {
private:
    std::shared_ptr<TransformMatrix> _object;

public:
    ObjectController(std::shared_ptr<TransformMatrix> transformMatrix);

    void update();
};

#endif //UTILS_OBJECTCONTROLLER_H
