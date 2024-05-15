#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* variable(int i, int j){
    char* res = malloc(10*sizeof(char));
    sprintf(res,"X_%d_%d",i,j);
    return res;
}

char* contrainte_une_ligne(int i, int n){
    char** l = malloc(n*sizeof(char*));
    for (int j = 0; j < n; j++){
        l[j] = variable(i,j);
    }
    char* phi1 = au_moins_une(l,n);
    char* phi2 = au_plus_une(l,n);
    char* phi3 = malloc(100*sizeof(char)); 
    strcpy(phi3, phi1); 
    strcat(phi3, "&");
    strcat(phi3, phi2);
    return phi3;
}

char* contrainte_toutes_lignes(int n){
    char** l = malloc(n*sizeof(char*));
    for (int i = 0; i < n; i++){
        char* phi = contrainte_une_ligne(i,n);
        l[i] = phi;
    }
    char* res = separation_et   (l,n);
    return res;
}


int main(){
    char* phi = contrainte_toutes_lignes(5);
    printf("%s\n", phi); 
    return 0;
}
