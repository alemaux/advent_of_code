#include<string>
#include<istream>
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;
const string str1 = "XMAS", str2 = "SAMX";
int height = 0, width = 0;

int check_horizontal(vector<vector<char>> vec, int i, int j){
    string str = (vec[i][j] == str1[0]) ? str1 : str2;
    if(j>width-4){
        return 0;
    }else{
        for(int k = 0; k<4; k++){
            if(vec[i][j+k] != str[k]){
                return 0;
            }
        }
        return 1;
    }
}

int check_vertical(vector<vector<char>> vec, int i, int j){
    string str = (vec[i][j] == str1[0]) ? str1 : str2;
    if(i>height-4){
        return 0;
    }else{
        for(int k = 0; k<4; k++){
            if(vec[i+k][j] != str[k]){
                return 0;
            }
        }
        return 1;
    }
}

int check_diagonal(vector<vector<char>> vec, int i, int j){
    string str = (vec[i][j] == str1[0]) ? str1 : str2;
    if(j>width-4||i>height-4){
        return 0;
    }else{
        for(int k = 0; k<4; k++){
            if(vec[i+k][j+k] != str[k]){
                return 0;
            }
        }
        return 1;
    }
}

int check_antidiagonal(vector<vector<char>> vec, int i, int j){
    string str = (vec[i][j] == str1[0]) ? str1 : str2;
    if(j<3||i>height-4){
        return 0;
    }else{
        for(int k = 0; k<4; k++){
            if(vec[i+k][j-k] != str[k]){
                return 0;
            }
        }
        return 1;
    }
}

int check_x(vector<vector<char>> vec, int i, int j){
    if(i<1||i==height-1||j<1||j==width-1){
        return 0;
    }else if(((vec[i-1][j-1]=="M"[0]&&vec[i+1][j+1]=="S"[0])||
            (vec[i-1][j-1]=="S"[0]&&vec[i+1][j+1]=="M"[0]))&&
            ((vec[i-1][j+1]=="M"[0]&&vec[i+1][j-1]=="S"[0])||
            (vec[i-1][j+1]=="S"[0]&&vec[i+1][j-1]=="M"[0]))&&
            vec[i][j]=="A"[0]){
        //for(int k = 0; k<3;k++) std::cout<<vec[i - 1 + k][j-1]<<" "<<vec[i - 1 + k][j]<<" "<<vec[i - 1 + k][j+1]<<endl;
        //cout<<endl;
        return 1;
    }else {
        return 0;
    }
}

int main(int argc, char* argv[]){
    string filename, line;
    int sum = 0, sumx = 0;
    if(argc<2){
        std::cout<<"Erreur : pas d'argument donné"<<endl;
        return 1;
    }else{
        filename = argv[1];
    }
    ifstream file(filename, ifstream::in);
    vector<vector<char>> data;
    while(getline(file, line)){
        vector<char> linevec;
        for(auto x:line){
            linevec.push_back(x);
        }
        data.push_back(linevec);
    }
    height = data.size(), width = data[0].size();
    for(int i = 0; i<height; i++){
        for(int j =0; j<width; j++){
            cout<<"i : "<<i<<" j : "<<j<<endl;
            sum += check_horizontal(data, i, j);
            sum += check_vertical(data, i, j);
            sum += check_diagonal(data, i, j);
            sum += check_antidiagonal(data, i, j);
            sumx += check_x(data, i, j);
            cout<< "\033[A\33[2K\r";
        }
    }
    std::cout << "Nombre d'occurence du mot XMAS : " << sum << endl;
    std::cout << "Nombre d'occurence du pattern X-MAS : " << sumx << endl;
    return 0;
}
