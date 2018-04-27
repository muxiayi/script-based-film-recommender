#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
using namespace std;

static bool cmp(const pair<int,int>&a, const pair<int,int>&b){
	return a.second>b.second;
}

int main(){
	ifstream fin("../classify_tag.txt");
	ofstream fout("../genre_names.txt");
	unordered_map<int,string>mp;
	mp[0]="Action"; mp[1]="Adventure";
	mp[2]="Animation"; mp[3]="Biography";
	mp[4]="Comedy"; mp[5]="Crime";
	mp[6]="Drama"; mp[7]="Family";
	mp[8]="Fantasy"; mp[9]="Film-Noir";
	mp[10]="History"; mp[11]="Horror";
	mp[12]="Music"; mp[13]="Musical";
	mp[14]="Mystery"; mp[15]="Romance";
	mp[16]="Sci-Fi"; mp[17]="Short";
	mp[18]="Sport"; mp[19]="Thriller";
	mp[20]="War"; mp[21]="Western";

	int genre_number;
	unordered_map<int,int>pq;
	for(int i=0;i<20;++i){
		fin>>genre_number;
		pq[genre_number]++;
	}
	vector<pair<int,int> >res;
	for(unordered_map<int,int>::iterator iter=pq.begin();iter!=pq.end();++iter)
		res.push_back(make_pair(iter->first, iter->second));
	sort(res.begin(),res.end());
	fout<<mp[res[0].first]<<endl;
	if(res[1].second>0)
		fout<<mp[res[1].first]<<endl;
	if(res[2].second>0)
		fout<<mp[res[2].first]<<endl;
}