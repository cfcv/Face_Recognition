#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main(){

	ofstream f;
	f.open("csv.txt");
	string aux;
	char buffer[2];
	for(int i = 0; i < 268; ++i){
		if(i <= 99){
			stringstream ss;
			ss << i;
			string str = ss.str();
			aux = "/home/cfcv/Desktop/git/Face_Recognition/src_get/IMG_"+str+".jpg;1";
			f << aux << endl;
		}
		else{
			stringstream ss;
			ss << i;
			string str = ss.str();
			aux = "/home/cfcv/Desktop/git/Face_Recognition/src_get/IMG_"+str+".jpg;2";
			f << aux << endl;
		}
	}
	return 0;
}