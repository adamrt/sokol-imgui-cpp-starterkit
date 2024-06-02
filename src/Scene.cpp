#include "Scene.h"

auto Scene::add_renderable(const Renderable& renderable) -> void
{
    renderables.push_back(renderable);
}

auto Scene::update(float delta_time) -> void
{
    for (auto& renderable : renderables) {
        renderable.update(delta_time);
    }
}

auto Scene::render(const glm::mat4& view_proj) -> void
{
    for (auto& renderable : renderables) {
        renderable.render(view_proj);
    }
}