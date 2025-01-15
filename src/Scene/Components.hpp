#include <../Utils/UUID.hpp>
#include <string>

using namespace wv;

struct IDComponent
{
    UUID ID;

    IDComponent() = default;
    IDComponent(const IDComponent &) = default;
};