//TODO: mudar parâmetros do face detection para a câmera da rasp
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){	
	cv::VideoCapture camera(0);
	cv::Mat frame, gray;
	cv::Point center;

	long int faceArea, imgArea, squareSide;
	int count = 100;
	string path = "/home/cfcv/Desktop/git/Face_Recognition/src_get/personnes/";
	string aux;
	fstream f;

	f.open("csv.txt");

	cout << "Instruções: " << endl;
	cout << "\t1 - Posicione o rosto dentro do quadrado." << endl;
	cout << "\t2 - Aperte Enter quando quiser tirar a foto" << endl;
	cout << "\t3 - Aperte p quando quiser parar" << endl;
	cout << "OB.: Com o objetivo de diversificar a base de dados \nMova a cabeça para pegar diversos ângulos do rosto e\nfaça expreções variadas" << endl;
	cout << "Entendido?";
	cin >> aux;

	int face_id = 0;
	cv::namedWindow("Get_Face", CV_WINDOW_NORMAL);
	cv::namedWindow("Gray", CV_WINDOW_NORMAL);
	//cv::namedWindow("Print", CV_WINDOW_NORMAL);

	while(true){
		//Lê imagem da câmera e armazena em frame
		camera >> frame;

		
		imgArea = frame.cols*frame.rows;
		faceArea = imgArea*0.3;
		squareSide = sqrt(faceArea);
		center.x = frame.rows/3;
		center.y = frame.cols/6;

		cv::Rect faceRect(center.x, center.y, squareSide, squareSide);
		cv::rectangle(frame, faceRect, cv::Scalar(0,255,0), 1);

		//Transforma para tons de cinza(apenas 1 canal)
		cv::cvtColor(frame, gray, CV_BGR2GRAY);
		cv::imshow("Gray", frame);
		cv::imshow("Get_Face", gray);
		

		int key = cv::waitKey(33);

		//Tecla enter
		if(key == 1048586){
			cout << "Enter pressionado" << endl;
			cv::Mat face = gray(faceRect);
			cv::imshow("Print", face);

			ostringstream convert;
			convert << count;
			
			cv::imwrite("IMG_"+convert.str()+".jpg", face);
			count++;
			//cv::waitKey(0);
		}
		//tecla p
		if(key == 1048688){
			break;
		}
		
		//Debug
		//cout << key << endl;
	}
	return 0;
}