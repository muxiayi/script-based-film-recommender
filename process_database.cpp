#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main() {
	ifstream fin("filelist.txt");
	string line;
	while(getline(fin,line)) {
		string path = line;
		string filename = line.substr(line.find_last_of('/') + 1, line.find_last_of('.') - line.find_last_of('/') - 1);
		string first_line = "MODEL=${MODEL:- \"models/"+filename+"\"}";
		string second_line = "RAW_TRAIN=${RAW_TRAIN:- data/" + path + "}";
		ofstream fout("./auto_phrase.sh");
		ifstream fin2("./temp.sh");
		fout<<first_line<<endl;
		fout<<second_line<<endl;
		string line2;
		while(getline(fin2, line2)) {
			fout<<line2<<endl;
		}
		fout.close();
		cout<<"Press any key for running autophrase for "<<filename<<endl;
		//getchar();
		system("./auto_phrase.sh");
		cout<<"This round finished."<<endl;
		//getchar();
	}	
	return 0;
}
