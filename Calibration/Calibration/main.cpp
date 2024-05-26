#include <iostream>
#include <vector>
#include <math.h>
#include<eigen3/Eigen/Eigen>

using namespace std;

//mEstimator
void mEstimator(vector<pair<double, double>>& data,
                Eigen::MatrixXd& system,
                Eigen::VectorXd& rhs  )
{
    auto size = data.size();


    Eigen::MatrixXd Weight = Eigen::MatrixXd::Identity(size,size);
    double delta = 0.01;
    Eigen::VectorXd x;
    for (int i=0; i<50; i++)
    {
        auto tmpSystem = system.transpose()*Weight*system;
        auto tmpRhs = system.transpose()*Weight*rhs;
        x = tmpSystem.partialPivLu().solve(tmpRhs);
        Eigen::VectorXd res = rhs - system*x;
        for (int j=0; j<res.size(); j++)
        {
            Weight(j,j) = 1/ (max(delta, fabs(res(j))));
        }
    }
    std::cout << x.transpose() << std::endl;
}


double findModelEstimate(double Z)
{
    return log10((Z/1024.0))*-95.3644 + -79.5837 + 0;  //WithEstimator
}

double findModelMNK(double Z)
{
    return log10((Z/1024.0))*-96.651 + -81.7126 + 0;  //WithMNK
}

//
// Ti = T0 + a + c*(log10(Zi)-log10(1024)) Модель. i = 1, 2, 3, .. 12.
// c = 1/k, a = c*log10(b), b = 1 + Rc/Ro.

int main()
{
    std::vector<std::pair<double, double>> data //first-Z second-T
    {
        {27.0,71.0}, {31.0,64.0}, {43.0,52.0}, {58.0,41.0}, {69.0,33.0}, {86.0,23.0}, {102.0,17.0}, {111.0,12.0}, {122.0,2.0}, {137.0,0.0}, {18.0,87.0}, {176.0,-5.0}
    };

    size_t i = data.size();
    size_t j = 3;

    Eigen::MatrixXd A(i, j);
    Eigen::VectorXd X(j);
    Eigen::VectorXd B(data.size());

    //Load data to system.
    for (int i = 0; i < data.size(); ++i) {
        B[i] = data[i].second; // to vector B
        A.row(i) << log10(data[i].first/1024.0), 1, 1; // to matrix A
    }

    //Slove МНК
    std::cout << "RUN MNK" << std::endl;
    Eigen::MatrixXd ans1 = A.colPivHouseholderQr().solve(B);
    std::cout << "Полученные коэффициенты c, a, T0:" << std::endl;
    std::cout << ans1.transpose() << std::endl;

//    std::cout << "Проверка Ti" << std::endl;
//    std::cout << A*ans1 << std::endl; // Проверка. A*ans = Ti

    std::cout << "RUN ESTIMATOR" << std::endl;
    mEstimator(data, A, B);


    std::cout << "Проверка полученных функциий:" << std::endl;


    std::cout << "Полная проверка Z = [0, 1024]:" << std::endl;
    for (int Z = 1; Z < 1025; Z++)
    {
        std::cout << "Z: " << Z << " mEstimator " << " T(Z) = " << findModelEstimate(Z)
                                << " MNK " << " T(Z) = " << findModelMNK(Z)
                                << " mEst - MNK: " << abs(findModelEstimate(Z)-findModelMNK(Z)) << std::endl;
    }



    return 0;

}
