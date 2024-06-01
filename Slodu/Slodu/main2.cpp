#include <iostream>
#include <vector>
#include <math.h>

double B1 = 1, B2 = 1, F = 10;

double f1(double x, double v, double t)
{
    return v;
}

double f2(double x, double v, double t)
{
    return (-B1*v*fabs(v)-B2*v+F);
}

class Runge4Solver
{
public:
    Runge4Solver (double x, double v, double t)
    {
        m_x.push_back(x);
        m_v.push_back(v);
        m_time.push_back(t);
    }

    double x() const {return m_x.back();}
    double v() const {return m_v.back();}
    double t() const {return m_time.back();}

    void CalcStep()
    {
        double x_prev = m_x.back();
        double v_prev = m_v.back();
        double t_prev = m_time.back();

        k_x[0] = f1(x_prev, v_prev, t_prev);
        k_v[0] = f2(x_prev, v_prev, t_prev);

        k_x[1] = f1(x_prev+k_x[0]*m_step/2, v_prev+k_v[0]*m_step/2, t_prev+m_step/2);
        k_v[1] = f2(x_prev+k_x[0]*m_step/2, v_prev+k_v[0]*m_step/2, t_prev+m_step/2);

        k_x[2] = f1(x_prev+k_x[1]*m_step/2, v_prev+k_v[1]*m_step/2, t_prev+m_step/2);
        k_v[2] = f2(x_prev+k_x[1]*m_step/2, v_prev+k_v[1]*m_step/2, t_prev+m_step/2);

        k_x[3] = f1(x_prev+k_x[2]*m_step, v_prev+k_v[2]*m_step, t_prev+m_step);
        k_v[3] = f2(x_prev+k_x[2]*m_step, v_prev+k_v[2]*m_step, t_prev+m_step);

        m_x.push_back(x_prev + m_step*(k_x[0]+2*k_x[1]+2*k_x[2]+k_x[3])/6.0);
        m_v.push_back(v_prev + m_step*(k_v[0]+2*k_v[1]+2*k_v[2]+k_v[3])/6.0);
        m_time.push_back(t_prev + m_step);

    }

private:
    std::vector<double> m_time, m_x, m_v;
    double k_x[4], k_v[4];
    double m_step = 0.001;
};

int main()
{
    Runge4Solver system(0,1,0); //Начальные условия
    while (system.t() < 3.15)
    {
        system.CalcStep();
        std::cout << "X=" << system.x()
                  << "\tV=" << system.v()
                  << "\tT=" << system.t() << std::endl;
    }
    return 0;
}
