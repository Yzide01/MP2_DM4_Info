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
    /*
    On va faire la négation de au moins 2 , (en effet pour N variables, on fera au total 2 parmi N paquets à évaluer qui sont chacun une conjonction de 2 variable, et l'ensemble est une négation d'une FN C
    Et donc en développement la négation , on a une FNC avec dans chaque 2 variables, la disjonction de la négation 2 variables 
    */
    int taille_res=100000;
    char *res =malloc(taille_res*sizeof(char));
    int indice =0;
    res[0] = '(';
    indice++;
    for(int i=0;i<n;i++){

      res[indice]='(';
      indice++;
      //Commencement de la création du paquet
      for (int j=i;j<n;j++){
        while(taille_res<indice+strlen(l[j])+strlen(l[i])+15){
        taille_res*=2;
        res=realloc(res,taille_res*sizeof(char));
        
      }
        if(strlen(l[j])!=1){//On traite le cas où ce n'est pas juste une variable
                
                res[indice]='(';
                res[indice+1]='~';
                indice+=2;
                for(int z=0;z<strlen(l[j]);z++){
                    res[indice+z]=l[j][z];
                }
                indice+=strlen(l[j]);
                res[indice]=')';
                indice++;
            }
        else{
            res[indice]='~';
            res[indice+1]=l[j][0];
            indice+=2;
            }
        res[indice ]='|';
        indice++;
        if(strlen(l[i])==1){
           res[indice]='~';
           indice++;
           res[indice]=l[i][0];
           indice++;
           res[indice]=')';//fermeture du paquet
           indice++;
        }
        else{
           for(int z=0;z<strlen(l[j]);z++){
                    res[indice+z]=l[j][z];
                }
                indice+=strlen(l[j]);
                res[indice]=')';
                res[indice+1]=')';// Fermeture du paquet 
            
        }

      }
      if(i!=(n-1)){
        res[indice]='&';
        indice++;
      }
    }
    res[indice] = ')';
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
    char* phi = au_plus_une(forms,3);
    printf("%s",phi);
    free(phi);

    return 0;
}

*/
