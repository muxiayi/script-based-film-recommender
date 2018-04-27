#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <python2.7/Python.h>
using namespace std;

struct compare
{
	bool operator()(const pair<string,double>& s1, const pair<string,double>& s2){
		return s1.second>s2.second;
	}
};

static bool cmp(const pair<int,int>&a, const pair<int,int>&b){
	return a.second>b.second;
}

//directly go to a specific line
std::ifstream& gotoLine(std::ifstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

int main() {
	//preprocess the text as input
	ifstream fin("../usertext.txt");
	ofstream fout("../data/processeduser.txt");
	string word;
	while(fin>>word) {
		if(word.back()=='.'||word.back()==','||word.back()=='?'||word.back()=='!')
			word=word.substr(0,word.length()-1);
		fout<<word<<' ';
	}
	fout.close();
	
	//get quality phrases from the input text
	string first_line = "MODEL=${MODEL:- \"models/processeduser\"}";
	string second_line = "RAW_TRAIN=${RAW_TRAIN:- data/processeduser.txt}";
	ofstream fout2("../auto_phrase.sh");
	ifstream fin2("../temp.sh");
	fout2<<first_line<<endl;
	fout2<<second_line<<endl;
	string line2;
	while(getline(fin2, line2)) {
		fout2<<line2<<endl;
	}
	fout2.close();
	system("cd ..; ./auto_phrase.sh;");  //must cd to the outside folder
	cout<<"Processing user input text finished."<<endl;

	//make comparison
	std::vector<pair<string,double> > vec;
	ifstream fin4("../autofilelist.txt");
	double percentage;
	string single_word;
	string autoline;

	//store the user file into a hashmap
	ifstream finuser("../models/processeduser/AutoPhrase_single-word.txt");
	unordered_map<string,double> usermp;
	while(finuser>>percentage>>single_word){
		if(percentage>0.2)
			usermp[single_word]=percentage;
		else
			break;
	}


	int flag = 0; // whether to stop searching, not done yet
	while(getline(fin4,autoline)){   //do the comparison with each document
		ifstream fintmp("../"+autoline);
		unordered_map<string,double> mp;
		while(fintmp>>percentage>>single_word){
			if(percentage>0.2)
				mp[single_word]=percentage;
			else
				break;
		}
		double cumulative=0;
		string thisword;
		for(auto iter=usermp.begin();iter!=usermp.end();++iter){
			if(mp.find(iter->first)!=mp.end()){
				thisword=iter->first;
				cumulative += iter->second * mp.find(iter->first)->second;
			}				
		}
		//we use min heap to ensure we only select the top 10 movies
		if(vec.size()<10){
			vec.push_back(std::make_pair(autoline, cumulative));
			push_heap(vec.begin(),vec.end(),compare());
		}
		else{
			if(cumulative<vec[0].second){
				flag++;
				continue;
			}
			else{
				pop_heap(vec.begin(),vec.end(),compare());
				vec.pop_back();
				vec.push_back(std::make_pair(autoline, cumulative));
				push_heap(vec.begin(),vec.end(),compare());
			}
		}
	}
	//print the ten most relevant movies
	ofstream foutfile("../similar_movie_from_script.txt");
	sort(vec.begin(),vec.end(),compare());
	for(int i=0;i<10;++i){
		string tmp=vec[i].first;
		string filename = tmp.substr(tmp.find_first_of('/') + 1, tmp.find_last_of('/') - tmp.find_first_of('/') - 1);
		double num=vec[i].second;
		foutfile<<filename<<endl; //<<' '<<num<<endl;
	}

	//calling svm.py to do classification
	Py_Initialize();
  	PyObject* PyFileObject = PyFile_FromString("svm.py", "r");
  	PyRun_SimpleFileEx(PyFile_AsFile(PyFileObject), "svm.py", 1);
  	Py_Finalize();

  	//calling cpp to group genre number to text categories
  	ifstream finss("../classify_tag.txt");
	ofstream foutss("../genre_names.txt");
	unordered_map<int,string>mpss;
	mpss[0]="Action"; mpss[1]="Adventure";
	mpss[2]="Animation"; mpss[3]="Biography";
	mpss[4]="Comedy"; mpss[5]="Crime";
	mpss[6]="Drama"; mpss[7]="Family";
	mpss[8]="Fantasy"; mpss[9]="Film-Noir";
	mpss[10]="History"; mpss[11]="Horror";
	mpss[12]="Music"; mpss[13]="Musical";
	mpss[14]="Mystery"; mpss[15]="Romance";
	mpss[16]="Sci-Fi"; mpss[17]="Short";
	mpss[18]="Sport"; mpss[19]="Thriller";
	mpss[20]="War"; mpss[21]="Western";

	int genre_number;
	unordered_map<int,int>pq;
	for(int i=0;i<20;++i){
		finss>>genre_number;
		pq[genre_number]++;
	}
	vector<pair<int,int> >res;
	for(unordered_map<int,int>::iterator iter=pq.begin();iter!=pq.end();++iter)
		res.push_back(make_pair(iter->first, iter->second));
	sort(res.begin(),res.end());
	foutss<<mpss[res[0].first]<<endl;
	if(res[0].second<20){
		foutss<<mpss[res[1].first]<<endl;
		if(res[0].second+res[1].second<20)
			foutss<<mpss[res[2].first]<<endl;
	}
		

	ifstream finrelated("../topic_words.txt");
	ofstream foutrelated("../three_genre_words.txt");
	string relatedline;
	while(getline(finrelated, relatedline)){
		std::size_t pos = relatedline.find(",");
		string beginning = relatedline.substr(0,pos);
		relatedline = relatedline.substr(pos+1);
		//foutrelated<<relatedline<<endl;	
		if((beginning == mpss[res[0].first])||
			(res[0].second<20&&beginning == mpss[res[1].first])||
			(res[0].second<20 && (res[0].second+res[1].second<20) && beginning == mpss[res[2].first])){
			for(int i=0;i<5;++i){
				pos = relatedline.find(",");
				beginning = relatedline.substr(0,pos);
				relatedline = relatedline.substr(pos+1);
				foutrelated<<beginning<<' ';
			}
			foutrelated<<endl;
		}
	}
	foutrelated.close();
	return 0;
}
