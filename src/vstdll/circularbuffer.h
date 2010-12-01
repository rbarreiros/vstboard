#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

class CircularBuffer
{
public:
    CircularBuffer();
    ~CircularBuffer();
    void Clear();
    void SetSize(unsigned int size);
    bool Put(float *buf, unsigned int size);
    bool Get(float *buf, unsigned int size);
    bool Skip(unsigned int size);
//    bool Keep(unsigned int size);
    unsigned int buffSize;
    unsigned int filledSize;
private:

    float *filledStart;
    float *filledEnd;
    float *bufStart;
    float *bufEnd;
    float *buffer;

};

#endif // CIRCULARBUFFER_H
