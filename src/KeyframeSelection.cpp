#include "KeyframeSelection.hpp"
#include "Utils.hpp"

KeyframeSelection::KeyframeSelection(string videoFile, vector< pair<int,int> > shots, double similarityThreshold, double minSimilarity) {
	this->videoFile = videoFile;
	this->shots = shots;
	this->similarityThreshold = similarityThreshold;
	this->minSimilarity = minSimilarity;
	this->nThreads = thread::hardware_concurrency();
}

void KeyframeSelection::extractKeyFrameShot(vector<Mat> histograms, vector<int> &keyframes) {
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
	
	vector<int> kf;
	
	while(true) {
		vector<int>::iterator maxVal = max_element(similar.begin(), similar.end());
		int maxIndex = std::distance(similar.begin(), maxVal);
		
		if(*maxVal < histograms.size() * this->minSimilarity && kf.size() > 0) {
			break;
		}
		
		Mat candidate = histograms[maxIndex];
		similar[maxIndex] = 0;
		
		bool aux = true;
		for(int i = 0; i < kf.size(); i++) {
			if(compareHistograms(candidate,histograms[kf[i]]) >= this->similarityThreshold) {
				aux = false;
			}
		}
		if(aux) {
			kf.push_back(maxIndex);
		}		
	}
	
	keyframes = kf;
}

vector< pair<int,int> > KeyframeSelection::getKeyFrames() {
	vector< vector<Mat> > histograms = this->extractVideoHistograms();
	vector< vector<int> > keyframes(histograms.size());
	
	vector<thread> pool;	
	for(int i = 0; i < histograms.size(); i++) {
		if(pool.size() >= this->nThreads) {
			for(int i = 0 ; i < pool.size(); i++) {
				pool[i].join();
			}
			pool.clear();
		}
		pool.push_back(thread(&KeyframeSelection::extractKeyFrameShot, this, histograms[i], std::ref(keyframes[i])));
	}	
	for(int i = 0 ; i < pool.size(); i++) {
		pool[i].join();
	}
	pool.clear();
	
	
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
	Mat frame;
	VideoCapture capture(videoFile);
	if(!capture.isOpened()) {
		cout << "ERROR! The video input file could not be read by OpenCV!" << endl;
		exit(1);
	}
	
	vector<thread> pool;	
	vector< pair<int,Mat> > tempHist;
	
	for(int i = 0; capture.read(frame); i++) {
		if(this->shots.size() == 0) {
			if(pool.size() >= this->nThreads) {	
				for(int i = 0 ; i < pool.size(); i++) {
					pool[i].join();
				}
				pool.clear();
			} 
			Mat t = Mat();
			frame.copyTo(t);
			pool.push_back(thread(&Utils::extractHistogram, t, i, std::ref(tempHist)));
		} else {
			if(i >= this->shots[this->shots.size()-1].second){
				break;
			}
			for(int s = 0; s < this->shots.size(); s++) {
				if(i < this->shots[s].first) {
					break;
				}
				if(this->shots[s].first <= i && i <= this->shots[s].second) {
					if(pool.size() >= this->nThreads) {
						for(int i = 0 ; i < pool.size(); i++) {
							pool[i].join();
						}
						pool.clear();
					}	
					Mat t = Mat();
					frame.copyTo(t);			
					pool.push_back(thread(&Utils::extractHistogram, t, i, std::ref(tempHist)));
				}
			}
		}		
	}
	for(int i = 0 ; i < pool.size(); i++) {
		pool[i].join();
	}
	
	std::sort(tempHist.begin(), tempHist.end(), Utils::pairCompare);
	vector< vector<Mat> > ret(std::max(1,(int)this->shots.size()), vector<Mat>());
	
	if(this->shots.size() == 0) {
		for(pair<int, Mat> p : tempHist) {
			ret[0].push_back(p.second);	
		}
	} else {
		for(pair<int, Mat> p : tempHist) {
			for(int s = 0; s < this->shots.size(); s++) {
				if(this->shots[s].first <= p.first && p.first <= this->shots[s].second) {
					ret[s].push_back(p.second);
					break;
				}
			}
		}
	}
	
	
	return ret;
}