#include "headers.h"
#include "gnuplot_i.hpp"
#define GNUPLOT_PATH "C:\\gnuplot\\bin"
using namespace std;

vector<Point> readFile(string fileName){
    vector<Point> points;
    ifstream file;
    file.open(fileName);
    if(!file.good()){
        return points;
    }
    while(!file.eof()){
        double x, y;
        file >> x >> y;
        points.push_back(Point(x, y));
    }
    file.close();
    return points;
}

double distance(double x1, double x2){
    return sqrt(pow((x1 - x2), 2));
}

void createNet(std::vector<RadialNeuron> &hiddenLayer, std::vector<Neuron> &outputLayer, int n, vector<Point> points){
    hiddenLayer.clear();
    outputLayer.clear();
    random_shuffle(points.begin(), points.end());
    for(int i = 0; i < n; i++){
        hiddenLayer.push_back(RadialNeuron(points[i%points.size()].getX()));
    }
    outputLayer.push_back(Neuron(n));
    updateRadiuses(hiddenLayer);
}

void updateRadiuses(std::vector<RadialNeuron> &hiddenLayer){
    if(hiddenLayer.size() >= 3){
        for(unsigned int i = 0; i < hiddenLayer.size(); i++){
            double neighbourIndex1 = findNeighbour(hiddenLayer, i, i);
            double neighbourIndex2 = findNeighbour(hiddenLayer, i, neighbourIndex1);
            hiddenLayer[i].updateR(hiddenLayer[neighbourIndex1], hiddenLayer[neighbourIndex2]);
        }
    }

    else if(hiddenLayer.size() >= 2){
        for(unsigned int i = 0; i < hiddenLayer.size(); i++){
            double neighbourIndex = findNeighbour(hiddenLayer, i, i);
            hiddenLayer[i].updateR(hiddenLayer[neighbourIndex]);
        }
    }
}

int findNeighbour(vector<RadialNeuron> &neurons, unsigned int neuronIndex, unsigned int neighbourIndex){
    unsigned int index=0;
    while(index == neuronIndex || index == neighbourIndex) index++;
    double minimum = distance(neurons[neuronIndex].getC() , neurons[index].getC());
    for (unsigned int i = 0; i < neurons.size(); i++){
        if ((distance(neurons[neuronIndex].getC() , neurons[i].getC()) < minimum) && (i != neuronIndex || i != neighbourIndex)){
            minimum = distance(neurons[neuronIndex].getC() , neurons[i].getC());
            index = i;
        }
    }
    return index;
}

int findMinDistance(double x, vector<RadialNeuron> &neurons){
    int index=0;
    double minimum = distance(x, neurons[0].getC());
    for (unsigned int i = 0; i < neurons.size(); i++){
        if ((distance(x, neurons[i].getC()) < minimum)){
            minimum = distance(x, neurons[i].getC());
            index = i;
        }
    }
    return index;
}

void netUpdate(vector<RadialNeuron> &hiddenLayer, vector<Neuron> &outputLayer, double input, double d){
    vector<double> hiddenLayerOutputs;
    int minDistanceIndex = findMinDistance(input, hiddenLayer);
    for(unsigned int i = 0; i < hiddenLayer.size(); i++){
        hiddenLayer[i].calcOutput(input);
        hiddenLayerOutputs.push_back(hiddenLayer[i].getOutput());
    }

    hiddenLayer[minDistanceIndex].updateC(input);
    if(hiddenLayer.size() >= 2){
        double neighbourIndex1 = findNeighbour(hiddenLayer, minDistanceIndex, minDistanceIndex);
        hiddenLayer[neighbourIndex1].updateC(input);
        if(hiddenLayer.size() >= 2){
            double neighbourIndex2 = findNeighbour(hiddenLayer, minDistanceIndex, neighbourIndex1);
            hiddenLayer[neighbourIndex2].updateC(input);
        }
    }

    updateRadiuses(hiddenLayer);

    outputLayer[0].calcOutput(hiddenLayerOutputs);
    outputLayer[0].updateWeights(hiddenLayerOutputs, d);
}

double netOutput(vector<RadialNeuron> &hiddenLayer, vector<Neuron> &outputLayer, double input){
    vector<double> hiddenLayerOutputs;
    for(unsigned int i = 0; i < hiddenLayer.size(); i++){
        hiddenLayer[i].calcOutput(input);
        hiddenLayerOutputs.push_back(hiddenLayer[i].getOutput());
    }

    outputLayer[0].calcOutput(hiddenLayerOutputs);
    return outputLayer[0].getOutput();
}

void makePlot(vector<Point> points1, vector<Point> points2){
    Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
    Gnuplot main_plot;
    main_plot.reset_all();
    main_plot.set_title( "wykres" );
    main_plot.set_xlabel( "Os x" );
    main_plot.set_ylabel( "Os y" );
    main_plot.set_style("points");
    main_plot.set_pointsize(2.0);

    vector<double> osX1, osY1, osX2, osY2;

    for(unsigned int i=0; i < points1.size(); i++){
        osX1.push_back(points1[i].getX());
        osY1.push_back(points1[i].getY());
    }
    main_plot.plot_xy(osX1, osY1, "punkty testowe");

    for(unsigned int i=0; i < points2.size(); i++){
        osX2.push_back(points2[i].getX());
        osY2.push_back(points2[i].getY());
    }
    main_plot.plot_xy(osX2, osY2, "punkty wyznaczone");

    getch();
}
