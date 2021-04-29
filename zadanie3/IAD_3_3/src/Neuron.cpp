#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Neuron.h"

#define alpha 0.01   //momentum
#define eta   0.01   //wspolczynnik nauki

using namespace std;

Neuron::Neuron(int n){
    for(int i = 0; i < n; i++){
        weights.push_back((double)rand()/(double)RAND_MAX);
        deltaWeights.push_back(0);
    }
    bias = (double)rand()/(double)RAND_MAX;
    deltaBias = 0;
}

Neuron::~Neuron(){
}

vector<double> Neuron::getWeights(){
    return weights;
}

vector<double> Neuron::getDeltaWeights(){
    return deltaWeights;
}

double Neuron::getBias(){
    return bias;
}

double Neuron::getDeltaBias(){
    return deltaBias;
}

double Neuron::getOutput(){
    return output;
}

void Neuron::calcOutput(vector<double> inputs){
    output = 0;

    for(unsigned int i = 0; i < weights.size(); i++)
        output += weights[i] * inputs[i];
    output += bias;
}

void Neuron::updateWeights(vector<double> inputs, double d){
    for(unsigned int i=0; i<weights.size(); i++){
        double tmpWeight=weights[i];
        double newWeight = (-eta*inputs[i]*(output-d)+alpha*deltaWeights[i]);
        weights[i] += newWeight;
        deltaWeights[i] = weights[i] - tmpWeight;
    }

    double tmpBias = bias;
    double newBias = -eta*1.0*(output-d)+alpha*deltaBias;
    bias += newBias;
    deltaBias = tmpBias - bias;
}
