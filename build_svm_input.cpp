#include <iostream>
#include <fstream>
using namespace std;

int main(){
	ifstream fin("classify_path.txt");
	ofstream fout("classify_input.txt");
	string line;
	while(fin>>line){
		string filename = line.substr(0,line.find_first_of('/'));
		string textname = line.substr(line.find_first_of('/')+1, line.find_first_of('.')-line.find_first_of('/')-7);
		string path = "models/"+textname+'/'+"AutoPhrase_single-word.txt";
		//cout<<path<<endl;
		ifstream tmpfin(path);
		double percentage;
		string single_word;
		while(tmpfin>>percentage>>single_word){
		if(percentage>0.8)
			fout<<single_word<<' '<<filename<<endl;
		else
			break;
		}
	}
}