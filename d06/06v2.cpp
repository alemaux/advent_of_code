#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<unordered_map>
#include<tuple>
#include<sstream>


using namespace std;

struct Position{
    char dir;
    int posx, posy;
};

const string outputFileName = "./output.txt";
const string posFileName = "./positions.txt";

int width, height;

Position currentPos;
Position nextPos;

vector<vector<char>> map;

unordered_map<char, tuple<int, int>> dirMap{
    {'^', tuple(0, -1)},
    {'>', tuple(1, 0)},
    {'v', tuple(0, 1)},
    {'<', tuple(-1, 0)}
};

vector<Position> potentialobs;

void savePos(fstream &file, Position pos){
    file << pos.dir << " " << pos.posx << " " << pos.posy << endl;
}

char turn90(char garde){
    string gardes = "^>v<";
    for(int i = 0; i<4; i++){
        if(gardes[i]==garde)return gardes[(i+1)%4];
    }
    return garde;
}

Position new_pos(Position current){
    tuple<int, int> direction = dirMap[current.dir];
    int posx= current.posx + get<0>(direction);
    int posy = current.posy + get<1>(direction);
    return Position{current.dir, posx, posy};
}

void update(Position &currentPos, Position newPos, bool doMark,fstream *ofs){
    char garde = currentPos.dir;
    if ((map[newPos.posy][newPos.posx] == '#')||(map[newPos.posy][newPos.posx] == 'O')){
        currentPos.dir = turn90(garde);
        if(doMark) map[currentPos.posy][currentPos.posx] = turn90(garde);
    }else{
        if(doMark) map[newPos.posy][newPos.posx] = garde;
        if(doMark) map[currentPos.posy][currentPos.posx] = 'X';
        currentPos = newPos;
        if(doMark)savePos(*ofs, currentPos);
    } 
}

bool isLoop(vector<Position> vect,  Position current){
    bool isLoop = false;
    for(Position pos : vect){
            if(current.dir==pos.dir&&current.posx==pos.posx&&current.posy==pos.posy){
                isLoop = true;
                //cout<<"path !";
            } 
        }
    //if(isLoop) cout<<current.dir<<" "<<current.posx<< " "<< current.posy<<endl; 
    return isLoop;
}

void checkFromWith(Position posinit, Position obstacle){
    if((map[obstacle.posy][obstacle.posx] != '@')&&(map[obstacle.posy][obstacle.posx] != '#')){
        map[obstacle.posy][obstacle.posx] = 'O';
        Position currentPosT = {posinit.dir, posinit.posx, posinit.posy};
        vector<Position> temp = {};
        Position nextPosT = new_pos(currentPosT);
        bool loop = isLoop(temp, currentPosT);
        temp.push_back(currentPosT);
        fstream fs;
        int longueur = 0;
        while(nextPosT.posx>-1&&nextPosT.posx<width&&nextPosT.posy>-1&&nextPosT.posy<height
                &&!loop){
            update(currentPosT, nextPosT, false, &fs);
            nextPosT = new_pos(currentPosT);
            loop |= isLoop(temp, currentPosT);
            temp.push_back(currentPosT);
            longueur ++;
        }
        if(loop){
            potentialobs.push_back(obstacle);
            //cout<<potentialobs.size()<<" : "<< obstacle.dir<< " " <<obstacle.posx<<" "<<obstacle.posy<<" : "<<longueur<<endl;
            map[obstacle.posy][obstacle.posx] = '@';
        }else map[obstacle.posy][obstacle.posx] = 'o';
    }
}

int main(int argc, char* argv[]){
    string inputFileName, line;
    if(argc <2){
        cout << "Erreur : pas d'argument donné"<<endl;
        return 1;
    }else inputFileName = argv[1];

    ifstream file(inputFileName, ifstream::in);
    fstream posofs(posFileName, ios::binary);

    //On choppe l'input
    int posx, posy = 0;
    while(getline(file, line)){
        posx = 0;
        vector<char> ligne;
        for(char x : line){
            ligne.push_back(x);
            if(x=='^'||x=='>'||x=='<'||x=='v') currentPos = {x, posx, posy};
            posx++;
        }
        map.push_back(ligne);
        posy ++;
    }
    Position initial = currentPos;
    height = map.size(), width = map[0].size();
    savePos(posofs, currentPos);
    nextPos = new_pos(currentPos);
    savePos(posofs, nextPos);
    while(nextPos.posx>-1&&nextPos.posx<width&&nextPos.posy>-1&&nextPos.posy<height){
        update(currentPos, nextPos, true, &posofs);
        nextPos = new_pos(currentPos);
    }


    ofstream fileout(outputFileName, ios::binary);
    for(int i = 0; i<map.size(); i++){
        for(int j = 0; j<map[0].size(); j++){
            fileout<< map[i][j];
        }
        fileout<<endl;
    }

    int sum = 0;
    for (vector<char> ligne : map){
        for(char x : ligne){
            if(x=='X'||x=='^'||x=='>'||x=='v'||x=='<') sum++;
        }
    }

    cout << "Nombre de cases visitées : "<<sum<<endl;
    
    Position lastPos;
    bool firstIt = true;
    ifstream readerFile(posFileName, ios::binary);
    while(getline(readerFile, line)){
        char w;
        int x, y;
        istringstream iss(line);
        if(iss >> w >> x >> y){
            if(!firstIt&&w==lastPos.dir){
                checkFromWith(initial, Position{w, x, y});
            }else firstIt = false;
            lastPos = Position{w, x, y};
        }
    }

    ofstream fileout2("out2.txt", ofstream::out);
    for(int i = 0; i<map.size(); i++){
        for(int j = 0; j<map[0].size(); j++){
            fileout2<< map[i][j];
        }
        fileout2<<endl;
    }

    cout << "Nombre de boucles : "<< potentialobs.size() << endl;
    return 0;
}