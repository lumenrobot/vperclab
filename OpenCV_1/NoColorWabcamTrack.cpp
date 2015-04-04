//motionTracking.cpp

//Written by  Kyle Hounslow, January 2014

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

//****AREAS MARKED WITH ASTERISKS HAVE BEEN MODIFIED BY MARIA RAMOS
//****DECEMBER 2014

#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

//our sensitivity value to be used in the threshold() function
const static int SENSITIVITY_VALUE = 40;                                        //*****************MODIFIED
//size of blur used to smooth the image to remove possible noise and
//increase the size of the object we are trying to track. (Much like dilate and erode)
const static int BLUR_SIZE = 20;                                                //*****************MODIFIED
//we'll have just one object to search for
//and keep track of its position.
int theObject[2] = { 0, 0 };
//bounding rectangle of the object, we will use the center of this as its position.
Rect objectBoundingRectangle = Rect(0, 0, 0, 0);


//int to string helper function
string intToString(int number){

	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}

vector< vector<Point> > lastContours;

void searchForMovement(Mat thresholdImage, Mat &cameraFeed){
	//notice how we use the '&' operator for the cameraFeed. This is because we wish
	//to take the values passed into the function and manipulate them, rather than just working with a copy.
	//eg. we draw to the cameraFeed in this function which is then displayed in the main() function.
	bool objectDetected = false;
	Mat temp;
	thresholdImage.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	//findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);// retrieves external contours

	//if contours vector is not empty, we have found some objects
	if (contours.size()>0)objectDetected = true;
	else objectDetected = false;

	if (objectDetected){
		vector< vector<Point> > largestContourVec;
		//the largest contour is found at the end of the contours vector
		//we will simply assume that the biggest contour is the object we are looking for.
		largestContourVec.push_back(contours.at(contours.size() - 1));
		//make a bounding rectangle around the largest contour then find its centroid
		//this will be the object's final estimated position.
		objectBoundingRectangle = boundingRect(largestContourVec.at(0));
		int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
		int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;

		//update the objects positions by changing the 'theObject' array values
		theObject[0] = xpos, theObject[1] = ypos;

		// save all contours
		lastContours.clear();
		for (int i = 0; i < contours.size(); i++) {
			lastContours.push_back(contours.at(i));
		}
		
	}
	if (!lastContours.empty() )
	{ 
		for (int j = 0; j < lastContours.size(); j++) {
			for (int i = 1; i < lastContours.at(j).size(); i++) {
				Point pt1 = lastContours.at(j).at(i - 1);
				Point pt2 = lastContours.at(j).at(i);
				line(cameraFeed, pt1, pt2, Scalar(0, 63 * (j / 4), 63 *  j, 2));
			}
		}
	}

	//make some temp x and y variables so we dont have to type out so much
	int x = theObject[0];
	int y = theObject[1];
	//draw some crosshairs on the object
	circle(cameraFeed, Point(x, y), 30, Scalar(0, 255, 0), 2);  //******************30 IS THE SIZE OF THE CIRCLE
	line(cameraFeed, Point(x, y), Point(x, y - 30), Scalar(0, 255, 0), 2);  //**********30 IS THE SIZE OF THE CROSS HAIR
	line(cameraFeed, Point(x, y), Point(x, y + 30), Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x - 30, y), Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x + 30, y), Scalar(0, 255, 0), 2);
	putText(cameraFeed, "(" + intToString(x) + "," + intToString(y) + ")", Point(x, y), 1, 1, Scalar(255, 0, 0), 2);//*********MODIFIED


	



}
int main(){

	////some boolean variables for added functionality
	//bool objectDetected = false;
	////these two can be toggled by pressing 'd' or 't'
	//bool debugMode = false;
	//bool trackingEnabled = false;
	////pause and resume code
	//bool pause = false;
	////set up the matrices that we will need
	////the two frames we will be comparing
	//Mat frame1, frame2;
	////their grayscale images (needed for absdiff() function)
	//Mat grayImage1, grayImage2;
	////resulting difference image
	//Mat differenceImage;
	////thresholded difference image (for use in findContours() function)
	//Mat thresholdImage;
	////video capture object.
	//VideoCapture capture;

	//capture.open(0);    //0 = built-in, 1 = external

	//if (!capture.isOpened()){
	//	cout << "ERROR ACQUIRING VIDEO FEED\n";
	//	getchar();
	//	return -1;
	//}

	//while (1){

	//	//we can loop the video by re-opening the capture every time the video reaches its last frame



	//	//check if the video has reach its last frame.
	//	//we add '-1' because we are reading two frames from the video at a time.
	//	//if this is not included, we get a memory error!
	//	//read first frame
	//	capture.read(frame1);
	//	//convert frame1 to gray scale for frame differencing
	//	cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
	//	//copy second frame
	//	capture.read(frame2);
	//	//convert frame2 to gray scale for frame differencing
	//	cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
	//	//perform frame differencing with the sequential images. This will output an "intensity image"
	//	//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
	//	cv::absdiff(grayImage1, grayImage2, differenceImage);
	//	//threshold intensity image at a given sensitivity value
	//	cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
	//	if (debugMode == true){
	//		//show the difference image and threshold image
	//		cv::imshow("Difference Image", differenceImage);
	//		cv::imshow("Threshold Image", thresholdImage);
	//	}
	//	else{
	//		//if not in debug mode, destroy the windows so we don't see them anymore
	//		cv::destroyWindow("Difference Image");
	//		cv::destroyWindow("Threshold Image");
	//	}
	//	//use blur() to smooth the image, remove possible noise and
	//	//increase the size of the object we are trying to track. (Much like dilate and erode)
	//	cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
	//	//threshold again to obtain binary image from blur output
	//	cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
	//	if (debugMode == true){
	//		//show the threshold image after it's been "blurred"
	//		imshow("Final Threshold Image", thresholdImage);
	//	}
	//	else {
	//		//if not in debug mode, destroy the windows so we don't see them anymore
	//		cv::destroyWindow("Final Threshold Image");
	//	}

	//	//if tracking enabled, search for contours in our thresholded image
	//	if (trackingEnabled){
	//		searchForMovement(thresholdImage, frame1);
	//	}
	//	//show our captured frame
	//	imshow("Frame1", frame1);
	//	//check to see if a button has been pressed.
	//	//this 10ms delay is necessary for proper operation of this program
	//	//if removed, frames will not have enough time to referesh and a blank 
	//	//image will appear.
	//	switch (waitKey(10)){                                   //*************MODIFIED

	//	case 27: //'esc' key has been pressed, exit program.
	//		return 0;
	//	case 116: //'t' has been pressed. this will toggle tracking
	//		trackingEnabled = !trackingEnabled;
	//		if (trackingEnabled == false) cout << "Tracking disabled." << endl;
	//		else cout << "Tracking enabled." << endl;
	//		break;
	//	case 100: //'d' has been pressed. this will debug mode
	//		debugMode = !debugMode;
	//		if (debugMode == false) cout << "Debug mode disabled." << endl;
	//		else cout << "Debug mode enabled." << endl;
	//		break;
	//	case 112: //'p' has been pressed. this will pause/resume the code.
	//		pause = !pause;
	//		if (pause == true){
	//			cout << "Code paused, press 'p' again to resume" << endl;
	//			while (pause == true){
	//				//stay in this loop until 
	//				switch (waitKey()){
	//					//a switch statement inside a switch statement? Mind blown.
	//				case 112:
	//					//change pause back to false
	//					pause = false;
	//					cout << "Code resumed." << endl;
	//					break;
	//				}
	//			}
	//		}


	//	}



	//}


////////////////////////////////

cv::Mat cameraMatrix; //intrinsic
cv::Mat distCoeffs;
//cameraMatrix = [2 0 2; 0 2 2; 0 0 1];
cameraMatrix = Mat::eye(3, 3, CV_64F);
cameraMatrix.at<double>(0, 0) = 2.0;
cameraMatrix.at<double>(1, 1) = 2.0;
cameraMatrix.at<double>(0, 2) = 2.0;
cameraMatrix.at<double>(1, 2) = 2.0;
cout << "E = " << endl << " " << cameraMatrix << endl << endl;
distCoeffs=Mat::zeros(8, 1, CV_64F);
cout << "E = " << endl << " " << distCoeffs << endl << endl;

cv::Mat M = camCalib.getIntrinsicsMatrix();
cv::Mat D = camCalib.getDistortionCoeffs();


std::vector<cv::Point2f> imagePoints;
std::vector<cv::Point3f> objectPoints;
//img points are green dots in the picture
imagePoints.push_back(cv::Point2f(271., 109.));
imagePoints.push_back(cv::Point2f(65., 208.));
imagePoints.push_back(cv::Point2f(334., 459.));
imagePoints.push_back(cv::Point2f(600., 225.));

//object points are measured in millimeters because calibration is done in mm also
objectPoints.push_back(cv::Point3f(0., 0., 0.));
objectPoints.push_back(cv::Point3f(-511., 2181., 0.));
objectPoints.push_back(cv::Point3f(-3574., 2354., 0.));
objectPoints.push_back(cv::Point3f(-3400., 0., 0.));

cv::Mat rvec(1, 3, cv::DataType<double>::type);
cv::Mat tvec(1, 3, cv::DataType<double>::type);
cv::Mat rotationMatrix(3, 3, cv::DataType<double>::type);

cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
cv::Rodrigues(rvec, rotationMatrix);


cv::Mat uvPoint = cv::Mat::ones(3, 1, cv::DataType<double>::type); //u,v,1
uvPoint.at<double>(0, 0) = 363.; //got this point using mouse callback
uvPoint.at<double>(1, 0) = 222.;
cv::Mat tempMat, tempMat2;
double s;
tempMat = rotationMatrix.inv() * cameraMatrix.inv() * uvPoint;
tempMat2 = rotationMatrix.inv() * tvec;
s = 285 + tempMat2.at<double>(2, 0); //285 represents the height Zconst
s /= tempMat.at<double>(2, 0);
std::cout << "P = " << rotationMatrix.inv() * (s * cameraMatrix.inv() * uvPoint - tvec) << std::endl;

	return 0;

}