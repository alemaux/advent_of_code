#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//#define DEBUG
struct Antenna{
    char freq;
    int x, y;
};

struct antinode{
    char status;
    int x, y;
    struct Antenna a1, a2;
};

int count_nodes = 0;
int width=50, height=50;

void load_file(char *argv[], char (*mp)[50][50], int* n){
    char* filename;
    filename = argv[1];
    
    char line[1024];
    FILE *fp = fopen(filename, "r");
    int j = 0;

    while (j<50){
        fgets(line, sizeof(line), fp);
        for(int i = 0; i<50; i++){
            (*mp)[i][j] = line[i];
            if(line[i] != '.'){
                (*n) ++;
            }
        }
        j++;
    } 
    fclose(fp);
}

void put_antinodes(struct Antenna a1, struct Antenna a2, struct antinode (*pn)[50*50]){
    int dist2 = (a1.x-a2.x)*(a1.x-a2.x) + (a1.y - a2.y)*(a1.y - a2.y);
    int fact = 0; 
    int x =  a1.x + (a2.x - a1.x) * fact,
            y = a1.y + (a2.y - a1.y) * fact;
    while( x < width && x >=0 &&y <height && y >= 0){
        x += (a2.x - a1.x),
        y +=  (a2.y - a1.y) ;
        if((x>=0&&x<50&&y>=0&&y<50)&&
                ((x!=a1.x&&y!=a1.y)||(x!=a2.x&&y!=a2.y))&&
                (*pn)[y * width + x].status !='#'){
            struct antinode ant = {'#', x, y, a1, a2};
            (*pn)[y * width + x] = ant;
            count_nodes ++;
        }
        fact++;
    }
}

int affiche(char map[50][50], struct antinode z[50*50]){
    int count = 0;
    struct antinode on_line[50];
    int k =0;
    for(int i = 0; i<50; i++){
        k = 0;
        for(int j = 0; j<50; j++){
            if (z[i*width + j].status != '.'){
                printf("\033[0;31m");
                if(map[i][j] != '.')printf("%c", map[i][j]);
                else if(z[i*width + j].status != '.'){
                    printf("%c", z[i*width+j].status);
                    on_line[k] = z[i*width +j];
                    k++;
                }
                printf("\033[0m");
                count ++;
            }else
            printf("%c", map[i][j]);
        }
        #ifdef DEBUG
        printf("\t");
        for(int l = 0; l<k; l++){
            struct antinode node = on_line[l];
            printf("(%d,%d) : s1 =(%c, %d, %d) s2 =(%c, %d, %d) ",
            node.x, node.y, node.a1.freq, node.a1.x, node.a1.y, node.a2.freq, node.a2.x, node.a2.y);
        }
        #endif
        printf("\n");
    }
    return count;
}

int main(int argc, char* argv[]){
    char* filename;
    char map[50][50];
    int nb_antennas = 0;
    if (argc<2){
        printf("Erreur : pas d'input donné");
        return 1;
    }else{
        char (*fm)[50][50] = &map;
        load_file(argv, fm, &nb_antennas);
    }

    struct Antenna *antennas;
    antennas = malloc(sizeof(struct Antenna));

    int k = 0;
    for(int i=0; i<50; i++){
        for(int j=0; j<50; j++){
            if(map[i][j]!='.'){
                struct Antenna ant = {map[i][j], j, i};
                antennas[k] = ant;
                k++;
            }
        }
    }
    struct antinode z[50*50];
    for(int i = 0; i<2500; i++){
        int x = i%width,
            y = i/width;
        struct Antenna a0 = {'.', 0, 0};
        struct antinode an = {'.', x, y, a0, a0};
        z[i] = an;
    }

    for(int i=0; i<nb_antennas; i++){
        struct Antenna a1 = antennas[i];
        for(int j=0; j<nb_antennas; j++){
            struct Antenna a2 = antennas[j];
            if(i!=j&&a1.freq == a2.freq){
                put_antinodes(a1, a2, &z);
            }
        }
        #ifdef DEBUG
        printf("\x1b[2J");
        printf("Antenne : %c %d %d \n", a1.freq, a1.x, a1.y);
        affiche(map, z);
        getchar();
        #endif
    }

    printf("Nombre de poisiton d'antinodes : %d\n", count_nodes);
    count_nodes = affiche(map, z);
    printf("Nombre de poisiton d'antinodes : %d\n", count_nodes);

    free(antennas);
    return 0;
}