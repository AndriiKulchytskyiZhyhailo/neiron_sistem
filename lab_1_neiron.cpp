#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

void perceptron_initialization(double* w, int n_s, int n_i);
void perceptron_teaching(double* w, int n_s, int n_i, double* P, int* T, int n_0, double alfa, int hist);
void perceptron_verification(double* w, int n_s, int n_i, double* P, int* T);

int main()
{
    const int n_s = 2;            // number of neurons
    const int n_i = 2;            // number of inputs
    const int n_p = 3;            // number of parameters for neuron (n_i+1)
    const int n_0 = 40;           // number of points for teaching (navchannia): 20 or 40
    const int n_1 = 600;          // number of points for verification

    double x_min = -7;
    double x_max = 7;
    double y_min = -7;
    double y_max = 7;

    double P_0[n_0][n_i];         // points for teaching (navchannia)
    double P_1[n_1][n_i];         // points for verification

    ofstream file_1;
    ofstream file_2;
    ofstream file_3;
    ofstream file_4;
    ofstream file_5;


    file_1.open("point_teaching_40.txt");      
    file_2.open("point_veryfict_40.txt");      
    file_3.open("linie_neuron___40.txt");        
    file_4.open("paramtr_neuron_40.txt");   
    file_5.open("veryfct_neuron_40.txt"); 

    double w_th[n_s][n_p] = { {-2.5,1,1},{2.5,-1,2} };    // theoretical parameters (theoretical lines)

    double x;
    double y;
    int n_linia = 2;
    for (int i_s = 0; i_s < n_s; i_s++)
        for (int i_0 = 0; i_0 < n_linia; i_0++)
        {
            x = x_min + i_0 * (x_max - x_min) / (n_linia - 1);
            y = -w_th[i_s][0] / w_th[i_s][2] - w_th[i_s][1] * x / w_th[i_s][2];
            file_3 << setw(10) << x << setw(10) << y << endl;
        }
    file_3 << endl;

    cout << "theoretical parameters:" << endl;
    file_4 << "theoretical parameters:" << endl;
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        cout << "neuron nr " << setw(1) << i_s << ": ";
        cout << setw(10) << w_th[i_s][0] << setw(10) << w_th[i_s][1] << setw(10) << w_th[i_s][2] << endl;
        file_4 << "neuron nr " << setw(1) << i_s << ": ";
        file_4 << setw(10) << w_th[i_s][0] << setw(10) << w_th[i_s][1] << setw(10) << w_th[i_s][2] << endl;
    }
    cout << endl;
    file_4 << endl;  
    //                        forming and print (for file_1) points for teaching
    srand(121);
    //    srand(time(NULL));

    double D_MAX = 1;
    int i_cykl = 0;
    int j = 0;
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        int i = 0;
        while (i < n_0 / n_s)
        {
            x = x_min + (x_max - x_min) * double(rand()) / double(RAND_MAX);
            y = y_min + (y_max - y_min) * double(rand()) / double(RAND_MAX);
            double d_linia = (w_th[i_s][0] + w_th[i_s][1] * x + w_th[i_s][2] * y) /
                sqrt(w_th[i_s][1] * w_th[i_s][1] + w_th[i_s][2] * w_th[i_s][2]);
            if (fabs(d_linia) < D_MAX)
            {
                P_0[j][0] = x;
                P_0[j][1] = y;
                file_1 << setw(10) << P_0[j][0] << "  " << setw(10) << P_0[j][1] << endl;
                i++;
                j++;
            }
            i_cykl++;
            if (i_cykl > 32000)
            {
                cout << "ERROR FORMING POINTS FOR TEACHING. TREBA ZBILSZYTY D_MAX ABO ZMENSHYTY n_0" << endl;
                return -1;
            }
        }
    }
    file_1.close();
   
    //                        forming and print (for file_2) points for veryfication

    for (int i = 0; i < n_1; i++)
    {
        P_1[i][0] = x_min + (x_max - x_min) * double(rand()) / double(RAND_MAX);
        P_1[i][1] = y_min + (y_max - y_min) * double(rand()) / double(RAND_MAX);
        file_2 << setw(10) << P_1[i][0] << "  " << setw(10) << P_1[i][1] << endl;
    }
    file_2.close();
   
    //                        forming matrix of correct answers (for points for teaching and veryfication)

    int T_0[n_0][n_s];
    int T_1[n_1][n_s];

    double z;
    for (int i_s = 0; i_s < n_s; i_s++)
        for (int i_p = 0; i_p < n_0; i_p++)
        {
            z = w_th[i_s][0] + w_th[i_s][1] * P_0[i_p][0] + w_th[i_s][2] * P_0[i_p][1];
            if (z >= 0) T_0[i_p][i_s] = 1;
            else      T_0[i_p][i_s] = 0;
        }
    for (int i_s = 0; i_s < n_s; i_s++)
        for (int i_p = 0; i_p < n_1; i_p++)
        {
            z = w_th[i_s][0] + w_th[i_s][1] * P_1[i_p][0] + w_th[i_s][2] * P_1[i_p][1];
            if (z >= 0) T_1[i_p][i_s] = 1;
            else      T_1[i_p][i_s] = 0;
        }
 
    //         initiation neurons and print parameters and lines to monitors and file

    double w_nr[n_s][n_p];       // matrix for neurons parameters (zmishchennia i vahy)    
    perceptron_initialization(*w_nr, n_s, n_i);
    cout << "neurons parameters (initiation):" << endl;
    file_4 << "neurons parameters (initiation):" << endl;
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        cout << "neuron nr " << setw(1) << i_s << ": ";
        cout << setw(10) << w_nr[i_s][0] << setw(10) << w_nr[i_s][1] << setw(10) << w_nr[i_s][2] << endl;
        file_4 << "neuron nr " << setw(1) << i_s << ": ";
        file_4 << setw(10) << w_nr[i_s][0] << setw(10) << w_nr[i_s][1] << setw(10) << w_nr[i_s][2] << endl;
    }
    cout << endl;
    file_4 << endl;
    for (int i_s = 0; i_s < n_s; i_s++)
        for (int i_0 = 0; i_0 < n_linia; i_0++)
        {
            x = x_min + i_0 * (x_max - x_min) / (n_linia - 1);
            y = -w_nr[i_s][0] / w_nr[i_s][2] - w_nr[i_s][1] * x / w_nr[i_s][2];
            file_3 << setw(10) << x << setw(10) << y << endl;
        }
    file_3 << endl;
    cout << endl;
  
    //         teaching neurons and print parameters and lines to monitors and file

    perceptron_teaching(*w_nr, n_s, n_i, *P_0, *T_0, n_0, 0.4, 1);
    cout << endl << endl;
    cout << "neurons parameters (teacing):" << endl;
    file_4 << "neurons parameters (teaching):" << endl;
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        cout << "neuron nr " << setw(1) << i_s << ": ";
        cout << setw(10) << w_nr[i_s][0] << setw(10) << w_nr[i_s][1] << setw(10) << w_nr[i_s][2] << endl;
        file_4 << "neuron nr " << setw(1) << i_s << ": ";
        file_4 << setw(10) << w_nr[i_s][0] << setw(10) << w_nr[i_s][1] << setw(10) << w_nr[i_s][2] << endl;
    }
    cout << endl;
    file_4 << endl;


    for (int i_s = 0; i_s < n_s; i_s++)
        for (int i_0 = 0; i_0 < n_linia; i_0++)
        {
            x = x_min + i_0 * (x_max - x_min) / (n_linia - 1);
            y = -w_nr[i_s][0] / w_nr[i_s][2] - w_nr[i_s][1] * x / w_nr[i_s][2];
            file_3 << setw(10) << x << setw(10) << y << endl;
        }
    file_3 << endl;
    file_3.close();
    file_4.close();
 
    //         veryfication neurons and print veryfication

    int n_error[n_s];             // errors number of neurons                                                                                                  
    for (int i_s = 0; i_s < n_s; i_s++)
        n_error[i_s] = 0;

    file_5 << "                LISTING POINTS" << endl << endl;
    for (int i_1 = 0; i_1 < n_1; i_1++)
    {
        file_5 << "nr: " << setw(3) << i_1 + 1;
        double PP[n_i];         // point for veryfication
        int TT[n_s];            // neurons veryfication anwers 
        for (int i_i = 0; i_i < n_i; i_i++)
        {
            PP[i_i] = P_1[i_1][i_i];
            file_5 << " " << setw(10) << PP[i_i];           // print point for veryfication
        }
        perceptron_verification(*w_nr, n_s, n_i, PP, TT);    // forming neurons anwers

        file_5 << "   ";
        for (int i_s = 0; i_s < n_s; i_s++)
        {
            file_5 << setw(1) << TT[i_s];     // print neurons anwers
            if (TT[i_s] != T_1[i_1][i_s])
            {
                n_error[i_s]++;  
                file_5 << " ERROR ";
            }
            else
                file_5 << "       ";

        }
        file_5 << "  ";
        for (int i_s = 0; i_s < n_s; i_s++)
            file_5 << setw(1) << T_1[i_1][i_s] << "      ";   // print correct anwers      

        file_5 << endl;
    }
    file_5 << endl << endl;
    for (int i_s = 0; i_s < n_s; i_s++)
        file_5 << "n_error[" << setw(1) << i_s + 1 << "] = " << n_error[i_s] << endl;


    return 0;
}

void perceptron_initialization(double* w, int n_s, int n_i)   // initiation
{
    srand(121);
    //    srand(time(NULL));
    for (int i_s = 0; i_s < n_s; i_s++)
        for (int i_i = 0; i_i <= n_i; i_i++)
            w[i_s * (n_i + 1) + i_i] = -1 + 2 * double(rand()) / double(RAND_MAX);
}

void perceptron_teaching(double* w, int n_s, int n_i, double* P, int* T, int n_0, double alfa, int hist) // teaching
{
    int n_teach = 25;                        // max kilkist krokiv navchannia 
    for (int i_s = 0; i_s < n_s; i_s++)            
    {
        double alfa_0 = alfa;
        for (int i_teach = 0; i_teach < n_teach; i_teach++)   // fiksujemo nr kroku navchannia
        {
            int k_error = 0;                                
            for (int i_0 = 0; i_0 < n_0; i_0++)                   // fiksujemo tochku dla navchannia
            {
               
                /////     formujemo neuron answers
                          
                double z = w[i_s * (n_i + 1)];
                for (int i_i = 0; i_i < n_i; i_i++)
                    z += w[i_s * (n_i + 1) + i_i + 1] * P[i_0 * n_i + i_i];
                int f;
                if (z >= 0) f = 1;
                else      f = 0;
                
                ////      porivniujemo z correct answers : error - korekta neurons parameters 
                            
                if (f != T[i_0 * n_s + i_s])
                {
                    k_error++;
                    w[i_s * (n_i + 1)] += (T[i_0 * n_s + i_s] - f) * alfa_0;
                    for (int i_i = 0; i_i < n_i; i_i++)
                        w[i_s * (n_i + 1) + i_i + 1] += (T[i_0 * n_s + i_s] - f) * alfa_0 * P[i_0 * n_i + i_i];
                }
            }

            alfa_0 *= 0.8;     // korekta parametru alfa
            if (hist == 1)     
            {
                cout << "nejron nr" << setw(1) << i_s << ": " << "krok " << setw(2) << i_teach;
                cout << "  error " << setw(2) << k_error << endl;
            }
            if (k_error == 0) break;    
        }
        if (hist == 1) cout << endl;
    }
}

void perceptron_verification(double* w, int n_s, int n_i, double* P, int* T)  //veryfication
{
    for (int i_s = 0; i_s < n_s; i_s++)
    {
        double z = w[i_s * (n_i + 1)];
        for (int i_i = 0; i_i < n_i; i_i++)
            z += w[i_s * (n_i + 1) + i_i + 1] * P[i_i];
        if (z >= 0) T[i_s] = 1;
        else      T[i_s] = 0;
    }
}