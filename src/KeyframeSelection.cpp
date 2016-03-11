#include "KeyframeSelection.hpp"
#include "Utils.hpp"

KeyframeSelection::KeyframeSelection(string videoFile, vector< pair<int,int> > shots, double similarityThreshold, double minSimilarity) {
	this->videoFile = videoFile;
	this->shots = shots;
	this->similarityThreshold = similarityThreshold;
	this->minSimilarity = minSimilarity;
}

vector<int> KeyframeSelection::extractKeyFrameShot(vector<Mat> histograms) {
	vector<int> similar(histograms.size(),0);
	
	/*
		Calculate the histograms' similarity among the frames.
	*/
	for(int i = 0; i < histograms.size(); i++) {
		for(int j = i + 1; j < histograms.size(); j++) {
			if(compareHistograms(histograms[i],histograms[j]) >= this->similarityThreshold) {
				similar[i] = similar[i] + 1;
				similar[j] = similar[j] + 1;
			}
		}
	}
	
	vector<int> keyframes;
	
	while(true) {
		vector<int>::iterator maxVal = max_element(similar.begin(), similar.end());
		int maxIndex = std::distance(similar.begin(), maxVal);
		
		if(*maxVal < histograms.size() * this->minSimilarity && keyframes.size() > 0) {
			break;
		}
		
		Mat candidate = histograms[maxIndex];
		similar[maxIndex] = 0;
		
		bool aux = true;
		for(int i = 0; i < keyframes.size(); i++) {
			if(compareHistograms(candidate,histograms[keyframes[i]]) >= this->similarityThreshold) {
				aux = false;
			}
		}
		if(aux) {
			keyframes.push_back(maxIndex);
		}		
	}
	
	return keyframes;	
}

vector< pair<int,int> > KeyframeSelection::getKeyFrames() {
	vector< vector<Mat> > histograms = this->extractVideoHistograms();
	vector< vector<int> > keyframes;
	for(int i = 0; i < histograms.size(); i++) {
		keyframes.push_back(extractKeyFrameShot(histograms[i]));
	}
	vector< pair<int,int> > ret;	

	for(int i = 0; i < keyframes.size(); i++) {
		for(int j = 0; j < keyframes[i].size(); j++) {
			if(this->shots.size() > 0) {
				ret.push_back(make_pair(i, keyframes[i][j] + this->shots[i].first));
			} else {
				ret.push_back(make_pair(i,keyframes[i][j]));
			}			
		}
	}	
	return ret;
}

double KeyframeSelection::compareHistograms(Mat histogram1, Mat histogram2) {
	return compareHist(histogram1,histogram2,CV_COMP_INTERSECT);
}

vector< vector<Mat> > KeyframeSelection::extractVideoHistograms() {
	vector<Mat> histograms;
	Mat frame;
	VideoCapture capture(videoFile);
	if(!capture.isOpened()) {
		cout << "ERROR! The video input file could not be read by OpenCV!" << endl;
		exit(1);
	}
	
	vector< vector<Mat> > ret(std::max(1,(int)this->shots.size()), vector<Mat>());

	
	for(int i = 0; capture.read(frame); i++) {
		cvtColor(frame,frame,CV_BGR2HSV);
		/*
		Used when there is not a shot segmentation input.
		*/
		if(this->shots.size() == 0) {			
			ret[0].push_back(Utils::extractHistogram(frame));
		} else {
			/*
			If there are frames after the last shots, ignore then...
			*/
			if(i >= this->shots[this->shots.size()-1].second){
				break;
			}			
			/*
			For every frame, check the corresponding shot.
			*/
			for(int s = 0; s < this->shots.size(); s++) {
				/*
				The frame is not in any shot!
				*/
				if(i < this->shots[s].first) {
					break;
				}
				if(this->shots[s].first <= i && i <= this->shots[s].second) {
					ret[s].push_back(Utils::extractHistogram(frame));
				}
			}
		}		
	}
	frame.release();
	capture.release();
	return ret;
}
