#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace cv;

class Utils {
	public:
		static Mat extractHistogram(Mat frame);
		static void writeOutputFile(string outFile, vector< pair<int,int> > keyframes);
		static bool checkFile(string name);
		static bool checkOutputFile(string name);
		static vector< pair<int,int> > parseCSV(string filePath);
		static vector< pair<int,int> > normalizePairs(vector< pair<int,int> > pairs, int val);
};