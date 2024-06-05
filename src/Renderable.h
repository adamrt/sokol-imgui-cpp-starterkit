#pragma once

#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "Mesh.h"
#include "State.h"
#include "Texture.h"

#include "sokol_gfx.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.glsl.h"

float random_float(float min, float max);
std::vector<float> parse_obj(const std::string filename);

class Renderable {
public:
    Renderable(State& _state, std::shared_ptr<Mesh> resources, std::shared_ptr<Texture>);
    ~Renderable();

    auto render(const glm::mat4& view_proj) -> void;
    auto update(float delta_time, float rotation_speed) -> void;
    auto set_model_matrix(const glm::mat4& matrix) -> void;
    auto translate(const glm::vec3& translation) -> void;
    auto rotate(float angle, const glm::vec3& axis) -> void;
    auto scale(const glm::vec3& scaling_factors) -> void;
    auto scale(float f) -> void;

    State& state;

private:
    std::shared_ptr<Mesh> shared_resources;
    std::shared_ptr<Texture> texture;
    sg_shader shader = {};
    sg_pipeline pipeline = {};
    sg_bindings bindings = {};
    glm::mat4 model_matrix = { 1.0f };
    float rspeed;
};
