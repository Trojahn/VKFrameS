#include "Utils.hpp"

Mat Utils::extractHistogram(Mat frame){
	float Hsize[] = {0,180};
	float Ssize[] = {0,256};
	float Vsize[] = {0,256};
	const float* space[] = {Hsize, Ssize, Vsize};
	int channels[] = {0,1,2};
	int Hbins = 8;
	int Sbins = 4;
	int Vbins = 4;
	int histogramBins[] = {Hbins, Sbins, Vbins};
	Mat histogram;

	calcHist(&frame,1,channels,Mat(),histogram,3,histogramBins,space);
	histogram = histogram / cv::sum(histogram)[0];

	return histogram;
}

void Utils::writeOutputFile(string outFile, vector< pair<int,int> > keyframes) {
	ofstream file (outFile.c_str());
	for(int i = 0; i < keyframes.size(); i++) {
		file << keyframes[i].first << "," << keyframes[i].second << endl;
	}
	file.close();
}

bool Utils::checkFile(string name) {
  if(FILE *file = fopen(name.c_str(),"r")) {
		fclose(file);
		return true;
	}
	return false;
}

bool Utils::checkOutputFile(string name) {
	if(FILE *file = fopen(name.c_str(),"w")) {
		fclose(file);
		return true;
	}
	return false;	
}

vector< pair<int,int> > Utils::normalizePairs(vector< pair<int,int> > pairs, int val) {
	for(int i = 0; i < pairs.size(); i++) {
		pairs[i].first = pairs[i].first + val;
		pairs[i].second = pairs[i].second + val;
	}
	return pairs;
}

vector< pair<int,int> > Utils::parseCSV(string filePath) {
	ifstream file(filePath.c_str());
	string line;
	string token;
	vector< pair<int,int> > ret;
	
	if(file.is_open()) {
		while(getline(file,line)) {
			istringstream str(line);
			vector<int> temp;
			while(getline(str,token,',')) {
				temp.push_back(atoi(token.c_str()));
			}
			ret.push_back(make_pair(temp[0],temp[1]));
			temp.clear();
		}
	}	
    file.close();
	return ret;
}