#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	string path =  "/home/cfcv/Desktop/git/Face_Recognition/src_get/personnes/IMG_";
	string fn_haar = string("/home/cfcv/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt.xml");
	int count = 100;
	int count_fixed = 0;
	int choice;
	cv::Mat frame; 
	cv::namedWindow("Readed", cv::WINDOW_NORMAL);
	cv::namedWindow("Cutted", cv::WINDOW_NORMAL);
	cv::CascadeClassifier face_cascade;
    cv::vector<cv::Rect> faces;
    cv::Mat face_area;

	face_cascade.load(fn_haar);
	
	while(true){
		ostringstream convert;
		convert << count;
		frame = cv::imread(path+convert.str()+".jpg",CV_LOAD_IMAGE_COLOR);

		if(frame.empty()){
			break;
		}
		//cv::cvtColor(frame, gray, CV_BGR2GRAY);
		face_cascade.detectMultiScale(frame, faces);
		cout << "faces -> " << faces.size() << endl;
		if(faces.size() == 1){
			cout << faces[0] << endl;
			face_area = frame(faces[0]);

			ostringstream convert3;
			convert3 << count_fixed;
			cv::imwrite("/home/cfcv/Desktop/git/Face_Recognition/src_get/personnes/fixed/IMG_"+convert3.str()+".jpg", face_area);
			count_fixed++;
		}
		else if(faces.size() > 1){
			cv::Mat aux = frame.clone();
			for(int i = 0; i < faces.size(); ++i){
				ostringstream convert2;
				convert2 << i;
				string box_text = convert2.str();
				int pos_x = std::max(faces[i].tl().x - 10, 0);
            	int pos_y = std::max(faces[i].tl().y - 10, 0);
            	cv::putText(aux, box_text, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0,255,0), 2.0);
				cv::rectangle(aux, faces[i], cv::Scalar(0,255,0), 1);
			}
			cv::imshow("Readed", aux);
			cout << "What is yout face?" << endl;
			cin >> choice;
			face_area = frame(faces[choice]);
			
			cv::imshow("Cutted", face_area);
			cv::waitKey(0);
			
			ostringstream convert4;
			convert4 << count_fixed;
			cv::imwrite("/home/cfcv/Desktop/git/Face_Recognition/src_get/personnes/fixed/IMG_"+convert4.str()+".jpg", face_area);
			count_fixed++;
		}
		//cv::imshow("Cutted", face_area);
		//cv::imshow("Readed", frame);
		//cv::waitKey(0);
		count++;
	}
	return 0;
}