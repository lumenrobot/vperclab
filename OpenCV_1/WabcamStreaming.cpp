#include "opencv2\highgui\highgui.hpp";

int main()
{
	cvNamedWindow("Streaming",CV_WINDOW_AUTOSIZE );
	CvCapture *capture = cvCaptureFromCAM(0);
	IplImage* frame;
	while (true)
	{
		frame= cvQueryFrame(capture);
		if (!frame)
			break;
		cvShowImage("Streaming", frame);
		char c = cvWaitKey(33);
		if (c == 27)
			break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("Streaming");
}