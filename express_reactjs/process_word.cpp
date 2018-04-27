#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>

using namespace std;

struct compare
{
	bool operator()(const pair<string,double>& s1, const pair<string,double>& s2){
		return s1.second>s2.second;
	}
};

int main() {
	//preprocess the text as input
	ifstream fin("../userword.txt");
	string single_word;
	unordered_map<string,double> usermp;
	while(fin>>single_word){
		usermp[single_word]=1.0;
	}

	//make comparison
	std::vector<pair<string,double> > vec;
	ifstream fin4("../autofilelist.txt");
	double percentage;
	string autoline;

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
	ofstream foutfile("../similar_movie_from_word.txt");
	sort(vec.begin(),vec.end(),compare());
	for(int i=0;i<10;++i){
		string tmp=vec[i].first;
		string filename = tmp.substr(tmp.find_first_of('/') + 1, tmp.find_last_of('/') - tmp.find_first_of('/') - 1);
		double num=vec[i].second;
		foutfile<<filename<<endl; //<<' '<<num<<endl;
	}
	return 0;
}
