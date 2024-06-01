#include <iostream>
#define USE_MATH_DEFINES
#include <cmath>
#include <vector>

double a = 13.0;
double b = 1.3;
double d = 0.5;

using namespace std;

double f(double x, double y)
{
    return (-sin(x));
}

double runge4(double t, double y0, double step, double (f)(double, double))
{
    double k1, k2, k3, k4;
    k1 = f(t, y0);
    k2 = f(t+step/2.0, y0+step*k1/2.0);
    k3 = f(t+step/2, y0+step*k2/2.0);
    k4 = f(t+step, y0+step*k3);
    double y = y0 + (k1 + 2.0*k2 + 2.0*k3 + k4)*step/6.0;
    return y;

}

int main()
{
    double initVal = 1; // Начальные условие
    double step = 0.1;
    std::vector<double> time, y;
    time.push_back(0);
    y.push_back(initVal);
    double errMAX = 0;

    for (double t = step; t < M_PI; t += step )
    {

        double dy = runge4(time.back(), y.back(), step, f);

        time.push_back(t);
        y.push_back(dy);


    }


    for (int i = 0; i<time.size(); i++)
    {

        std::cout << "cos(t)=" << cos(time[i]) << " y(t)=" << y[i] << std::endl;
        if ( abs(cos(time[i]) - y[i]) > errMAX) {errMAX = abs(cos(time[i]) - y[i]); }
    }
    std::cout << errMAX << std::endl;



    return 0;
}
