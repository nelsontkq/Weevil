#pragma once

#include <entt/entt.hpp>
#include <entt/process/process.hpp>
#include "../ECS.hpp"

/**
 * A process that runs update systems.
 */
class UpdateProcess : public entt::process<UpdateProcess, u_int64_t> {
public:
    explicit UpdateProcess(ECS &ecs)
        : ecs_(ecs)
    {}

    void update(u_int64_t dt) {
        // Run all update systems
        ecs_.run_update(dt);
    }

private:
    ECS &ecs_;
};
