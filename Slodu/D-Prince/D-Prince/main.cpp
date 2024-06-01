#include <iostream>
#include <eigen3/Eigen/Eigen>

using namespace std;

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

class DPSolver
{
public:
    DPSolver()
    {
        steps.resize(7);
        A <<    0.0,              0.0,              0.0,              0.0,          0.0,             0.0,           0.0,
                1.0/5.0,          0.0,              0.0,              0.0,          0.0,             0.0,           0.0,
                3.0/40.0,         9.0/40.0,         0,              0,          0,             0,           0,
                44.0/45.0,        -56.0/15.0,       32.0/9,         0,          0,             0,           0,
                19372.0/6561.0,   -25360.0/2187.0,  64448.0/6561,   -212.0/729, 0,             0,           0,
                9017.0/3168.0,    -355.0/33.0,      46732.0/5247,   49.0/176,   -5103.0/18656, 0,           0,
                35.0/384.0,       0.0,              500.0/1113,     125.0/192,  -2187.0/6784,   11.0/84,    0;

        b1 <<   35.0/384.0,       0.0,              500.0/1113,     125.0/192,  -2187.0/6784,   11.0/84,    0;
        b2 <<   5179.0/57600.0,   0.0,              7571.0/16695,   393./640,   -92097./339200, 187./2100,  1.0/40;
        steps <<0,              1.0/5.0,          3.0/10,         4.0/5,      8./9,           1,          1;

        m_step = 0.01;
        m_max = 0.0001;
        m_min = 0.0000001;
    }

    void CalcStep()
    {
        Eigen::MatrixXd tmp(7, m_values.size());
        Eigen::VectorXd x1, x2;
        double diff = 1;
        do
        {
            Eigen::VectorXd val = m_values;
            tmp.row(0) = RecalcSystem(m_time, val);
            for (int i = 1; i < 7; i++)
            {
                Eigen::VectorXd val = m_values;
                for (int j = 0; j < i; j++)
                    val += tmp.row(j) * A(i,j) * m_step;
                tmp.row(i) = RecalcSystem(m_time + steps(i) * m_step, val);
            }
            x1 = b1 * tmp;
            x2 = b2 * tmp;
            diff = (x1 - x2).cwiseAbs().maxCoeff() * m_step;
            if (diff > m_max)
                m_step /= 2;
            if (diff < m_min)
                m_step *= 2;
            std::cout << "Step: " << m_step << endl;
        }

        while (diff > m_max);

        m_values = m_values + x1 * m_step;
        m_time += m_step;
    }

    void InitValues (Eigen::VectorXd const & initVals, double initTime)
    {
        m_values = initVals;
        m_time = initTime;
    }

    double t() const {return m_time;}
    Eigen::VectorXd vals() const {return m_values;}

protected:
    virtual Eigen::VectorXd RecalcSystem(double time, Eigen::VectorXd& val) = 0;
    Eigen::VectorXd m_values, steps;
    double m_time;
    double m_max = 0.1, m_min = 0.0001;

    double m_step = 0.01;
    Eigen::Matrix<double, 7, 7> A;
    Eigen::Matrix<double, 1, 7> b1, b2;
};

class HomeWork_Task_1 : public DPSolver
{
     double a = 0.0, b = 0.0, d = 0.0;

    virtual Eigen::VectorXd RecalcSystem(double time, Eigen::VectorXd& val) override
    {
        Eigen::VectorXd ret (1);
        ret[0] = a*time - b*val[0];
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

class HomeWork_Task_2 : public DPSolver
{
    virtual Eigen::VectorXd RecalcSystem(double time, Eigen::VectorXd& val) override
    {
        Eigen::VectorXd ret (2);
        ret[0] = (9.0*val[0]) + (24.0*val[1]) + (5.0*cos(time)) - ((1.0/3.0)*sin(time));
        ret[1] = (-24.0*val[0]) - (51.0*val[1]) - (9.0*cos(time)) + ((1.0/3.0)*sin(time));
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

//-----------------УРАВНЕНИЕ 1---------------------//
    HomeWork_Task_1 system_1;
    Eigen::VectorXd val(1);
    system_1.set_abd(2.5, 1.3, 0.5);

    val << (system_1.get_d());
    system_1.InitValues(val, 0);
    double err = 0;
    while (system_1.t() < 1)
    {
        system_1.CalcStep();
        Eigen::VectorXd vals = system_1.vals();
        std::cout << "y=" << vals[0]
                          << "\tu=" << system_1.analytical_Solution(system_1.t())
                          << "\tt=" << system_1.t() << std::endl;
        double sys_err = abs(system_1.analytical_Solution(system_1.t()) - vals[0]);

        if (err < sys_err )
        {
            err = sys_err;
        }
    }
        std::cout << std::endl << "max(|u - y|)=" << err << std::endl << std::endl;

std::cout << "SYS2" << std::endl;

//------------------СИСТЕМА УРАВНЕНИЙ 2-----------//
    HomeWork_Task_2 system_2;
    Eigen::VectorXd init_2(2);
    init_2 << 4.0/3.0, 2.0/3.0;


    system_2.InitValues(init_2, 0);
    double  u1_global_err = 0.0, u2_global_err = 0.0,
            u1_local_err = 0.0, u2_local_err = 0.0,
            u1_analitic = 0.0, u2_analitic = 0.0;

    while (system_2.t() <= 1.0)
    {
        system_2.CalcStep();
        system_2.analytical_Solution(system_2.t(), u1_analitic, u2_analitic);

        Eigen::VectorXd vals = system_2.vals();
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
