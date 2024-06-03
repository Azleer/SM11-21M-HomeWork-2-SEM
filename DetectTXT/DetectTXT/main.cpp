#include <iostream>
#include <algorithm>
#include<list>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace  cv;

bool isRect(cv::RotatedRect rect)
{
    if ((rect.size.height/rect.size.width > 0.8 && rect.size.height/rect.size.width < 1.2))
    {
        return true;
    }
    return false;
}

bool isSmallRect(cv::RotatedRect rect)
{
    if     ((rect.size.height/rect.size.width > 0.1 && rect.size.height/rect.size.width < 0.6) ||
            (rect.size.height/rect.size.width > 3 && rect.size.height/rect.size.width < 6))
    {
       return true;
    }
    else if ((rect.size.height/rect.size.width > 1.0 && rect.size.height/rect.size.width < 1.4) ||
             (rect.size.height/rect.size.width > 0.6 && rect.size.height/rect.size.width < 1))
    {
        return true;
    }
    return false;
}

void rmBlick(Mat& Red, Mat& Src)
{
    int H = 0, S = 2, V = 1;
    int R = 2 , G = 1 , B = 0;

    cvtColor(Src, Src, cv::COLOR_BGR2HSV);
    for (size_t X = 0; X < Src.cols; X++)
    {
        for (size_t Y = 0; Y < Src.rows; Y++ )
        {
           bool is_white = Src.at<cv::Vec3b>(Y,X)[V] < 150;
           bool is_S = Src.at<cv::Vec3b>(Y,X)[S] > 150;

            if(is_white && is_S)
            {
                Red.at<cv::Vec3b>(Y,X)[R] = 255;
                Red.at<cv::Vec3b>(Y,X)[G] += 0;
                Red.at<cv::Vec3b>(Y,X)[B] += 0;
            }
        }
    }


    cvtColor(Src, Src, cv::COLOR_HSV2BGR);
    medianBlur(Red, Red, 3);
    GaussianBlur(Red, Red, Size(9, 9), 0, 0, BORDER_DEFAULT);


    for (size_t X = 0; X < Red.cols; X++)
    {
        for (size_t Y = 0; Y < Red.rows; Y++ )
        {
           bool is_red = Red.at<cv::Vec3b>(Y,X)[R] > 10 && Red.at<cv::Vec3b>(Y,X)[R] < 50;



            if(is_red)
            {
                Red.at<cv::Vec3b>(Y,X)[R] = 255;
                Red.at<cv::Vec3b>(Y,X)[G] = 0;
                Red.at<cv::Vec3b>(Y,X)[B] = 0;
            }
        }
    }


    morphologyEx(Red,Red, MorphTypes::MORPH_ERODE ,
    getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
    Point(-1,-1), 3);

    for (size_t X = 0; X < Red.cols; X++)
    {
        for (size_t Y = 0; Y < Red.rows; Y++ )
        {
           bool is_red = Red.at<cv::Vec3b>(Y,X)[R] > 10 && Red.at<cv::Vec3b>(Y,X)[R] < 255;



            if(is_red)
            {
                Red.at<cv::Vec3b>(Y,X)[R] = 255;
                Red.at<cv::Vec3b>(Y,X)[G] = 0;
                Red.at<cv::Vec3b>(Y,X)[B] = 0;
            }
        }
    }

    medianBlur(Red, Red, 3);
    GaussianBlur(Red, Red, Size(9, 9), 0, 0, BORDER_DEFAULT);

    for (size_t X = 0; X < Red.cols; X++)
    {
        for (size_t Y = 0; Y < Red.rows; Y++ )
        {
           bool is_red = Red.at<cv::Vec3b>(Y,X)[R] > 5 && Red.at<cv::Vec3b>(Y,X)[R] < 30;

            if(is_red)
            {
                Red.at<cv::Vec3b>(Y,X)[R] = 255;
                Red.at<cv::Vec3b>(Y,X)[G] = 0;
                Red.at<cv::Vec3b>(Y,X)[B] = 0;
            }
        }
    }

        morphologyEx(Red,Red, MorphTypes::MORPH_ERODE,
        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
        Point(-1,-1), 3);

        for (size_t X = 0; X < Red.cols; X++)
        {
            for (size_t Y = 0; Y < Red.rows; Y++ )
            {
               bool is_red = Red.at<cv::Vec3b>(Y,X)[R] > 5 && Red.at<cv::Vec3b>(Y,X)[R] < 255;

                if(is_red)
                {
                    Red.at<cv::Vec3b>(Y,X)[R] = 255;
                    Red.at<cv::Vec3b>(Y,X)[G] = 0;
                    Red.at<cv::Vec3b>(Y,X)[B] = 0;
                }
            }
        }

        morphologyEx(Red,Red, MorphTypes::MORPH_ERODE,
        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
        Point(-1,-1), 1);
        medianBlur(Red, Red, 3);

        morphologyEx(Red,Red, MorphTypes::MORPH_ERODE,
        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
        Point(-1,-1), 1);

        GaussianBlur(Red, Red, Size(9, 9), 0, 0, BORDER_DEFAULT);

        for (size_t X = 0; X < Red.cols; X++)
        {
            for (size_t Y = 0; Y < Red.rows; Y++ )
            {
               bool is_red = Red.at<cv::Vec3b>(Y,X)[R] > 2 && Red.at<cv::Vec3b>(Y,X)[R] < 255;

                if(is_red)
                {
                    Red.at<cv::Vec3b>(Y,X)[R] = 255;
                    Red.at<cv::Vec3b>(Y,X)[G] = 0;
                    Red.at<cv::Vec3b>(Y,X)[B] = 0;
                }
            }
        }

        morphologyEx(Red,Red, MorphTypes::MORPH_ERODE,
        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
        Point(-1,-1), 1);

        morphologyEx(Red,Red, MorphTypes::MORPH_GRADIENT,
        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
        Point(-1,-1), 1);
}

Mat FindArea(Mat& image, Mat& megasrs)
{
    morphologyEx(image,image, MorphTypes::MORPH_DILATE ,
    getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
    Point(-1,-1), 5);

    RNG rng(12345);
    Mat src, src_gray, grad;
    int ksize = 3, scale = 1, delta = 0;
    int ddepth = CV_16S;
    GaussianBlur(image, src, Size(9, 9), 0, 0, BORDER_DEFAULT);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    Canny(src_gray, grad, 30, 90);
    vector<vector<Point> > contours;
    findContours( grad, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
    vector<vector<Point> >hull( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );
    }
    Mat drawing = Mat::zeros(grad.size(), CV_8UC1 );

    std::vector<std::pair<int,float>> listOfSize;
    for( size_t i = 0; i< contours.size(); i++ )
    {
        auto rect = cv::minAreaRect(contours[i]);
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        if (isRect(rect))
        {
            listOfSize.push_back({i, rect.size.area()});
            continue;
        }
    }

    std::pair<int, float> biggestCntr;
    for (auto cntr : listOfSize) {
        if (cntr.second > biggestCntr.second)
        {
            biggestCntr.second = cntr.second;
            biggestCntr.first = cntr.first;
        }
    }

    Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), cv::FILLED);
    drawContours( drawing, hull, (int)biggestCntr.first, color, -1);
    morphologyEx(drawing,drawing, MorphTypes::MORPH_ERODE ,
    getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
    Point(-1,-1), 3);
    Mat res;
    bitwise_and(megasrs, megasrs, res, drawing );
    return res;
}

vector<vector<Point>> FindSmallArea(Mat& image, Mat& megasrs)
{
    RNG rng(12345);
    Mat src, src_gray, grad;
    int ksize = 3, scale = 1, delta = 0;
    int ddepth = CV_16S;
    GaussianBlur(image, src, Size(9, 9), 0, 0, BORDER_DEFAULT);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    Canny(src_gray, grad, 30, 90);

    vector<vector<Point>> contours;
    findContours( grad, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
    vector<vector<Point> >hull( contours.size() );

    for( size_t i = 0; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );
    }
    Mat drawing = Mat::zeros(grad.size(), CV_8UC1 );

    std::list<std::pair<int,float>> listOfSize;
    for( int i = 0; i< hull.size(); i++)
    {
        auto rect = cv::minAreaRect(hull[i]);
        if (isSmallRect(rect))
        {
            listOfSize.push_back({i, rect.size.area()});
        }
    }

    std::sort(hull.begin(), hull.end(), [](std::vector<Point>& first, std::vector<Point>& second )
    {
        auto rectFirs = cv::minAreaRect(first);
        auto rectSecond = cv::minAreaRect(second);

        return rectFirs.size.area() > rectSecond.size.area();
    });

    vector<vector<Point>> result;
    result.push_back(hull[4]);
    result.push_back(hull[8]);
    result.push_back(hull[14]);

    return result;
}

Mat selectRed (Mat& src)
{
    cv::Mat procImage_BGR = src.clone();
    cv::Mat procImage_HSV = src.clone();

    cv::cvtColor(procImage_BGR, procImage_HSV, cv::COLOR_BGR2HSV);

    int H = 0, S = 2, V = 1;
    int R = 2 , G = 1 , B = 0;

    Mat binary = src.clone();
    for (size_t X = 0; X < binary.cols; X++)
    {
        for (size_t Y = 0; Y < binary.rows; Y++ )
        {
            binary.at<cv::Vec3b>(Y,X)[R] = 0;
            binary.at<cv::Vec3b>(Y,X)[G] = 0;
            binary.at<cv::Vec3b>(Y,X)[B] = 0;
        }
    }

    for (size_t X = 0; X < procImage_BGR.cols; X++)
    {
        for (size_t Y = 0; Y < procImage_BGR.rows; Y++ )
        {
           bool is_red = procImage_HSV.at<cv::Vec3b>(Y,X)[S] <= 255;
           bool red_S = (procImage_HSV.at<cv::Vec3b>(Y,X)[S] >= 140) && (procImage_HSV.at<cv::Vec3b>(Y,X)[S] <= 255); //140
           bool red_V = (procImage_HSV.at<cv::Vec3b>(Y,X)[V] >= 160) && (procImage_HSV.at<cv::Vec3b>(Y,X)[V] <= 255); //160

            if(is_red && red_S && red_V)
            {
                binary.at<cv::Vec3b>(Y,X)[R] = 255;
                binary.at<cv::Vec3b>(Y,X)[G] = 0;
                binary.at<cv::Vec3b>(Y,X)[B] = 0;
            }
        }
    }
    return binary;
}

void PrintResult(Mat& src, vector<vector<Point>> hull)
{
    RNG rng(12345);
    Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    drawContours( src, hull, (int)0, color);
    drawContours( src, hull, (int)1, color);
    drawContours( src, hull, (int)2 , color);

    auto rect1 = cv::minAreaRect(hull[0]);
    auto rect2 = cv::minAreaRect(hull[1]);
    auto rect3 = cv::minAreaRect(hull[2]);
//    auto i = Rect(rect1.center.y, rect1.center.x, rect1.size.width, rect1.size.height);



//      Mat res1 = src(Range(), Range());
      Mat res1 = src(Rect(rect1.center.x - rect1.size.width/2, rect1.center.y - rect1.size.height/2, rect1.size.width, rect1.size.height));
      Mat res2 = src(Rect(rect2.center.x - rect2.size.width/2, rect2.center.y - rect2.size.height/2, rect2.size.width, rect2.size.height));
      Mat res3 = src(Rect(rect3.center.x - rect3.size.width/2, rect3.center.y - rect3.size.height/2, rect3.size.width, rect3.size.height));



    cv::imshow("res1", res1);
    cv::imshow("res2", res2);
    cv::imshow("res3", res3);
}

void TextDecetc(Mat& src)
{
    Mat imageProc = src.clone();
    Mat RedArea = selectRed(imageProc);
    Mat CuttoutRedArea = FindArea(RedArea, src);
    Mat DataArea = selectRed(CuttoutRedArea);
    rmBlick(DataArea, CuttoutRedArea);
    auto hull = FindSmallArea(DataArea, src);
    PrintResult(src, hull);
    cv::waitKey(0);
}

int main()
{
    cv::Mat src0, src1, src2, src3, src4;

    src0 = cv::imread("test0.png", cv::IMREAD_COLOR);
    src1 = cv::imread("test1.png", cv::IMREAD_COLOR);
    src2 = cv::imread("test2.png", cv::IMREAD_COLOR);
    src3 = cv::imread("test3.png", cv::IMREAD_COLOR);
    src4 = cv::imread("test4.png", cv::IMREAD_COLOR);

    if (src0.empty() || src1.empty() || src2.empty() || src3.empty() || src4.empty() ) return -1;

    TextDecetc(src0);
    TextDecetc(src1);
    TextDecetc(src2);
    TextDecetc(src3);
    TextDecetc(src4);

    return 0;
}



