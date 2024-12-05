#include<iostream>
#include<string>
#include<fstream>
#include<regex>
#include<typeinfo>

using namespace std;
vector<vector<int>> comp_table;
vector<bool> is_sorted_table;

bool is_n1_before_n2(vector<int> line, int n1, int n2){
    for(int i = 0; i<line.size(); i++){
        if(line[i] == n2){
            for(int j = i+1; j<line.size(); j++){
                if(line[j] == n1) return false;
            }
        }
    }
    return true;
}

void find_and_swap_vect_elements(vector<int> &line, int n1, int n2){
    for(int i = 0; i<line.size(); i++){
        if(line[i] == n2){
            for(int j = i+1; j<line.size(); j++){
                if(line[j] == n1){
                    int temp = line[i];
                    line[i] = line[j];
                    line[j] = temp;
                }
            }
        }
    }
}

bool is_vect_sorted(vector<int> vector){
    for(int k = 0; k<comp_table.size(); k++){
         if (!is_n1_before_n2(vector, comp_table[k][0], comp_table[k][1])) return false;
    }
    return true;
}

int main(int argc, char* argv[]){
    string filename, line;
    if(argc<2){
        cout<<"Erreur : pas d'argument donné"<<endl;
        return 1;
    }else{
        filename = argv[1];
    }
    ifstream file(filename, ifstream::in);
    smatch m;
    regex e("(\\d+)\\|(\\d+)");
    while(getline(file, line), regex_match(line, m, e)){
        vector<int> vec;
        vec.push_back(stoi(m[1]));
        vec.push_back(stoi(m[2]));
        comp_table.push_back(vec);
    }
    vector<vector<int>> updates;
    while(getline(file, line)){
        vector<int> vec;
        string value;
        istringstream iss(line);
        while(getline(iss, value, ',')){
            vec.push_back(stoi(value));
        }
        updates.push_back(vec);
        is_sorted_table.push_back(true);
    }
    //for(vector<int> x : updates) {for(int y : x) cout<<y<<" "; cout<<endl;}
    int sum = 0, sum2 = 0;
    for(int k = 0; k<comp_table.size(); k++){
        for(int l = 0; l<updates.size(); l++){
            is_sorted_table[l] = is_sorted_table[l] && is_n1_before_n2(updates[l], comp_table[k][0], comp_table[k][1]);
        }
    }
    for(int i = 0; i<updates.size(); i++){
        //cout<<is_sorted_table[i]<<endl;
        if (is_sorted_table[i]) sum+=updates[i][(int) updates[i].size()/2];
    }
    cout<<"Somme des éléments du milieu des updates correctement rangées : "<< sum<<endl;
    
    for(int i = 0; i<updates.size(); i++){
        if (!is_sorted_table[i]){
            do{
                for(int j = 0; j<comp_table.size(); j++){
                    find_and_swap_vect_elements(updates[i], comp_table[j][0], comp_table[j][1]);
                }
            }while(!is_vect_sorted(updates[i]));
        }
    }
    for(int i = 0; i<updates.size(); i++){
        //cout<<is_sorted_table[i]<<endl;
        if (!is_sorted_table[i]) sum2+=updates[i][(int) updates[i].size()/2];
    }
    cout<<"Somme des éléments du milieu des updates une fois correctement rangées : "<< sum2<<endl;
    
    return 0;
}