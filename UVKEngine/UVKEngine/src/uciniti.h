#ifndef UCINITI_H
#define UCINITI_H

/***************************************************************/
// Uciniti engine core includes
/***************************************************************/
// -- Universal --
#include "Uciniti/Core/application.h"
#include "Uciniti/Core/log.h"
#include "Uciniti/Core/input.h"
#include "Uciniti/Core/entry_point.h"
#include "Uciniti/Core/time_step.h"

// -- Events --
#include "Uciniti/Core/Events/event.h"
#include "Uciniti/Core/Events/application_event.h"
#include "Uciniti/Core/Events/key_event.h"
#include "Uciniti/Core/Events/mouse_event.h"

// -- Math --
#include <glm/gtc/matrix_transform.hpp>

/***************************************************************/
//Uciniti engine renderer includes
/***************************************************************/
// -- Base --
#include "Uciniti/Renderer/renderer.h"
#include "Uciniti/Renderer/shader.h"
#include "Uciniti/Renderer/pipeline.h"
#include "Uciniti/Renderer/mesh.h"

// -- Vulkan --
//#include "Uciniti/Platform/Vulkan/vulkan_renderer.h"

#endif // !UCINITI_H