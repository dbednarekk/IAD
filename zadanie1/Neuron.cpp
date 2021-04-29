//
// Created by Damian on 17.04.2020.
//

#include <cstdlib>
#include <cmath>
#include "Neuron.h"

Neuron::Neuron() {}

void Neuron::randomWagi(int ile_wejsc, bool bias)
{
    for (int i=0; i<ile_wejsc; i++)
    {
        wagi.push_back(rand()/double(RAND_MAX));
        deltaWagi.push_back(wagi[i]);
    }
    if(bias)
        biasWaga=rand()/double(RAND_MAX);
    else
        biasWaga=0;
    deltabiasWaga=biasWaga;
}

void Neuron::ObliczB_wyj(double blad)
{
    bb = blad * (wyjscie*(1-wyjscie));
}
void Neuron::ObliczB_ukryte(double bledy[], double wagi[])
{
    double s = 0;
    for(int i=0; i<4; i++)
    {
        s += bledy[i]*wagi[i];
    }
    bb = s *(wyjscie*(1-wyjscie));
}
void Neuron::zmienWagi(vector<double> wejscia, bool bias, double alfa, double beta)
{
    for(unsigned int i=0; i<wagi.size(); i++)
    {
        double tmpWaga=wagi[i];
        double nowaDeltaWagi = (-alfa * wejscia[i] * bb + beta * deltaWagi[i]);
        wagi[i] += nowaDeltaWagi;
        deltaWagi[i]= wagi[i] - tmpWaga;
    }
    if(bias)
    {
        double tmpBiasWaga=biasWaga;
        double nowaDeltaWagi = -alfa * 1.0 * bb + beta * deltabiasWaga;
        biasWaga+=nowaDeltaWagi;
        deltabiasWaga= tmpBiasWaga - biasWaga;
    }
}
void Neuron::funkcja_aktywacji(vector<double> wejscia)
{
    double suma = 0;
    for (unsigned int i=0; i<wagi.size(); i++) {
        suma += wagi[i] * wejscia[i];
    }
    suma+=biasWaga;
    wyjscie= 1.0 / (1.0 + exp(-suma));
}
double Neuron::getWagi(int n)
{
    return wagi[n];
}
double Neuron::getWyjscie()
{
    return wyjscie;
}
double Neuron::getB()
{
    return bb;
}
