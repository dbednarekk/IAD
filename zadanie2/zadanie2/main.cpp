#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <algorithm>
#include "gnuplot_i.hpp"

#define GNUPLOT_PATH "C:\\gnuplot\\bin"

#define lambda_0    0.5
#define r_0         0.5
#define lambda_min  0.01
#define eta_min     0.01
double r = r_0;
double lambda = lambda_0;

using namespace std;
double obliczDystans(vector<double> punkt1, vector<double> punkt2);

class Neuron{
private:
    vector<double> wagiP;
    int wygraneP;
    double eta_0;
    double eta;
public:
    Neuron(){
        for (int i=0; i<2; i++)
            wagiP.push_back(rand()%21 - 10);
        wygraneP=1;
        do{
            eta_0 =1 - rand()/double(RAND_MAX);
        }while(eta_0 == 0);
        eta = eta_0;
    }

    //gettery
    double getWagiP(int index){
        return wagiP[index];
    }
    vector<double> getWagMP(){
        return wagiP;
    }
    int getWygraneP(){
        return wygraneP;
    }
    double getEta(){
        return eta;
    }
    void zwiekszWygrane(){
        wygraneP++;
    }
    void aktualizacjaWagKoh(double d, vector<double> punktP){
        double G;
        if(r!=0) G=exp(-d*d/(2*r*r));
        else G=1;
        for(unsigned int i=0; i<wagiP.size(); i++){
            wagiP[i] = wagiP[i] + eta * G * (punktP[i]-wagiP[i]);
        }
    }
    void aktualizacjaWagNeurGas(int m, vector<double> punktP){
        double G;
        G=exp(-(double)m/lambda);
        for(unsigned int i=0; i<wagiP.size(); i++){
            wagiP[i] = wagiP[i] + eta * exp(-(double)m/lambda) * (punktP[i]-wagiP[i]);
        }
    }
    void aktualizacjaEta(int iter, int maxIter){
        eta = eta_0 * pow((eta_min/eta_0),((double)iter/(double)maxIter));
    }
};
class Neuron_K{
private:
    vector<double> waga;
    vector<double> deltaWagi;
    double dystans;
    vector<vector<double>>najblize;
public:
    Neuron_K(vector<double> punkty){
         waga.push_back(punkty[0]);
         waga.push_back(punkty[1]);
    }
    void setNajblize(vector<double> punkt){
        najblize.push_back(punkt);
    }
    void przesunNeutron(){
        double deltaX=0;
        double deltaY=0;
        if(najblize.size()>0){
        for(int i=0;i<najblize.size();i++){
            deltaX+=najblize[i][0];
            deltaY+=najblize[i][1];
        }
        deltaX= deltaX/najblize.size();
        deltaY= deltaY/najblize.size();
           deltaWagi.clear();
        deltaWagi.push_back(deltaX);
        deltaWagi.push_back(deltaY);
        dystans = obliczDystans(waga,deltaWagi);

        setX(deltaX);
        setY(deltaY);
        }

    }
    double obliczErr(){
        double err=0;
        if(najblize.size()>0){
        for(int i =0;i<najblize.size();i++){
            err+=obliczDystans(najblize[i],getWagi());
        }
        err= err/najblize.size();
        return err;
        }
        else return 0;
    }
    //gettery
    double getX(){
        return waga[0];
    }
    double getY(){
        return waga[1];
    }
    vector<double> getWagi(){
        return waga;
    }
    vector<vector<double>> getNajblizsze(){
        return najblize;
    }
    vector<double> getDeltaWaga(){
        return deltaWagi;
    }
    double getDystans(){
        return dystans;
    }
    //setery
    void setX(double value){
        waga[0]=value;
    }
    void setY(double value){
        waga[1]=value;
    }
};

vector<vector<double>> wczytajDane(string nazwaPliku);
void wykresOstateczny(vector<vector<double>> punkty, vector<Neuron_K> &neurony);
int znajdzNajblizszyNeuron(vector<double> punkt, vector<Neuron_K> &neurony);
void tworzWykres(vector<vector<double>> punkty, vector<Neuron_K> &neurony);
//funkcje
vector<vector<double>> wczytajPlik(string fileName);
vector<Neuron> tworzNeutronP(int n);
void ostatecznyWykresTworzenieP(vector<vector<double>> punktyP, vector<Neuron> &neuronyP);
void wykresTworz(vector<vector<double>> punktyP, vector<Neuron> &neuronyP);
double dystansObliczenie(vector<double> punktP1, vector<double> punktP2);
double dystansObliczenieWygrany(vector<double> punktP1, Neuron neuron);
int szuajMinDystans(vector<double> punktP, vector<Neuron> &neuronyP);
int szuajMinDystansWygranko(vector<double> punktP, vector<Neuron> &neuronyP);
void AktualizujPromien(int iter, int maxIter);
void AktualizujEtas(int iter, int maxIter, vector<Neuron> &neuronyP);
void AktualizujLambda(int iter, int maxIter);
void sortowanieNeuronow(vector<Neuron> &neuronyP, vector<double> punktP);

class SortNeuronsByDistance{
    public:
    SortNeuronsByDistance(vector<double> punktP): punktP(punktP){}
    vector<double> punktP;
    bool operator () (Neuron neuron1, Neuron neuron2) {return dystansObliczenieWygrany(punktP, neuron1) <= dystansObliczenieWygrany(punktP, neuron2);}
};

int main(){
    srand(time(NULL));
    vector<vector<double>> punktyP = wczytajPlik("data.txt");
    vector<vector<double>> punkty = wczytajDane("data.txt");
    if (punktyP.empty()){ cout<<"Blad w poborze danych"<<endl;  return 0;}
	if (punkty.empty()){ cout<<"Blad w poborze danych"<<endl;  return 0;}
    int liczbaNeuronow;
    cout<<"Podaj liczbe neuronow: "<<endl;
	cin>>liczbaNeuronow;
    if(liczbaNeuronow <= 0) { cout<<"Bledna liczba neuronow\n"; return 0;}
    vector<Neuron> neuronyP = tworzNeutronP(liczbaNeuronow);
    int tryb;
    cout << "Wybierz rodzaj: "<<endl<<"[1]-Algorytm Kohonena"<<endl<<"[2]-Algorytm gazu neuronowego"<<endl<<"[3]-Algorytm k-srednie"<<endl<<"Twoj wybor: ";
    cin >> tryb;
    if(tryb!=1 && tryb!=2 && tryb!=3) { cout<<"Bledna opcja\n";    return 0;}

    int iter=0;
    double dystansP;
    double err=100;
    if(tryb == 1){
        int maxIter;
        cout << "Podaj maksymalna liczbe iteracji: "; cin >> maxIter;
        if(maxIter <= 0) { cout<<"Bledna ilosc iteracji\n";    return 0;}
        int minimumDistanceIndex;
        while(iter < maxIter){
            random_shuffle(punktyP.begin(), punktyP.end());
            iter++;
           // wykresTworz(punktyP, neuronyP);
            err = 0;
            for (unsigned int i = 0; i < punktyP.size(); i++){
                minimumDistanceIndex = szuajMinDystansWygranko(punktyP[i], neuronyP);
                dystansP = dystansObliczenie(punktyP[i], neuronyP[minimumDistanceIndex].getWagMP());
                err += dystansP * dystansP;
                for(unsigned int j = 0; j < neuronyP.size(); j++ ){
                    dystansP = dystansObliczenie(neuronyP[j].getWagMP(),neuronyP[minimumDistanceIndex].getWagMP());
                    if(dystansP <= r)
                        neuronyP[j].aktualizacjaWagKoh(dystansP, punktyP[i]);
                }
            }
            AktualizujEtas(iter, maxIter, neuronyP);
            AktualizujPromien(iter, maxIter);
            err /= punktyP.size();
        }
	ostatecznyWykresTworzenieP(punktyP, neuronyP);
    cout<<"Zakonczono po: "<<iter<<endl;
    cout<<"Blad kwantyzacji: "<<err<<endl;
    }
    else if(tryb==2){
    int maxIter;
    cout << "Podaj maksymalna liczbe iteracji: "; cin >> maxIter;
    if(maxIter <= 0) { cout<<"Bledna ilosc iteracji\n";    return 0;}
        while(iter < maxIter){
            random_shuffle(punktyP.begin(), punktyP.end());
            iter++;
            //wykresTworz(punktyP, neuronyP);
            err = 0;
            for (unsigned int i = 0; i < punktyP.size(); i++){
                sortowanieNeuronow(neuronyP, punktyP[i]);
                neuronyP[0].zwiekszWygrane();
                dystansP = dystansObliczenie(punktyP[i], neuronyP[0].getWagMP());
                err += dystansP * dystansP;
                for(unsigned int j = 0; j < neuronyP.size(); j++ ){
                        neuronyP[j].aktualizacjaWagNeurGas(j, punktyP[i]);
                }
            }
            AktualizujEtas(iter, maxIter, neuronyP);
            AktualizujLambda(iter, maxIter);
            err /= punktyP.size();
        }
	ostatecznyWykresTworzenieP(punktyP, neuronyP);
    cout<<"Zakonczono po: "<<iter<<endl;
    cout<<"Blad kwantyzacji: "<<err<<endl;
    }
     else if(tryb==3){
    double dystans;
    double bladKwan=100;
    double iteracje=0;
     vector<Neuron_K> neuronyK;
     random_shuffle(punkty.begin(), punkty.end());
    for(int i=0;i<liczbaNeuronow;i++){
        Neuron_K neuronK = Neuron_K(punkty[i]);
        neuronyK.push_back(neuronK);
    }

    double stabilizacja;

        do{

            random_shuffle(punkty.begin(), punkty.end());
            //tworzWykres(punkty, neuronyK);
            bladKwan=0;
            for(int i=0;i<punkty.size();i++){
               int index= znajdzNajblizszyNeuron(punkty[i],neuronyK);
               neuronyK[index].setNajblize(punkty[i]);

            }
            for(int j=0;j<neuronyK.size();j++){
                 neuronyK[j].przesunNeutron();
                stabilizacja+= neuronyK[j].getDystans();
            }
             for(int i=0;i<neuronyK.size();i++){
                bladKwan+= neuronyK[i].obliczErr();
            }

            bladKwan= bladKwan/neuronyK.size();
            stabilizacja= stabilizacja/neuronyK.size();
            iteracje++;
    }while((stabilizacja>0.01) );
        wykresOstateczny(punkty, neuronyK);
        cout<<"zakonczono po "<<iteracje<<" iteracjach"<<endl;
        cout<<"bladKwantyzacji: "<<bladKwan<<endl;

     }
    return 0;
}
vector<vector<double>> wczytajPlik(string fileName){
    vector<vector<double> > allInputs;
    vector<double> input;
    ifstream file;
    file.open("data.txt");
    if(!file.good()){return allInputs;}
    double value;
    string s_value;
    while(!file.eof()){
        getline(file,s_value,',');
        value = atof(s_value.c_str());
        input.push_back(value);
        getline(file,s_value);
        value = atof(s_value.c_str());
        input.push_back(value);
        allInputs.push_back(input);
        input.clear();
    }
    file.close();
    return allInputs;
}
vector<Neuron> tworzNeutronP(int n){
    vector<Neuron> neuronyP;
    while(n){
        Neuron neuron;
        neuronyP.push_back(neuron);
        n--;
    }
    return neuronyP;
}
void wykresTworz(vector<vector<double>> punktyP, vector<Neuron> &neuronyP){
    Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
    Gnuplot main_plot;
    main_plot.reset_all();
    main_plot.set_title( "wykres" );
    main_plot.set_xlabel( "Os x" );
    main_plot.set_ylabel( "Os y" );
    main_plot.set_xrange( -11, 13 );
    main_plot.set_yrange( -13, 11 );
    main_plot.set_grid();
    main_plot.set_style("dots");

    vector<double> osX;
    vector<double> osY;
    vector<double> neuronsX;
    vector<double> neuronsY;

    for(unsigned int i=0; i < punktyP.size(); i++){
        osX.push_back(punktyP[i][0]);
        osY.push_back(punktyP[i][1]);
    }
    for(unsigned int i=0; i < neuronyP.size(); i++){
        neuronsX.push_back(neuronyP[i].getWagiP(0));
        neuronsY.push_back(neuronyP[i].getWagiP(1));
    }
    main_plot.plot_xy(osX,osY);
    main_plot.set_style("points");
    main_plot.set_pointsize(2.0);
    main_plot.plot_xy(neuronsX,neuronsY);
    Sleep(1000);
    main_plot.remove_tmpfiles();
}
void ostatecznyWykresTworzenieP(vector<vector<double>> punktyP, vector<Neuron> &neuronyP){
    Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
    Gnuplot main_plot;
    main_plot.reset_all();
    main_plot.set_title( "wykres" );
    main_plot.set_xlabel( "Os x" );
    main_plot.set_ylabel( "Os y" );
    main_plot.set_xrange( -11, 13 );
    main_plot.set_yrange( -13, 11 );
    main_plot.set_grid();
    main_plot.set_style("dots");

    vector<double> osX;
    vector<double> osY;
    vector<double> neuronsX;
    vector<double> neuronsY;

    for(unsigned int i=0; i < punktyP.size(); i++){
        osX.push_back(punktyP[i][0]);
        osY.push_back(punktyP[i][1]);
    }
    for(unsigned int i=0; i < neuronyP.size(); i++){
        neuronsX.push_back(neuronyP[i].getWagiP(0));
        neuronsY.push_back(neuronyP[i].getWagiP(1));
    }
    main_plot.plot_xy(osX,osY);
    main_plot.set_style("points");
    main_plot.set_pointsize(2.0);
    main_plot.plot_xy(neuronsX,neuronsY);
    getch();
}
double dystansObliczenie(vector<double> punktP1, vector<double> punktP2){
    return sqrt(pow((punktP1[0] - punktP2[0]), 2) + pow((punktP1[1] - punktP2[1]), 2));
}
double dystansObliczenieWygrany(vector<double> punktP1, Neuron neuron){
    return sqrt(pow((punktP1[0] - neuron.getWagiP(0)), 2) + pow((punktP1[1] - neuron.getWagiP(1)), 2)) * neuron.getWygraneP();
}
int szuajMinDystans(vector<double> punktP, vector<Neuron> &neuronyP){
    int index=0;
    double minimum = dystansObliczenie(punktP, neuronyP[0].getWagMP());
    for (unsigned int i = 0; i < neuronyP.size(); i++){
        if ((dystansObliczenie(punktP, neuronyP[i].getWagMP()) < minimum)){
            minimum = dystansObliczenie(punktP, neuronyP[i].getWagMP());
            index = i;
        }
    }
    return index;
}
int szuajMinDystansWygranko(vector<double> punktP, vector<Neuron> &neuronyP){
    int index=0;
    double dystansP = dystansObliczenieWygrany(punktP, neuronyP[0]);
    double minimum = dystansP;
    for (unsigned int i = 0; i < neuronyP.size(); i++){
            dystansP = dystansObliczenieWygrany(punktP, neuronyP[i]);
        if (dystansP < minimum){
            minimum = dystansP;
            index = i;
        }
    }
    neuronyP[index].zwiekszWygrane();
    return index;
}
void AktualizujPromien(int iter, int maxIter){
        r = r_0 - (r_0/maxIter) * iter;
}
void AktualizujEtas(int iter, int maxIter, vector<Neuron> &neuronyP){
    for(unsigned int i=0; i< neuronyP.size(); i++){
            neuronyP[i].aktualizacjaEta(iter, maxIter);
    }
}
void AktualizujLambda(int iter, int maxIter){
    lambda = lambda_0 * (lambda_min/lambda_0),((double)iter/(double)maxIter);
}
void sortowanieNeuronow(vector<Neuron> &neuronyP, vector<double> punktP){
    sort(neuronyP.begin(), neuronyP.end(), SortNeuronsByDistance(punktP));
}




void wykresOstateczny(vector<vector<double>> punkty, vector<Neuron_K> &neurony){
    Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
    Gnuplot main_plot;
    main_plot.reset_all();
    main_plot.set_title( "wykres" );
    main_plot.set_xlabel( "Os x" );
    main_plot.set_ylabel( "Os y" );
    main_plot.set_xrange( -11, 13 );
    main_plot.set_yrange( -13, 11 );
    main_plot.set_grid();
    main_plot.set_style("dots");

    vector<double> osX;
    vector<double> osY;
    vector<double> neuronsX;
    vector<double> neuronsY;

    for(unsigned int i=0; i < punkty.size(); i++){
        osX.push_back(punkty[i][0]);
        osY.push_back(punkty[i][1]);
    }
    for(unsigned int i=0; i < neurony.size(); i++){
        neuronsX.push_back(neurony[i].getX());
        neuronsY.push_back(neurony[i].getY());
    }
    main_plot.plot_xy(osX,osY);
    main_plot.set_style("points");
    main_plot.set_pointsize(2.0);
    main_plot.plot_xy(neuronsX,neuronsY);
    getch();
}
vector<vector<double>> wczytajDane(string nazwaPliku){
    vector<vector<double> > allInputs;
    vector<double> input;
    ifstream plik;
    plik.open("data.txt");
    if(!plik.good()){return allInputs;}
    double value;
    string s_value;
    while(!plik.eof()){
        getline(plik,s_value,',');
        value = atof(s_value.c_str());
        input.push_back(value);
        getline(plik,s_value);
        value = atof(s_value.c_str());
        input.push_back(value);
        allInputs.push_back(input);
        input.clear();
    }
    plik.close();
    return allInputs;
}
double obliczDystans(vector<double> punkt1, vector<double> punkt2){
    return sqrt(pow((punkt1[0] - punkt2[0]), 2) + pow((punkt1[1] - punkt2[1]), 2));
}
int znajdzNajblizszyNeuron(vector<double> punkt, vector<Neuron_K> &neurony){
    int index=0;
    double minimum = obliczDystans(punkt, neurony[0].getWagi());
    for (unsigned int i = 0; i < neurony.size(); i++){
        if ((obliczDystans(punkt, neurony[i].getWagi()) < minimum)){
            minimum = obliczDystans(punkt, neurony[i].getWagi());
            index = i;
        }
    }
    return index;
}
void tworzWykres(vector<vector<double>> punkty, vector<Neuron_K> &neurony){
    Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
    Gnuplot main_plot;
    main_plot.reset_all();
    main_plot.set_title( "wykres" );
    main_plot.set_xlabel( "Os x" );
    main_plot.set_ylabel( "Os y" );
    main_plot.set_xrange( -11, 13 );
    main_plot.set_yrange( -13, 11 );
    main_plot.set_grid();
    main_plot.set_style("dots");

    vector<double> osX;
    vector<double> osY;
    vector<double> neuronsX;
    vector<double> neuronsY;

    for(unsigned int i=0; i < punkty.size(); i++){
        osX.push_back(punkty[i][0]);
        osY.push_back(punkty[i][1]);
    }
    for(unsigned int i=0; i < neurony.size(); i++){
        neuronsX.push_back(neurony[i].getX());
        neuronsY.push_back(neurony[i].getY());
    }
    main_plot.plot_xy(osX,osY);
    main_plot.set_style("points");
    main_plot.set_pointsize(2.0);
    main_plot.plot_xy(neuronsX,neuronsY);
    Sleep(1000);
    getch();
    main_plot.remove_tmpfiles();
}
