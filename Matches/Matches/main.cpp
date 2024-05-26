#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/features2d.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

//#include <opencv4/opencv2/xfeatures2d.hpp>

using namespace std;

cv::Mat CLAHE(cv::Mat& src)
{
    cv::Mat dst;
    cv::cvtColor(src, src, cv::COLOR_BGR2HSV);
    cv::Mat channels[3];
    split(src, channels);
    cv::Ptr<cv::CLAHE> pClahe = cv::createCLAHE(80, cv::Size(5,5));
    pClahe->apply(channels[2], channels[2]);
    cv::merge(channels, 3, dst);
    cv::cvtColor(dst, dst, cv::COLOR_HSV2BGR);
    return dst;
}

void PrintHparameters(cv::Mat& H, double& X, double &Y, double &ANGLE)
{
    X += H.at<double>(0,2);
    Y += H.at<double>(1,2);
    ANGLE += atan2(H.at<double>(0,1), H.at<double>(0,0));


    std::cout << H << std::endl;
    std::cout << "----------------------" << std::endl;
    std::cout << "Смещение X: " << X << std::endl;
    std::cout << "Смещение Y: " << Y << std::endl;
    std::cout << "Угол поворта: " << ANGLE  << std::endl << std::endl;

}

int main()
{
    double X = 0, Y = 0, ANGLE = 0;
    cv::Mat src, src2, descr1, descr2, img_matches;
    cv::VideoCapture cap("sample_mpg.avi");
    if (!cap.isOpened()) return -1;

    cv::namedWindow("Display", cv::WINDOW_AUTOSIZE);
    cv::Ptr<cv::FeatureDetector> detector;
    cv::Ptr<cv::DescriptorExtractor> extractor;
    cv::BFMatcher matcher;
    std::vector<cv::KeyPoint> keys1, keys2;
    std::vector<cv::DMatch> matches;
    cv::Mat imgeWithKeypoints;
    detector = cv::ORB::create();
    extractor = cv::ORB::create();


    bool stop = false;
    double rate = cap.get(cv::CAP_PROP_FPS);
    std::cout << "FPS: " << rate << std::endl;
    int delay = 1000 / rate;


    while (!stop)
    {

        bool result = cap.grab();
        if (result)
        {
            cap >> src;
            //src = CLAHE(src);
        }
        else { continue; }

        result = cap.grab();
        if (result)
        {
            cap >> src2;
            //src2 = CLAHE(src2);
        }
        else { continue; }

        detector->detect(src, keys1);
        detector->detect(src2, keys2);

        extractor->compute(src, keys1, descr1);
        extractor->compute(src2, keys2, descr2);

        matcher.match(descr1, descr2, matches);
        cv::drawMatches(src, keys1, src2, keys2, matches, img_matches);

        std::vector<cv::Point2f> obj;
        std::vector<cv::Point2f> scene;
        for (size_t i = 0; i < matches.size(); i++)
        {
            obj.push_back(keys1[matches[i].queryIdx].pt);
            scene.push_back(keys1[matches[i].trainIdx].pt);
        }


        cv::Mat H = cv::findHomography(obj, scene, cv::RANSAC);
        PrintHparameters(H, X, Y, ANGLE);




         cv::imshow("Display", img_matches);

         int key = cv::waitKey(delay);
         if (key == 27) stop = true;
    }
    return 0;
}
