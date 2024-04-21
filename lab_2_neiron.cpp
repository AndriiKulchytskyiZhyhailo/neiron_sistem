#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

int merezha_cheminga(int* e, int* x, int n_s, int n_i, double* y, double E_max, int max_iteracji, int hist);
void f_compatibility(int* x, int* e, int n_i, int n_s, int* y);

int main()
{
    const int n_s = 2;            // number of neurons
    const int n_i = 16;           // number of inputs
    const int n_0 = 14;           // number of sygnal

    int e_s[n_s][n_i] = { {-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1},
                          { 1,-1,-1, 1, 1, 1,-1, 1, 1,-1, 1, 1, 1,-1,-1, 1} };

    int x_s[n_0][n_i] = { {-1, 1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1},
                          { 1,-1,-1, 1, 1, 1,-1, 1, 1,-1, 1, 1, 1,-1,-1, 1},
                          { 1,-1, 1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1, 1, 1,-1},
                          {-1, 1,-1, 1, 1, 1,-1, 1, 1,-1, 1, 1, 1,-1,-1, 1},
                          { 1, 1, 1, 1,-1,-1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1},
                          {-1, 1, 1, 1, 1, 1,-1, 1, 1,-1, 1, 1,-1, 1, 1,-1},
                          {-1, 1,-1,-1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1, 1, 1},
                          { 1,-1, 1,-1, 1, 1,-1, 1,-1,-1,-1, 1,-1, 1, 1, 1},
                          { 1, 1, 1,-1, 1, 1, 1, 1, 1,-1, 1, 1, 1,-1, 1, 1},
                          {-1,-1,-1, 1, 1, 1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1},
                          {-1, 1, 1, 1, 1, 1,-1, 1, 1,-1, 1, 1, 1,-1,-1, 1},
                          { 1,-1,-1,-1, 1,-1,-1, 1, 1,-1,-1, 1,-1,-1,-1,-1},
                          {-1,-1,-1,-1, 1,-1,-1, 1, 1,-1, 1,-1, 1,-1, 1,-1},
                          { 1,-1, 1, 1,-1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1,-1} };

    int* x = new int[n_i];
    int compability[n_0][n_s];
    int* y_comp = new int[n_s];
    double* y_m = new double[n_s];

    int i_0;
    for (i_0 = 0; i_0 < n_0; i_0++)
    {
        for (int i_i = 0; i_i < n_i; i_i++)
            x[i_i] = x_s[i_0][i_i];
        f_compatibility(x, *e_s, n_i, n_s, y_comp);
        for (int i_s = 0; i_s < n_s; i_s++)
            compability[i_0][i_s] = y_comp[i_s];
    }

    ofstream file;
    file.open("merezha_cheminga.txt");      // points for testing (testuvannia)
    file << "ETALONNI VEKTORY e:" << endl << endl;
    cout << "ETALONNI VEKTORY e:" << endl << endl;
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        file << "nr " << setw(1) << i_s + 1 << " : ";
        cout << "nr " << setw(1) << i_s + 1 << " : ";
        for (int i_i = 0; i_i < n_i; i_i++)
        {
            file << setw(2) << e_s[i_s][i_i] << " ";
            cout << setw(2) << e_s[i_s][i_i] << " ";
        }
        file << endl;
        cout << endl;
    }
    file << endl << endl << "VYSLANI SYHNALY x:" << endl << endl;
    cout << endl << endl << "VYSLANI SYHNALY x:" << endl << endl;
    for (int i_0 = 0; i_0 < n_0; i_0++)
    {
        file << "nr " << setw(2) << i_0 + 1 << " : ";
        cout << "nr " << setw(2) << i_0 + 1 << " : ";
        for (int i_i = 0; i_i < n_i; i_i++)
        {
            file << setw(2) << x_s[i_0][i_i] << " ";
            cout << setw(2) << x_s[i_0][i_i] << " ";
        }
        file << "   comp: ";
        cout << "   comp: ";
        for (int i_s = 0; i_s < n_s; i_s++)
        {
            file << setw(2) << compability[i_0][i_s] << "  ";
            cout << setw(2) << compability[i_0][i_s] << "  ";
        }
        file << endl;
        cout << endl;
    }

    cout << endl << endl << "VIDPOVIDI MEREZHI" << endl << endl;
    file << endl << endl << "VIDPOVIDI MEREZHI" << endl << endl;
    cout << "LEHKO PEREDBACHUVANI VYPADKY" << endl << endl;
    file << "LEHKO PEREDBACHUVANI VYPADKY" << endl << endl;

    for (int i_0 = 0; i_0 < n_0; i_0++)
    {
        if (i_0 == 5)
        {
            cout << endl << endl << "VYPADKY DO TESTUVANNIA: COMP > T" << endl << endl;
            file << endl << endl << "VYPADKY DO TESTUVANNIA: COMP > T" << endl << endl;
        }
        for (int i_i = 0; i_i < n_i; i_i++)
            x[i_i] = x_s[i_0][i_i];
        int iter = merezha_cheminga(*e_s, x, n_s, n_i, y_m, 0.0001, 1000, 0);

        cout << "x nr " << setw(2) << i_0 + 1 << ": ";
        file << "x nr " << setw(2) << i_0 + 1 << ": ";

        cout << " comp: ";
        file << " comp: ";
        for (int i_s = 0; i_s < n_s; i_s++)
        {
            cout << setw(2) << compability[i_0][i_s] << "  ";
            file << setw(2) << compability[i_0][i_s] << "  ";
        }
        for (int i_s = 0; i_s < n_s; i_s++)
        {
            cout << "   nejr " << setw(1) << i_s + 1 << ": " << setw(8) << setprecision(4) << y_m[i_s];
            file << "   nejr " << setw(1) << i_s + 1 << ": " << setw(8) << setprecision(4) << y_m[i_s];
        }
        if (iter <= 1000)
        {
            cout << "   itr= " << setw(3) << iter << endl;
            file << "   itr= " << setw(3) << iter << endl;
        }
        else
        {
            cout << "   NEMA STABILIZACII" << endl;
            file << "   NEMA STABILIZACII" << endl;
        }
    }
    file.close();
    return 0;
}

double f_aktyvacii(double s, double T)
{
    if (s < 0) return 0;
    if (s > T) return T;
    return s;
}

double norma(double* y1, double* y2, int n)
{
    double norm = 0;
    for (int i = 0; i < n; i++)
        norm += (y1[i] - y2[i]) * (y1[i] - y2[i]);
    return sqrt(norm);
}

int merezha_cheminga(int* e, int* x, int n_s, int n_i, double* y, double E_max, int max_iteracji, int hist)
{
    double eps = 1. / double(n_i);
    double T = n_i / 2.;
    double* y1 = new double[n_s];
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        y1[i_s] = 0;
        for (int i_i = 0; i_i < n_i; i_i++)
            y1[i_s] += e[i_s * n_i + i_i] * x[i_i];
        y1[i_s] = f_aktyvacii(y1[i_s] / 2., T);
    }
    int i;
    for (i = 0; i < max_iteracji; i++)
    {
        for (int i_s = 0; i_s < n_s; i_s++)
        {
            y[i_s] = y1[i_s];
            for (int j_s = 0; j_s < n_s; j_s++)
                if (j_s != i_s)  y[i_s] -= eps * y1[j_s];
            y[i_s] = f_aktyvacii(y[i_s], T);
        }
        double norm = norma(y, y1, n_s);
        if (hist == 1)
            cout << "nr iteracji = " << setw(3) << i << "  norma = " << setw(10) << norm << endl;
        if (norm < E_max) break;
        for (int i_s = 0; i_s < n_s; i_s++)
            y1[i_s] = y[i_s];
    }
    delete[] y1;
    return i + 1;
}

void f_compatibility(int* x, int* e, int n_i, int n_s, int* y)
{
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        y[i_s] = 0;
        for (int i_i = 0; i_i < n_i; i_i++)
            if (x[i_i] == e[i_s * n_i + i_i])  y[i_s]++;
    }
}