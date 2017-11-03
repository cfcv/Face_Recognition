/*#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <fstream>

using namespace std;

void readCsv(const string pathName, vector<cv::Mat>& images, vector<int>& labels, char separator);

int main(int argc, char const *argv[])
{
    string face_cascade_path1 = "/home/cfcv/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt.xml";
    string face_cascade_path2 = "/home/cfcv/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt2.xml";
    string face_cascade_path5 = "/home/cfcv/opencv-3.1.0/data/lbpcascades/lbpcascade_profileface.xml";
    string pathName = "/home/cfcv/Desktop/git/Face_Recognition/src_reco/csv.txt";
    vector<cv::Mat> images;
    vector<int> labels;
    vector<cv::Rect> faces;
    cv::Mat frame, gray;
    cv::CascadeClassifier face_cascade;

    readCsv(pathName, images, labels, ';');

    cv::VideoCapture camera(0);

    if(! camera.isOpened()){
        cout << "Não foi possível abrir câmera." << endl;
    }

    if(!face_cascade.load(face_cascade_path1)){
        cout << "face_cascade_path error" << endl;
    }

    cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();
    model->train(images, labels);

    cv::namedWindow("Original", CV_WINDOW_NORMAL);
    int k = 0;
    char key;

    while(true){
        k++;
        camera >> frame;

        cv::cvtColor(frame, gray, CV_BGR2GRAY);
        if(k%15 == 0){
            k = 0;
            face_cascade.detectMultiScale(gray, faces, 1.1);
        }

        for (int i = 0; i < faces.size(); ++i)
        {
            cv::rectangle(frame, faces[i], cv::Scalar(0,255,0));
        }
        cv::imshow("Original", frame);
        key = (char)cv::waitKey(1);

        if(key == 'p'){
            break;
        }
    }
    return 0;
}

void readCsv(const string pathName, vector<cv::Mat>& images, vector<int>& labels, char separator){
    fstream f(pathName.c_str());
    if(!f.is_open()){
        cout << "Arquivo não encontrado!" << endl;
        return;
    }

    string line, path, id;
    
    while(getline(f,line)){
        stringstream piece(line);
        getline(piece, path, separator);
        getline(piece, id);

        //Debug: mostra na tela o caminho e o id
        //cout << "path" << path << " -> " << id << endl;
        images.push_back(cv::imread(path));
        labels.push_back(atoi(id.c_str()));
    }
}*/
/*/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

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
    
    for (int i = 0; i < images.size(); ++i)
    {
        imshow("teste", images[i]);
        waitKey(0);
    }
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
                if(confidence >= 100){
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