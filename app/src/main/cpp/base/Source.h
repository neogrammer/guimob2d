//
// Created by jlhar on 3/10/2026.
//

#ifndef RAYMOB_SOURCE_H
#define RAYMOB_SOURCE_H

template <class T>
class Source
{
    T resource;
public:
    virtual void Create(T resource_)  = 0;
    virtual const T& Get() const
    {
        return resource;
    }

};

#endif //RAYMOB_SOURCE_H
