#include <string.h>
#include <stdio.h>
#include "boolean.h"

#define BLANK ' '
#define NEWLINE '\n'
#define NULLREF '\0'
#define NEXT '>'

//DEFINISI TIPE DAN VARIABEL
typedef int IdxType;
typedef char string[1000];
typedef string state;
typedef struct{
	state Qawal;
	state Transition;
	state Qakhir;
}graph;

FILE *in;
string baris,initialState,input, namafile;;
graph TabTrans[100];
string TabLintasan[100],TabState[100],TabFinalState[100],TabAcceptedInput[100];
IdxType i,j,k;
int lengthOfFinalState = 0,lengthInput=1,lengthOfLintasan=0, row = 1;
boolean accepted;

//DEKLARASI FUNGSI DAN PROSEDUR
void getDaftarState(string f, string TabInput[]);
//I.S menerima masukan string f dan array of string TabInput
//F.S mengolah string f dan memparsing f kedalam TabInput
void salinDelta(string f, int k);
// I.S menrima string yang mengambarkan transisi
// F.s Menghasilkan tabel transisi
void bacaFileEksternal(string fileName);
//I.S menerima masukan berupa file yang berisi DFA
//F.S Membaca isi file dan menyimpan isi file didalam memory internal 
void bacaInputUser();
//I.S membaca masukan berupa array of character
//F.S memasukan ke dalam memory internal
void cekDFA();
// melakukan pengecekan apkah valid input user terhadap tabel transition 
void printLintasan(int panjangLintasan);
// menampilkan lintasan yang ditempuh
boolean isFinalState(int i, int j, int x);
//mengasilkan true jika tablintasan ke j didalam tabel transition.

int main(){
    printf("Masukkan nama file yang akan dibaca: ");
    gets(namafile);
    bacaFileEksternal(namafile);
    bacaInputUser();
    cekDFA();
    printLintasan(lengthOfLintasan);
    if(accepted && isFinalState(i,j,lengthOfFinalState)){
        printf("\nMasukan DITERIMA\n");
    } else {
        printf("\nMasukan tidak DITERIMA\n");
    }
	return 0;
}

//IMPLEMENTASI FUNGSI DAN PROSEDUR
void getDaftarState(string f, string TabInput[]){
    int i=0,j,k=0;
	while(f[i] != NEWLINE ){
		int j=0;
		while((f[i] != BLANK )&&(f[i] != NEWLINE)){
			TabInput[k][j] = f[i]; 
			i++;
			j++;
		}if(f[i] != NEWLINE ){
			i++;
		}
		k++;
    }
}

void salinDelta(string f, int k){
    char word[100];
    int i=0,j=0;
    memset(word,'\0',sizeof(word));
	while(f[i]!=BLANK)
	{
		word[j]=f[i];
		i++;
		j++;
    }
    strcpy(TabTrans[k].Qawal,word);
    i++;
	j=0;
    memset(word,'\0',sizeof(word));
    while(f[i]!=BLANK)
	{
		word[j]=f[i];
		i++;
		j++;
    }
    strcpy(TabTrans[k].Transition,word);
	i++;
    j=0;
    memset(word,'\0',sizeof(word));
    while(f[i]!=NEWLINE && f[i] != NULLREF)
	{
		word[j]=f[i];
		i++;
		j++;
    }
    strcpy(TabTrans[k].Qakhir,word);
}

void bacaFileEksternal(string fileName){
    in = fopen(fileName,"r");
    fgets(baris,sizeof(baris),in);
    getDaftarState(baris,TabState); 
    fgets(baris,sizeof(baris),in);
    strtok(baris,"\n");
    strcpy(initialState,baris);
	fgets(baris,sizeof(baris),in);
	getDaftarState(baris,TabFinalState);
	while(strcmp(TabFinalState[lengthOfFinalState],"\0") != 0 && strcmp(TabFinalState[i]," ") != 0){
        lengthOfFinalState++;
    }
    fgets(baris,sizeof(baris),in);
    getDaftarState(baris,TabAcceptedInput);
    while(!feof(in)){
        fgets(baris,sizeof(baris),in);
        salinDelta(baris, row);		
		row++;
    }
    fclose(in);
}

void bacaInputUser(){
    printf("Masukkan input string dibatasi '>':\n");
    strcpy(TabLintasan[0],initialState);
    gets(input);
    i=0;
    while(input[i] != NULLREF){
        j=0;
        while((input[i] != NEXT)&&(input[i] != NULLREF)){
            TabLintasan[lengthInput][j] = input[i];
            j++;
            i++;
        }
        lengthInput++;
        if(input[i] == NEXT){
            i++;
        }
    }
}

void cekDFA(){
    for(j=0;j<lengthInput-1;j++){
        accepted = false;
        for(i=1;i<=row && !accepted;i++){
            if(strcmp(TabLintasan[j],TabTrans[i].Qawal) == 0 && strcmp(TabLintasan[j+1],TabTrans[i].Transition) == 0){
                lengthOfLintasan++;
                strcpy(TabLintasan[lengthOfLintasan],TabTrans[i].Qakhir);
                accepted = true;
            }
        }
        if(!accepted){
            lengthOfLintasan++;
            strcpy(TabLintasan[lengthOfLintasan],"rejected");
            break;
        }
    }
}

void printLintasan(int panjangLintasan){
    printf("\nLintasan yang ditempuh DFA:\n");
    printf("start---> ");
    for(int i=0;i<=panjangLintasan;i++){
        printf("%s",TabLintasan[i]);
        if(i != panjangLintasan){
            printf(" -> ");
        }
    }
}

boolean isFinalState(int i, int j, int x){
    boolean yesFinal=false;
    for(i=0;i<=x && !yesFinal;i++){
        if(strcmp(TabLintasan[j],TabFinalState[i]) == 0){
            yesFinal=true;
        }
    }
    return yesFinal;
}