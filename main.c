/* 
 * File:   main.c
 * Author: Administrator
 *
 * Created on June 4, 2020, 1:10 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define HASH_TABLE_SIZE 50000

typedef struct hashTable hash_Tb;
typedef struct hashItem hash_It;

struct hashTable{
  hash_It** items;
  int size;
  int count;  
};

struct hashItem {
    char* key;
    char* value;
};

unsigned long hashFunc(char* str) {
    unsigned int hash = 0;
    int c;
    while (c = *str++)
        hash += c;
    return hash;
}

hash_It* createItem(char* key,char* value){
    hash_It* item = (hash_It*)malloc(sizeof(hash_It));
    item->value = (char*)malloc(strlen(value)+1);
    item->key = (char*)malloc(strlen(key)+1);
    strcpy(item->key,key);
    strcpy(item->value,value);
    return item;
}

hash_Tb* createTable(int size){
    hash_Tb* table= (hash_Tb*)malloc(sizeof(hash_Tb));
     table->size = size;
    table->count = 0;
    table->items = (hash_It**)calloc(table->size,sizeof(hash_It*)); 
     for(int i = 0;i<table->size;i++){
         table->items[i] = NULL;
     }
    return table;
}

void freeItem(hash_It* item){
    free(item->key);
    free(item->value);
    free(item);
}

void freeTable(hash_Tb* table){
    for(unsigned long i=0;i<table->size;i++){
       if(table->items[i]) 
        freeItem(table->items[i]);
    }
    free(table->items);
    free(table);
}


void hashTableInsert(hash_Tb* table,char* key,char* value){
    unsigned long index = hashFunc(key);
    hash_It* item = createItem(key,value);
    table->items[index] = item;
    table->count++;
}

char* searchTable(hash_Tb* table,char* key){
    unsigned long index = hashFunc(key);
    if(!table->items[index]){
        return NULL;
    }
    return table->items[index]->value;
}

void hashTableDel(hash_Tb* table,char* key){
    unsigned long index = hashFunc(key);
    hash_It* item = table->items[index];
    if(item==NULL){
        printf("Key doesn't exist!\n");
        return;
    }
    else{
        table->items[index] = NULL;
        freeItem(item);
        table->count--;
        return;
    }
}

void printSearch(hash_Tb* table,char* key){
    char* val = searchTable(table,key);
    if(!val){
        printf("%s doesn't exist!\n",key);
        return;
    }
    else{
        printf("%s : %s\n",key,val);
        return;
    }
}

void saveFile(FILE * fp,hash_Tb* table){
    fp = fopen("index.dat","w");
     for(int i=0;i<table->size;i++){
        if(table->items[i]){
              fprintf(fp,"%d ",i);
              fprintf(fp,"%s\n",table->items[i]->key);
        } 
     }
    fclose(fp);
  
    fp = fopen("value.dat","w");
      for(int i=0;i<table->size;i++){
        if(table->items[i]){
          fprintf(fp,"%d",i);
          fprintf(fp," %s\n",table->items[i]->value);
        }
     }
    fclose(fp);
}

hash_It* allocateItems(){
    hash_It* item = (hash_It*)malloc(sizeof(hash_It));
    item->key = (char*)malloc(50*sizeof(char));
    item->value = (char*)malloc(50*sizeof(char));
    return item;
}

void loadFile(FILE *fp,hash_Tb* table){
    char buffer[50];
    int index;
    int i=0;
    char * token;
    hash_It** item = table->items;
    fp=fopen("index.dat","r");
    while(fgets(buffer, 50, fp)!=NULL){
        token = strtok(buffer," ");
        i=0;
        while(token!=NULL){
            if(i==0){
              index = atoi(token);
              i++;
            }else if(i!=0){
                item[index] = (hash_It*)malloc(sizeof(hash_It));
                item[index]->key = (char*)malloc(strlen(token)+1);
                strcpy(item[index]->key,token);
                i++;
            }     
            printf("%s",token);
            token = strtok(NULL,buffer);
        }
    }
    
    fclose(fp);
    i=0;
    fp=fopen("value.dat","r");
    while(fgets(buffer, 50, fp)!=NULL){
         token = strtok(buffer," ");
         i=0;
         while(token!=NULL){
             if(i==0){
                 index = atoi(token);
                 i++;
             }
             else if(i==1){
                item[index]->value = (char*)malloc(80*sizeof(char));
                strcpy(item[index]->value,token);
                i++;
             }
             else{
                 strcat(item[index]->value," ");
                 strcat(item[index]->value,token);
             }
            printf("%s",token);
            token = strtok(NULL,buffer); 
         }
    }
    fclose(fp);
}

void Menu(){
    
    printf("=================DICTIONARY APP====================\n");
    printf("                 1.Create word\n");
    printf("                 2.Edit word\n");
    printf("                 3.Look up meaning\n");
    printf("                 0.Exit\n");
}
int getInt(char prompt[31]) {
	int num;
	char enter;
	do {
		printf("%s",prompt);
		if(scanf("%d%c",&num,&enter) !=2) {
			printf("Invalid input!\n");
			while (getchar() != '\n' );
		} else if(enter !='\n') {
			printf("Invalid input\n");
			while (getchar() != '\n' );
		} else if(num>3||num<0) {
			printf("Invalid input!\n");
		} else
			return num;
	} while(1);
}


bool check_File(FILE * fp,char fileIndex[],char fileValue[]){
   if (fp = fopen(fileIndex, "r")) {
      fclose(fp);
      printf("file exists\n");
      return true;
   } else {
      printf("file doesn't exist\n");
      printf("Creatingfile ...\n");
      printf("SUCCESS!...\n");
      fp=fopen(fileIndex,"w");
      fclose(fp);
      fp=fopen(fileValue,"w");
      fclose(fp);
      return false;
   }
}

bool checkExistWord(hash_Tb* table,char* key){
    int index = hashFunc(key);
    if(!table->items[index]){
        return true;
    }else{
        return false;
    }
}

void printTable(hash_Tb* table){
    printf("===================TABLE============================\n");
    for(int i =0;i<table->size;i++){
        if(table->items[i]){
            printf("%s : %s\n",table->items[i]->key,table->items[i]->value);
        }
    }
}

int main() {
    hash_Tb* table = createTable(HASH_TABLE_SIZE);
    int select;
    char word[30];
    char mean[50];
    char file1[] = "index.dat";
    char file2[] = "value.dat";
    FILE * fp;
       bool check;
    check = check_File(fp,file1,file2);
    loadFile(fp,table);
    while(true){
          Menu();
    select= getInt("Choose option");
    switch(select){
        case 1:
            printf("Enter new word: ");
            gets(word);
            if(checkExistWord(table,word)){
             printf("Enter meaning: ");
            gets(mean);
            hashTableInsert(table,word,mean);
            saveFile(fp,table);
            }else{
                printf("Sorry,this word already exist!\n");
            }
            break;
        case 2:
            printf("Enter word: ");
            gets(word);
            printf("Enter meaning: ");
            gets(mean);
            hashTableInsert(table,word,mean);
            saveFile(fp,table);
            break;
        case 3:
            printf("Enter word to lookup: ");
            gets(word);
            printSearch(table,word);
            break;
        default:
            return 0;
    }
    }
    freeTable(table);
    return 0;
}