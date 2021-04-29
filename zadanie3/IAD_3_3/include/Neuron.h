#ifndef NEURON_H
#define NEURON_H

#include <vector>

class Neuron
{
private:
    std::vector<double> weights;
    std::vector<double> deltaWeights;
    double bias;
    double deltaBias;
    double output;
public:
    Neuron(int n);
    virtual ~Neuron();
    std::vector<double> getWeights();
    std::vector<double> getDeltaWeights();
    double getBias();
    double getDeltaBias();
    double getOutput();
    void calcOutput(std::vector<double> inputs);
    void updateWeights(std::vector<double> inputs, double d);
};

#endif // NEURON_H
