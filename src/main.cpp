#include <array>
#include <iostream>
#include <memory>

#include "Camera.h"
#include "Model.h"
#include "Pipeline.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "State.h"
#include "Texture.h"
#include "utils.h"

// GLM
#include "glm/glm.hpp"

// Sokol
#define SOKOL_IMPL
#define SOKOL_GLCORE
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

// ImGUI
#define SOKOL_IMGUI_IMPL
#include "imgui.h"
#include "sokol_imgui.h"

auto init() -> void
{
    auto state = State::get_instance();
    auto resources = ResourceManager::get_instance();

    auto textured_shader = resources->add_shader("textured", std::make_shared<Shader>(textured_shader_desc(sg_query_backend())));
    auto colored_shader = resources->add_shader("colored", std::make_shared<Shader>(colored_shader_desc(sg_query_backend())));
    auto textured_pipeline = resources->add_pipeline("textured", std::make_shared<Pipeline>(textured_shader));
    auto colored_pipeline = resources->add_pipeline("colored", std::make_shared<Pipeline>(colored_shader));

    auto model_texture = std::make_shared<Texture>("res/cube.png");
    auto model_mesh = std::make_shared<Mesh>("res/cube.obj");

    auto normalized_scale = model_mesh->normalized_scale();

    for (int i = 0; i < 128; i++) {
        float x = rndf(-10.0f, 10.0f);
        float y = rndf(-10.0f, 10.0f);
        float z = rndf(-10.0f, 10.0f);
        auto model = std::make_shared<TexturedModel>(model_mesh, model_texture);
        model->translation = glm::vec3(x, y, z);
        model->scale = normalized_scale;
        state->scene.add_model(model);
    }

    auto light_mesh = std::make_shared<Mesh>("res/cube.obj");
    state->light_mesh = light_mesh; // Allows GUI to add lights

    state->scene.add_light(std::make_shared<Light>(light_mesh, glm::vec4 { 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec4 { 20.0f, 0.0f, 0.0f, 0.0f }));
    state->scene.add_light(std::make_shared<Light>(light_mesh, glm::vec4 { 1.0f, 0.0f, 0.0f, 1.0f }, glm::vec4 { 0.0f, 20.0f, 0.0f, 0.0f }));
    state->scene.add_light(std::make_shared<Light>(light_mesh, glm::vec4 { 0.5f, 0.5f, 0.5f, 1.0f }, glm::vec4 { 0.0f, 0.0f, 20.0f, 0.0f }));
}

auto input(sapp_event const* event) -> void
{
    auto state = State::get_instance();

    if (event->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (event->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_quit();
        }
    }

    if (simgui_handle_event(event)) {
        return;
    }

    switch (event->type) {
    case SAPP_EVENTTYPE_MOUSE_DOWN:
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            sapp_lock_mouse(true);
        }
        break;
    case SAPP_EVENTTYPE_MOUSE_UP:
        if (event->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            sapp_lock_mouse(false);
        }
        break;
    case SAPP_EVENTTYPE_MOUSE_SCROLL:
        state->camera.zoom(event->scroll_y * -0.5f);
        break;
    case SAPP_EVENTTYPE_MOUSE_MOVE:
        if (sapp_mouse_locked()) {
            state->camera.orbit(event->mouse_dx * 0.25f, event->mouse_dy * 0.25f);
        }
        break;
    default:
        break;
    }
}

auto frame() -> void
{
    auto state = State::get_instance();
    const float delta = (float)sapp_frame_duration();

    // Update
    state->camera.update();
    state->scene.update(delta);
    for (auto& model : state->scene.models) {
        model->rotation += state->rotation_speed * delta;
        model->update(delta);
    }

    // Render
    state->renderer.begin_frame();
    state->scene.render();
    state->gui.render();
    state->renderer.end_frame();
}

sapp_desc sokol_main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    sapp_desc desc = {};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.event_cb = input;
    desc.window_title = "Dear ImGui (sokol-app)";
    desc.ios_keyboard_resizes_canvas = false;
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;
    desc.logger.func = slog_func;
    return desc;
}
