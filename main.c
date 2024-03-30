#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

//Structures and pointers to them declaration--------------------------------------------------------------------

struct Course{ //This struct contains the information of the courses
    char courseName[40];
    int  creditHours;
    char courseCode[9];
    char cousrseDep[20];
    char topics[150];
};
typedef struct Course *courseInfo;

struct HashNode;
typedef struct HashNode* Hash;

struct HashNode{
    struct Course course;
    char status;
};

//Public Variables------------------------------------------------------------------------------------------------

Hash Table, Table2; //first table for linear, second for double
int numOfElements=0, numOfElements2=0; //first for linear table elements, second for double table elements
int TableSize, TableSize2; //first tablesize for linear, second for double
int collision=0, collision2=0; //first for linear, second for double
FILE *fOutPtr; //to print out the double info in it

//Functions declaration--------------------------------------------------------------------------------------------

void menu();
courseInfo createNode(char courseName[40], int creditHours, char courseCode[9], char courseDep[20], char topics[150]);
char *trimwhitespace(char *str);
int countlines(FILE *filename);
void loadInfo();
int getFirstPrime (int num);
int charSum (char* courseName);
int hashIndex(char* courseName);
Hash createTable (int size);
void printHashTable();
int hashFunction(char* courseName);
void rehash(struct Course c);
void InsertNodeToHash(struct Course c, Hash table, int sizeOfInsert, char status);
int hashFunction1(char* courseName);
int hashFunction2(char* courseName);
int hashIndex2(char* courseName);
void rehash2(struct Course c);
void InsertNodeToHash2(struct Course c, Hash table, int sizeOfInsert, char status);
void printHashTable2();
int findInLinear(char* courseName);
int findInDouble(char* courseName);
void find();
void deleteLinear(char* courseName);
void deleteDouble(char* courseName);
void deleteFromBoth();
void insertFromUser();
void printNode(courseInfo c);
void printToFile();

//------------------------------------------------------------------------------------------------------------------

void menu(){ //this function is used to print the menu.
    printf("-------------------------------------------------Menu-----------------------------------------------------------\n");
    printf("\t..........................................................................................\n");
    printf("\t:\t1. Print hashed tables (including empty spots).\t\t\t\t\t :\n");
    printf("\t:\t2. Print out table size and the load factor.\t\t\t\t\t :\n");
    printf("\t:\t3. Print out the used hash functions. \t\t\t\t\t\t :\n");
    printf("\t:\t4. Insert a new record to hash table.\t\t\t\t\t         :\n");
    printf("\t:\t5. Search for a specific word. \t\t\t\t\t\t         :\n");
    printf("\t:\t6. Delete a specific record \t\t\t\t\t\t         :\n");
    printf("\t:\t7. Compare between the two methods in terms of number of collisions occurred.    :\n");
    printf("\t:\t8. Save hash table back to a file named (saved_courses.txt).\t\t         :\n");
    printf("\t:\t9. Exit.\t\t\t\t\t\t\t\t         :\n");
    printf("\t..........................................................................................\n\n");
    printf("What is your Choice?\n");
}

char *trimwhitespace(char *str){ //this function used to trim all spaces from the words to that strcmp work properly
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int countlines(FILE *filename){
    // count the number of lines in the file called filename
    int ch=0;
    int lines=1;

    if (filename == NULL){
        return 0;
    }

    while ((ch = fgetc(filename)) != EOF){ //read till the end of file
        if (ch == '\n') //if newline lines++
            lines++;
    }
    fclose(filename);
    return lines;
}

void loadInfo(){ //this function used to load info from file and put it into the hash table
    courseInfo node;
    FILE *infofile;
    infofile = fopen("courses.txt", "r"); //the file to read from
    char info[250]; //to store the line in it

    if(infofile == NULL){
        printf("file courses.txt cannot be found");
        exit(1);
    }

    char *token; //to divide the string and store it in it
    char Name[40];
    char *ptrName;
    int creditHours;
    char credit[9];
    char courseCode[9];
    char *ptrCode;
    char courseDep[20];
    char *ptrDep;
    char topics[150];
    char *ptrTopics;

    while(fgets(info, 250, infofile)){ //while not end of file and still reading

        token = strtok(info, ":"); //read the first part of the line

        ptrName = trimwhitespace(token); //to trim it and store it
        strcpy(Name, ptrName);

        token = strtok(NULL, "#"); //read the second part
        strcpy(credit, token);
        creditHours = atoi(credit);

        token = strtok(NULL, "#");
        ptrCode = trimwhitespace(token);
        strcpy(courseCode, ptrCode);

        token = strtok(NULL, "/");
        ptrDep = trimwhitespace(token);
        strcpy(courseDep, ptrDep);

        token = strtok(NULL, "/");
        ptrTopics = trimwhitespace(token);
        strcpy(topics, ptrTopics);

        node = createNode(Name, creditHours, courseCode, courseDep, topics); //creating new node of bus and storing it into the array
        InsertNodeToHash(*node, Table, TableSize, '-'); //adding it to the first table
        InsertNodeToHash2(*node, Table2, TableSize2, '-'); //adding it to the second table
    }
}

int getFirstPrime (int num) { //this function is used to get the first prime number after the specific size
    bool isPrime;
    int initialValue = num;
    while(1)
    {
        isPrime=true;
        for(int i=2; i<num; i++)
        {
            if(num%i==0)
            {
                isPrime=false;
                break;
            }
        }
        if(isPrime)
            return num;
        num++; //if not prime add 1 till the num is prime
    }
}

int charSum (char* courseName) { //used to count all the characters of word in ASCI
    int sumOfChars = 0;
    while(*courseName != '\0'){
        sumOfChars += *courseName++;
    }
    return sumOfChars;
}

courseInfo createNode(char courseName[40], int creditHours, char courseCode[9], char courseDep[20], char topics[150]){
    //this function is used to create a new course node
    courseInfo node = (courseInfo) malloc(sizeof (struct Course)); //initializing space and the node
    strcpy(node->courseName, courseName);
    node->creditHours = creditHours;
    strcpy(node->courseCode, courseCode);
    strcpy(node->topics, topics);
    strcpy(node->cousrseDep, courseDep);
    return node;
}

Hash createTable (int size){ //this function is used to create the hash table
    Hash table = (Hash)malloc(sizeof(struct HashNode)*size); //this will initialize the array table of struct of hash
    if(table == NULL){
        printf("\tOut of Memory. Can't Create The Hash Table\n");
        exit(1);
    }

    numOfElements = 0;

    for(int i=0; i<size; i++){ //initialling all info in the array to it's zero
        strcpy(table[i].course.courseName, "");
        table[i].course.creditHours = 0;
        strcpy(table[i].course.courseCode, "");
        strcpy(table[i].course.topics, "");
        strcpy(table[i].course.cousrseDep, "");
        table[i].status='E'; //first always empty
    }
    return table;
}

//Linear hashing-------------------------------------------------------------------

int hashFunction(char* courseName){ //returns char ASCI summation mod table size
    return (charSum(courseName) % TableSize);
}

int hashIndex(char* courseName){ //this function is used to get the index needed
    int i=0;
    while (i < TableSize){ //this used to check all table, but not always all i used
        int index = (hashFunction(courseName) + i) % TableSize; //index function
        if(Table[index].status == 'E' || Table[index].status == 'D'){ //if empty or deleted add it in place of it
           if(index > TableSize2){
                return index - TableSize2; //if index bigger than the table size we do that to become circular
            }else{
                return index;
            }
        }
        collision++; //if the index is not empty or deleted add one to collision
        i++; //add 1 to i, to check the next index
    }
}

void rehash(struct Course c){ //this function is used to rehash the table if the load factor is above 0.7
    int tempTableSize = getFirstPrime(ceil(TableSize*2));//get the new table size
    Hash tempTable = createTable(tempTableSize); //make the new table

    for(int i=0; i<TableSize; i++){
        if(Table[i].status == 'D' || Table[i].status=='O'){ //get all info from the old table to the new table depending on the new table indexes
            InsertNodeToHash(Table[i].course, tempTable, tempTableSize, Table[i].status);
        }
    }

    Table = tempTable; //updating the table

    TableSize = tempTableSize;//update the tableSize

    InsertNodeToHash(c, Table, TableSize, '-');//insert the last node that led to the hashing
}

void InsertNodeToHash(struct Course c, Hash table, int sizeOfInsert, char status){
    double loadFactor = (numOfElements + 1) / (double)sizeOfInsert; //checking if adding the node will affect the load factor before adding
    int i=0;
    if(loadFactor <= 0.7){ //in not affecting
        int index = hashIndex(c.courseName); //get the index
        table[index].course = c; //add the info to index
        numOfElements++; //add 1 to elements
        if(status == 'D'){ //used when moving from table to table to keep the right value
            table[index].status = 'D';
        }else{ //if anything else then occupied
            table[index].status = 'O';
        }
    }
    else if (loadFactor>0.7){ //if it affects the load factor then rehash it
        rehash(c);
    }
}

void printHashTable(){
    //all design purposes.
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|  INDEX  |  \tSTATUS\t     |  \t\t\t\tINFORMATION\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
    for(int i=0; i < TableSize; i++){
        printf("|    %d    |", i);
        if(Table[i].status == 'O'){
            printf("  \tOCCUPIED     |");
            printf("\tCourse Name\t  ||  %s\n|\t  |\t\t     |  Credit Hours\t  ||  %d\n|\t  |\t\t     |  Course Code\t  ||  %s\n|\t  |\t\t     |  Course Department ||  %s\n|\t  |\t\t     |  Course Topics\t  ||  %s\n", Table[i].course.courseName, Table[i].course.creditHours, Table[i].course.courseCode, Table[i].course.cousrseDep, Table[i].course.topics);
            printf("-------------------------------------------------------------------------------------------------------------------------------\n");
        }else if(Table[i].status == 'E'){
            printf("  \tEMPTY\t     |\n");
            printf("-------------------------------------------------------------------------------------------------------------------------------\n");
        }else{
            printf("  \tDELETED\t     |\n");
            printf("-------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }
}

//Double Hashing-------------------------------------------------------------------

int hashFunction1(char* courseName){ //returns char ASCI summation mod table size
    return (charSum(courseName) % TableSize2);
}

int hashFunction2(char* courseName){  //returns 1 + char ASCI summation mod (table size -3)
    int size = TableSize2 - 3;
    return (1 + (charSum(courseName) % size));
}

int hashIndex2(char* courseName){ //this function is used to get the index needed
    int i=0;
    while (i < TableSize2){ //this used to check all table, but not always all i used
        int index = (hashFunction1(courseName) + i*hashFunction2(courseName)) % TableSize2; //index function
        if(Table2[index].status == 'E' || Table2[index].status == 'D'){ //if empty or deleted add it in place of it
            if(index > TableSize2){
                return index - TableSize2; //if index bigger than the table size we do that to become circular
            }else{
                return index;
            }
        }
        collision2++; //if the index is not empty or deleted add one to collision
        i++; //add 1 to i, to check the next index
    }
}

void rehash2(struct Course c){ //this function is used to rehash the table if the load factor is above 0.7
    int tempTableSize = getFirstPrime(ceil(TableSize2*2));//get the new table size
    Hash tempTable = createTable(tempTableSize); //make the new table

    for(int i=0; i<TableSize2; i++){
        if(Table2[i].status == 'D' || Table2[i].status=='O'){ //get all info from the old table to the new table depending on the new table indexes
            InsertNodeToHash2(Table2[i].course, tempTable, tempTableSize, Table2[i].status);
        }
    }

    Table2 = tempTable; //updating the table

    TableSize2 = tempTableSize; //update the tableSize

    InsertNodeToHash2(c, Table2, TableSize2, '-'); //insert the last node that led to the hashing
}

void InsertNodeToHash2(struct Course c, Hash table, int sizeOfInsert, char status){
    double loadFactor = (numOfElements + 1) / (double)sizeOfInsert; //checking if adding the node will affect the load factor before adding
    int i=0;
    if(loadFactor <= 0.7){ //in not affecting
        int index = hashIndex2(c.courseName); //get the index
        table[index].course = c; //add the info to index
        numOfElements2++; //add 1 to elements
        if(status == 'D'){ //used when moving from table to table to keep the right value
            table[index].status = 'D';
        }else{//if anything else then occupied
            table[index].status = 'O';
        }
    }
    else if (loadFactor>0.7){ //if it affects the load factor then rehash it
        rehash2(c);
    }
}

void printHashTable2(){
    //all design purposes.
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
    printf("|  INDEX  |  \tSTATUS\t     |  \t\t\t\tINFORMATION\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
    for(int i=0; i < TableSize2; i++){
        printf("|    %d    |", i);
        if(Table2[i].status == 'O'){
            printf("  \tOCCUPIED     |");
            printf("\tCourse Name\t  ||  %s\n|\t  |\t\t     |  Credit Hours\t  ||  %d\n|\t  |\t\t     |  Course Code\t  ||  %s\n|\t  |\t\t     |  Course Department ||  %s\n|\t  |\t\t     |  Course Topics\t  ||  %s\n", Table2[i].course.courseName, Table2[i].course.creditHours, Table2[i].course.courseCode, Table2[i].course.cousrseDep, Table2[i].course.topics);
            printf("-------------------------------------------------------------------------------------------------------------------------------\n");
        }else if(Table2[i].status == 'E'){
            printf("  \tEMPTY\t     |\n");
            printf("-------------------------------------------------------------------------------------------------------------------------------\n");
        }else{
            printf("  \tDELETED\t     |\n");
            printf("-------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }
}

//Find and delete functions---------------------------------------------------------

int findInLinear(char* courseName){ //this function is used to find a course in the hash table
    int i=0;
    while (i < TableSize) {
        int index = (hashFunction(courseName) + i) % TableSize; //get the first index
        if(Table[index].status=='O' && strcmp(Table[index].course.courseName, courseName)==0){ //if occupied and found
            if(index > TableSize){ //get the index
                return index - TableSize;
            }else{
                return index;
            }
        }
        i++; //if it's empty or deleted or occupied with other value then it will add one to i to index
    }
    return -1; //if not found return -1
}

int findInDouble(char* courseName){ //this function is used to find a course in the hash table
    int i=0, searchI=0;
    while (i < TableSize2){
        int index = (hashFunction1(courseName) + searchI) % TableSize2;//get the first index
        if(Table2[index].status=='O' && strcmp(Table2[index].course.courseName, courseName)==0){//if occupied and found
            if(index > TableSize2){ //get the index
                return index - TableSize2;
            }else{
                return index;
            }
        }
        i++; //then it's like adding collision to the function
        searchI = i*hashFunction2(courseName); //the second function value in double hashing
    }
    return -1; //if not found return -1
}

void find(){ //this function is used to get the course name and search for it in the table of ur choice
    int x;
    int index;
    printf("Please enter 1 if u want to find course from Linear hashing table\n\t and 2 if u want to find course from double hashing table\nChoice -> ");
    scanf("%d", &x);
    while(x != 1 && x != 2){
        printf("Please enter 1 or 2 only!!\nyour choice -> ");
        scanf("%d", &x);
    }

    printf("\n");
    char courseName[40];
    printf("Please type the course name:\n");
    gets(courseName);
    gets(courseName);

    if(x==1){
        index = findInLinear(courseName);
        if(index == -1){
            printf("There is no course with that name in linear hashing table!\n");
        }else{
            printf("\n");
            printf("Found at index --> %d\n", index);
            printf("-----------------------------------------------------------------------------------------------------------\n");
            printNode(&Table[index].course);
            printf("-----------------------------------------------------------------------------------------------------------\n");
            printf("\n");
        }
    }else{
        index = findInDouble(courseName);
        if(index == -1){
            printf("There is no course with that name in double hashing table!\n");
        }else{
            printf("\n");
            printf("Found at index --> %d\n", index);
            printf("-----------------------------------------------------------------------------------------------------------\n");
            printNode(&Table2[index].course);
            printf("-----------------------------------------------------------------------------------------------------------\n");
            printf("\n");
        }
    }
}

void deleteLinear(char* courseName){
    int index = findInLinear(courseName);
    if(index != -1){ //if found element
        Table[index].status = 'D'; //make the status as deleted
        numOfElements--; //-1 from elements num
        printf("Deleted successfully from linear hashing table!\n");
    }else{
        printf("There is no course with that name in linear hashing table.\n");
    }
}

void deleteDouble(char* courseName){
    int index = findInDouble(courseName);
    if(index != -1){ //if found element
        Table2[index].status = 'D'; //make the status as deleted
        numOfElements2--; //-1 from elements num
        printf("Deleted successfully from double hashing table!\n");
    }else{
        printf("There is no course with that name in double hashing table.\n");
    }
}

void deleteFromBoth(){ //used to delete from both tables
    char courseName[40];
    printf("Please enter the course name to delete it -> \n");
    gets(courseName);
    gets(courseName);
    printf("\n");
    deleteLinear(courseName);
    deleteDouble(courseName);
    printf("\n");
}

//---------------------------------------------------------------------------------

void insertFromUser(){ //adding new course to the hashing tables but from user, not file.
    char courseName[40];
    int creditHours;
    char courseCode[9];
    char cousrseDep[20];
    char topics[150];

    printf("\nPlease enter the course name:\n");
    gets(courseName);
    gets(courseName);

    printf("Please enter the credit hours:\n");
    scanf("%d", &creditHours);

    printf("Please enter course code:\n");
    gets(courseCode);
    gets(courseCode);

    printf("Please enter course department:\n");
    gets(cousrseDep);

    printf("Please enter course topics as (topic1, topic2, ...., topic n):\n");
    gets(topics);

    courseInfo node = createNode(courseName, creditHours, courseCode, cousrseDep, topics);
    printf("\nNew node -->\n\n");
    printNode(node);
    printf("\n\n");
    InsertNodeToHash(*node, Table, TableSize, '-');
    InsertNodeToHash2(*node, Table2, TableSize2, '-');
}

void printNode(courseInfo c){ //printing design puposes
    printf("Course Name\t  ||  %s\nCredit Hours\t  ||  %d\nCourse Code\t  ||  %s\nCourse Department ||  %s\nCourse Topics\t  ||  %s\n", c->courseName, c->creditHours, c->courseCode, c->cousrseDep, c->topics);
}

void printToFile(){ //printing double table info to file.
    //foe design purposes
    fprintf(fOutPtr ,"Double Hashing Table ===>\n\n");
    fprintf(fOutPtr, "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fOutPtr ,"|  INDEX  |  \tSTATUS\t     |  \t\t\t\tINFORMATION\n");
    fprintf(fOutPtr, "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for(int i=0; i < TableSize2; i++){
        fprintf(fOutPtr ,"|    %d    |", i);
        if(Table2[i].status == 'O'){
            fprintf(fOutPtr ,"  \tOCCUPIED     |");
            fprintf(fOutPtr ,"\tCourse Name\t  ||  %s\n|\t  |\t\t     |  Credit Hours\t  ||  %d\n|\t  |\t\t     |  Course Code\t  ||  %s\n|\t  |\t\t     |  Course Department ||  %s\n|\t  |\t\t     |  Course Topics\t  ||  %s\n", Table2[i].course.courseName, Table2[i].course.creditHours, Table2[i].course.courseCode, Table2[i].course.cousrseDep, Table2[i].course.topics);
            fprintf(fOutPtr, "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }else if(Table2[i].status == 'E'){
            fprintf(fOutPtr ,"  \tEMPTY\t     |\n");
            fprintf(fOutPtr, "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }else{
            fprintf(fOutPtr ,"  \tDELETED\t     |\n");
            fprintf(fOutPtr, "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }
}

int main(){
    FILE *info_lines; //this will be used to get lines of the courses file
    info_lines = fopen("courses.txt", "r");
    if(info_lines == NULL){ //if file not found
        printf("file courses.txt cannot be found");
        exit(1);
    }
    int numOfLines = countlines(info_lines);
    //initialize the size for tables
    TableSize  = getFirstPrime(ceil(numOfLines*2));
    TableSize2 = getFirstPrime(ceil(numOfLines*2));
    //--------------------------------------------------
    //creating both tables
    Table = createTable(TableSize);
    Table2 = createTable(TableSize2);
    loadInfo(); //loading file info to hashing tables.
    //------------------------------------------------------------------------------------------------------
    printf("\t\t\t\t         Welcome to my program\n");
    printf("\t       This program reads info and store them using hashing methods. Check it out!\n\n");
    int selection = 0; //selection to know which thing from the menu it wants to do
    menu(); //printing menu on the screen for the user
    scanf("%d", &selection); //reading the selection

    while(selection != 9){ //while the user did not choose exit
        switch(selection){
        case 1: //if first choice
            printf("\nTable 1 (Linear Hashing)------>\n\n");
            printHashTable();
            printf("\n\n\n");
            printf("Table 2 (Double Hashing)------>\n\n");
            printHashTable2();
            printf("\n\n");
            break;
        case 2: //if second choice
            printf("\nTable 1 (Linear Hashing)------>   ");
            printf("Table Size = %d\n", TableSize);
            double loadFactor = numOfElements / (double)TableSize;
            printf("\t\t\t\t  Table Load Factor = %.2f\n", loadFactor);
            printf("\n");

            printf("\nTable 2 (Double Hashing)------>   ");
            printf("Table Size = %d\n", TableSize2);
            double loadFactor2 = numOfElements2 / (double)TableSize2;
            printf("\t\t\t\t  Table Load Factor = %.2f\n", loadFactor2);
            printf("\n");
            break;
        case 3: // if choice 3
            printf("\nTable 1 (Linear Hashing)------>   ");
            printf("Whole Function => ((hashFunction(courseName) + i) mod TableSize1)\n\n");
            printf("\t\t\t\t  hashFunction => (charSum(courseName) mod TableSize)\n\n");
            printf("\t\t\t\t  Summary => So this function basically gets the summation of ASCI characters and mod \n\t\t\t\t\t     with the table size if there is no collision.\n");
            printf("\t\t\t\t\t     If there is a collision it adds the number of collisions for that index to \n\t\t\t\t\t     the result so that it will get the first possible empty index in the hash array\n\n");

            printf("\nTable 2 (Double Hashing)------>   ");
            printf("Whole Function => ((hashFunction1(courseName) + i*hashFunction2(courseName)) mod TableSize2)\n\n");
            printf("\t\t\t\t  hashFunction1 => (charSum(courseName) mod TableSize2)\n\n");
            printf("\t\t\t\t  hashFunction2 => (1 + ( charSum(courseName) mod (TableSize2-3) ) )\n\n");
            printf("\t\t\t\t  Summary => So this function basically gets the summation of ASCI characters and mod \n\t\t\t\t\t     with the table size if there is no collision.\n");
            printf("\t\t\t\t\t     If there is a collision it multiplies the collision time with the second function.\n\t\t\t\t\t     The second function adds 1 to the summation of ASCI characters with mod table \n\t\t\t\t\t     size -3 which for me was more convenient than other possible functions\n\n");
            break;
        case 4: //if choice 4
            insertFromUser();
            break;
        case 5:
            find();
            break;
        case 6:
            deleteFromBoth();
            break;
        case 7:
            printf("\nTable 1 (Linear Hashing)------>   Number of collisions = %d\n", collision);
            printf("\nTable 2 (Double Hashing)------>   Number of collisions = %d\n", collision2);
            printf("\n-As I noticed they might be equivalent sometimes.\n-Also, I noticed that the bigger the table size is, the less collisions in double hashing than linear hashing\n\n");
            break;
        case 8:
            fOutPtr = fopen("saved_courses.txt", "w");
            printToFile();
            fclose(fOutPtr);
            printf("\nSaving The Contents To The File Is Complete\n\n");
            break;
        }
        menu(); //putting the menu again on screen
        scanf("%d", &selection); //reading the selection for the while loop
    }
    return 0;
}
