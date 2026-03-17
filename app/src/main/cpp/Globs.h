#ifndef GLOBS_H__
#define GLOBS_H__

#include <memory>
#include <utility>

class Entity;
class Component;

#define sptent std::shared_ptr<Entity>
#define uptent std::unique_ptr<Entity>
#define sptcpt std::shared_ptr<Component>
#define uptcpt std::unique_ptr<Component>
#define spstate std::shared_ptr<GameState>
#define upstate std::unique_ptr<GameState>

#define weak std::weak_ptr<GameState>
#define test(weak) ((weak.lock()) ? true : false)



namespace glb
{
    extern unsigned WW;
    extern unsigned WH;
}

#endif
