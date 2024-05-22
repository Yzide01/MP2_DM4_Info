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
    int *taille_phi1=malloc(sizeof(int));
    int *taille_phi2=malloc(sizeof(int));
    char* phi1 = au_moins_une(l,n,taille_phi1);
    
    char* phi2 = au_plus_une(l,n,taille_phi2);

    char* phi3 = malloc(((*taille_phi1)+ (*taille_phi2)+1)*sizeof(char)); 

    strcpy(phi3, phi1); 
    strcat(phi3,"&");
    strcat(phi3, phi2);
    strcat(phi3,"\0");


    for(int j=0;j<n;j++){
       free(l[j]);
    }
    free(phi1);
    free(phi2);
    
    free (l);
    free(taille_phi1);
    free(taille_phi2);
    return phi3;

}


    char* contrainte_toutes_lignes(int n){
        
        char** l = malloc(n*sizeof(char*));
        for (int i = 0; i < n; i++){
            char* phi = contrainte_une_ligne(i,n);
            l[i] = phi;
        }
        
        char* res = separation_et(l,n);
        for(int j=0;j<n;j++){
            free(l[j]);
        }
        free(l);

        return res;
        
    }

char * contrainte_une_colonne(int i ,int n ){
    char** l=malloc(n*sizeof(char*));
    for(int j=0;j<n;j++){
        l[j]=variable(j,i);
    }
    int *taille_phi=malloc(sizeof(int));
    char* phi = au_plus_une(l,n,taille_phi);
    for(int j=0;j<n;j++){
       free(l[j]);
    }
    
    free(taille_phi);
    free (l);
    return phi;
}

char *contrainte_toutes_colonnes(int n ){
    char** l = malloc(n*sizeof(char*));
    char *phi=NULL;
    for (int i = 0; i < n; i++){
        phi = contrainte_une_colonne(i,n);
        l[i] = phi;
    }
    char* res = separation_et (l,n);
    for(int j=0;j<n;j++){
        free(l[j]);
    }
        free(l);
        return res;
    }
    //Diagonale -1 -> diagonale qui commence à [1][0]
char *contrainte_une_diagonale(int i,int n ){
        
    char **l=malloc((n-abs(i))*sizeof(char*));

    
        if (i<0){
        i= -i ;
        int colonne=0; 
        for(int j=i;j<n;j++){
            l[colonne]=variable(j,colonne);
            colonne++;
        }
        
        }
        else{
        int ligne=0;
        for(int j=i;j<n;j++){
            l[ligne]=variable(ligne,j);
            ligne++;
        }
        }
        
        
        int *taille_phi=malloc(sizeof(int));
        char *phi=au_plus_une(l,(n-abs(i)),taille_phi);
        for(int j=0;j<n-abs(i);j++){
            free(l[j]);
        }
        

        
        free(l);
        free(taille_phi);
        
        
        return phi;


    }

char *contrainte_toute_diagonale(int n ){
    char** l = malloc(((2*n)-1)*sizeof(char*));
        
    l[0]=contrainte_une_diagonale(0,n);

        
    for (int i = -1; i > (-n); i--){
        //printf("%d\n",i);
        l[-i] = contrainte_une_diagonale(i,n);
    }
        
    for(int i=1;i<n ;i++){
        //printf("%d\n",i);
        l[i+n-1]=contrainte_une_diagonale(i,n);
        //printf("%s\n\n",l[i+n-1]);
    }
    char* res = separation_et (l,(2*n)-1);
    
    for(int j=0;j<2*n-1;j++){
        free(l[j]);
    }
        
    free(l);
    return res;
}

    void gen_formule_n_dames(int n,char* filename){
    char *contrainte1=contrainte_toutes_lignes(n);
    char *contrainte2=contrainte_toutes_colonnes(n);
    char *contrainte3=contrainte_toute_diagonale(n);

    char **l=malloc(3*sizeof(char*));
    l[0]=contrainte1;
    l[1]=contrainte2;
    l[2]=contrainte3;

    char *formule_resolution_pb_n_dames=separation_et(l,3);;
    printf("%s",formule_resolution_pb_n_dames);
    free(l[0]);
    free(l[1]);
    free(l[2]);
    free(l);
    
    FILE *fichier=fopen(filename,"w");
    fputs(formule_resolution_pb_n_dames,fichier);
    fclose(fichier);


    free(formule_resolution_pb_n_dames);


    }

    int main(){
        gen_formule_n_dames(8,"8dames.txt");
        
        return 0;
    }
