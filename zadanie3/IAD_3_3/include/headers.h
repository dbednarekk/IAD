#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "conio.h"
#include "Point.h"
#include "Neuron.h"
#include "RadialNeuron.h"

std::vector<Point> readFile(std::string fileName);
double distance(double x1, double x2);
void createNet(std::vector<RadialNeuron> &hiddenLayer, std::vector<Neuron> &outputLayer, int n, std::vector <Point> points);
void updateRadiuses(std::vector<RadialNeuron> &hiddenLayer);
int findNeighbour(std::vector<RadialNeuron> &neurons, unsigned int neuronIndex, unsigned int neighbourIndex);
double netOutput(std::vector<RadialNeuron> &hiddenLayer, std::vector<Neuron> &outputLayer, double input);
void netUpdate(std::vector<RadialNeuron> &hiddenLayer, std::vector<Neuron> &outputLayer, double input, double d);
void makePlot(std::vector<Point> points1, std::vector<Point> points2);
