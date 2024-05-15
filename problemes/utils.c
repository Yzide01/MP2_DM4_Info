#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* au_moins_une(char** l, int n){
    char* res = malloc(100*sizeof(char));
    res[0] = '(';
    int indice = 1;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < strlen(l[i]);j++){
            res[indice] = l[i][j];
            indice ++;
        }
        if (i != n - 1){ 
            res[indice] = ' ';
            res[indice+1] = '|';
            res[indice+2] = ' ';
            indice = indice + 3;
        }
    }
    res[indice] = ')';
    return res;
}


char* au_plus_une(char**l,int n){
    char *res =malloc(100*sizeof(char));
    int indice =0;
    char* non_neg=NULL ;
    for(int i=0;i<n;i++){//pour chaque truc non nég
        res[indice]='(';
        indice++;
        for(int j=0;j<n;j++){
            if (i!=j){
                res[indice]='~';
            }

            if(len(l[j])!=1){//On traite le cas où ce n'est pas juste une variable
                res[indice+1]='(';
                indice+=2;
                for(int z=0;z<len(l[j]);z++){
                    res[indice+z]=l[j][z];
                }
                indice+=len(l[j]);
                res[indice]=')';
                indice++;
            }
            else{
                res[indice+1]=l[j][0];
                indice++;
            }
            if (j!=n-1){//Evite le et inutile à la fin 
                res[indice]='&';
                indice++;
            }

            
            
        }
        res[indice]=')';
        res[indice+1]='|';
        indice+=2;
    }//Il manque maintenant que le cas où tout est faux . 
    return res;

}
char* separation_et(char** l, int n){//attention modification à faire,fonction auxiliaire pour la Q30
    char* res = malloc(100*sizeof(char));

    res[0] = '(';
    int indice = 1;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < strlen(l[i]);j++){
            res[indice] = l[i][j];
            indice ++;
        }
        if (i != n - 1){ 
            res[indice] = ' ';
            res[indice+1] = '&';
            res[indice+2] = ' ';
            indice = indice + 3;
        }
    }
    res[indice] = ')';
    return res;
}


/*
int main(){

    //tests
    char* forms[3] = {"(x & ~y)","y","z"};
    char* phi = au_moins_une(forms,3);
    for (int i = 0; i<20; i++){
        printf("%c",phi[i]);
    }
    free(phi);

    return 0;
}
*/