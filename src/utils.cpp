#include <utils.h>
#include <cmath>

MovingAverage::MovingAverage(unsigned int m_nbElements) :
    elements(std::vector<int>(m_nbElements, 0)),
    nbElements(m_nbElements), index(0), filledElements(0), total(0.0f) {}

float MovingAverage::getFirst() {
    return this->elements[0];
}

float MovingAverage::getLast() {
    return this->elements[this->nbElements-1];
}

float MovingAverage::addElement(int element) {

    if (this->filledElements < this->nbElements) {
        this->elements[this->filledElements] = element;
        this->filledElements++;
        this->index = (this->index + 1) % this->nbElements;
        this->total += element;
    }

    else {
        this->total -= this->elements[this->index];
        this->elements[this->index] = element;
        this->total += element;
        this->index = (this->index + 1) % this->nbElements;
    }

    return this->total/this->filledElements;
}
