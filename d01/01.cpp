#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>


using namespace std;
vector<int> vec1, vec2;

int import_file(string fileName){
    ifstream file(fileName, ifstream::in);
    string line;
    int x, y;
    int i = 0;
    while(getline(file, line)){
        istringstream iss(line);
        if(iss >> x >>y){
            vec1.push_back(x);
            vec2.push_back(y);
        }
        i++;
    }
    file.close();
    return i;
}

void sort_vec(vector<int> &v){
    int n = v.size();
    for(int i = 0; i< n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(v[j]>v[j+1]){
                swap(v[j], v[j+1]);
            }
        }
    }
}

int main(){
    int n = import_file("./input");
    sort_vec(vec1),sort_vec(vec2);
    int sum = 0;
    for(int i = 0; i<n; i++){
        sum += abs(vec1[i] - vec2[i]);
    }
    cout << "La différence est de : " << sum << endl;

    int j = 0;
    sum = 0;
    for(int i = 0; i<n; i++){
        int x = vec1[i];
        int occurence = 0;
        while(vec2[j]<=x){
            if(vec2[j] == x){
                occurence++;
            }
            j++;
        }
        sum += x*occurence;
        j -= occurence;
    }
    cout << "Le score de similarité est de : " << sum <<endl;
    return 0;
}