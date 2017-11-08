#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <fstream>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int main(int argc, const char *argv[]) {
    string fn_haar = string("/home/cfcv/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt.xml");
    string fn_csv = string("/home/cfcv/Desktop/git/Face_Recognition/src_reco/csv.txt");
    int deviceId = atoi("0");
    vector<Mat> images;
    vector<int> labels;
    vector<int> prediction;
    Mat gray;
    Mat frame;
    Scalar x;
    string box_text;
    CascadeClassifier face_cascade;
    vector<Rect> faces;
    
    read_csv(fn_csv, images, labels);
    
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    
    //for (int i = 0; i < images.size(); ++i)
    //{
    //    imshow("teste", images[i]);
    //    waitKey(0);
    //}
    Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();
    model->train(images, labels);
    
    face_cascade.load(fn_haar);
    VideoCapture camera(0);
    
    if(!camera.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    int k = 0;
    char key;
    double confidence;

    while(true) {
        k++;
        camera >> frame;
        
        cvtColor(frame, gray, CV_BGR2GRAY);
        
        if(k%15 == 0){
            face_cascade.detectMultiScale(gray, faces);
            prediction.resize(faces.size());
            for (int i = 0; i < faces.size(); ++i)
            {
                Mat face_area = gray(faces[i]);
                Mat face_resized;
                resize(face_area, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
                model->predict(face_resized, prediction[i], confidence);
                if(confidence >= 90){
                    prediction[i] = -1;
                }
                cout << "id:" << prediction[i] << " conf:" << confidence << endl;
            }
        }

        for(int i = 0; i < faces.size(); i++) {
            if(prediction[i] == 1){
                box_text = "Carlos";
                x = Scalar(0,255,0);
            }
            else if(prediction[i] == 2){
                box_text = "Guilherme";
                x = Scalar(0,255,0);
            }
            else{
                box_text = "Desconhecido";
                x = Scalar(0,0,255);
            }
            rectangle(frame, faces[i], x, 1);
            //string box_text = format("Prediction = %d", prediction[i]);
            int pos_x = std::max(faces[i].tl().x - 10, 0);
            int pos_y = std::max(faces[i].tl().y - 10, 0);
            putText(frame, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, x, 2.0);
        }
        imshow("face_recognizer", frame);
        key = (char)waitKey(1);
        if(key == 'p')
            break;
    }
    return 0;
}