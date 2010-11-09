#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

class CircularBuffer
{
public:
    CircularBuffer();
    ~CircularBuffer();
    bool Put(float *buf, unsigned int size);
    bool Get(float *buf, unsigned int size);
    unsigned int filledSize;
private:
    unsigned int buffSize;
    float *filledStart;
    float *filledEnd;
    float *bufStart;
    float *bufEnd;
    float *buffer;

};

#endif // CIRCULARBUFFER_H
