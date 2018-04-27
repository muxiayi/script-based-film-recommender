#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <python2.7/Python.h>
using namespace std;

//directly go to a specific line
std::ifstream& gotoLine(std::ifstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

int main(){
	//calling MeTA search engine to do classification
	FILE* file;
    int argc;
    char * argv[2];

    argc = 2;
    argv[0] = "./search_engine/search_eval.py";
    argv[1] = "./search_engine/config.toml";
 
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    PySys_SetArgv(argc, argv);
    file = fopen("./search_engine/search_eval.py","r");
    PyRun_SimpleFile(file, "./search_engine/search_eval.py");
    Py_Finalize();

    //use cpp to map index to movie scripts
	ifstream fin("./relevance.txt");
	ofstream fout("./search_outcome.txt");
	char tmp;
	vector<int>vec;
	vector<int>copyvec;
	while(fin.get(tmp)){
		if(tmp=='('){
			int num;
			fin>>num;
			cout<<num<<endl;
			vec.push_back(num);			
		}
	}
	if(vec.size()==0){
		fout<<"No related scripts in the library for searching...";
	}
	for(int i=0;i<vec.size();++i){
		ifstream fin2("./search_engine/cranfield/cranfield.dat");
		ifstream fin3("../filelist.txt");
		gotoLine(fin2, vec[i]);
		gotoLine(fin3, vec[i]);
		string text;
		string mname;
		getline(fin2,text);
		getline(fin3,mname);
		fout<<"Script Name: "<<mname.substr(mname.find_last_of('/')+1,mname.find_last_of('.')-mname.find_last_of('/')-1)<<endl;
		fout<<"Abstract: "<<text.substr(0,90)<<endl<<text.substr(100,100)<<endl<<endl;
	}
	

	return 0;
}