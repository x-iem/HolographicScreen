#ifndef UTILS_H
#define UTILS_H

#include <vector>

class MovingAverage {
    public:
        MovingAverage(unsigned int m_nbElements);
        float addElement(int element);
        float getFirst();
        float getLast();
    private:
        std::vector<int> elements;
        unsigned int index;
        unsigned int nbElements;
        unsigned int filledElements;
        float total;
};
#endif
