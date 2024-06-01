#include <iostream>
#include <vector>
#include <math.h>

struct prmtr
{
    double a = 0.0;
    double b = 0.0;
    double d = 0.0;
};

std::vector<prmtr> variants  {
    {0.0,     0.0,      0.0},
    {1.0,   0.2,    1.0},
    {-2.0,  0.2,    1.0},
    {0.3,   0.2,    1.0},
    {-0.7,  0.7,    1.0},
    {1.2,   0.7,    0.0},
    {-0.8,   0.7,    0.0},
    {2.5,   2.1,    0.0},
    {-2,   2.1,    0.0},
    {0.3,   2.1,    0.5},
    {-0.7,   1.3,    0.5},
    {1.2,   1.3,    0.5},
    {-0.8,   1.3,    0.5},
    {2.5,   1.3,    0.5},

};

class Runge4Solver
{
public:

    Runge4Solver(){}
    void InitValues (std::vector<double>& initVals, double initTime)
    {
        m_values = initVals;
        m_time = initTime;
    }
    void CalcStep()
    {
        std::vector<double> tmp,
        partialVals(m_values.size()),
        previousVals = m_values;
        tmp = RecalcSystem(m_time);
        for (int i = 0; i<tmp.size(); i++)
        {
            partialVals[i] = tmp[i];
            m_values[i] = previousVals[i]+tmp[i]*m_step*0.5;
        }
            tmp = RecalcSystem(m_time+m_step*0.5);
        for (int i = 0; i<tmp.size(); i++)
        {
            partialVals[i] += 2*tmp[i];
            m_values[i] = previousVals[i]+tmp[i]*m_step*0.5;
        }
        tmp = RecalcSystem(m_time+m_step*0.5);
        for (int i = 0; i<tmp.size(); i++)
        {
            partialVals[i] += 2*tmp[i];
            m_values[i] = previousVals[i]+tmp[i]*m_step;
        }
        tmp = RecalcSystem(m_time+m_step);
        for (int i = 0; i<tmp.size(); i++)
        {
            partialVals[i] += tmp[i];
            partialVals[i] *= m_step/6.0;
            m_values[i]= previousVals[i]+partialVals[i];
        }
        m_time+=m_step;
    }

    double t() const {return m_time;}
    std::vector<double> vals() const {return m_values;}
protected:
    virtual std::vector<double> RecalcSystem(double time) =0;
    std::vector<double> m_values;
    double m_time;
    double m_step = 0.0001;
};

class TestRunge4 : public Runge4Solver
{
    double B1 = 1;
    double B2 = 1;
    double F = 10;

    virtual std::vector<double> RecalcSystem(double time)
    {
        std::vector<double> ret(m_values.size());
        ret[0] = m_values[1];
        ret[1] = -B1*pow(m_values[1],2) - B2*m_values[1] + F;
        return ret;

    }
};

class HomeWork_Task_1 : public Runge4Solver
{
    double a = 0.0, b = 0.0, d = 0.0;

    virtual std::vector<double> RecalcSystem(double time)
    {
        std::vector<double> ret(m_values.size());
        ret[0] = a*time - b*m_values[0];

        return ret;
    }

public:
    double get_d() {return d;}
    double analytical_Solution (double t)
    {
        return a/b * (t - 1.0/b) + (d+(a/pow(b,2.0)))*exp(-b*t);
    }
    void set_abd (double a1, double b1, double d1)
    {
        a = a1; b = b1; d = d1;
    }
};

class HomeWork_Task_2 : public Runge4Solver
{
    virtual std::vector<double> RecalcSystem(double time)
    {
        std::vector<double> ret(m_values.size());
        ret[0] = 9.0*m_values[0] + 24.0*m_values[1] + 5.0*cos(time) - (1.0/3.0)*sin(time);
        ret[1] = -24.0*m_values[0] - 51.0*m_values[1] - 9.0*cos(time) + (1.0/3.0)*sin(time);
        return ret;
    }
public:
    void analytical_Solution (double t, double &u1, double &u2)
    {
        u1 = 2.0*exp(-3.0*t) - exp(-39.0*t) + (1.0/3.0)*cos(t);
        u2 = -exp(-3.0*t) + 2.0*exp(-39.0*t) - (1.0/3.0)*cos(t);
    }
};

int main()
{
//-----------------------РЕШЕНИЕ ПЕРВОГО ЗАДАНИЯ---------------//
//    int variant = 1;
//    for (; variant < variants.size(); ++variant) {
//        std::cout << "Вариант №" << variant << std::endl;

//        HomeWork_Task_1 system_1;
//        std::vector<double> init_1;
//        system_1.set_abd(variants[variant].a, variants[variant].b, variants[variant].d);

//        init_1.push_back(system_1.get_d());
//        system_1.InitValues(init_1, 0.0);
//        double err = 0;

//        while (system_1.t() <= 1)
//        {
//            system_1.CalcStep();
//            std::vector<double> vals = system_1.vals();
//            std::cout << "y=" << vals[0]
//                      << "\tu=" << system_1.analytical_Solution(system_1.t())
//                      << "\tt=" << system_1.t() << std::endl;
//            double sys_err = abs(system_1.analytical_Solution(system_1.t()) - vals[0]);


//            if (err <= sys_err )
//            {
//                err = sys_err;
//            }
//        }
//            std::cout << std::endl << "max(|u - y|)=" << err << std::endl << std::endl;
//            std::cout << "Для всех вариантов рассогласоваие не превышает 0.001 при h = 0.001"<< std::endl;
//        }


    //-----------------------РЕШЕНИЕ ВТОРОГО ЗАДАНИЯ---------------//

    HomeWork_Task_2 system_2;
    std::vector<double> init_2;

    init_2.push_back(4.0/3.0);
    init_2.push_back(2.0/3.0);
    system_2.InitValues(init_2, 0);
    double u1_global_err = 0, u2_global_err = 0,
           u1_local_err = 0, u2_local_err = 0,
           u1_analitic = 0, u2_analitic = 0;

    while (system_2.t() <= 1)
    {
        system_2.CalcStep();
        system_2.analytical_Solution(system_2.t(), u1_analitic, u2_analitic);

        std::vector<double> vals = system_2.vals();
            std::cout << "{u1=" << vals[0]
                      << " u2=" << vals[1]
                      << "}, \t{u1_a=" << u1_analitic
                      << " u2_a=" << u2_analitic
                      << "}, \tt=" << system_2.t() << std::endl;
        u1_local_err = abs(u1_analitic - vals[0]);
        u2_local_err = abs(u2_analitic - vals[1]);

        if (u1_global_err < u1_local_err)
            u1_global_err = u1_local_err;
        if (u2_global_err < u2_local_err)
            u2_global_err = u2_local_err;


    }
        std::cout << std::endl << "max global err u1=" << u1_global_err
                               << "\tmax global err u2=" << u2_global_err
                               << std::endl << std::endl;


    return 0;
}

