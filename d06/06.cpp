#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<unordered_map>
#include<tuple>

using namespace std;

vector<char> map;
int currentxpos, currentypos;
int newxpos, newypos;
int height = 0, width;
unordered_map<char, tuple<int, int>> dir{
    {'^', tuple(0, -1)},
    {'>', tuple(1, 0)},
    {'v', tuple(0, 1)},
    {'<', tuple(-1, 0)}
};

char turn90(char garde){
    string gardes = "^>v<";
    for(int i = 0; i<4; i++){
        if(gardes[i]==garde)return gardes[(i+1)%4];
    }
    return garde;
}

int new_pos(){
    int currentpos = currentypos * width + currentxpos;
    char garde = map[currentpos];
    tuple<int, int> direction = dir[garde];
    newxpos = currentxpos + get<0>(direction);
    newypos = currentypos + get<1>(direction);
    return currentpos + width * get<1>(direction) + get<0>(direction);
}

void update(int newpos){
    int currentpos = currentxpos + currentypos * width;
    char garde = map[currentpos];
    if (map[newpos] == '#'){
        map[currentpos] = turn90(garde);
    }else{
        map[newpos] = garde;
        map[currentpos] = 'X';
        currentxpos = newxpos, currentypos = newypos;
    } 
}

int main(int argc, char* argv[]){
    string filename, line;
    if(argc <2){
        cout << "Erreur : pas d'argument donné"<<endl;
        return 1;
    }else filename = argv[1];
    ifstream file(filename, ifstream::in);
    int j=0;
    while(getline(file, line)){
        j = 0;
        for(char x : line){
            width = line.size();
            map.push_back(x);
            if(x=='^'||x=='>'||x=='<'||x=='v'){
                currentxpos = j, currentypos = height;
            }
            j++;
        }
        height ++;
    }
    int newpos = new_pos();
    cout<<"height : "<< height<<" width : "<<width<<endl;
    while(newxpos>-1&&newxpos<width&&newypos>-1&&newypos<height){
        update(newpos);
        newpos = new_pos();
    }

    ofstream fileout("./output.txt", ios::binary);
    for(int i = 0; i<map.size(); i++){
        if (i%width == 0){
            fileout<< map[i]<<endl;
        }else fileout<<map[i]<<" ";
    }

    int sum = 0;
    for (char x : map){
        if(x=='X'||x=='^'||x=='>'||x=='v'||x=='<') sum++;
    }
    cout << "Nombre de cases visitées : "<<sum<<endl;
    return 0;
}