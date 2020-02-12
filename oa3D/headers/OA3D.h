#ifndef OA3D_H
#define OA3D_H

/*
 *#include <assimp/Importer.hpp>
 *#include <assimp/postprocess.h>
 *#include <assimp/scene.h>
 *#include "btBulletDynamicsCommon.h"
 */

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLFW/glfw3.h"

#include <memory>
#include <vector>
#include <iterator>

static const int mWidth = 2400;
static const int mHeight = 900;

namespace OA3D
{


template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

}


#endif
