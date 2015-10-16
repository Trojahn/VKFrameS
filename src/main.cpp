#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Utils.hpp"
#include "KeyframeSelection.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;
using namespace cv;

double SIMILARITY_THRESHOLD = 0.95;
double MIN_SIMILARITY = 0.2;

int main(int argc, char* argv[]) {
	if(argc < 3 && argc > 4) {
		cout << "Incorrect parameter count." << endl;
		cout << "Please, use './VKFrames <videoFilePath> <outputFilePath.csv> [<videoShotSegmentation.csv>]'" << endl;
		return 1;
	}	
	
	string videoPath = string(argv[1]);
	if(!Utils::checkFile(videoPath)) {
		cout << "The videoFilePath seems to be invalid or cannot be read" << endl;
		return 1;
	}
	
	string outputPath = string(argv[2]);
	if(Utils::checkFile(outputPath)) {		
		while(true) {
			string in;
			cout << "File '" << outputPath << "' already exists. Overwrite ? [y/N]" << endl;
			getline(std::cin,in);
			if(in == "Y" || in == "y") {
				break;
			}
			if(in == "" || in == "N" || in == "n") {
				return 1;
			}
		}
	} else {
		if(!Utils::checkOutputFile(outputPath)) {
			cout << "The outputFilePath seems to be invalid or cannot be written" << endl;
			return 1;
		}
	}
	
	vector< pair<int,int> > shots;
	if(argc == 4) {
		string inputCSV = string(argv[3]);
		if(!Utils::checkFile(inputCSV)) {
			cout << "The videoShotSegmentation seems to be invalid or cannot be read. Ignoring." << endl;			
		} else {
			shots = Utils::normalizePairs(Utils::parseCSV(inputCSV),-1);
		}		
	}
	
	KeyframeSelection kfs(videoPath, shots, SIMILARITY_THRESHOLD, MIN_SIMILARITY);
	vector< pair<int,int> > keyframes = Utils::normalizePairs(kfs.getKeyFrames(),1);
	
	Utils::writeOutputFile(outputPath,keyframes);	
	return 0;
}