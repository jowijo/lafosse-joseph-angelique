#ifndef ISUBJECT_HPP
#define ISUBJECT_HPP

#include "IObserver.hpp"
#include <vector>

class ISubject {
public:
    virtual void attach(IObserver& observer) = 0;
    virtual void detach(IObserver& observer) = 0;
    virtual void notify() = 0; 

    virtual ~ISubject() {} 
};

#endif // ISUBJECT_HPP