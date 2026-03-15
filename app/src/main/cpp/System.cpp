#include "Systems.h"

void System::addEntity(sptent entity)
{
    entities.push_back(entity);
}

void System::clearEntities()
{
    entities.clear();
}
