#pragma once
/*
 * A scene contains a list of Graphic objects. It is not
 * structured in any special way. Every frame the objects
 * collected, culled, sorted and drawn in opaque & transparent
 * passes.
 * Graphics are frequently added and removed from the scene.
 * A scene may also contain Lights.
 */
#include "Common.h"

class Scene {
};