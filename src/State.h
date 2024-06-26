#pragma once

#include <array>
#include <iostream>

#include "Camera.h"
#include "GUI.h"
#include "Renderer.h"
#include "Scene.h"

#include "sokol_gfx.h"

class State {
public:
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    static State* get_instance()
    {
        if (instance == nullptr) {
            instance = new State();
        }
        return instance;
    }
    Renderer renderer;
    GUI gui;
    Scene scene;
    Camera camera;

    std::shared_ptr<Mesh> light_mesh;

    float rotation_speed = 0.0f;
    sg_color clear_color = { 0.0f, 0.5f, 0.7f, 1.0f };

    glm::vec4 ambient_color = { 1.0, 1.0, 1.0, 1.0f };
    float ambient_strength = 0.2f;
    int render_mode = 0;
    bool use_lighting = true;

private:
    State() {};
    static State* instance;
};
