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
	for(int i = 0; i <416; ++i){
			stringstream ss;
			ss << i;
			string str = ss.str();
			aux = "/home/cfcv/Desktop/git/Face_Recognition/src_get/personnes/fixed/IMG_"+str+".jpg;1";
			f << aux << endl;
	}
	return 0;
}