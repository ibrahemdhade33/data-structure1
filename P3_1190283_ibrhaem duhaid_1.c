#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxchar 1000
#define lf 1.33333333333
/*
 *
 *
 */
//defin a struct for the info
struct pinfo{
    char name[30];
    char gender[2];
    char dateofadmission[20];
    char dateofbirth[20];
    char ill[20];
    char adress [100];
    char blodtype[4];
};
/*
 *
 *
 *
 *
 *
 */
//the struct for the hash table
struct hash{
    char status;
    struct pinfo p;
};
typedef struct hash h;
/*
 *
 *
 *
 */
//the struct for the avl tree
struct  node {
   struct pinfo pdata;
    struct node *left,*right;
    int hight;
};
/*
 *
 *
 */
typedef  struct node *bst;
bst root1=NULL;
/*
 *
 *
 *
 *
 *
 */
//define the functions and variables of the hash table to use it under the main function
int size=0;
unsigned int size1=0;
h *hashtable;
int i=0;
unsigned int m=0;
int isprime(int n);
void inorderhash(bst root);
unsigned int hashvalue(char name[30],unsigned int size,unsigned int count);
int getprimesize(int s);
void hash(struct pinfo p);
double loadfactor();
void printhastablewithembtyslot();
h *rehash(h *temp);
void creathashtable();
void searchhash(char name[30]);
void deleterecord(char name[30]);
void printhastablewithembtyslotfile(FILE *outin );
/*
 *
 *
 *
 */
int isembty(){
    return root1==NULL;
}
/*
 *
 *
 */
//function to return which height is bigger
int maxhight(int x, int y){
    if(x>=y)
        return x;
    return y ;
}
/*
 *
 *
 *
 *
 *
 *
 *
 */
//function to get the height of aany node in the avl tree
int getHeight (bst  t){

    if(t==NULL)
        return -1;
   else
       return t->hight;
}
/*
 *
 *
 *
 *
 *
 *
 */
//function that return the balance factor of any node in the avl tree
int getBalance(bst N) {
    if (N == NULL)
        return 0;
    return getHeight(N->left) - getHeight(N->right);
}
/*
 *
 *
 *
 *
 *
 */
//find the max value in the tree
bst max(bst root){
    if(isembty())
        printf("the tree is empty, there is no max value\n");
    else if(root->right==NULL)
        return root;
    else
        return max(root->right);
    return NULL;
}
/*
 *
 *
 *
 *
 *
 */
//find the min value in the tree
bst min(bst root){
    if(isembty())
        printf("the tree is empty, there is no min value\n");
    else if(root->left==NULL)
        return root;
    else
        return min(root->left);
    // return NULL;
}
/*
 *
 *
 *
 *
 */
//make the tree empty
void freetree(bst root){

    if(root!=NULL){
        freetree(root->left);
        freetree(root->right);
        free(root);
        root=NULL;

    }
    printf("the tree has been freed  \n");
}
/*
 *
 *
 *
 *
 */
//single rotate left
bst srl(bst x) {
    bst y;
    y         = x->right;
    x->right  = y->left;
    y->left = x;

    x->hight = 1 + maxhight(getHeight(x->left) , getHeight(x->right));
    y->hight = 1 + maxhight(getHeight(x) , getHeight(y->right));

    return y;
}
/*
 *
 *
 */
//single rotate right
bst srr(bst y) {
    bst x;

    x = y->left;

    y->left  = x->right;
    x->right = y;

    y->hight = 1 + maxhight(getHeight(y->right) , getHeight(y->left));
    x->hight = 1 + maxhight(getHeight(y) , getHeight(x->left));

    return x;
}
/*
 *
 *
 *
 *
 */
//double rotate left
bst drl(bst K3) {
    K3->left  = srl(K3->left);
    return srr(K3);
}
/*
 *
 *
 *
 */
//double rotate right
bst drr(bst K3) {
    K3->right  = srr(K3->right);
    return srl(K3);
}
/*
 *
 *
 *
 */
//delete node from tree
bst deletefromtree(bst root,char element[30]){

    if(root==NULL) {
        printf("the element %d is not in the tree\n",element);
        return NULL;
    }
    else if(strcmp(element,root->pdata.name)==0)
    {
        bst temp;
        if(!root->left && !root->right ) {
            temp=root;
            free(temp);
            root = NULL;

        }
        else if(root->left && !root->right)
        {
            temp=root;
            root=root->left;
            free(temp);

        }
        else if(!root->left && root->right)
        {
            temp=root;
            root->left=root->right;
            root=root->left;
            free(temp);
            return root;
        }
        else {
            bst minnode=min(root->right);

            strcpy(root->pdata.name,minnode->pdata.name);
            root->right= deletefromtree(root->right,minnode->pdata.name);
            return root;
        }
    }
    else if (strcmp(element,root->pdata.name)<0)
        root->left= deletefromtree(root->left,element);
    else if (strcmp(element,root->pdata.name)>0)
        root->right= deletefromtree(root->right,element);
    if(root==NULL)
        return root;
    else
        root->hight=1+ maxhight(getHeight(root->right), getHeight(root->left));
    //delete from right sub tree

    if (getHeight(root->left) - getHeight(root->right) == 2 &&
        getHeight(root->left->left) - getHeight(root->left->right) == 1)
        root = srr(root);
    else if (getHeight(root->left) - getHeight(root->right) == 2 &&
             getHeight(root->left->left) - getHeight(root->left->right) == 0)
        root = srr(root);
    else  if(getHeight(root->left) - getHeight(root->right) == 2 &&
             getHeight(root->left->left) - getHeight(root->left->right) == -1){
        root->left = srl(root->left);
        root = srr(root);
    }


    else if(getHeight(root->left)- getHeight(root->right)==-2&&
            getHeight(root->right->right)- getHeight(root->right->left)==1)
        root= srl(root);
    else  if(getHeight(root->left)- getHeight(root->right)==-2&&
             getHeight(root->right->right)- getHeight(root->right->left)==0)
        root= srl(root);
    else if(getHeight(root->left)- getHeight(root->right)==-2&&
            getHeight(root->right->right)- getHeight(root->right->left)==-1)  {
        root->right = srr(root->right);
        root = srl(root);
    }

    return root;
}
/*
 *
 *
 *
 *
 *
 *
 */
//insert new node to the tree
bst insert(bst root , struct pinfo element ){

    if(root==NULL) {
        root = (bst) malloc(sizeof(struct node));
        root->pdata=element;
        root->left=root->right=NULL;
        size++;
    }
    else if(strcmp(element.name,root->pdata.name)<0)
        root->left = insert(root->left, element);
    else if(strcmp(element.name,root->pdata.name)>0) {
        root->right = insert(root->right, element);

    }
    else
        return root;
    root->hight=1+ maxhight(getHeight(root->right), getHeight(root->left));
    int bf= getBalance(root);
    if (bf > 1 && strcmp(element.name , root->left->pdata.name)<0)
      root= srr(root);

    // Right Right Case
    if (bf < -1 &&strcmp(element.name , root->right->pdata.name)>0 )
  root= srl(root);

    // Left Right Case
    if (bf > 1 && strcmp(element.name , root->left->pdata.name)>0)
    {
      root= drl(root);
    }

    // Right Left Case
    if (bf < -1 &&strcmp(element.name , root->right->pdata.name)<0 )
    {
       root=drr(root);
    }

    /* return the (unchanged) node pointer */
    return root;
}
/*
 *
 *
 *
 *
 *
 *
 *
 */
//find and update patient
void findandupdate(bst root,char element[30]){
    struct pinfo update;
    int todo;
    if(root==NULL)
        printf("the element is not in the tree\n");
    else  if(strcmp(element,root->pdata.name)==0){
        printf(" the record  founded and it is %s\n",root->pdata.name);
        printf("do you want to update the information of the patient ?,if yes press 1, else press any other key you want...\n");

        scanf("%d",&todo);
        if(todo==1){
            update=root->pdata;
             todo=0;
             char str[50]="";
            printf("please chose what you want to update : \n"
                   "1-name : \n"
                   "2-gender : \n"
                   "3-date of admission : \n"
                   "4-date of birth : \n"
                   "5-illness : \n"
                   "6-address : \n"
                   "7-blood type : \n"
                   "to back to menu enter 8 : \n");
//char todo1='\0';
            scanf("%d",&todo);
            while(todo!=8 ){
                switch (todo) {
                    case 1:
                        printf("enter new name : \n");
                        getchar();
                        gets(str);
                        strcpy(update.name,str);

                        break;
                    case 2:
                        printf("enter the new gender : \n");
                        getchar();
                        gets(str);
                        strcpy(update.gender,str);

                        break;
                    case 3:
                        printf("enter the new date of admission : \n");
                        getchar();
                        gets(str);
                        strcpy(update.dateofadmission,str);

                        break;
                    case 4:
                        printf("enter the new date of birth : \n");
                        getchar();
                        gets(str);
                        strcpy(update.dateofbirth,str);

                        break;
                    case 5:
                        printf("please enter a new illness : \n");
                        getchar();
                        gets(str);
                        strcpy(update.ill,str);

                        break;
                    case 6:
                        printf("please update the address : \n");
                        getchar();
                        gets(str);
                        strcpy(update.adress,str);
                        break;
                    case 7:
                        printf("please update the address : \n");
                        getchar();
                        gets(str);
                        strcpy(update.blodtype,str);
                        break;
                    default:
                        printf("out of range\n");
                        break;
                }
                printf("please chose what you want to update : \n"
                       "1-name : \n"
                       "2-gender : \n"
                       "3-date of admission : \n"
                       "4-date of birth : \n"
                       "5-illness : \n"
                       "6-address : \n"
                       "7-blood type : \n"
                       "to back to menu enter 8 : \n");
                scanf("%d",&todo);

            }
            if(strcmp(update.name,root->pdata.name)!=0) {
                root1 = deletefromtree(root1, element);
                root1= insert(root1,update);
            }
            else
                root->pdata=update;
        }
    }
    else if (strcmp(element,root->pdata.name)<0)
        findandupdate(root->left,element);
    else if (strcmp(element,root->pdata.name)>0)
        findandupdate(root->right,element);

}
/*
 *
 *
 *
 *
 *
 */
//print the patient in the alphabetical order
void inorder(bst root){
    if(root!=NULL) {
        inorder(root->left);
        printf(" name : %s, ", root->pdata.name);
        printf("gender : %s, ", root->pdata.gender);
        printf("date of admission : %s, ", root->pdata.dateofadmission);
        printf("date of birth : %s, ", root->pdata.dateofbirth);
        printf("illness : %s, ", root->pdata.ill);
        printf("address : %s, ", root->pdata.adress);
        printf("blode type : %s", root->pdata.blodtype);
        printf("\n");
        inorder(root->right);
    }
}
/*
 *
 *
 *
 *
 *
 *
 */
//print the patient whose have the sam illness
void inordersearch(bst root,char ill[20]){
    if(root!=NULL) {
        inordersearch(root->left,ill);
        if(strcmp(ill,root->pdata.ill)==0)
        printf(" %s\n ", root->pdata.name);
        inordersearch(root->right,ill);
    }
}
/*
 *
 *
 *
 *
 *
 */
//read file
void readfile(char filename[30]) {
    struct pinfo p;
   FILE *fp;
    char str[maxchar];
int size7=0;

    fp = fopen(filename, "a+");
    fprintf(fp,"\n");
    fclose(fp);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s", filename);
    }
    while (fgets(str, maxchar, fp) != NULL ) {
        char *token;
int f=0;
        /* get the first token */
        token = strtok(str, "#");

    strcpy(p.name, token);
    f++;

        //printf( " %s\n", token );
        /* walk through other tokens */
        while( token != NULL && f!=7 && strcmp(token,"\n")!=0) {
            token = strtok(NULL, "#");
            if(f==1) {
                strcpy(p.gender,token);
                f++;
            }
            else if(f==2){
                strcpy(p.dateofadmission,token);
            f++;
            }
           else if(f==3){
                strcpy(p.dateofbirth,token);
                f++;
            }
            else if(f==4){
                strcpy(p.ill,token);
                f++;
            }
            else if(f==5) {
                strcpy(p.adress, token);
                f++;
            }
            else if (f==6) {
                strcpy(p.blodtype, token);
                 f++;
            }
           // printf( " %s\n", token );

        }
if(strcmp(token,"\n")==0)
    break;
        root1=insert(root1,p);
        size7++;
    }

   // printf("%d\n",size7);
size=size7;
    fclose(fp);
}
/*
 *
 *
 *
 *
 */
//print with in order for the file
bst inorderfile(bst root,FILE *outin){


    if(root!=NULL) {
        inorderfile(root->left,outin);
        fprintf(outin," name : %s, ", root->pdata.name);
        fprintf(outin,"gender : %s, ", root->pdata.gender);
        fprintf(outin,"date of admission : %s, ", root->pdata.dateofadmission);
        fprintf(outin,"date of birth : %s, ", root->pdata.dateofbirth);
        fprintf(outin,"illness : %s, ", root->pdata.ill);
        fprintf(outin,"address : %s, ", root->pdata.adress);
        fprintf(outin,"blode type : %s", root->pdata.blodtype);
        inorderfile(root->right,outin);
    }


}
int main() {
    FILE *outin;

int todo=0;
    struct pinfo new;
    char str[30];
    char ill[20];
    printf("------------------------------------------------------------------------------------------------------------------\n"
           "please chose from the menu, or enter 8 to exit from the program : \n\n"
           "1-Read the file patients.txt and load the data to the avl tree : \n"
           "2-Insert a new patient from user with all its associated data : \n"
           "3-Find a patient and give the user the option to update the information of the patient if found : \n"
           "4-List all patients in lexicographic order with their associated information : \n"
           "5-List all patients that have the same illness : \n"
           "6-Delete a patient from the system : \n"
           "7-Save all words in file “patients_hash.data : \n"
           "8-exit from the system : \n"
           "------------------------------------------------------------------------------------------------------------------\n");
    scanf("%d",&todo);
    while (todo !=8){
        switch (todo) {
            case 1:
                readfile("patients.txt");
                printf("the file has been redden successfully\n");
                break;
            case 2:
                printf("please enter the name : ");
                getchar();

                gets(new.name);
                printf("please enter the gender : ");
               // getchar();
                gets(new.gender);

                printf("please enter the date of admision : ");
                //getchar();

                gets(new.dateofadmission);
                printf("please enter the date of birth : ");
                //getchar();

                gets(new.dateofbirth);
                printf("please enter the illness : ");
                //getchar();

                gets(new.ill);
                printf("please enter the address : ");
               // getchar();

                gets(new.adress);
                printf("please enter the blood type : ");
                getchar();

                gets(new.blodtype);
                root1=insert(root1,new);
                printf("the new record added to the avl tree\n");
                break;
            case 3:
                printf("enter the name of patient to search for : \n");
                getchar();
                gets(str);
                findandupdate(root1,str);
                break;
            case 4:
                printf("the patient in the lexicographic order is : \n\n ");
                inorder(root1);
                break;
            case 5:
                printf("please enter the illness : \n");
                getchar();
                gets(ill);
                inordersearch(root1,ill);
                break;
            case 6:
                printf("please enter the name of the patient : \n");
                getchar();
                gets(str);
                root1= deletefromtree(root1,str);
                printf("the patient deleted (:(:(:\n");
                break;
            case 7:
                outin=fopen("patients_hash.txt", "a+");
                fprintf(outin,"the data that stored in the tree is : \n-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
                inorderfile(root1,outin);
                fprintf(outin,"-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
                fclose(outin);
                printf("the data from avl tree saved to file successfully (:(:(: \n\n");
                printf("if you want to go to hashing part enter 1 ,or any other number to back to menu  : \n");

                int todohash=0;
                scanf("%d",&todohash);

                if(todohash==1) {
                    creathashtable();
                    inorderhash(root1);

                    printf("------------------------------------------------------------------------------------------------------------------\n"
                           "please chose from the menu, or enter 8 to back to the avl tree menu : \n\n"
                           "1-Print hashed table (i.e., print the entire table to the screen including empty spots). : \n"
                           "2-Print out table size : \n"
                           "3-Print out the used hash function : \n"
                           "4-Insert a new record into the hash table : \n"
                           "5-Search for a specific patient : \n"
                           "6-Delete a specific record : \n"
                           "7-Save hash table back to file : \n"
                           "8-back to menu  : \n"
                           "------------------------------------------------------------------------------------------------------------------\n");
                    scanf("%d",&todohash);
                    while (todohash!=8){

                        switch(todohash){
                            case 1:
                                printf("the hash table with embty slot is : \n\n");
                                printhastablewithembtyslot();
                                break;
                            case 2:
                                printf("the size of the hash table is : %d\n",size1);
                                break;
                            case 3:
                                printf("find the sum of the asci code of the name \n: while(the counter >0 ) { \nsum=sum +(int)name[counter]\ncounter--\n}\n");
                                printf("the hash value=((sum mod table size)+(count*(1+hv mod (table size-1)))) mod table size, count=0,1,2,3,4,...... used for collision \n"
                                       "the method used is double hashing\n"
                                       "the size at first decided by multiply the size of the avl tree by 1.3333333333 and chose the first prime number after the result to save the balanced factor about 0.75\n"
                                       "for rehashing the the size of the table multiply by 2 then chose the first prime number after the result \n ");
                                break;
                            case 4:
                                printf("please enter the name : ");
                                getchar();

                                gets(new.name);
                                printf("please enter the gender : ");
                                getchar();

                                gets(new.gender);
                                printf("please enter the date of admision : ");
                                getchar();

                                gets(new.dateofadmission);
                                printf("please enter the date of birth : ");
                                getchar();

                                gets(new.dateofbirth);
                                printf("please enter the illness : ");
                                getchar();
                                gets( new.ill);
                                printf("please enter the address : ");
                                getchar();

                                gets(new.adress);
                                printf("please enter the blood type : ");
                                getchar();
                                gets(new.blodtype);
                                hash(new);
                                printf("the recored added to the hash table\n");
                                break;
                            case 5:
                                printf("enter the name you want to search in the hash table : \n");
                                getchar();
                                gets(str);
                                searchhash(str);
                                break;
                            case 6:
                                printf("enter the name you want to delete from the hash table : \n");
                                getchar();
                                gets(str);
                                deleterecord(str);

                                break;
                            case 7:
                                outin=fopen("patients_hash.txt", "a+");
                                fprintf(outin,"the data that stored in the hash table is : \n-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
                              printhastablewithembtyslotfile(outin);
                                fprintf(outin,"-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
                                fclose(outin);
                                printf("data saved to file (:(:(:\n");
                                break;
                            default:
                                printf("out of range (;(;\n");

                        }


                    printf("------------------------------------------------------------------------------------------------------------------\n"
                           "please chose from the menu, or enter 8 to back to the avl tree menu : \n\n"
                           "1-Print hashed table (i.e., print the entire table to the screen including empty spots). : \n"
                           "2-Print out table size : \n"
                           "3-Print out the used hash function : \n"
                           "4-Insert a new record into the hash table : \n"
                           "5-Search for a specific patient : \n"
                           "6-Delete a specific record : \n"
                           "7-Save hash table back to file : \n"
                           "8-back to menu  : \n"
                           "------------------------------------------------------------------------------------------------------------------\n");
                    scanf("%d", &todohash);
                    }
                }
                break;
            default:
                printf("out of range!!!\n");
                break;
        }
        printf("\n------------------------------------------------------------------------------------------------------------------\n"
               "please chose from the menu : \n\n"
               "1-Read the file patients.txt and load the data to the avl tree : \n"
               "2-Insert a new patient from user with all its associated data : \n"
               "3-Find a patient and give the user the option to update the information of the patient if found : \n"
               "4-List all patients in lexicographic order with their associated information : \n"
               "5-List all patients that have the same illness : \n"
               "6-Delete a patient from the system : \n"
               "7-Save all words in file “patients_hash.data : \n"
               "8-exit from the system : \n"
               "------------------------------------------------------------------------------------------------------------------\n");
todo=0;
        scanf("%d",&todo);
    }
    //inorder(root1);
    printf("thanks for using our program.......bye bye (:(:(:(:3>3> \n\n");
    return 0;
}
/*
 *
 *
 *
 *
 *
 */
//creat the hash table
void creathashtable(){
i=0;
    size1=(int)size*lf;

    size1= getprimesize(size1);

    hashtable= (h *)malloc(size1* sizeof(struct hash));
    for (int i = 0; i < size1; ++i) {
        hashtable[i].status='e';
    }
}
/*
 *
 *
 *
 *
 */
//chech the number is prime or not
int isprime(int n) {
    int flag = 0;
    if (n == 1)
        flag= 1;
    else
    {
    for (int i = 2; i <= n / 2; i++) {

         if (n % i == 0) {
            flag = 1;
            break;
        } else
            flag = 0;
    }
    }
    return flag;
}
/*
 *
 *
 *
 *
 *
 */
//load the data form avl tree to the hash table
void inorderhash(bst root){
    if(root!=NULL ) {
        inorderhash(root->left);
        hash(root->pdata);
        inorderhash(root->right);
    }
}
/*
 *
 *
 *
 *
 *
 */
//calculate the index
unsigned int hashvalue(char name[30],unsigned int size3, unsigned int count){
   unsigned int hv=0;
   unsigned int len= strlen(name);
    //printf("%d\n",len);

    for ( int j = len-1; j >=0 ; j--) {
        hv+=(int)name[j];
    }
    hv=((hv%size3)+(count*((1+hv%(size3-1)))))%size3;
    return hv;
}
/*
 *
 *
 *
 *
 *
 */
//to get prime size for the table
int getprimesize(int s){
    while(isprime(s)==1)
    {
        s+=1;
    }
    return s;
}
/*
 *
 *
 *
 *
 *
 *
 */
//hash function to stored the data from tree to the table
 void hash(struct pinfo p){
    unsigned int count=0;
    unsigned int y= hashvalue(p.name,size1,count);
        LOOP : if(hashtable[y].status=='e' || hashtable[y].status=='d'){

        hashtable[y].p=p;
        hashtable[y].status='o';
        i++;

    }else{
        count++;
        y= hashvalue(p.name,size1,count);
        goto LOOP;
    }
        count=0;
        if(loadfactor()>0.75)
        {
            h *temp=(h *)malloc(size1* sizeof(struct hash));
            for (int j = 0; j < size1; ++j) {
                temp[j].status=hashtable[j].status;
                temp[j].p=hashtable[j].p;
            }
            m=size1;
           // printf("%d\n",size1);
            size1=size1*2;
            size1= getprimesize(size1);
            hashtable=(h *)malloc(size1* sizeof(struct hash));
            for (int i = 0; i < size1; ++i) {
                hashtable[i].status='e';
            }

           hashtable= rehash(temp);
        }
}
/*
 *
 *
 *
 *
 *
 */
//calculate the load factor
double loadfactor(){
    return (double)i/size1;
}
/*
 *
 *
 *
 *
 *
 */
//to rehash the data if the laod factor is more than 0.75
h *rehash(h *temp){
int j=0;
    unsigned int count=0;
    unsigned int y;
   while(j<m){
       if(temp[j].status=='o'){
        y= hashvalue(temp[j].p.name,size1,count);
       LOOP : if(hashtable[y].status=='e' || hashtable[y].status=='d'){

       hashtable[y].p=temp[j].p;
       hashtable[y].status='o';
       j++;
       }
       else{
       count++;
       y= hashvalue(temp[j].p.name,size1,count);
       goto LOOP;
   }
       }
       else
           j++;
       count=0;
   }
    return hashtable;
}
/*
 *
 *
 *
 *
 *
 *
 */
//print the hash table
void printhastablewithembtyslot(){
    for (int j = 0; j < size1; ++j) {
        printf("at index %d ->",j);
        if(hashtable[j].status=='o') {
            printf("patient name : %s,", hashtable[j].p.name);
            printf("patient gender : %s,", hashtable[j].p.gender);
            printf("patient date of admission : %s, ", hashtable[j].p.dateofadmission);
            printf("patient date of birth : %s,", hashtable[j].p.dateofbirth);
            printf("patient illness : %s,", hashtable[j].p.ill);
            printf("patient address : %s,", hashtable[j].p.adress);
            printf("patient blood type : %s", hashtable[j].p.blodtype);
        }
        else
            printf("empty\n");
    }
}
/*
 *
 *
 *
 *
 *
 *
 */
//search for a record in the hash table
void searchhash(char name[30]){
    unsigned int count=0;
    unsigned int y= hashvalue(name,size1,count);
    LOOP:   if(strcmp(hashtable[y].p.name,name)==0 &&hashtable[y].status!='d'){
        printf("the patient found at index %d and his information is : \n\n",y);
        printf("patient name : %s,", hashtable[y].p.name);
        printf("patient gender : %s,", hashtable[y].p.gender);
        printf("patient date of admission : %s, ", hashtable[y].p.dateofadmission);
        printf("patient date of birth : %s,", hashtable[y].p.dateofbirth);
        printf("patient illness : %s,", hashtable[y].p.ill);
        printf("patient address : %s,", hashtable[y].p.adress);
        printf("patient blood type : %s", hashtable[y].p.blodtype);
    }
   else if(hashtable[y].status=='e' ||hashtable[y].status=='d' )
        printf("the patient is not in the table ):):\n");
    else {
        count++;
        y= hashvalue(name,size1,count);
        goto LOOP;
    }
}
/*
 *
 *
 *
 *
 *
 */
//delete record form the hash table
void deleterecord(char name[30]){
    unsigned int count=0;
    unsigned int y= hashvalue(name,size1,count);
    LOOP: if(strcmp(hashtable[y].p.name,name)==0){
        hashtable[y].status='d';
    printf("the patient deleted (:(:\n");
    }
    else if(hashtable[y].status=='e')
        printf("the patient you want to delete is not in the hash table ):):):\n");
    else {
    count++;
    y= hashvalue(name,size1,count);
    goto LOOP;
    }
}
/*
 *
 *
 *
 *
 *
 */
//print the hash table to file
void printhastablewithembtyslotfile(FILE *outin ){
    for (int j = 0; j < size1; ++j) {
        fprintf(outin,"at index %d ->",j);
        if(hashtable[j].status=='o') {
            fprintf(outin,"patient name : %s,", hashtable[j].p.name);
            fprintf(outin,"patient gender : %s,", hashtable[j].p.gender);
            fprintf(outin,"patient date of admission : %s, ", hashtable[j].p.dateofadmission);
            fprintf(outin,"patient date of birth : %s,", hashtable[j].p.dateofbirth);
            fprintf(outin,"patient illness : %s,", hashtable[j].p.ill);
            fprintf(outin,"patient address : %s,", hashtable[j].p.adress);
            fprintf(outin,"patient blood type : %s", hashtable[j].p.blodtype);
        }
        else
            fprintf(outin,"empty\n");
    }
}
/*
 ibrahem dhade
 1190283
 Dr.Ahmad Abu snina
 */