# Introduction
VKFrameS (**V**ideo **K**ey**frames** **S**elector) is a video shot keyframes selector which aims to provide a set of unique keyframes for a given shot-segmented video. 

# Requirements
*   A C++11 compiler, like g++
*   A functional OpenCV installation (2.4.x and 3.1 compatible)
*   An appropriate set of CODECs

# Arguments
1.  The video from which the keyframes will be selected.
2.	The desired number of keyframes for each video shot/segment. 0 for *auto*.
3.  The CSV ouput file which describe the selected keyframes.
4.  (Optional) A CSV file with video shot segmentation.

# Output
VKFrameS will save its output into a CSV file (the third provided argument). In the output file, each line corresponds to a selected keyframe. 
The first value (before the comma) is the corresponding shot number. The second value (after the comma) is the global frame number of the keyframe. Both the first frame and shot are represented as '1'.

VKFrameS may select multiple keyframes for each shot. In this case, the printing order reflects the "*quality*" of each keyframe: the first shot keyframe is "*better*" than the second keyfame, and so on. VKFrames will always select at least one keyframe for each non-empty shot.

If it is specified a particular number of keyframes (a non zero value in the second argument), the VKFrameS will do its best to return the specified number of keyframes for each shot/segment. Please do note that if the number of desired keyframes is larger than the shot/segmente length, 
the program will fallback to the *auto* behaviour (variable keyframe number, at least one for each shot/segment) for THAT particular shot/segment (a warning will alert you of this).

Has it is not provided a shot segmentation (the last optional parameter), all detected keyframes will be presented as **1,N**, where **N** is the frame number.

# Examples
#### Using a shot.csv file with the 'video.avi' shot segmentation. 
		$ ./VKFrameS video.avi 0 output.csv shots.csv
		$ cat shots.csv
			1,240
			241,260
			272,290
			291,313
			320,325
			326,700
		$ cat output.csv
			1,123
			1,43
			2,258
			3,288
			3,289
			4,296
			5,321
			6,440
			6,599
			6,382
			
#### Using a shot.csv file with the 'video.avi' shot segmentation. Selecting only one keyframe. 
		$ ./VKFrameS video.avi 1 output.csv shots.csv
		$ cat shots.csv
			1,240
			241,260
			272,290
			291,313
			320,325
			326,700
		$ cat output.csv
			1,123
			2,258
			3,288
			4,296
			5,321
			6,440
		
#### Without the shot segmentation
		$ ./VKFrameS video.avi 0 output.csv
		$ cat output.csv
			1,76
			1,115
			1,296
			1,440

# Publications
VKFrameS was mentioned, described or used in the following publications:

de Souza, T. T. and Goularte, R., 2013. Video shot representation based on histograms. In *28th Annual ACM Symposium on Applied Computing*. ACM: New York, NY, USA, pp. 961--966.

Trojahn, T. H. and Goularte, R., 2013. Video Scene Segmentation by Improved Visual Shot Coherence. In *19th Brazilian Symposium on Multimedia and the Web*. ACM: New York, NY, USA, pp. 23--30.

Trojahn, T. H., 2014. *Automatic scenes video segmentation based on shot coherence*. M.S. thesis. Universidade de São Paulo. Avaliable in http://www.teses.usp.br/teses/disponiveis/55/55134/tde-20052014-152446/. (In portuguese).

# Authors
VKFrameS was developed by Tiago H. Trojahn under the coordination of Rudinei Goularte

