#pragma once

#include <string> // std::string


namespace halberd
{
namespace syntax
{
    class declaration
    {
    public:
        // Ensure all derived types are non-copyable
        declaration(const declaration&) = delete;
        declaration& operator=(const declaration&) = delete;

        virtual ~declaration() = 0;

        const char* get_name() const noexcept;

    protected:
        declaration(std::string name) noexcept;

    private:
        std::string _name;
    };
}
}
