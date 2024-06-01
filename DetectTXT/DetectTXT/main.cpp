#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


using namespace std;
using namespace  cv;

void FLD(Mat& image);
Mat FindArea(Mat& image);
Mat selectColor(Mat& src);
Mat selectRed (Mat& src);
void TextDecetc(Mat& src);
Mat selectGlare(Mat& src);

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

void FLD(Mat& image)
{
    cvtColor(image, image, COLOR_BGR2GRAY);
    int length_threshold = 10;
    float distance_threshold = 1.41421356f;
    double canny_th1 = 30.0;
    double canny_th2 = 90.0;
    int canny_aperture_size = 3;
    bool do_merge = true;
    Ptr<cv::ximgproc::FastLineDetector> fld = cv::ximgproc::createFastLineDetector(length_threshold,
    distance_threshold, canny_th1, canny_th2, canny_aperture_size,
    do_merge);
    vector<Vec4f> lines_fld;
    fld->detect(image, lines_fld);
    Mat line_image_fld(image);
    fld->drawSegments(line_image_fld, lines_fld);
    imshow("FLD result", line_image_fld);
}

Mat FindArea(Mat& image)
{
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
    convexHull( contours[0], hull[0] );
    int mostBigContour = hull[0].size();
    for( size_t i = 1; i < contours.size(); i++ )
    {
        convexHull( contours[i], hull[i] );

    }

    for(size_t i = 0; i < contours.size(); i++)
    {
        if (hull[i].size() < mostBigContour)
        {
            hull[i].clear();
        }
        else
        {
            mostBigContour = hull[i].size();
        }
    }



    Mat drawing = Mat::zeros(grad.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        //drawContours( drawing, contours, (int)i, color );

        drawContours( drawing, hull, (int)i, color );
    }
    imshow("hull", drawing);

    return drawing;
}

Mat selectColor(Mat& src)
{
    cv::Mat procImage_BGR = src.clone();
    cv::Mat procImage_HSV = src.clone();

    cv::cvtColor(procImage_BGR, procImage_HSV, cv::COLOR_BGR2HSV);

    int H = 0, S = 2, V = 1;
    int R = 2 , G = 1 , B = 0;

    Mat binary = src.clone();
    for (size_t X = 0; X <= binary.cols; X++)
    {
        for (size_t Y = 0; Y <= binary.rows; Y++ )
        {
            binary.at<cv::Vec3b>(Y,X)[R] = 0;
            binary.at<cv::Vec3b>(Y,X)[G] = 0;
            binary.at<cv::Vec3b>(Y,X)[B] = 0;
        }
    }


    cvtColor(procImage_BGR, procImage_BGR, COLOR_BGR2HSV);
    Mat channels[3];
    split(procImage_BGR,channels);
    Ptr<CLAHE> pClahe = createCLAHE(90, Size(5,5));
    pClahe->apply(channels[2],channels[2]);
    merge(channels, 3, procImage_BGR);
    cvtColor(procImage_BGR,procImage_BGR, COLOR_HSV2BGR);
    imshow("pCLAHE", procImage_BGR);

    medianBlur(binary, binary, 5);


    for (size_t X = 0; X < procImage_BGR.cols; X++)
    {
        for (size_t Y = 0; Y < procImage_BGR.rows; Y++ )
        {

//           bool is_red = (procImage_HSV.at<cv::Vec3b>(Y,X)[H] <= (255/360)*30) && (procImage_HSV.at<cv::Vec3b>(Y,X)[H] <= (255/360)*0)
//                        || ((procImage_HSV.at<cv::Vec3b>(Y,X)[H] >= (255/360)*330) && (procImage_HSV.at<cv::Vec3b>(Y,X)[H] <= (255/360)*360) );


            bool red = (procImage_BGR.at<cv::Vec3b>(Y,X)[R] >= 90 && procImage_BGR.at<cv::Vec3b>(Y,X)[R] <= 200);
           bool blue = (procImage_BGR.at<cv::Vec3b>(Y,X)[B] <= 50);
           bool green = (procImage_BGR.at<cv::Vec3b>(Y,X)[G] <= 60);


            if(red && blue && green)
            {
                binary.at<cv::Vec3b>(Y,X)[R] = 255;
                binary.at<cv::Vec3b>(Y,X)[G] = 255;
                binary.at<cv::Vec3b>(Y,X)[B] = 255;
            }
        }
    }



//imshow("BINARY", binary);
    morphologyEx(binary,binary, MorphTypes::MORPH_CLOSE,
    getStructuringElement(MorphShapes::MORPH_CROSS,Size(3,3)),
    Point(-1,-1), 1);

    morphologyEx(binary,binary, MorphTypes::MORPH_CLOSE,
    getStructuringElement(MorphShapes::MORPH_CROSS,Size(3,3)),
    Point(-1,-1), 1);

    morphologyEx(binary,binary, MorphTypes::MORPH_CLOSE,
    getStructuringElement(MorphShapes::MORPH_CROSS,Size(5,5)),
    Point(-1,-1), 1);

//    morphologyEx(binary,binary, MorphTypes::MORPH_GRADIENT,
//    getStructuringElement(MorphShapes::MORPH_CROSS,Size(5,5)),
//    Point(-1,-1), 1);

    cvtColor(binary, binary, COLOR_BGR2GRAY);
    //Canny(binary, binary, 50, 200, 3);

    FLD(binary);

    vector<Vec2f> lines;
    HoughLines(binary, lines, 1, CV_PI/180, 150, 0, 0);








//


    imshow("MORF", binary);
//    imshow("HSV", procImage_HSV);
//    imshow("BGR", procImage_BGR);

    return binary;
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
           bool red_S = (procImage_HSV.at<cv::Vec3b>(Y,X)[S] >= 140) && (procImage_HSV.at<cv::Vec3b>(Y,X)[S] <= 255);
           bool red_V = (procImage_HSV.at<cv::Vec3b>(Y,X)[V] >= 160) && (procImage_HSV.at<cv::Vec3b>(Y,X)[V] <= 255);

            if(is_red && red_S && red_V)
            {
                binary.at<cv::Vec3b>(Y,X)[R] = 255;
                binary.at<cv::Vec3b>(Y,X)[G] = 0;
                binary.at<cv::Vec3b>(Y,X)[B] = 0;
            }
        }
    }

    imshow("RED", binary);
    return binary;
}

Mat selectGlare(Mat& src)
{
    int H = 0, S = 2, V = 1;
    int R = 2 , G = 1 , B = 0;


    Mat tmp = src.clone();
    cvtColor(src, tmp, COLOR_BGR2GRAY);


    for (size_t X = 0; X < tmp.cols; X++)
    {
        for (size_t Y = 0; Y < tmp.rows; Y++ )
        {
            if (tmp.at<uchar>(Y,X) >= 130)
            {
                tmp.at<uchar>(Y,X) = 100;
            }

        }
    }

    medianBlur(tmp, tmp, 3);


    cvtColor(tmp,tmp, COLOR_GRAY2BGR);
    imshow("GLARE", tmp);
    return tmp;

}

void TextDecetc(Mat& src)
{
    imshow("Src", src);
    Mat imageProc = src.clone();


   // selectGlare(imageProc);

    Mat  binary = selectRed(imageProc);


//        morphologyEx(binary,binary, MorphTypes::MORPH_ERODE,
//        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
//        Point(-1,-1), 2);



        morphologyEx(binary,binary, MorphTypes::MORPH_DILATE,
        getStructuringElement(MorphShapes::MORPH_RECT,Size(3,3)),
        Point(-1,-1), 5);









    auto temp = FindArea(binary);


    imshow("Resul", binary);
    cv::waitKey(0);
}
