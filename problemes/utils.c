#include <stdlib.h>
#include <string.h>
#include <stdio.h>


char* au_moins_une(char** l, int n,int *taille_formule){
    int taille_res=5000;
    char* res = malloc(taille_res*sizeof(char));
    res[0] = '(';//ouverture principale
    int indice = 1;
    for (int i = 0; i < n; i++){
        while(taille_res<indice+strlen(l[i])+10){
            taille_res*=2;
            res=realloc(res,taille_res*sizeof(char));

        }
        for (int j = 0; j < strlen(l[i]);j++){
            res[indice] = l[i][j];
            indice ++;
        }
        if (i != n - 1){ 
            res[indice] = '|';
            indice ++;
        }
    }
    res[indice] = ')';//fermeture principale
    indice++;
    res[indice]='\0';
    *taille_formule=taille_res;
    return res;
}


char* au_plus_une(char**l,int n,int *taille_sauvegarde){
    /*
    On va faire la négation de au moins 2 , (en effet pour N variables, on fera au total 2 parmi N paquets à évaluer qui sont chacun une conjonction de 2 variable, et l'ensemble est une négation d'une FN C
    Et donc en développement la négation , on a une FNC avec dans chaque 2 variables, la disjonction de la négation 2 variables 
    */
    int taille_res=5000;
    char *res =malloc(taille_res*sizeof(char));
    int indice =0;
    res[indice]='(';//ouverture principale
    indice++;
    if(n==1){
      
       while(taille_res<indice+2*strlen(l[0])+15){
            taille_res*=2;
            res=realloc(res,taille_res*sizeof(char));
        }
        for(int i=0;i<strlen(l[0]);i++){
           res[indice]=l[0][i];
           indice++;
        }
        res[indice]='|';
        indice++;
        res[indice]='~';
        indice++;
        for(int i=0;i<strlen(l[0]);i++){
           res[indice]=l[0][i];
           indice++;
        }
        res[indice]=')';//fermeture principale 1
        indice++;
        res[indice]='\0';
        return res;
    }
    for(int i=0;i<n-1;i++){
      res[indice]='(';//ouverture
      indice++;
    
      //Commencement de la création du paquet
      for (int j=i+1;j<n;j++){// attention chg avec le +1s
        while(taille_res<indice+strlen(l[j])+strlen(l[i])+15){
            taille_res*=2;
            res=realloc(res,taille_res*sizeof(char));
        }

        res[indice]='(';
        indice++;
        
        if(strlen(l[j])!=1){//On traite le cas où ce n'est pas juste une variable
                
                res[indice]='~';
                indice++;
                for(int z=0;z<strlen(l[j]);z++){
                    res[indice]=l[j][z];
                    indice++;
                }
                
                
            }
        else{
            res[indice]='~';
            indice++;
            res[indice]=l[j][0];
            indice++;
            }
        
        res[indice ]='|';
        indice++;
        if(strlen(l[i])==1){
           res[indice]='~';
           indice++;
           res[indice]=l[i][0];
           indice++;
           
        }
        else{
           res[indice]='~';
           indice++;
           for(int z=0;z<strlen(l[i]);z++){
                    res[indice]=l[i][z];
                    indice++;
                }
            
        }
        res[indice]=')';
        indice++;

        if(j!=(n-1)){
           res[indice]='&';
           indice++;
        }

      }
      res[indice]=')';
      indice++;

      if(i!=(n-2)){
        res[indice]='&';
        indice++;
      }


    }
    res[indice]=')';//fermeture principale 2
    indice++;   
    res[indice]='\0';

    *taille_sauvegarde=taille_res;
    return res;

}


char* separation_et(char** l, int n) { // attention modification à faire, fonction auxiliaire pour la Q30
    int taille_res = 5000;
    char* res = malloc(taille_res * sizeof(char));

    res[0] = '(';
    int indice = 1;
    int taille_l_i = 0;
    for (int i = 0; i < n; i++) {
        taille_l_i = strlen(l[i]);
        while (taille_res < indice + taille_l_i + 3) { 
            taille_res *= 2;
            res = realloc(res, taille_res * sizeof(char));
        }
        for (int j = 0; j < taille_l_i; j++) {
            res[indice] = l[i][j];
            indice++;
        }
        if (i != (n - 1)) {
            res[indice] = '&';
            indice++;
        }
    }
    res[indice] = ')';
    res[indice + 1] = '\0';
    
    return res;
}
