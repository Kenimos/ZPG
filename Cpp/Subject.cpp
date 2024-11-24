#include "Subject.h"
#include <algorithm>
#include <iostream>

void Subject::attach(Observer *observer)
{
    if (observer == nullptr)
    {
        std::cerr << "Warning: Attempted to attach a null observer." << std::endl;
        return;
    }
    observers.push_back(observer);
}

void Subject::detach(Observer *observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::notifyObservers()
{
    for (Observer *observer : observers)
    {
        if (observer)
        {
            observer->update(this);
        }
        else
        {
            std::cerr << "Warning: Null observer encountered during notification." << std::endl;
        }
    }
}
