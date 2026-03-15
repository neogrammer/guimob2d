#ifndef ENTITY_H__
#define ENTITY_H__

#include <memory>
#include <utility>
#include <vector>

#include "Components.h"
#include "Globs.h"

class Entity
{
    std::vector<sptcpt> mComponents;

public:
    void addComponent(sptcpt component)
    {
        mComponents.push_back(component);
    }

    template <typename T>
    bool hasComponent() const
    {
        for (const auto& c : mComponents)
        {
            if (std::dynamic_pointer_cast<T>(c))
            {
                return true;
            }
        }
        return false;
    }

    template <typename T>
    std::shared_ptr<T> getComponent() const
    {
        for (const auto& c : mComponents)
        {
            if (auto casted = std::dynamic_pointer_cast<T>(c))
            {
                return casted;
            }
        }
        return nullptr;
    }
};

#endif
