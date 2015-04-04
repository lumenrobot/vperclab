#include <opencv2\highgui\highgui.hpp>

int main()
{
	cvNamedWindow("Video Display", CV_WINDOW_AUTOSIZE);
	CvCapture * capture = cvCreateFileCapture("C:\\Users\\sigit_000\\Downloads\\Video\\Opening a Video with Visual Studio 2013 and OpenCV 2.4.10.mp4");
	IplImage* frame;
	while (true)
	{
		frame = cvQueryFrame(capture);
		if (!frame)
			break;
		cvShowImage("Video Display", frame);
		char c = cvWaitKey(20);
		if (c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	//cvDestroyWindow()

	return 0;
}