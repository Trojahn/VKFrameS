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
	if(argc < 4 || argc > 5) {
		cout << "Incorrect parameter count." << endl;
		cout << "Please, use './VKFrames <videoFilePath> <nKeyframes> <outputFilePath.csv> [<videoShotSegmentation.csv>]'" << endl;
		return 1;
	}	
	
	string videoPath = string(argv[1]);
	if(!Utils::checkFile(videoPath)) {
		cout << "The videoFilePath seems to be invalid or cannot be read" << endl;
		return 1;
	}
	
	int nKeyframes = atoi(argv[2]);
	string outputPath = string(argv[3]);
	if(Utils::checkFile(outputPath)) {		
		while(true) {
			string in;
			cout << "File '" << outputPath << "' already exists. Overwrite [y/N]? ";
			getline(std::cin,in);
			if(in == "Y" || in == "y") {
				break;
			}
			return 0;
		}
	} else {
		if(!Utils::checkOutputFile(outputPath)) {
			cout << "The outputFilePath seems to be invalid or cannot be written" << endl;
			return 1;
		}
	}
	vector< pair<int,int> > shots;
	if(argc == 5) {
		string inputCSV = string(argv[4]);
		if(!Utils::checkFile(inputCSV)) {
			cout << "The videoShotSegmentation seems to be invalid or cannot be read. Ignoring." << endl;			
		} else {
			/* The method requires that '0' be the first frame of the shot segmentation. This enforces it! */
			shots = Utils::parseCSV(inputCSV);
			int displacement = shots[0].first;
			shots = Utils::normalizePairs(shots,-displacement);
		}		
	}
	bool check = false;
	for(int i = 0; i < shots.size(); i++) {
		if(shots[i].second == shots[i].first) {
			cout << "WARNING: The " << (i+1) << " contains only a single frame (" << shots[i].first << "," << shots[i].second << "). " << endl;
		}
		if(shots[i].second < shots[i].first) {
			cout << "ERROR: The " << (i+1) << " shot seems broken (" << shots[i].first << "," << shots[i].second << "). " << endl;
			check = true;
		}
	}
	if(check) {
		exit(1);
	}

	KeyframeSelection kfs(videoPath, shots, SIMILARITY_THRESHOLD, MIN_SIMILARITY, nKeyframes);
	vector< pair<int,int> > keyframes = Utils::normalizePairs(kfs.getKeyFrames(),1);
	Utils::writeOutputFile(outputPath,keyframes);	
	return 0;
}
