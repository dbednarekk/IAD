#include "RadialNeuron.h"
#include "headers.h"

#define eta   0.1   //wspolczynnik nauki

using namespace std;

RadialNeuron::RadialNeuron(double c){
    this->c = c;
    this->r = 1;
}

RadialNeuron::~RadialNeuron(){
}

double RadialNeuron::getC(){
    return c;
}

double RadialNeuron::getR(){
    return r;
}

double RadialNeuron::getOutput(){
    return output;
}

void RadialNeuron::setC(double c){
    this->c = c;
}

void RadialNeuron::setR(double r){
    this->r = r;
}

void RadialNeuron::updateR(RadialNeuron neighbour1, RadialNeuron neighbour2){
    r = 0;
    r += pow(distance(c , neighbour1.getC()), 2);
    r += pow(distance(c , neighbour2.getC()), 2);
    r /= 2;
    r = sqrt(r);
}

void RadialNeuron::updateR(RadialNeuron neighbour){
    r = 0;
    r += pow(distance(c , neighbour.getC()), 2);
    r = sqrt(r);
}

void RadialNeuron::calcOutput(double input){
        output = exp((-pow(distance(input, c), 2)) / (2*pow(r, 2)));
}

void RadialNeuron::updateC(double input){
    c += eta*(input - c);
}
