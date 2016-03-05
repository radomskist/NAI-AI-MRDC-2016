#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <cstdlib>


//using namespace std;
//using namespace cv;

std::vector<cv::Vec3i> postFind(cv::Mat &source)
{
  /*
  returns a cv::Vec3i where the i bit is (x , y) , confidence
  confidence is an integer representing the number of lines averaged to 
  return that base point.  The higher the confidence the more likely a
  post is there.  Can either add programming to cull out low confidence points
  in this program or in main;


  Possible to do:
  have multiple passes at different saturation and indicate different confidence
  scale expected height by distance from cam
  experiment with best way to merge lines from same post

  Notes:
  uncomment line before push_back for our output statement to leave out lines that
  lack confidence of 2 or more. 
  */ 
  cv::Mat postRange, postCan, sourceGray; //create images

  //if getting an image in grayscale already then this is unnecessary
  cv::cvtColor(source, sourceGray, CV_BGR2GRAY);
  //args (source img, array upper, array lower, output)

  //inRange(fullImageHSV, Scalar(0,0,0), Scalar (180,255,255), postRange);
  //let's try thresholding instead
  
  /* 0: Binary
    1: Binary Inverted
    2: Threshold Truncated
    3: Threshold to Zero
    4: Threshold to Zero Inverted
  */  
  //threshold( src, dst, threshold_value, max_BINARY_value,threshold_type );
  cv::threshold( sourceGray, postRange, 30, 255, 0);

  cv::Canny(postRange, postCan, 50, 200, 3);  //canny detector to find edges

  //makes a std::vector of 4 int to store the form of Xo, Yo, X1, Y1
  std::vector<cv::Vec4i> lines;

  //source img, vec to store in, resolution in pixels, angle in rads, intersections, min length, min gap
  cv::HoughLinesP(postCan, lines, 1, CV_PI/180, 30, 55, 10);

  cv::Vec4i line1, line2; //vec to store produced points in

  //line1 = lines[0];
  //lines[0] = line1;  
  //remove in final is for visualization
  //cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC1);  //make a cv::Mat to store output in 500 is arbitrary
  
  std::vector<cv::Vec4i> processedLines; //need to process the lines to remove multiples
  int lastx = 0;
  int uniquePosts = 0;
  int count = 1;
  std::vector<int> total;
  
  for( size_t i=0; i < lines.size(); i++)
  {
    line1 = lines[i];

 //general logic for this loop is if the change in x < the height of a post it's likely not a post
    //so then we average together our post lines and get a good number for the base
    //also the total represents our confidence/ number of lines generated

    //cv::Mat img;
    //no need to draw, is simply visual, uncomment 4th for that output
    //loop to draw produced lines remove in final since this is a visual thing only
    //img, pt1, pt2, color, thickness, type, shift
    //line( img, Point(line1[0], line1[1]), Point(line1[2], line1[3]), Scalar(128,255,255), 3, LINE_AA);
    //cout << endl << "lines: " << l[0] << l[1] << l[2] << l[3];
    //cout << endl << "(" << line1[0] << " , " << line1[1] << ")";

    if(uniquePosts == 0)
    {
      processedLines.push_back(line1);
      total.push_back(1); //set up the counter for this post
      uniquePosts++;
      lastx = line1[0];

    }
    else if(abs(line1[2]-line1[0]) >= abs(line1[3]-line1[1])) //should catch horizontal lines
    {
      //this is a horizontal line
      //this space is intentionally blank
    }
    else if(abs(line1[0] - lastx) <= abs(line1[3] - line1[1]))
    {
      //case if this is another line in our post
      line2 = processedLines[uniquePosts - 1];
      for(size_t n = 0; n < 3; n++)
      {

        line1[n] += line2[n];

      } 
      processedLines.pop_back();
      processedLines.push_back(line1); //store the sum in the our processed lines

      count ++; //increment count
      total.pop_back();
      total.push_back(count);
      lastx = line1[0];
    }
    else
    {
      processedLines.push_back(line1);
      total.push_back(1);
      uniquePosts ++;
      count = 1;
      lastx = line1[0];
    }

  }
  
  //could easily avoid using these variables if the assignment statements are making this
  //innefficient
  int x, y, t, deltaY, deltaX; //x final, y final, total
  std::vector<cv::Vec3i> lineOut;
  cv::Vec3i basePoint;
  int slope = 0;  //slope is actually c^2
  for(size_t i = 0; i < processedLines.size(); i++)
  {
    line1 = processedLines[i];

    //cv::line( postRange, cv::Point((line1[0]/t), (line1[1]/t)), cv::Point((line1[2] / t), (line1[3]/t)), cv::Scalar(128,255,255), 3, cv::LINE_AA);

    x = line1[0] / total[i]; //average of xs
    y = line1[1] / total[i]; //average of ys
    t = total[i];  //confidence number
    deltaX = (line1[2] / t - line1[0] / t);
    deltaY = (line1[3] / t - line1[1] / t);
    
    //now to straighten the lines
    if(deltaX > 0)
    {
      x = x + .5 * deltaX;      
    }
    else
    {
      x = x - .5 * deltaX;  
    }
    
    if(deltaY > 0)
    {
      y = y + .5 * abs(deltaY);  
    }
    else
    {
      y = y - .5 * abs(deltaY);  
    } 

    slope = .5 * sqrt( pow( deltaX, 2.0) + pow( deltaY , 2.0));
 
    //first one adds 1/2 delta x to x,
    //next we add 1/2 delta y to y0
    //then we subtract 1/2 c^2 from this new y center
    //this gives us a new basepoint translated as if the line was vertical
    basePoint[0] = x;

    basePoint[1] = y + slope;
    basePoint[2] = t;
  
    //cv::line( postCan, cv::Point(basePoint[0], basePoint[1]), cv::Point((basePoint[0]), (y - slope)), cv::Scalar(128,255,255), 3, cv::LINE_AA);
    //if(basePoint[2] >= 2)
      lineOut.push_back(basePoint);   
  }
  //imshow("source", source); //should show source then lines we detect
  //namedWindow( "Display window", WINDOW_AUTOSIZE );
  //imshow("detected lines", postLines);
  //cv::imshow("source", postRange); 
  //cv::imshow("postCan", postCan);
  //imshow("source", postRange);

  //cv::waitKey();
  return lineOut;
}

/*
int main()
{

  cv::Mat image;
  image = cv::imread("posts2.jpeg", 1); //read in test image

  std::vector<cv::Vec3i> ayy; //store produced std::vector
  ayy = postFind(image); //load in image
  //cout << ayy;


  return 0;
}*/


