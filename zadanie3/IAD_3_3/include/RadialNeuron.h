#ifndef RADIALNEURON_H
#define RADIALNEURON_H

#include <vector>

class RadialNeuron
{
private:
    double c;
    double r;
    double output;
public:
    RadialNeuron(double c);
    virtual ~RadialNeuron();
    double getC();
    double getR();
    double getOutput();
    void setC(double c);
    void setR(double r);
    void updateR(RadialNeuron neighbour1, RadialNeuron neighbour2);
    void updateR(RadialNeuron neighbour);
    void updateC(double input);
    void calcOutput(double inputs);
};

#endif // RADIALNEURON_H
