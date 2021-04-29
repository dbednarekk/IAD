#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <algorithm>
#include "Neuron.h"
#include "Gnuplot_i.hpp"
#define GNUPLOT_PATH "C:/gnuplot/bin"
using namespace std;
int main() {
    srand(time(NULL));
    double alfa= 0.6;                       //wspołczynnik nauki
    double beta=0.6;                           //momentum
    int liczba_neuronow;
    bool bias;
    cout << "Podaj Liczbe neuronow na warstwie ukrytej :";
    cin >> liczba_neuronow;
    cout << "Bias? 1->tak, 0->nie ";
    cin >> bias;
    vector<vector<double>> dane(4, vector<double> (4));
    ifstream wejscie;
    wejscie.open("../dane.txt");
    if(!wejscie.is_open()) cout<<"Nie udało się otworzyć pliku \n";
    else {
        for (int j = 0; j < 4; j++) {

            for (int i = 0; i < 4; i++) {
                wejscie >> dane[j][i];
                cout << dane[j][i] << " ";
            }
            cout<<endl;
        }
            cout << "Wczytano plik \n";

    }
    wejscie.close();

    vector<Neuron> warstwa_wyjsciowa(4);
    vector<Neuron> warstwa_ukryta(liczba_neuronow);
    for(int i=0;i<4;i++)                                // losowe wagi na początek dla kazdego neuronu
    {
        warstwa_wyjsciowa[i].randomWagi(liczba_neuronow, bias);
        if(i < liczba_neuronow)
            warstwa_ukryta[i].randomWagi(4, bias);
    }
    vector<double> bladGnuplot;
    vector<double> epokiGnuplot;
    vector<double> wyjscia;
    double bladEpoki;
    int epoka = 0;
    do
    {
        bladEpoki=0;
        double bladSr = 0;
        random_shuffle(dane.begin(), dane.end());             //przemieszanie danych
        for(int d=0; d < dane.size(); d++)
        {
            wyjscia.clear();

            for(int i=0; i < liczba_neuronow; i++)              //obliczenie funkcji aktywacji, czyli obliczenie wyjscia dla neuronów w warstwie ukrytej
            {
                warstwa_ukryta[i].funkcja_aktywacji(dane[d]);
                wyjscia.push_back(warstwa_ukryta[i].getWyjscie());
            }

            double b_wyjsciowe[4];
            for(int i=0; i<4; i++)                              //to samo dla warstwy wyjsciowej, z tym że dalej obliczamy B, której nam jest porzebne aby obliczyć delte i zmienic wagi
            {
                warstwa_wyjsciowa[i].funkcja_aktywacji(wyjscia);

                warstwa_wyjsciowa[i].ObliczB_wyj(warstwa_wyjsciowa[i].getWyjscie() - dane[d][i]);
                b_wyjsciowe[i]= warstwa_wyjsciowa[i].getB();
            }
            for(int i = 0; i < liczba_neuronow; i++)
            {
                double pom[4];
                for(int w=0; w<4; w++)                      // zapisujemy dla każdego neruonu w warswtie ukrytej wagi i B warstw wyjsciowych, dla 1 neuronu w warswtwie ukrytej zapisujemy wage 1 i B neuronu 1 z warstwy wyjsciowej, wage 1 i B 2 neuronu z warstwy wyjsciowej i tak dalej.
                {
                    pom[w] = warstwa_wyjsciowa[w].getWagi(i);
                }
                warstwa_ukryta[i].ObliczB_ukryte(b_wyjsciowe, pom);         // obliczamy B warstwy ukrytej dla neuronu, czyli sume B i wag neuronów z warstwy wyjsciowej
            }
            for(int i=0; i<4; i++)
                warstwa_wyjsciowa[i].zmienWagi(wyjscia, bias, alfa, beta);      //zmieniamy wagi, czyli w(t+1) = w(t) - alfa*delta + beta* delta*w(t-1
            for(int i=0; i < liczba_neuronow; i++)
                warstwa_ukryta[i].zmienWagi(dane[d], bias, alfa, beta);      //to samo
            wyjscia.clear();


            for(int i=0; i < liczba_neuronow; i++)                                  //obliczamy ponownie wyjscia z neuronow z poprawionymi wagami
            {
                warstwa_ukryta[i].funkcja_aktywacji(dane[d]);
                wyjscia.push_back(warstwa_ukryta[i].getWyjscie());
            }
            for(int i=0; i<4; i++)
            {
                warstwa_wyjsciowa[i].funkcja_aktywacji(wyjscia);

                bladSr+= ((warstwa_wyjsciowa[i].getWyjscie() - dane[d][i]) * (warstwa_wyjsciowa[i].getWyjscie() - dane[d][i])) / 2.0;   //obliczamy błąd sredniokwadratowy
            }
            bladSr/=4.0;
            bladEpoki += bladSr;
        }
        bladEpoki/=4.0;

        bladGnuplot.push_back(bladEpoki);
        epokiGnuplot.push_back(epoka);
        epoka++;
    }while(bladEpoki>=0.01 && epoka<10000); //Powtarzamy dopoki nie osiagniemy wystaczajaco niski blad


    //Ostateczne porownanie wynikow
    cout<<endl<<"Wynik: "<<endl;
    for(int j=0;j<4;j++)
    {
        wyjscia.clear();
        for(int i=0; i < liczba_neuronow; i++)
        {
            warstwa_ukryta[i].funkcja_aktywacji(dane[j]);
            wyjscia.push_back(warstwa_ukryta[i].getWyjscie());
        }
        for(int i=0;i<4;i++)
        {
            warstwa_wyjsciowa[i].funkcja_aktywacji(wyjscia);
            cout << warstwa_wyjsciowa[i].getWyjscie()<<" ";
        }

        cout<< '\n';
    }
    cout<<"Epoki: "<<epoka<<endl;
    /*  ofstream dane("../epoki.txt");
      for(int i=0;i<epokiGnuplot.size();i++)
      {
          dane<<epokiGnuplot[i]<<endl;
      }
      dane.close();*/
 /*   ofstream blady("../blad.txt");
    for(int i=0;i<2000;i++)
    {
        blady<<bladGnuplot[i]<<endl;
    }*/
    //Rysowanie
    Gnuplot::set_GNUPlotPath(GNUPLOT_PATH);
    Gnuplot main_plot;
    main_plot.set_title("Blad sredniokwadratowy");
    main_plot.set_xlabel("Epoka");
    main_plot.set_ylabel("Wartosc Bledu");
    main_plot.set_grid();
    main_plot.set_xrange(0, epoka);
    main_plot.set_yrange(0, 1);
    main_plot.set_style("lines");
    main_plot.plot_xy(epokiGnuplot, bladGnuplot, "blad");
    getchar();
    getchar();

    return 0;
}

