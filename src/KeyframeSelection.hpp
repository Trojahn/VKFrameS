#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <iterator>
#include <unistd.h>
#include <algorithm>
#include <thread>

using namespace std;
using namespace cv;

class KeyframeSelection {
	private:
		string videoFile;
		vector< pair<int,int> > shots;
		double similarityThreshold;
		double minSimilarity;
		int nThreads;
		vector< vector<Mat> > extractVideoHistograms();
		double compareHistograms(Mat histogram1, Mat histogram2);
		void extractKeyFrameShot(vector<Mat> histograms, vector<int> &keyframes);
		
	public:
		KeyframeSelection(string videoFile, vector< pair<int,int> > shots, double similarityThreshold, double minSimilarity);
		vector< pair<int,int> > getKeyFrames();
};