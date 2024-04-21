#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

void ident_null(double* point, double* center, int N, int n, int k, int* point_nr_center);
int iden_center(double* point, double* center, int* point_nr_center, int N, int k, int n);
int identyfikacia(double* point_0, double* center, int k, int n);
void f_nm_point_center(int* point_nr_center, int N, int k, int* nr_point_center);
void mod_null(double* point, double* center, int N, int n, int k, int* point_nr_center, int* nr_point_center);
int read_points_file(double* point, char* file_name, int N);
double max_vector(double* vector, int n);

double miara(double* x, double* c);

int main()
{
    int znak = 0;
    char file_name[40] = "points.txt";
    const int N = 2000;   // number of points
    const int n = 15;     // number of centers 
    const int max_iter = 100;
    double point[2][N];
    double point_0[2];
    int point_nr_center_0[N];   // identyficator point : center (old)
    int point_nr_center_1[N];   // identyficator point : center (new)
    double center[2][n];        // center
    int nr_point_center[n];     // number points in klaster

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////       READ POINTS AND WRITE (KONTROL READ)

    if (read_points_file(*point, file_name, N))      // read points
    {
        cout << "ERROR: FILE_READ: points.txt IS NO" << endl;
        cout << "ENTER KEYWORD 1 AND ENTER" << endl;
        cin >> znak;
        return 1;
    }

    ofstream file_0_points;                    // write points (kontrol read)
    file_0_points.open("points_0.txt");
    for (int i = 0; i < N; i++)
        file_0_points << setw(12) << point[0][i] << "  " << setw(12) << point[1][i] << endl;
    file_0_points.close();

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /////       NORMUVANNIA DANYCH : TOCHKY BUDUT' V KVADRATI [0,1]x[0,1]
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    double max_points[2];
    for (int j = 0; j < 2; j++)
    {
        double* temp_vector = new double[N];
        for (int i = 0; i < N; i++)
            temp_vector[i] = point[j][i];
        max_points[j] = max_vector(temp_vector, N);
    }
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < N; i++)
            point[j][i] = point[j][i] / max_points[j];

    /*    ofstream file_0_normal;
        file_0_normal.open("points_n.txt");
        for (int i=0;i<N;i++)
            file_0_normal << setw(12) << point[0][i] << "  " << setw(12) << point[1][i] << endl;
        file_0_normal.close();*/

        ////////////////////////////////////////////////////////////////////////////////////////////////////

    ofstream file_c0_hist;
    file_c0_hist.open("center0_historia.txt");
    ofstream file_c_hist;
    file_c_hist.open("center_historia.txt");
    ofstream file_p2_hist;
    file_p2_hist.open("points_2_historia.txt");
    ofstream file_p3_hist;
    file_p3_hist.open("points_3_historia.txt");
    ofstream file_p5_hist;
    file_p5_hist.open("points_5_historia.txt");
    ofstream file_p9_hist;
    file_p9_hist.open("points_9_historia.txt");
    ofstream file_p12_hist;
    file_p12_hist.open("points_12_historia.txt");
    ofstream file_p15_hist;
    file_p15_hist.open("points_15_historia.txt");

    for (int i = 0; i < N; i++)
    {
        point_nr_center_0[i] = 0;
        point_nr_center_1[i] = 0;
    }

    int k = 1;      // perszyj centr : serednie aryfmetyczne koordynat tochok: funkcia mod_null()
    mod_null(*point, *center, N, n, k, point_nr_center_0, nr_point_center);
    cout << "c_x= " << setw(10) << center[0][0] << "  c_y= " << setw(10) << center[1][0];
    //    cout << " n_c= " << setw(5) << nr_point_center[0] << endl << endl << endl;
    cout << endl << endl << endl;

    file_c_hist << setw(10) << center[0][0] * max_points[0] << "  ";
    file_c_hist << setw(10) << center[1][0] * max_points[1] << endl << endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /////      DVA CENTRY: POCHATKOVE NABLYZHENNIA -eps i +eps VID POPEREDNIOHO POLOZHENNIA CENTRU 
    ////////////////////////////////////////////////////////////////////////////////////////////////////    
    double eps = 0.01;
    k++;
    center[0][1] = center[0][0] - eps;
    center[1][1] = center[1][0] - eps;
    center[0][0] += eps;
    center[1][0] += eps;

    file_c_hist << setw(10) << center[0][0] * max_points[0] << "  ";
    file_c_hist << setw(10) << center[1][0] * max_points[1] << endl;
    file_c_hist << setw(10) << center[0][1] * max_points[0] << "  ";
    file_c_hist << setw(10) << center[1][1] * max_points[1] << endl << endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

   

    while (k <= n)
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////   PIDHOTOVKA DO ITERACIJNOHO PROCESU STABILIZACII POLOZHENNIA CENTRIV
        /////////////////////////////////////////////////////////////////////////////////////////////////////               
        int epoch = 0;
        ident_null(*point, *center, N, n, k, point_nr_center_0);       // pochatkova identyfikacja pisla
        // vvedennia novoho centru
        mod_null(*point, *center, N, n, k, point_nr_center_0, nr_point_center);  // pochatkova zmina polozhennia centriv
        // obchyslennia kilkosti tochok v klasteri
/*        cout << endl << endl;
        for (int i_k=0;i_k<k;i_k++)
        {
             cout << "c_x= " << setw(10) << center[0][i_k] << "  c_y= " << setw(10) << center[1][i_k];
             cout << " n_c= " << setw(5) << nr_point_center[i_k]<< endl;
        }
        cout << endl << endl;
        getch();*/

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////            ITERACIJNYJ PROCES
        //////////////////////////////////////////////////////////////////////////////////////////////////////////

        while (1)
        {
            epoch++;
            if (epoch > max_iter)    // perevyszczeno zadanu dopustymu kilkist' iteracji
            {
                cout << "NEMAJE ZBIZHNOSTI PRY ZADANOMU OBMEZHENNIU NA KILKIST' ITERACIJ";
                cout << "ENTER KEYWORD 1 AND ENTER" << endl;
                cin >> znak;
                return 1;
            }

            int flag = 0;          // kilkist' perechodiv toczky z klasteru do klasteru v susidnich iteraciach

            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            ///////              CYKL UTOCHNENNIA POLOZHENNIA CENTRIV
            //////////////////////////////////////////////////////////////////////////////////////////////////////////

            for (int i = 0; i < N; i++)
            {
                point_0[0] = point[0][i];
                point_0[1] = point[1][i];
                point_nr_center_1[i] = identyfikacia(point_0, *center, k, n);  // do jakoho centru pisla utochnennia   
                int j_id = point_nr_center_1[i];       // do jakoho klasteru nalezhyt' toczka v aktualnij iteracii
                int i_id = point_nr_center_0[i];       // do jakoho klasteru nalezhala toczka v poperesdnij iteracii
                center[0][j_id] = center[0][j_id] + (point_0[0] - center[0][j_id]) / (nr_point_center[j_id] + 1);
                center[1][j_id] = center[1][j_id] + (point_0[1] - center[1][j_id]) / (nr_point_center[j_id] + 1);

                if (j_id != i_id)            // koly tochka zminyla klaster 
                {
                    center[0][i_id] = center[0][i_id] - (point_0[0] - center[0][i_id]) / (nr_point_center[i_id] - 1);
                    center[1][i_id] = center[1][i_id] - (point_0[1] - center[1][i_id]) / (nr_point_center[i_id] - 1);
                    flag++;
                }
            }

            f_nm_point_center(point_nr_center_1, N, k, nr_point_center);   // aktualna kilkist' tochok v klasteri
            cout << "flag = " << setw(5) << flag << endl;

            if (!flag) break;            // zhodna tochka ne zminyla klasteru: vvedeni centry stabilni

            for (int i = 0; i < N; i++)        // poky nemaje stabilizacii: zapamiatovujemo polozhennia toczky
                point_nr_center_0[i] = point_nr_center_1[i];   // v ostannij iteracii
        }

        cout << endl << endl;         // polozhennia centriv pisla stabilizacii 
        cout << "nm centers = " << setw(2) << k << endl << endl;
        for (int i_k = 0; i_k < k; i_k++)
        {
            cout << "  c_x= " << setw(10) << center[0][i_k] * max_points[0];
            cout << "  c_y= " << setw(10) << center[1][i_k] * max_points[1];
            //             cout << "  n_c= " << setw(5) << nr_point_center[i_k]<< endl;
            cout << endl;
        }
        cout << "ENTER KEYWORD 1 AND ENTER" << endl;
        cin >> znak;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////      VYPYSUVANNIA ISTORII DO FAJLIV
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        for (int i_k = 0; i_k < k; i_k++)
        {
            file_c_hist << setw(10) << center[0][i_k] * max_points[0] << "  ";
            file_c_hist << setw(10) << center[1][i_k] * max_points[1] << endl;
        }
        file_c_hist << endl;
        if (k == 2)
        {
            for (int i_cnt = 0; i_cnt < k; i_cnt++)
                for (int i_pkt = 0; i_pkt < N; i_pkt++)
                    if (i_cnt == point_nr_center_1[i_pkt])
                    {
                        file_p2_hist << setw(10) << point[0][i_pkt] * max_points[0] << "  ";
                        file_p2_hist << setw(10) << point[1][i_pkt] * max_points[1] << "  ";
                        file_p2_hist << setw(10) << point_nr_center_1[i_pkt] << endl;
                    }
        }
        if (k == 3)
        {
            for (int i_cnt = 0; i_cnt < k; i_cnt++)
                for (int i_pkt = 0; i_pkt < N; i_pkt++)
                    if (i_cnt == point_nr_center_1[i_pkt])
                    {
                        file_p3_hist << setw(10) << point[0][i_pkt] * max_points[0] << "  ";
                        file_p3_hist << setw(10) << point[1][i_pkt] * max_points[1] << "  ";
                        file_p3_hist << setw(10) << point_nr_center_1[i_pkt] << endl;
                    }
        }
        if (k == 5)
        {
            for (int i_cnt = 0; i_cnt < k; i_cnt++)
                for (int i_pkt = 0; i_pkt < N; i_pkt++)
                    if (i_cnt == point_nr_center_1[i_pkt])
                    {
                        file_p5_hist << setw(10) << point[0][i_pkt] * max_points[0] << "  ";
                        file_p5_hist << setw(10) << point[1][i_pkt] * max_points[1] << "  ";
                        file_p5_hist << setw(10) << point_nr_center_1[i_pkt] << endl;
                    }
        }
        if (k == 9)
        {
            for (int i_cnt = 0; i_cnt < k; i_cnt++)
                for (int i_pkt = 0; i_pkt < N; i_pkt++)
                    if (i_cnt == point_nr_center_1[i_pkt])
                    {
                        file_p9_hist << setw(10) << point[0][i_pkt] * max_points[0] << "  ";
                        file_p9_hist << setw(10) << point[1][i_pkt] * max_points[1] << "  ";
                        file_p9_hist << setw(10) << point_nr_center_1[i_pkt] << endl;
                    }
        }
        if (k == 12)
        {
            for (int i_cnt = 0; i_cnt < k; i_cnt++)
                for (int i_pkt = 0; i_pkt < N; i_pkt++)
                    if (i_cnt == point_nr_center_1[i_pkt])
                    {
                        file_p12_hist << setw(10) << point[0][i_pkt] * max_points[0] << "  ";
                        file_p12_hist << setw(10) << point[1][i_pkt] * max_points[1] << "  ";
                        file_p12_hist << setw(10) << point_nr_center_1[i_pkt] << endl;
                    }
        }
        if (k == 15)
        {
            for (int i_cnt = 0; i_cnt < k; i_cnt++)
                for (int i_pkt = 0; i_pkt < N; i_pkt++)
                    if (i_cnt == point_nr_center_1[i_pkt])
                    {
                        file_p15_hist << setw(10) << point[0][i_pkt] * max_points[0] << "  ";
                        file_p15_hist << setw(10) << point[1][i_pkt] * max_points[1] << "  ";
                        file_p15_hist << setw(10) << point_nr_center_1[i_pkt] << endl;
                    }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////           VVODYMO CHERHOVYJ CENTER
        ///////////////////////////////////////////////////////////////////////////////////////////////////////

        int id = iden_center(*point, *center, point_nr_center_1, N, k, n);
        k++;
        if (k > n) break;
        cout << endl << endl;
        center[0][k - 1] = center[0][id] + eps;
        center[1][k - 1] = center[1][id] + eps;
        center[0][id] -= eps;
        center[1][id] -= eps;

        for (int i_k = 0; i_k < k; i_k++)
        {
            file_c0_hist << setw(10) << center[0][i_k] * max_points[0] << "  ";
            file_c0_hist << setw(10) << center[1][i_k] * max_points[1] << endl;
        }
        file_c0_hist << endl;

        /*        for (int i_k=0;i_k<k;i_k++)
                {
                     cout << "c_x= " << setw(10) << center[0][i_k] << "  c_y= " << setw(10) << center[1][i_k];
                     cout << " n_c= " << setw(5) << nr_point_center[i_k]<< endl;
                }
                getch();*/
    }
    file_c_hist.close();
    file_c0_hist.close();
    file_p2_hist.close();
    file_p3_hist.close();
    file_p5_hist.close();
    file_p9_hist.close();
    file_p12_hist.close();
    file_p15_hist.close();
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////   VYPYSUJEMO ROZVIAZOK
    /////////////////////////////////////////////////////////////////////////////////////////////////////    

    cout << endl << endl << endl;
    cout << "                 CENTERS SOLUTION" << endl << endl;
    ofstream file_0_center;
    file_0_center.open("center_solution.txt");
    for (int i = 0; i < n; i++)
    {
        cout << "nr_c = " << setw(2) << i + 1 << "  ";
        cout << "x_c = " << setw(12) << center[0][i] * max_points[0] << "  ";
        cout << "y_c = " << setw(12) << center[1][i] * max_points[1] << endl;
        file_0_center << setw(12) << center[0][i] * max_points[0] << "  ";
        file_0_center << setw(12) << center[1][i] * max_points[1] << endl;
    }
    file_0_center.close();
    cout << "ENTER KEYWORD 1 AND ENTER" << endl;
    cin >> znak;

    return 0;
}

double miara(double* x, double* c)
{
    return sqrt(pow(x[0] - c[0], 2) + pow(x[1] - c[1], 2));
}

void f_nm_point_center(int* point_nr_center, int N, int k, int* nr_point_center)
{
    for (int j = 0; j < k; j++)
        nr_point_center[j] = 0;
    for (int i = 0; i < N; i++)
    {
        int pozycia = point_nr_center[i];
        nr_point_center[pozycia]++;
    }
}

int iden_center(double* point, double* center, int* point_nr_center, int N, int k, int n)
{
    double* E_center = new double[k];
    for (int j = 0; j < k; j++)
        E_center[j] = 0;
    for (int i = 0; i < N; i++)
    {
        int pozycia = point_nr_center[i];
        double center_0[2];
        center_0[0] = center[0 * n + pozycia];
        center_0[1] = center[1 * n + pozycia];
        double point_0[2];
        point_0[0] = point[0 * N + i];
        point_0[1] = point[1 * N + i];
        E_center[pozycia] += miara(point_0, center_0);
    }
    double E_max = E_center[0];
    int id = 0;
    for (int j = 1; j < k; j++)
    {
        if (E_max < E_center[j])
        {
            E_max = E_center[j];
            id = j;
        }
    }
    delete[] E_center;
    return id;
}

int identyfikacia(double* point_0, double* center, int k, int n)
{

    double center_0[2];
    center_0[0] = center[0 * n + 0];
    center_0[1] = center[1 * n + 0];
    double miara_min = miara(point_0, center_0);
    int id = 0;
    for (int j = 1; j < k; j++)
    {
        center_0[0] = center[0 * n + j];
        center_0[1] = center[1 * n + j];
        if (miara_min > miara(point_0, center_0))
        {
            miara_min = miara(point_0, center_0);
            id = j;
        }
    }
    return id;
}

void ident_null(double* point, double* center, int N, int n, int k, int* point_nr_center)
{
    double point_0[2];
    double center_0[2];
    for (int i = 0; i < N; i++)
    {
        point_0[0] = point[0 * N + i];
        point_0[1] = point[1 * N + i];
        point_nr_center[i] = 0;
        center_0[0] = center[0 * n + 0];
        center_0[1] = center[1 * n + 0];
        double miara_min = miara(point_0, center_0);
        for (int j = 1; j < k; j++)
        {
            center_0[0] = center[0 * n + j];
            center_0[1] = center[1 * n + j];
            if (miara_min > miara(point_0, center_0))
            {
                miara_min = miara(point_0, center_0);
                point_nr_center[i] = j;
            }
        }
    }
}

void mod_null(double* point, double* center, int N, int n, int k, int* point_nr_center, int* nr_point_center)
{
    for (int j = 0; j < k; j++)
    {
        center[0 * n + j] = 0;
        center[1 * n + j] = 0;
        nr_point_center[j] = 0;
    }
    for (int i = 0; i < N; i++)
    {
        int temp = point_nr_center[i];
        center[0 * n + temp] += point[0 * N + i];
        center[1 * n + temp] += point[1 * N + i];
        nr_point_center[temp]++;
    }
    for (int j = 0; j < k; j++)
    {
        center[0 * n + j] = center[0 * n + j] / nr_point_center[j];
        center[1 * n + j] = center[1 * n + j] / nr_point_center[j];
    }
}

int read_points_file(double* point, char* file_name, int N)
{
    ifstream file_read;
    file_read.open(file_name);
    if (!file_read.good())
        return 1;
    int N_1 = 5;
    int N_0 = 1000;
    int nr = 0;
    for (int i = 0; i < N_0; i++)
        for (int j = 0; j < N_1; j++)
        {
            int temp_1;
            int temp_2;
            file_read >> temp_1;
            file_read >> temp_2;
            if ((j == 1) || (j == 3))
            {
                point[0 * N + nr] = temp_1;
                point[1 * N + nr] = temp_2;
                nr++;
            }
            /*        if (!((i+1)%100))
                    {
                         cout << "i= " << setw(5)  << i+1;
                         cout << "  x= " << setw(10) << temp_1;
                         cout << "  y= " << setw(10) << temp_2;
                         if ((j==1)||(j==3))
                         {
                              cout << "  x= " << setw(10) << point[0*2+nr-1] << "  ";
                              cout << "  y= " << setw(10) << point[1*2+nr-1] << endl;
                         }
                         cout << endl;
                         getch();
                    }*/
        }
    file_read.close();
    return 0;
}

double max_vector(double* vector, int n)
{
    double ekstr = vector[0];
    if (n == 0) return ekstr;
    for (int i = 1; i < n; i++)
        if (ekstr < vector[i])
            ekstr = vector[i];
    return ekstr;
}

