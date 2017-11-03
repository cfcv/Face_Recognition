/*
    Nombre:     Juan Paúl Ortiz González
    Curso:      Visión Artificial
    Profesor:   Vladimir Robles

    UNIVERSIDAD POLITECNICA SALESIANA
    DICIEMBRE - 2012

 ***********************************************************************
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

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    ifstream file(filename.c_str(), ifstream::in);
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
    // Obtener la ruta de acceso al archivo CSV:
    string fn_haar = string("C:/Users/Paul/Documents/Codeblocks/proyecto2/haarcascades/haarcascade_frontalface_alt_tree.xml");
    string fn_csv = string("C:/Users/Paul/Documents/Codeblocks/proyecto2/csv.ext");

    int deviceId = atoi("0");
    // Vectores que continen las imagenes y sus etiquetas:
    vector<Mat> images;
    vector<int> labels;
    // Lectura de datos (Si no existe un nombre de archivo valido entrega un error en pantalla):
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // cerrar la aplicación
        exit(1);
    }

    // Obtenemos el tamaño de la primera imagen. Necesitaremos esto
    // después para redimensionar las imagenes a su original tamaño
    // y necesitamos redimensionar las caras entrantes a este tamaño:
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    // Crear un FaceRecognizer y entrenarlo con las imagenes dades:
    Ptr<FaceRecognizer> model = createEigenFaceRecognizer(23,2500.0);//createFisherFaceRecognizer(); //createEigenFaceRecognizer();  2700
    model->train(images, labels);
    // Esto es todo para aprender el modelo de reconocimiento de caras.
    // Ahora se necesita crear un clasificador para la tarea de detección de caras.
    // Se usaras un clasificador haar cascade que se especifico en el path fn_haar:

    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    // Obtener un identificador para el dispositivo de vídeo:
    VideoCapture cap(deviceId);
    namedWindow("face_recognizer", CV_WINDOW_AUTOSIZE);

    // Compruebe si se puede usar este dispositivo en absoluto:

    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    // Vector que contine rectangulos de las caras:
    vector< Rect_<int> > faces;
    // Mantiene el frame del dispositivo de video:
    Mat frame;
    waitKey(1500);
    for(;;) {
        cap >> frame;
        // Clona el actual frame:
        Mat original = frame.clone();
        // Convierte el actual frame en escala de grises:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Encontrar las caras en el frame:
        haar_cascade.detectMultiScale(gray, faces);
        // En este punto se tiene la posicion de las caras en la variable faces.
        // Ahora obtenemos las caras, haciendo una predicción y anotación en el
        // video.
        for(int i = 0; i < faces.size(); i++) {
            // Proceso cara a cara:
            Rect face_i = faces[i];
            // Recortar la cara de la imagen. Simple con OpenCV C++:
            Mat face = gray(face_i);
            //Cambiar el tamaño de la cara es necesario para Eigenfaces y Fisherfaces:
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            // Ahora se realiza la prediccion:
            int prediction = model->predict(face_resized);
            // Y finalmente se escribe los resultados en la imagen original!
            // Primero dibujar un rectangulo rojo alrededor de la cara detectada:
            rectangle(original, face_i, CV_RGB(255, 0,0), 1);
            // Crear el texto para la caja:
            string box_text = format("Sujeto = %d", prediction);
            // calcular la posicion para el texto anotado:
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
            // Y ahora poner en la imagen:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        }
        // Mostrar los resultados:
        imshow("face_recognizer", original);
        // Y visualizarlo:
        char key = (char) waitKey(1);
        // Salir del loop presionando ESC:
        if(key == 27){
            destroyAllWindows();
            break;
        }



    }
    return 0;
}

-------------------------------

#include <iostream>
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

    //READ CVS FILE -----------
    readCsv(pathName, images, labels, ';');
    //Debug: Mostra todas as imagens lidas
    /*for(int i = 0; i < images.size(); ++i){
        cv::imshow("teste"+i, images[i]);
        cv::waitKey(0);
    }*/
    
    int img_height = images[0].rows;
    int img_width = images[0].cols;

    //TRAIN THE MODEL WITH THE IMAGES READED ------------
    cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer(23,2500.0);
    model->train(images, labels);

    cv::VideoCapture camera(0);

    if(! camera.isOpened()){
        cout << "Não foi possível abrir câmera." << endl;
    }

    if(!face_cascade.load(face_cascade_path1)){
        cout << "face_cascade_path error" << endl;
    }

    cv::namedWindow("Original", CV_WINDOW_NORMAL);
    int i = 0;
    double pre;
    cv::Mat face_area1 = images[images.size()-1]; 
    images.pop_back();
    while(true){
        camera >> frame;

        cv::cvtColor(frame, gray, CV_BGR2GRAY);
        if(i%15 == 0){
            face_cascade.detectMultiScale(gray, faces, 1.1);
            i = 0;
            cout << "Faces: " << faces.size() << endl;
            for(int j = 0; j < faces.size(); ++j){
                cv::Mat face_area = gray(faces[j]); 
                cv::Mat face_resized;
                cv::resize(face_area, face_resized, cv::Size(img_width, img_height), 1.0, 1.0, CV_INTER_CUBIC);
                cv::imshow("teste", face_resized);

                pre = model->predict(face_resized);
                cout << "predict " << j << ":" << pre << endl;
            }
        }
        
        for(int j = 0; j < faces.size(); ++j){
            rectangle(frame, faces[j], cv::Scalar(0,255,0));
        }
        cv::imshow("Original", frame);
        char key = (char)cv::waitKey(1);
        if(key == 'p'){
            break;  
        }
        //cout << (int)key << endl;
        i++;
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
}