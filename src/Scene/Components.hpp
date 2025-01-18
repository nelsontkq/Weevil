#ifndef WV_COMPONENTS_HPP
#define WV_COMPONENTS_HPP

#include <../Utils/UUID.hpp>
#include <string>

using namespace wv;

struct IDComponent
{
    UUID ID;

    IDComponent() = default;
    IDComponent(const IDComponent &) = default;
};
