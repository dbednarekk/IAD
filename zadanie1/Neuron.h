#include <vector>
using namespace std;
class Neuron {
private:
    vector<double> wagi;
    vector<double> deltaWagi;
    double wyjscie;
    double bb;
    double biasWaga;
    double deltabiasWaga;
public:
    Neuron();

    void randomWagi(int ile_wejsc, bool bias);



    void ObliczB_wyj(double blad);

    void ObliczB_ukryte(double bledy[], double wagi[]);


    void zmienWagi(vector<double> wejscia, bool bias, double alfa, double beta);

    void funkcja_aktywacji(vector<double> wejscia);


    double getWagi(int n);

    double getWyjscie();

    double getB();
};