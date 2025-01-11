#pragma once

#include <entt/entt.hpp>
#include <entt/process/process.hpp>
#include "../ECS.hpp"

/**
 * A process that runs update systems.
 */
class UpdateProcess : public entt::process<UpdateProcess, float> {
public:
    explicit UpdateProcess(ECS &ecs)
        : ecs_(ecs)
    {}

    void update(float dt, void *data, auto succeed, auto fail) {
        // Run all update systems
        ecs_.run_update(dt);
    }

private:
    ECS &ecs_;
};
