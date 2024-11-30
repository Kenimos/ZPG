#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

class Subject
{
public:
    virtual ~Subject() {};
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notifyObservers();

protected:
    std::vector<Observer*> observers;
};

#endif
