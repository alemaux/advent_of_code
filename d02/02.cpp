#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>bg
#include<string>

using namespace std;

bool is_safe(vector<int> vec){
	bool decreasing = vec[0] < vec[1] ? false : true, safe = true; 
	for(int i = 0; i<vec.size()-1; i++){
		if(decreasing&&((vec[i] - vec[i+1])<1||(vec[i] - vec[i+1])>3)){
			safe = false;
		}else if(!decreasi	ng&&((vec[i+1]-vec[i])>3||(vec[i+1]-vec[i])<1)){
			safe = false;
		}
	}
	return safe;
}

vector<int> delete_n(vector<int> v, int n){
	vector<int> newvec = {};
	for(int i = 0; i<v.size(); i++){
		if(i != n) newvec.push_back(v[i]);
	}
	return newvec;
}

int main(int argc, char* argv[]){
	string filename = argv[1];
	ifstream file(filename, ifstream::in);
	string line;
	int clines = 0, csafe = 0, cpsafe = 0;
	while(getline(file, line)){
		clines ++;
		istringstream iss(line);
		vector<int> vec = {};
		int x;
		while(iss >> x){
			vec.push_back(x);
		}
		if(is_safe(vec)){csafe++, cpsafe++;}
		else {
			bool safe = false;
			for(int i = 0; i< vec.size(); i++){
				safe |= is_safe(delete_n(vec, i));
			}
			if(safe) cpsafe++;
		}
	}
	cout << "Nombre de lignes : " << clines << endl;
	cout << "Nombre de lignes safes : " << csafe << endl;
	cout << "Nombre de lignes pas safes : " << clines - csafe << endl;
	cout << "Nombre de lignes presque safes : " << cpsafe << endl;
	cout << "Nombre de lignes même pas quasi-safes : " << clines - cpsafe << endl;
	return 0;
}
