/*
 * main.c
 *    MINI PROJET SDA : SYSTEME DE FICHIER
 *  Creer le: 25/01/2020
 *  par: TOUIL ZOUHEIR ;KASSEL MOHAMMED ISSAM ; AIT AZIZ AMINA ;ACHOUCH MOUNIA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <string.h>
#include <stdint.h>
#include<windows.h>
#include<conio.h>
/* les fonctions crées
//File System Functions
fs* newFileSystem();
void destroy(fs*);
void ls(fs*,char*);
char* pwd(fs*,int);
char* recPWD(node*,char*,int,int);
void mkdir(fs*,char*);
void recAddDir(node*,char*);
void touch(fs*,char*);
void recAddFile(node*,char*);
void order(fs*);
void recOrder(node*);
void cd(fs*,char*);
void rm(fs*,char*,int);
void recremove(node*);
void rmf(fs*,char*);
void find(fs*,char*)
void recFind(fs*,node*,char*);
void findHelp(fs*,node*,char*);
void renommer_node(node* n,char* newname)
*/

// la structure d'un noeud de l'arbre

typedef struct node node;
struct node{
	char name[40];
	char type;
	node* P; //pere
	node* FC; //les fils
	node* NS; // les freres
};

// la structure de l'arbre d'un systeme de fichiers

struct filesystem{
	node* root;
	node* CD;
};

// initialiser l'arbre (creation de la racine)

typedef struct filesystem fs;
fs* newFileSystem(){
	fs* f = (fs*)malloc(sizeof(fs));
	f -> root = (node*)malloc(sizeof(node));
	strcpy(f -> root -> name, "/");
	f -> root -> FC = NULL;
	f -> root -> NS = NULL;
	f -> root -> P = NULL;
	strcpy(f -> root -> name, "/");
	f -> CD = f -> root;
	return f;
}

//chemin absolu rucursive

char* recPWD(node* n, char* PWD, int first, int finder){
	char* temp;
	char temp2[21];
	if(n != NULL){
		temp = strdup(PWD);  //strdup returne un pointeur
		if(strcmp(n -> name, "/") != 0 && first > 0){
			strcpy(temp2, n -> name);
			strcat(temp2, "/");
			strcpy(PWD, temp2);
		}
		else{
			printf('\0');
			strcpy(PWD, n -> name);
		}
		strcat(PWD, temp);
		recPWD(n -> P, PWD, 1, finder);
	}
	free(temp);
	return PWD;
}


char* pwd(fs* f,int finder){
	char PWD[1000];
	memset(PWD, 0, 1000);
	if(f -> CD == f -> root){
		strcpy(PWD, "/");
		return PWD;
	}
	return recPWD(f -> CD, PWD, 0, finder);
}

// comparer 2 noeuds (type / nom / taille )

int comparison(node* n1, node* n2){
	int size, x = 0;
	if(n1 -> type == 'D'){
		if(n2 -> type == 'F')
			return -1;
	}
	if(n1 -> type == 'F'){
		if(n2 -> type == 'D')
			return 1;
	}
	if(strcmp(n1 -> name, n2 -> name) == 0)
		return 0;
	if (strlen(n1 -> name) < strlen(n2 -> name))
		return -1;
	else if(strlen(n1 -> name) > strlen(n2 -> name))
		return 1;
	else
		size = strlen(n1 -> name);
	for(x = 0; x < size - 1; x++){
		char c1 = n1 -> name[x];
		char c2 = n2 -> name[x];
		if(c1 == c2)
			continue;
		if(c1 == '\0')
			return -1;
		if(c2 == '\0')
			return 1;
		if(c1 == '.')
			return -1;
		if(c2 == '.')
			return 1;
		if(c1 == '-')
			return -1;
		if(c2 == '-')
			return 1;
		if(c1 == '_')
			return -1;
		if(c2 == '_')
			return 1;
		if(c1 < c2)
			return -1;
		if(c1 > c2)
			return 1;
	}
	return 0;
}

// fonction pour creer un repertoire

void mkdir(fs* f, char* path){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){ // si le systeme de fichiers est vide autre que root
			temp = (node*)malloc(sizeof(node));
			node* newDir = (node*)malloc(sizeof(node));
			strcpy(newDir -> name, token);
			newDir -> type = 'D';
			newDir -> FC = NULL;
			newDir -> NS = NULL;
			newDir -> P = f -> root;
			f -> root -> FC = newDir;
			recAddDir(f -> root -> FC, token);
                        printf("\033[1;32m");
					printf("repertoire creer avec succes.\n");
                    printf("\033[0m");
			return;
		}

		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){ // correspondance trouvee, mais le repertoire trouve est vide
				token = strtok(NULL, "/");
				if(token == NULL){
                                        printf("\033[1;31m");
					printf("impossible de creer le repertoire.\n");
                                        printf("\033[0m");
					return;
				}
				node* newDir = (node*)malloc(sizeof(node));
				strcpy(newDir -> name, token);
				newDir -> type = 'D';
			    newDir -> FC = NULL;
			    newDir -> NS = NULL;
			    newDir -> P = temp;
				temp -> FC = newDir;
				temp = temp -> FC;
				recAddDir(temp, token);
                                printf("\033[1;32m");
					printf("repertoire creer avec succes.\n");
                    printf("\033[0m");

				return;
			}
			else{ // correspondance trouvee dans le repertoire courant, continuez vers l'interieur
				temp = temp -> FC;
				token = strtok(NULL, "/");
			}
		}
		else{ //correspondance introuvable, regardez l'entree suivante dans le repertoire courant
			if(temp -> NS == NULL){ //repertoire courant epuise, ajoutez le repertoire dans le repertoire courant
				node* newDir = (node*)malloc(sizeof(node));
				strcpy(newDir -> name, token);
				newDir -> type = 'D';
				newDir -> FC = NULL;
				newDir -> NS = NULL;
				newDir -> P = temp -> P;
				 printf("\033[1;32m");
					printf("repertoire creer avec succes.\n");
                    printf("\033[0m");
				//////  ordoner//////
				node* temp2 = temp -> P -> FC;
				int added = 0;
				newDir -> NS = temp2; //ajouter au debut du repertoire courant
				temp -> P -> FC = newDir;
				node* prev = newDir -> NS;
				while(newDir -> NS != NULL){
					if(comparison(newDir, newDir -> NS) == 1){ //si la priorite est inferieure, descendez dans la liste
						if(newDir == temp -> P -> FC){
							temp -> P -> FC = temp2;
						}
						node* temp3 = newDir -> NS -> NS;
						node* temp4 = newDir -> NS;
						newDir -> NS -> NS = newDir;
						newDir -> NS = temp3;
						if(prev != temp4){
							prev -> NS = temp4;
							prev = prev -> NS;
						}

						added = 1;
					}
					else{

						added = 1;
						break;
					}
				}
				////// ORDERING //////
				if(added == 0){
					temp -> NS = newDir;
					temp = temp -> NS;
				}
				recAddDir(temp, token);
				return;
			}
			else
				temp = temp -> NS;
		}
	}
}
//fonction de creation des repertoires recusrsivement

void recAddDir(node* n, char* token){
	token = strtok(NULL, "/");
	if(token != NULL){
		node* newNode = (node*)malloc(sizeof(node));
		strcpy(newNode -> name, token);
		newNode -> type = 'D';
		newNode -> FC = NULL;
		newNode -> NS = NULL;
		newNode -> P = n;
		n -> FC = newNode;
		recAddDir(n -> FC, token);
	}
}

// fonction pour creer un fichier

void touch(fs* f, char* path){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){ //si le systeme de fichiers est vide autre que root
			temp = (node*)malloc(sizeof(node));
			node* newFile = (node*)malloc(sizeof(node));  //pour creer le systeme de fichier
			strcpy(newFile -> name, token);
			newFile -> type = 'D';
			newFile -> FC = NULL;
			newFile -> NS = NULL;
			newFile -> P = f -> root;
			f -> root -> FC = newFile;
			token = strtok(NULL, "/"); ///
			recAddFile(f -> root -> FC, token);  //creation des fichier recursivement

			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){ //correspondance trouvee, mais le repertoire trouve est vide
				token = strtok(NULL, "/");
				if(token == NULL){
                    printf("\033[1;31m");
					printf("impossible de creer le fichier.\n");
                    printf("\033[0m");
					return;
				}
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				newFile -> type = 'D';
			    newFile -> FC = NULL;
			    newFile -> NS = NULL;
			    newFile -> P = temp;
				temp -> FC = newFile;
				temp = temp -> FC;
				token = strtok(NULL, "/"); ///
				recAddFile(temp, token);
				 printf("\033[1;32m");
					printf("fichier creer avec succes.\n");
                    printf("\033[0m");
				return;
			}
			else{ //correspondance trouvee dans le repertoire courant, continuez vers l'interieur
				temp = temp -> FC;
				token = strtok(NULL, "/");
			}
		}
		else{ //correspondance introuvable, regardez l'entree suivante dans le repertoire courant
			if(temp -> NS == NULL){ //repertoire courant epuise, ajoutez le repertoire dans le repertoire courant
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				token = strtok(NULL, "/");
				if(token == NULL)
					newFile -> type = 'F';
				else
					newFile -> type = 'D';
				newFile -> FC = NULL;
				newFile -> NS = NULL;
				newFile -> P = temp -> P;
				printf("\033[1;32m");
					printf("fichier creer avec succes.\n");
                    printf("\033[0m");
				////// ORDERING //////
				node* temp2 = temp -> P -> FC;
				int added = 0;
				newFile -> NS = temp2; // ajouter au debut du repertoire courant
				temp -> P -> FC = newFile;
				node* prev = newFile -> NS;
				while(newFile -> NS != NULL){
					if(comparison(newFile, newFile -> NS) == 1){ //
//si la priorite est inferieure, descendez dans la liste
						if(newFile == temp -> P -> FC){
							temp -> P -> FC = temp2;
						}
						node* temp3 = newFile -> NS -> NS;
						node* temp4 = newFile -> NS;
						newFile -> NS -> NS = newFile;
						newFile -> NS = temp3;
						if(prev != temp4){
							prev -> NS = temp4;
							prev = prev -> NS;
						}

						added = 1;
					}
					else{

						added = 1;
						break;
					}
				}
				////// ORDERING //////
				if(added == 0){
					temp -> NS = newFile;
					temp = temp -> NS;
				}
				recAddFile(newFile, token);
				return;
			}
			else
				temp = temp -> NS;
		}
	}
}
void copier(fs* f, char* path){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){ //si le systeme de fichiers est vide autre que root
			temp = (node*)malloc(sizeof(node));
			node* newFile = (node*)malloc(sizeof(node));  //pour creer le systeme de fichier
			strcpy(newFile -> name, token);
			newFile -> type = 'D';
			newFile -> FC = NULL;
			newFile -> NS = NULL;
			newFile -> P = f -> root;
			f -> root -> FC = newFile;
			token = strtok(NULL, "/"); ///
			recAddFile(f -> root -> FC, token);  //creation des fichier recursivement

			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){ //correspondance trouvee, mais le repertoire trouve est vide
				token = strtok(NULL, "/");
				if(token == NULL){
                    printf("\033[1;31m");
					printf("impossible de copier le fichier.\n");
                    printf("\033[0m");
					return;
				}
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				newFile -> type = 'D';
			    newFile -> FC = NULL;
			    newFile -> NS = NULL;
			    newFile -> P = temp;
				temp -> FC = newFile;
				temp = temp -> FC;
				token = strtok(NULL, "/"); ///
				recAddFile(temp, token);
				 printf("\033[1;32m");
					printf("fichier a ete copie avec succes.\n");
                    printf("\033[0m");
				return;
			}
			else{ //correspondance trouvee dans le repertoire courant, continuez vers l'interieur
				temp = temp -> FC;
				token = strtok(NULL, "/");
			}
		}
		else{ //correspondance introuvable, regardez l'entree suivante dans le repertoire courant
			if(temp -> NS == NULL){ //repertoire courant epuise, ajoutez le repertoire dans le repertoire courant
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				token = strtok(NULL, "/");
				if(token == NULL)
					newFile -> type = 'F';
				else
					newFile -> type = 'D';
				newFile -> FC = NULL;
				newFile -> NS = NULL;
				newFile -> P = temp -> P;
				printf("\033[1;32m");
					printf("fichier a ete copie avec succes.\n");
                    printf("\033[0m");
				////// ORDERING //////
				node* temp2 = temp -> P -> FC;
				int added = 0;
				newFile -> NS = temp2; // ajouter au debut du repertoire courant
				temp -> P -> FC = newFile;
				node* prev = newFile -> NS;
				while(newFile -> NS != NULL){
					if(comparison(newFile, newFile -> NS) == 1){ //
//si la priorite est inferieure, descendez dans la liste
						if(newFile == temp -> P -> FC){
							temp -> P -> FC = temp2;
						}
						node* temp3 = newFile -> NS -> NS;
						node* temp4 = newFile -> NS;
						newFile -> NS -> NS = newFile;
						newFile -> NS = temp3;
						if(prev != temp4){
							prev -> NS = temp4;
							prev = prev -> NS;
						}

						added = 1;
					}
					else{

						added = 1;
						break;
					}
				}
				////// ORDERING //////
				if(added == 0){
					temp -> NS = newFile;
					temp = temp -> NS;
				}
				recAddFile(newFile, token);
				return;
			}
			else
				temp = temp -> NS;
		}
	}
}



// la fonction de creation des fichier recursivement

void recAddFile(node* n, char* token){
	if(token == NULL){
		n -> type = 'F';
	}
	if(token != NULL){
		node* newNode = (node*)malloc(sizeof(node));
		strcpy(newNode -> name, token);
		newNode -> type = 'D';
		newNode -> FC = NULL;
		newNode -> NS = NULL;
		newNode -> P = n;
		n -> FC = newNode;
		token = strtok(NULL, "/");
		recAddFile(n -> FC, token);
	}
}


//la fonction pour ordonner le system de fichier

void order(fs* f){   //entree: un systeme de fichier
	recOrder(f -> root -> FC);
}
//la fonction pour ordonner recursivement le system de fichier
void recOrder(node* n){
	if(n -> NS != NULL){
		printf("%s", n -> name);
		if(comparison(n, n -> NS) == 1){
			node* temp = n;
			n = n -> NS;
			n -> NS = temp;
			recOrder(n);
		}
	}
	if(n -> NS != NULL){
		recOrder(n -> NS);
	}
}
//la fonction renommer


// la fonction rechercher d'un fichier ou  repertoire

void find(fs* f, char* name){
	printf("A la recherche de '%s':\n ", name);
	printf("\e[0;101m");
	 printf(" \t(s'il exist vous aurez tout les chemins absolus de cet element :>; sinon vous aurez rien :<)\n");
	 printf("\033[0m");
	recFind(f,f -> root -> FC, name);
}
// la fonction rechercher recursivement d'un fichier ou repertoire
void recFind(fs* f, node* n, char* name){
    findHelp(f, n, name);
	if(n -> FC != NULL){
		recFind(f, n -> FC, name);
	}
	if(n -> NS != NULL){
		recFind(f, n -> NS, name);
	}

}

// fonction pour l'execution du recherche recursive
void findHelp(fs* f, node* n, char* name){
	int i = 0;
	int j = 0;
	int c = 0;
	char PWD[1000];
	memset(PWD, 0, 1000);
	int complete = strlen(name);
	for(i = 0; i < strlen(n -> name); i++){
		if(n -> name[i] == name[j]){
			c++;
			j++;
			if(c == complete){
				node* temp = f -> CD;
				f -> CD = n;
				strcpy(PWD, pwd(f,1));
				printf("%c ", n -> type);
				printf("%s\n", PWD);
                f -> CD = temp;

				break;
                    }
		}
	}
}
void renommer(fs* f, char* path){
    char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){ //si le systeme de fichiers est vide autre que root
			temp = (node*)malloc(sizeof(node));
			node* newFile = (node*)malloc(sizeof(node));  //pour creer le systeme de fichier
			strcpy(newFile -> name, token);
			newFile -> type = 'D';
			newFile -> FC = NULL;
			newFile -> NS = NULL;
			newFile -> P = f -> root;
			f -> root -> FC = newFile;
			token = strtok(NULL, "/"); ///
			recAddFile(f -> root -> FC, token);  //creation des fichier recursivement

			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){ //correspondance trouvee, mais le repertoire trouve est vide
				token = strtok(NULL, "/");
				if(token == NULL){
                    printf("\033[1;31m");
					printf("impossible de renommer le fichier.\n");
                    printf("\033[0m");
					return;
				}
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				newFile -> type = 'D';
			    newFile -> FC = NULL;
			    newFile -> NS = NULL;
			    newFile -> P = temp;
				temp -> FC = newFile;
				temp = temp -> FC;
				token = strtok(NULL, "/"); ///
				recAddFile(temp, token);
				 printf("\033[1;32m");
					printf("fichier renommer avec succes.\n");
                    printf("\033[0m");
				return;
			}
			else{ //correspondance trouvee dans le repertoire courant, continuez vers l'interieur
				temp = temp -> FC;
				token = strtok(NULL, "/");
			}
		}
		else{ //correspondance introuvable, regardez l'entree suivante dans le repertoire courant
			if(temp -> NS == NULL){ //repertoire courant epuise, ajoutez le repertoire dans le repertoire courant
				node* newFile = (node*)malloc(sizeof(node));
				strcpy(newFile -> name, token);
				token = strtok(NULL, "/");
				if(token == NULL)
					newFile -> type = 'F';
				else
					newFile -> type = 'D';
				newFile -> FC = NULL;
				newFile -> NS = NULL;
				newFile -> P = temp -> P;
				printf("\033[1;32m");
					printf("fichier renommer avec succes.\n");
                    printf("\033[0m");
				////// ORDERING //////
				node* temp2 = temp -> P -> FC;
				int added = 0;
				newFile -> NS = temp2; // ajouter au debut du repertoire courant
				temp -> P -> FC = newFile;
				node* prev = newFile -> NS;
				while(newFile -> NS != NULL){
					if(comparison(newFile, newFile -> NS) == 1){ //
//si la priorite est inferieure, descendez dans la liste
						if(newFile == temp -> P -> FC){
							temp -> P -> FC = temp2;
						}
						node* temp3 = newFile -> NS -> NS;
						node* temp4 = newFile -> NS;
						newFile -> NS -> NS = newFile;
						newFile -> NS = temp3;
						if(prev != temp4){
							prev -> NS = temp4;
							prev = prev -> NS;
						}

						added = 1;
					}
					else{

						added = 1;
						break;
					}
				}
				////// ORDERING //////
				if(added == 0){
					temp -> NS = newFile;
					temp = temp -> NS;
				}
				recAddFile(newFile, token);
				return;
			}
			else
				temp = temp -> NS;
		}
	}
}
void renommerrec(fs* f, char* path, int force){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){
			return;
		}
		node* temp2 = f -> root -> FC;
		node* temp3 = f -> root -> FC;
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){
				token = strtok(NULL, "/");
				if(token == NULL){
					node* parent = temp -> P;
					temp2=temp->NS;
					parent -> FC = NULL;
					parent->FC=temp2;
					free(temp);

					return;
				}
				else{
					return;
				}
			}
			else{
				token = strtok(NULL, "/");
				if(token == NULL){
					if(force == 1){
						recRemove(temp);
						return;
					}
					else{
						return;
					}
				}
				temp = temp -> FC;
			}
		}
		else{
			if(temp -> NS == NULL){

				return;
			}
			else{
				temp = temp -> NS;
			}
		}
	}
}



// supprimer un fichier ou un repertoire

void rm(fs* f, char* path, int force){
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;
	while(1){
		if(temp == NULL){
                        printf("\033[1;31m");
			printf("impossible de supprimer le fichier ou le repertoire.\n");
                        printf("\033[0m");
			return;
		}
		node* temp2 = f -> root -> FC;
		node* temp3 = f -> root -> FC;
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){
				token = strtok(NULL, "/");
				if(token == NULL){
					node* parent = temp -> P;
					temp2=temp->NS;
					parent -> FC = NULL;
					parent->FC=temp2;
					free(temp);

					return;
				}
				else{   printf("\033[1;31m");
					printf("impossible de supprimer le fichier ou le repertoire.\n");
                        printf("\033[0m");
					return;
				}
			}
			else{
				token = strtok(NULL, "/");
				if(token == NULL){
					if(force == 1){
						recRemove(temp);
						return;
					}
					else{   printf("\033[1;31m");
						printf("impossible de supprimer le repertoire '%s' il n est pas vide.\n", path);
                            printf("\033[0m");
						return;
					}
				}
				temp = temp -> FC;
			}
		}
		else{
			if(temp -> NS == NULL){
                                printf("\033[1;31m");
		                printf("impossible de supprimer le fichier ou le repertoire.\n");
                                printf("\033[0m");
				return;
			}
			else{
				temp = temp -> NS;
			}
		}
	}
}

//la fonction pour supprimer un element recursivement

void recRemove(node* n){
	if(n -> FC != NULL){
		recRemove(n -> FC);
	}
	if(n -> NS != NULL){
		recRemove(n -> NS);
	}
	node* parent = n -> P;
	parent -> FC = NULL;
	free(n);

}





// naviguer entre les repertoires

void cd(fs* f, char* path){
	if(strcmp(path, "/") == 0){
		f -> CD = f -> root;
		return;
	}
	char* token = strtok(path, "/");
	node* temp = f -> root -> FC;

	while(1){
		if(token == NULL || (strcmp(token, ".") != 0 && strcmp(token, "..") != 0)){ //retourner a la methods cd normale
			break;
		}
		if(strcmp(token, ".") == 0){
			temp = f -> CD;
			token = strtok(NULL, "/");
			if(token == NULL){
				f -> CD = temp;
				return;
			}
		}
		else if(strcmp(token, "..") == 0){
			if(f -> CD == f -> root)
				temp = f -> root;
			else{
				temp = f -> CD -> P;
				f -> CD = f -> CD -> P;
			}
			token = strtok(NULL, "/");
			if(token == NULL){
				f -> CD = temp;
				return;
			}
		}
	}

	while(1){
		if(temp == NULL){
                        printf("\033[1;31m");
			printf("impossible de changer le repertoire .\n");
                        printf("\033[0m");
			return;
		}
		if(strcmp(temp -> name, token) == 0){
			if(temp -> FC == NULL){
				token = strtok(NULL, "/");
				if(token == NULL){
					if(temp -> type == 'D'){
						f -> CD = temp;
						return;
					}
					else{   printf("\033[1;31m");
						printf("impossible de changer le repertoire .\n");
                            printf("\033[0m");
						return;
					}
				}
				else{   printf("\033[1;31m");
					printf("impossible de changer le repertoire .\n");
                        printf("\033[0m");
					return;
				}
			}
			else{
				token = strtok(NULL, "/");
				if(token == NULL){
					if(temp -> type == 'D'){
						f -> CD = temp;
						return;
					}
					else{   printf("\033[1;31m");
						printf("impossible de changer le repertoire .\n");
                            printf("\033[0m");
						return;
					}
				}
				temp = temp -> FC;
			}
		}
		else{
			if(temp -> NS == NULL){
                                printf("\033[1;31m");
				printf("impossible de changer le repertoire .\n");
                                printf("\033[0m");
				return;
			}
			else{
				temp = temp -> NS;
			}
		}
	}
}

// lister le contenu du repertoire courant
void ls(fs* f, char* path){
	if(strcmp(path, "") == 0){
		node* temp = f -> CD -> FC;
		char* PWD = pwd(f,0);
		printf("lister les elements pour'%s':\n", PWD);
		printf("\e[0;101m");
        printf(" \t(s'il exist vous aurez tout les elements avec ses types :>; sinon vous aurez rien :<)\n");
        printf("\033[0m");
		while(temp != NULL){
			if(temp -> type == 'D'){
				printf("\033[1;34m");
				printf("D ");
				printf("%s\n", temp -> name);
                temp = temp -> NS;
                printf("\033[0m");
			}
			else
                {
                    printf("\033[1;36m");
				printf("F ");
				printf("%s\n", temp -> name);
                temp = temp -> NS;
                 printf("\033[0m");
                }
		}
	}
	else{
		char* token = strtok(path, "/");
		node* temp = f -> root -> FC;
		while(1){
			if(temp == NULL){
                                printf("\033[1;31m");
				printf("impossible de lister les elements du repertoire.\n");
                                printf("\033[0m");
				return;
			}
			if(strcmp(token, ".") == 0){
				temp = f -> CD;
				token = strtok(NULL, "/");
				if(token == NULL)
					break;
			}
			if(strcmp(token, "..") == 0){
				if(f -> CD == f -> root){
					temp = f -> root;
				}
				else{
					temp = f -> CD -> P;
					f -> CD = f -> CD -> P;
				}
				token = strtok(NULL, "/");
				if(token == NULL)
					break;
			}
			if(strcmp(temp -> name, token) == 0){
				if(temp -> FC == NULL){ //correspondance trouvŽe, mais le rŽpertoire trouvŽ est vide
					token = strtok(NULL, "/");
					if(token == NULL)
						break;
					else{   printf("\033[1;31m");
						printf("impossible de lister les elements du repertoire.\n");
                            printf("\033[0m");
						return;
					}
				}
				else{ //
//correspondance trouvee dans le repertoire courant, continuez a l'interieur
					token = strtok(NULL, "/");
					if(token == NULL){
						break;
					}
					temp = temp -> FC;
				}
			}
			else{ //correspondance introuvable, regardez l'entree suivante dans le repertoire courant
				if(temp -> NS == NULL){ //repertoire courant epuise, ajoutez le repertoire dans le repertoire courant
                        printf("\033[1;31m");
					printf("impossible de lister les elements du repertoire.\n");
                        printf("\033[0m");
					return;
				}
				else
					temp = temp -> NS;
			}
		}
		if(temp -> type == 'F'){
                        printf("\033[1;31m");
			printf("impossible de lister les elements. '%s' est un fichier.\n", path);
                        printf("\033[0m");
			return;
		}
		else{
			printf("lister les elements pour : '/%s':\n", path);
        printf("\e[0;101m");
        printf(" \t(s'il exist vous aurez tout les elements avec ses types :>; sinon vous aurez rien :<)\n");
        printf("\033[0m");
			temp = temp -> FC;
			while(temp != NULL){
				if(temp -> type == 'D')
                {
                    printf("\033[1;34m");
				printf("D ");
				printf("%s\n", temp -> name);
                temp = temp -> NS;
                printf("\033[0m");
                }
				else
					{
					    printf("\033[1;36m");
				printf("F ");
				printf("%s\n", temp -> name);
                temp = temp -> NS;
                 printf("\033[0m");
					}

			}
		}
	}
}
void ls1(fs* f, char* path){
	if(strcmp(path, "") == 0){
		node* temp = f -> CD -> FC;
		char* PWD = pwd(f,0);
		printf("lister pour l'element '%s':\n", PWD);
        printf("\e[0;101m");
        printf(" \t(s'il exist vous aurez tout les repertoires :>; sinon vous aurez rien :<)\n");
        printf("\033[0m");
		while(temp != NULL){
			if(temp -> type == 'D'){
				printf("\033[1;34m");
				printf("D ");
				printf("%s\n", temp -> name);
                printf("\033[0m");
                }
                temp = temp -> NS;

		}
	}

}
void ls2(fs* f, char* path){
	if(strcmp(path, "") == 0){
		node* temp = f -> CD -> FC;
		char* PWD = pwd(f,0);
		printf("lister pour l'element %s':\n", PWD);
        printf("\e[0;101m");
        printf(" \t(s'il exist vous aurez tout les fichiers:>; sinon vous aurez rien :<)\n");
        printf("\033[0m");
		while(temp != NULL){
			if(temp -> type == 'F'){
			printf("\033[1;36m");
				printf("F ");
				printf("%s\n", temp -> name);
                 printf("\033[0m");
                }
                temp = temp -> NS;

		}
	}

}


// la fonction HELP
void HELP(char* str){
    if(strcmp(str, "creer-f") == 0)
    {

        printf("\t=============================================================================================================\n");
        printf("\t| creer-f <nom>:  cree un fichier vide, sans aucune donnee associee.                                        |\n");
        printf("\t=============================================================================================================\n");
    }
    if(strcmp(str, "creer-rep") == 0){
        printf("\t=============================================================================================================\n");
        printf("\t| creer-rep <nom>:   cree un repertoire vide, sans aucun fils.                                              |\n");
        printf("\t=============================================================================================================\n");
         printf("\t=============================================================================================================\n");
        printf("\t|NOTE:cette commande  affichera une erreur si la ressource specifiee n'existe pas.                          |\n");
        printf("\t=============================================================================================================\n");
    }
    if(strcmp(str, "rechercher") == 0){
        printf("\t=============================================================================================================\n");
        printf("\t| rechercher <nom>:    recherche toutes les ressources portant le nom specifie dans le systeme de fichiers. |\n");
        printf("\t=============================================================================================================\n");
        printf("\t=============================================================================================================\n");
        printf("\t|NOTE: <rechercher> elle donne Le chemin de toutes les ressources rencontrees .                              \n");
        printf("\t=============================================================================================================\n");
    }
    if(strcmp(str, "chemin") == 0){
        printf("\t=============================================================================================================\n");
        printf("\t| chemin:    affiche le chemin vers le repertoire courant.                                                  |\n");
        printf("\t=============================================================================================================\n");
    }
    if(strcmp(str, "lister") == 0){
        printf("\t=============================================================================================================\n");
        printf("\t| lister:    liste tous les elements du repertoire courant.avec d'autres parametres  -D/-F/..               |\n");
        printf("\t=============================================================================================================\n");
    }
    if(strcmp(str,"--")==0)
        {
printf("La structure du systeme de fichiers peut distinguer 11 commandes differentes:\n");
printf("\n");
printf("\t=============================================================================================================\n");
printf("\t| creer-f <nom>:  cree un fichier vide, sans aucune donnee associee.                                        |\n");
printf("\t=============================================================================================================\n");
printf("\t| creer-rep <nom>:   cree un repertoire vide, sans aucun fils.                                              |\n");
printf("\t=============================================================================================================\n");
printf("\t| cd <nom>:   prend le chemin et passe d'un repertoire a un autre.                                          |\n");
printf("\t=============================================================================================================\n");
printf("\t| copier <nom> <path>:    copie un fichier dans le path indique                                             |\n");
printf("\t=============================================================================================================\n");
printf("\t| supprimer <nome>:supprime un fichier ou bien repertoire vide !!                                           |\n");
printf("\t=============================================================================================================\n");
printf("\t| supprimer -f <nom>:    supprime recursivement un repertoire et tous ses descendants.                      |\n");
printf("\t=============================================================================================================\n");
printf("\t|NOTE:cette commande  affichera une erreur si la ressource specifiee n'existe pas.                          |\n");
printf("\t=============================================================================================================\n");
printf("\t| rechercher <nom>:    recherche toutes les element portant le nom specifie dans le systeme de fichiers.    |\n");
printf("\t=============================================================================================================\n");
printf("\t| renommer <oldname><newname>:     pour renommer un fichier et le donner le newname                         |\n");
printf("\t=============================================================================================================\n");
printf("\t| lister:    liste tous les elements du repertoire courant.Avec d'autre parametre -D/-F/..                  |\n");
printf("\t=============================================================================================================\n");
printf("\t| chemin:    affiche le chemin absolu vers le repertoire courant.                                           |\n");
printf("\t=============================================================================================================\n");
printf("\t| help --: si vous avez un probleme tapez help -- !!                                                        |\n");
printf("\t=============================================================================================================\n");
printf("\t|NOTE: Si vous tapez help <nome du commande> vous aurez les information necessaire !!                       |\n");
printf("\t=============================================================================================================\n");
printf("\t| quitter:    termine l'execution du programme de gestion du systeme de fichiers.                           |\n");
printf("\t=============================================================================================================\n");
                    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction PRINCIPALE
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    char choix;
    int n=0;
    //la gestion de l'interface

do{     system ("cls");
        system ("Color 79 ");

        printf("\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("\t\t\t\t\t@@         ");
        printf("\033[1;31m");
        printf("mini-projet SDA");
        printf("\033[1;34m");
        printf("            @@\n");
        printf("\t\t\t\t\t||                                    ||\n");
        printf("\t\t\t\t\t@@  Encadre par:                      @@\n");
        printf("\t\t\t\t\t||     ");
        printf("\033[1;30m");
        printf("* Touhami Ouazzani Khadija");
        printf("\033[1;34m");
        printf("     ||\n");
        printf("\t\t\t\t\t||     ");
        printf("\033[1;30m");
        printf("* Lazrek mohamed          ");
        printf("\033[1;34m");
        printf("     ||\n");

        printf("\t\t\t\t\t||                                    ||\n");
        printf("\t\t\t\t\t@@   Realise par :                    @@\n");
        printf("\t\t\t\t\t||     ");
        printf("\033[1;30m");
        printf("* TOUIL ZOUHEIR           ");
        printf("\033[1;34m");
        printf("     ||\n");        printf("\t\t\t\t\t||     ");
        printf("\033[1;30m");
        printf("* KASSEL MAHEMMED ISSAM   ");
        printf("\033[1;34m");
        printf("     ||\n");        printf("\t\t\t\t\t||     ");
        printf("\033[1;30m");
        printf("* AIT AZIZ AMINA          ");
        printf("\033[1;34m");
        printf("     ||\n");        printf("\t\t\t\t\t@@     ");
        printf("\033[1;30m");
        printf("* ACHOUCH MOUNIA          ");
        printf("\033[1;34m");
        printf("     @@\n");
        printf("\t\t\t\t\t||                                    ||\n");
        printf("\t\t\t\t\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("\033[1;31m");
        printf("\t\t\t\t\t        System de gestion de fichier    \n");
        printf("\033[1;34m");
        printf("\t\t\t\t\t________________________________________\n");
        printf("\t\t\t\t\t| |                                    |\n");
        printf("\t\t\t\t\t|1|      Ouvrir le gestionnaire        |\n");
        printf("\t\t\t\t\t| |                                    |\n");
        printf("\t\t\t\t\t========================================\n");
        printf("\t\t\t\t\t| |                                    |\n");
        printf("\t\t\t\t\t|2|     Les indications d'utilisation  |\n");
        printf("\t\t\t\t\t| |                                    |\n");
        printf("\t\t\t\t\t========================================\n");
        printf("\t\t\t\t\t| |                                    |\n");
        printf("\t\t\t\t\t|3|      Quitter l'application         |\n");
        printf("\t\t\t\t\t|_|____________________________________|\n");
        printf("\n\r Tapez svp votre choix ?  (1/2/3) \n");
        choix=toupper(getch());
        switch(choix){
			case '1' :system ("cls");
                     system ("color 7");
                        printf("\t\t\t\t\t________________________________________\n");
                        printf("\t\t\t\t\t||                                    ||\n");
                        printf("\t\t\t\t\t||     System de gestion de fichier   ||\n");
                        printf("\t\t\t\t\t||                                    ||\n");
                        printf("\t\t\t\t\t========================================\n");

    fs* f = newFileSystem();
    printf("\033[5;33m");
    printf("\t\t\t\t\tBienvenu dans le systeme de fichiers :) !! \n" );
    printf("\033[0m");

	char command[20];
	char path[1000];
	char nom[1000];
	char newname[40];
	char name[40];
	while(scanf("%s", command) > 0){
                if((strcmp(command, "renommer") != 0) && (strcmp(command, "copier") != 0)&& (strcmp(command, "creer-rep") != 0) && (strcmp(command, "creer-f") != 0) && (strcmp(command, "supprimer") != 0) && (strcmp(command, "supprimer -f") != 0) && (strcmp(command, "cd") != 0) && (strcmp(command, "chemin") != 0) && (strcmp(command, "lister") != 0 )&& (strcmp(command, "lister-D") != 0 )&& (strcmp(command, "lister-F") != 0 )&& (strcmp(command, "rechercher") != 0) && (strcmp(command, "quitter") != 0) && (strcmp(command, "help") != 0 )){
			printf("\033[1;31m");
			printf("La Command %s est introuvable :( pour plus d'information tapez help -- !!\n",command);
            printf("\033[0m");
		}
		if(strcmp(command, "creer-rep") == 0){
        if(n==0){scanf("%s",path );mkdir(f, path);cd(f, path);} //la creation de l'arbre pour la 1 er fois
                else{
			scanf("%s",nom );
			char* PWD = pwd(f,0);
			int i;
			for (i = 0; PWD[i]!='\0'; i++);
			PWD[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
			mkdir(f, PWD);
                }
		}
		 if(strcmp(command, "renommer") ==0){


            scanf("%s", nom);
            scanf("%s",newname );
			char* PWD = pwd(f,0);
			int i;
			for (i = 0; PWD[i]!='\0'; i++);
			PWD[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
				renommerrec(f, PWD, 0);


			char* PWDy = pwd(f,0);
			int k;
			for (k = 0; PWDy[k]!='\0'; k++);
			PWD[k]='/';
			k++;

            for (int j = 0; newname[j]!='\0'; j++, k++)
                {
                    PWDy[k] = newname[j];
                }
                PWDy[k] = '\0';
			renommer(f, PWDy);

                }

		if(strcmp(command, "creer-f") == 0){
            scanf("%s",nom );
			char* PWD = pwd(f,0);
			int i;
			for (i = 0; PWD[i]!='\0'; i++);
			PWD[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
			touch(f, PWD);
		}

		if(strcmp(command, "supprimer") == 0){
			scanf("%s", nom);
			if(strcmp(nom, "-f") == 0){
            scanf("%s", nom);
			char* PWD = pwd(f,0);
			int i;
			for (i = 0; PWD[i]!='\0'; i++);
			PWD[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
				rm(f, PWD, 1);
			}
			else{
			char* PWD = pwd(f,0);
			int i;
			for (i = 0; PWD[i]!='\0'; i++);
			PWD[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
				rm(f, PWD, 0);
			}
		}
		if(strcmp(command, "supprimer -f") == 0){
		    scanf("%s", nom);
			char* PWD = pwd(f,0);
			int i;
			for (i = 0; PWD[i]!='\0'; i++);
			PWD[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
				rm(f, PWD, 1);
		}
		if(strcmp(command, "rechercher") == 0){
			scanf("%s", name);
			find(f, name);
		}
		if(strcmp(command, "cd") == 0){
			scanf("%s", nom);
                if(strcmp(nom, "..") != 0)
                {
                char* PWD = pwd(f,0);
                int i;
                for (i = 0; PWD[i]!='\0'; i++);
                PWD[i]='/';
                i++;

                for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    PWD[i] = nom[j];
                }
                PWD[i] = '\0';
			   cd(f, PWD);
                }
                else
                    cd(f,nom);
		}

		if(strcmp(command, "quitter") == 0){
			system ("cls");
                        system ("Color 79");
			           printf("\t\t\t\t\t________________________________________\n");
                       printf("\t\t\t\t\t||                                    ||\n");
                       printf("\t\t\t\t\t||    Fin du Programme Gestionnair    ||\n");
                       printf("\t\t\t\t\t||                                    ||\n");
                       printf("\t\t\t\t\t========================================\n");
                exit(0);
		}
		if(strcmp(command, "chemin") == 0){
			char* PWD = pwd(f,0);
			printf("%s\n", PWD);
		}
		if(strcmp(command, "lister-D") == 0){
			memset(path, 0, 1000);
			gets(path);
			if(path[0] == '0'){
				strcpy(path, "");
			}
			else{
				char* p = path;
				p += 1;
				strcpy(path, p);
			}
			ls1(f, path);
		}
		else
            if(strcmp(command, "lister-F") == 0){
			memset(path, 0, 1000);
			gets(path);
			if(path[0] == '0'){
				strcpy(path, "");
			}
			else{
				char* p = path;
				p += 1;
				strcpy(path, p);
			}
			ls2(f, path);
            }
        if(strcmp(command, "lister") == 0){
			memset(path, 0, 1000);
			gets(path);
			if(path[0] == '0'){
				strcpy(path, "");
			}
			else{
				char* p = path;
				p += 1;
				strcpy(path, p);
			}
			ls(f, path);
            }
            if(strcmp(command, "copier") == 0)
            {
                scanf("%s",nom );
                scanf("%s",path);
			int i;
			for (i = 0; path[i]!='\0'; i++);
			path[i]='/';
			i++;

            for (int j = 0; nom[j]!='\0'; j++, i++)
                {
                    path[i] = nom[j];
                }
                path[i] = '\0';
			copier(f, path);

            }




        if (strcmp(command, "help") == 0){
                        scanf("%s", path);
                        HELP(path);
                }
        if(n=1)
            {
                char* PWD = pwd(f,0);
			printf("%s  >>\t", PWD);
            }
            n=1;

	}
	case '2' : system ("cls");
                    system ("Color 79");
                       printf("\t\t\t\t\t________________________________________\n");
                       printf("\t\t\t\t\t||                                    ||\n");
                       printf("\t\t\t\t\t||    les indications d'utilisation   ||\n");
                       printf("\t\t\t\t\t||                                    ||\n");
                       printf("\t\t\t\t\t========================================\n");
                       printf("\n");
printf("La structure du systeme de fichiers peut distinguer les commandes suivantes:\n");
printf("\n");
printf("\t=============================================================================================================\n");
printf("\t| creer-f <nom>:  cree un fichier vide, sans aucune donnee associee.                                        |\n");
printf("\t=============================================================================================================\n");
printf("\t| creer-rep <nom>:   cree un repertoire vide, sans aucun fils.                                              |\n");
printf("\t=============================================================================================================\n");
printf("\t| cd <nom>:   prend le chemin et passe d'un repertoire a un autre.                                          |\n");
printf("\t=============================================================================================================\n");
printf("\t| copier <nom> <path>:    copie un fichier dans le path indique                                             |\n");
printf("\t=============================================================================================================\n");
printf("\t| supprimer <nome>:supprime un fichier ou bien repertoire vide !!                                           |\n");
printf("\t=============================================================================================================\n");
printf("\t| supprimer -f <nom>:    supprime recursivement un repertoire et tous ses descendants.                      |\n");
printf("\t=============================================================================================================\n");
printf("\t|NOTE:cette commande  affichera une erreur si la ressource specifiee n'existe pas.                          |\n");
printf("\t=============================================================================================================\n");
printf("\t| rechercher <nom>:    recherche toutes les element portant le nom specifie dans le systeme de fichiers.    |\n");
printf("\t=============================================================================================================\n");
printf("\t|renommer <oldname><newname>:     pour renommer un fichier et le donner le newname                          |\n");
printf("\t=============================================================================================================\n");
printf("\t| lister:    liste tous les elements du repertoire courant.Avec d'autre parametre -D/-F/..                  |\n");
printf("\t=============================================================================================================\n");
printf("\t| chemin:    affiche le chemin absolu vers le repertoire courant.                                           |\n");
printf("\t=============================================================================================================\n");
printf("\t| help --: si vous avez un probleme tapez help -- !!                                                        |\n");
printf("\t=============================================================================================================\n");
printf("\t|NOTE: Si vous tapez help <nome du commande> vous aurez les information necessaire !!                       |\n");
printf("\t=============================================================================================================\n");
printf("\t| quitter:    termine l'execution du programme de gestion du systeme de fichiers.                           |\n");
printf("\t=============================================================================================================\n");


			           printf("\nAppuyer sur une touche pour revenir au menu\n");
                       getch();
			           break;
            case '3' : system ("cls");
                        system ("Color 79");
			           printf("\t\t\t\t\t________________________________________\n");
                       printf("\t\t\t\t\t||                                    ||\n");
                       printf("\t\t\t\t\t||       Quitter l'application        ||\n");
                       printf("\t\t\t\t\t||                                    ||\n");
                       printf("\t\t\t\t\t========================================\n");
                exit(0);
                 break;

            default:
                    {
                        printf("numero de menu invalide\n");
                        Sleep(500);
                    }
        }
}while(1==1);

}
