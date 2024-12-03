#include<string>
#include<istream>
#include<iostream>
#include<fstream>
#include<regex>


using namespace std;

const regex e1("mul\\((\\d+),(\\d+)\\)");
int main(int argc, char* argv[]){
    string filename, line;
    if(argc<2){
        cout<<"Erreur : pas d'argument donné"<<endl;
        return 1;
    }else{
        filename = argv[1];
    }
    ifstream file(filename, ifstream::in);
    int sum = 0;
    while(getline(file, line)){
        smatch m;
        while(regex_search(line, m, e1)){
            sum += (stoi(m[1]))*(stoi(m[2]));
            line = m.suffix().str();
        }
    }
    cout << "Somme des produits valables : " << sum << endl;
    
    ifstream file2(filename, ifstream::in);
    sum = 0;
    bool enabled = true;
    const regex e2("(mul\\((\\d+),(\\d+)\\))|do\\(\\)|don't\\(\\)");
    while(getline(file2, line)){
        smatch m;
        regex d("do\\(\\)"), dn("don't\\(\\)");
        while(regex_search(line, m, e2)){
            if(regex_match(m.str(), d)){
                enabled = true;
            }else if(regex_match(m.str(), dn)){
                enabled = false;
            }else if(enabled){
                sum += (stoi(m[2]))*(stoi(m[3]));
            }
            line = m.suffix().str();
        }
    }
    cout << "Somme de tout ça avec les conditions : " << sum <<endl;
    return 0;
}