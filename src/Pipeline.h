#pragma once

#include <memory>
#include <string>

#include "Shader.h"

#include <sokol_gfx.h>

class Pipeline {
public:
    Pipeline(std::shared_ptr<Shader> shader);
    ~Pipeline();

    static auto standard_desc() -> sg_pipeline_desc;

    sg_pipeline get_pipeline() const { return pipeline; }

private:
    sg_pipeline pipeline = {};
};
