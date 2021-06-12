#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct studentInfo{
    int studentNumber;
    char lastName[15];
    char firstName[15];
    char course[15];
    int yearLevel;
    int age;
    char sex[15];
    int finalGrade;
};
typedef struct studentInfo studentInfo;

//Necessary Functions
void enrolStudent();
void dropStudent();
void editStudent();
void displayStudents();
void filterLastName();
void filterStudentNum();
void filterYearLevel();
void filterCourse();
void filterSex();
void filterPass();
void filterFail();
void sortLastName();
void sortStudentNum();

//Additional Functions
void menu();
void printStudentInfo(struct studentInfo *s);
int verifyStudentNum(int x);

int main(){
    
    int choice;

    do{
        menu();
        scanf("%i", &choice);
        printf("\n\n");

        switch(choice){
            case 1: //Enrol a student
                enrolStudent();
                break;
            case 2: // Drop a student
                dropStudent();
                break;
            case 3: //Edit a student
                editStudent();
                break;
            case 4: // Display all students
                displayStudents();
                break;
            case 5: //Filter students by last name
                filterLastName();
                break;
            case 6: //Filter students by student number
                filterStudentNum();
                break;
            case 7: //Filter students by year level
                filterYearLevel();
                break;
            case 8: //Filter students by course
                filterCourse();
                break;
            case 9: //Filter students by sex
                filterSex();
                break;
            case 10: //Filter students by passing
                filterPass();
                break;
            case 11: //Filter students by failing
                filterFail();
                break;
            case 12: //Sort students by last name
                sortLastName();
                break;
            case 13: //Sort students by student number
                sortStudentNum();
                break;
        }
        
    }while(choice != 14);

    return 0;
}

void enrolStudent(){

    FILE *fp;
    fp = fopen("studentlist.bin", "a+");

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        studentInfo s;

        int tempStudNum = 0, tempYear = 0, tempAge = 0, tempFinalGrade = 0;
        char tempLastName[15], tempFirstName[15], tempCourse[15], tempSex[15];

        //gets input from user for all fields
        do{
            printf("Student number: \n");
            scanf("%i", &tempStudNum);
        }while(verifyStudentNum(tempStudNum) == 1);

        printf("Last Name: \n");
        scanf("%s", &tempLastName);

        for(int i = 0; tempLastName[i] != '\0';i++){
            if(i == 0){
                if(tempLastName[i] >= 'a' && tempLastName[i] <= 'z'){
                    tempLastName[i] -= 32;
                }
            }else{
                if(tempLastName[i] >= 'A' && tempLastName[i] <= 'Z'){
                    tempLastName[i] += 32;
                }
            }
        }

        printf("First name: \n");
        scanf("%s", &tempFirstName);

        for(int i = 0; tempFirstName[i] != '\0';i++){
            if(i == 0){
                if(tempFirstName[i] >= 'a' && tempFirstName[i] <= 'z'){
                    tempFirstName[i] -= 32;
                }
            }else{
                if(tempFirstName[i] >= 'A' && tempFirstName[i] <= 'Z'){
                    tempFirstName[i] += 32;
                }
            }
        }

        printf("Course: \n");
        scanf("%s", &tempCourse);

        for(int i = 0; tempCourse[i] != '\0'; i++){
            if(tempCourse[i] >= 'a' && tempCourse[i] <= 'z'){
                tempCourse[i] -= 32;
            }
        }

        do{
            printf("Year level: \n");
            scanf("%d", &tempYear);
        }while(tempYear < 0);
        
        do{
            printf("Age: \n");
            scanf(" %d", &tempAge);
        }while(tempAge < 0);
        
        do{
            printf("Sex: \n");
            scanf("%s", &tempSex);

            for(int i = 0; tempSex[i] != '\0';i++){
                if(i == 0){
                    if(tempSex[i] >= 'a' && tempSex[i] <= 'z'){
                        tempSex[i] -= 32;
                    }
                }else{
                    if(tempSex[i] >= 'A' && tempSex[i] <= 'Z'){
                        tempSex[i] += 32;
                    }
                }
            }

        }while(strcmp("Female", tempSex) != 0 && strcmp("Male", tempSex) != 0);

        do{
            printf("Final grade: \n");
            scanf(" %d", &tempFinalGrade);
        }while(tempFinalGrade < 0 || tempFinalGrade > 100);
        
        s.studentNumber = tempStudNum;
        strcpy(s.lastName, tempLastName);
        strcpy(s.firstName, tempFirstName);
        strcpy(s.course, tempCourse);
        s.yearLevel = tempYear;
        s.age = tempAge;
        strcpy(s.sex, tempSex);
        s.finalGrade = tempFinalGrade;

        //transfers input to file
        fseek(fp, sizeof(studentInfo)*(s.studentNumber - 1), SEEK_SET);
        fwrite(&s, sizeof(studentInfo), 1, fp);

        fclose(fp);
    }
    
}

void dropStudent(){

    FILE *fp, *fp1;
    fp = fopen("studentlist.bin", "r");
    fp1 = fopen("tempStudentlist.bin", "w");

    if(!fp || !fp1){
        printf("File could not be opened.\n");
    }else{
        
        studentInfo s;
        int tempStudentNum, ok = 0, n;

        printf("Enter student number of the student you want to drop: ");
        scanf("%d", &tempStudentNum);

        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(tempStudentNum == s.studentNumber){
                ok = 1;
            }else{
                fwrite(&s, sizeof(studentInfo), 1, fp1);
            }
            
        }

        fclose(fp);
        fclose(fp1);

        if(ok == 1){
            fp = fopen("studentlist.bin", "w");
            fp1 = fopen("tempStudentlist.bin", "r");

            while(fread(&s, sizeof(studentInfo), 1, fp1)){
                fwrite(&s, sizeof(studentInfo), 1, fp);
            }

            fclose(fp);
            fclose(fp1);
            
        }else{
            printf("\nThe record was not found.\n");
        }
    }
}

void editStudent(){

    FILE *fp, *fp1;
    fp = fopen("studentlist.bin", "r");
    fp1 = fopen("tempStudentlist.bin", "w");

    if(!fp || !fp1){
        printf("File could not be opened.\n");
    }else{
        
        studentInfo s;
        int tempStudentNum, found = 0, n;

        printf("Enter student number of the record you want to edit: ");
        scanf("%d", &tempStudentNum);

        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(tempStudentNum == s.studentNumber){
                found = 1;
                do{
                    printf("1. Student Number\n");
                    printf("2. Last name\n");
                    printf("3. First Name\n");
                    printf("4. Course\n");
                    printf("5. Year Level\n");
                    printf("6. Age\n");
                    printf("7. Sex\n");
                    printf("8. Final Grade\n");
                    printf("9. Exit\n");

                    printf("\nEnter the number of operation you want to do: ");
                    scanf("%i", &n);
                    
                    switch(n){
                        case 1:
                            printf("\nStudent Number (%d): \n", s.studentNumber);
                            scanf("%d", &s.studentNumber);//++check if Valid
                            break;
                        case 2: 
                            printf("Last Name (%s): \n", s.lastName);
                            scanf("%s", &s.lastName);
                            break;
                        case 3:
                            printf("First Name (%s): \n", s.firstName);
                            scanf("%s", &s.firstName);
                            break;
                        case 4:
                            printf("Course (%s): \n", s.course);
                            scanf("%s", &s.course);
                            break;
                        case 5:
                            printf("Year Level (%d): \n", s.yearLevel);
                            scanf("%d", &s.yearLevel);
                            break;
                        case 6:
                            printf("Age (%d): \n", s.age);
                            scanf("%d", &s.age);
                            break;
                        case 7: 
                            printf("Sex(%s): \n", s.sex);
                            scanf("%s", &s.sex);
                            break;
                        case 8:
                            printf("Final Grade (%d): \n", s.finalGrade);
                            scanf("%d", &s.finalGrade);
                            break;
                    }
                }while(n != 9);
            }
            fwrite(&s, sizeof(studentInfo), 1, fp1);
        }

        fclose(fp);
        fclose(fp1);

        if(found){
            fp = fopen("studentlist.bin", "w");
            fp1 = fopen("tempStudentlist.bin", "r");

            while(fread(&s, sizeof(studentInfo), 1, fp1)){
                fwrite(&s, sizeof(studentInfo), 1, fp);
            }

            fclose(fp);
            fclose(fp1);
            
        }else{
            printf("\nThe record was not found.\n");
        }
    }
}

void displayStudents(){
    
    FILE *fp;
    fp = fopen("studentlist.bin", "rb");
    if(!fp){
        printf("File could not be opened.\n");
    }else{

        studentInfo s;

        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            printStudentInfo(&s);
        }

        fclose(fp);
    }

}

void filterLastName(){
    FILE *fp;
    studentInfo s;
    fp = fopen("studentlist.bin", "r");

    char tempLastName[15];
    int ok = 0;

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        printf("Enter the last name you want to filter: ");
        scanf("%s", tempLastName);
        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(strcmp(tempLastName, s.lastName) == 0){
                printStudentInfo(&s);
                ok = 1;
            }
        }

        if(!ok){
            printf("\nThe record was not found.\n");
        }

        fclose(fp);

    }
}

void filterStudentNum(){
    FILE *fp;
    studentInfo s;
    fp = fopen("studentlist.bin", "r");

    char tempNum;
    int ok = 0;
    if(!fp){
        printf("File could not be opened.\n");
    }else{

        printf("Enter the student number you want to filter: ");
        scanf("%d", &tempNum);
        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(tempNum == s.studentNumber){
                printStudentInfo(&s);
                ok = 1;

            }
        }
        fclose(fp);
    }

    if(!ok){
        printf("\nThe record was not found.\n");
    }
}

void filterYearLevel(){
    FILE *fp;
    studentInfo s;

    fp = fopen("studentlist.bin", "r");

    char tempNum;
    int ok = 0;

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        printf("Enter the year level you want to filter: ");
        scanf("%d", &tempNum);
        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(tempNum == s.yearLevel){
                printStudentInfo(&s);
                ok = 1;
            }
        }
        fclose(fp);
        if(!ok){
            printf("\nThe record was not found.\n");
        }
    }


}

void filterCourse(){
    FILE *fp;
    studentInfo s;
    fp = fopen("studentlist.bin", "r");

    char tempCourse[15];
    int ok =  0;

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        printf("Enter the course you want to filter: ");
        scanf("%s", tempCourse);
        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(strcmp(tempCourse, s.course) == 0){
                printStudentInfo(&s);
                ok = 1;
            }
        }
        if(!ok){
            printf("\nThe record was not found.\n");
        }
        fclose(fp);
    }

    
}

void filterSex(){
    FILE *fp;
    studentInfo s;
    fp = fopen("studentlist.bin", "r");

    char tempSex[15];
    int ok = 0;

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        printf("Enter the sex you want to filter: ");
        scanf("%s", tempSex);
        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(strcmp(tempSex, s.sex) == 0){
                printStudentInfo(&s);
                ok = 1;
            }
        }
        if(!ok){
            printf("\nThe record was not found.\n");
        }
        fclose(fp);
    }

    
}

void filterPass(){
    FILE *fp;
    studentInfo s;
    fp = fopen("studentlist.bin", "r");

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(s.finalGrade >= 60){
                printStudentInfo(&s);
            }
        }
        fclose(fp);

    }   
}

void filterFail(){
    FILE *fp;
    studentInfo s;
    fp = fopen("studentlist.bin", "r");

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(s.finalGrade < 60){
                printStudentInfo(&s);
            }
        }
        fclose(fp);

    }   
}

void sortLastName(){

    FILE *fp;
    fp = fopen("studentlist.bin", "r");

    if(!fp){
        printf("File could not be opened.\n");
    }else{

        studentInfo *s;
        char temp[15];

        fseek(fp, 0, SEEK_END);
        int  numRecords = ftell(fp)/sizeof(studentInfo);
        rewind(fp);

        s = (studentInfo*)calloc(numRecords, sizeof(studentInfo));

        for(int i = 0; i<numRecords; i++){
            fread(&s[i], sizeof(studentInfo), 1, fp);
        }

        for(int i = 0; i < numRecords; i++){
            for(int j = 0; j <numRecords-i-1; j++){
                if(strcmp(s[j].lastName, s[j+1].lastName) > 0){
                    strcpy(temp, s[j].lastName);
                    strcpy(s[j].lastName, s[j+1].lastName);
                    strcpy(s[j+1].lastName, temp);
                }
            }
        }

        for(int i = 0; i < numRecords; i++){
            printf("\n\nStudent Number: %d\n", s[i].studentNumber);
            printf("Last Name: %s\n", s[i].lastName);
            printf("First Name: %s\n", s[i].firstName);
            printf("Course: %s\n", s[i].course);
            printf("Year Level: %d\n", s[i].yearLevel);
            printf("Age: %d\n", s[i].age);
            printf("Sex: %s\n", s[i].sex);
            printf("Final Grade: %i\n", s[i].finalGrade);
        }

        free(s);
        fclose(fp);

    }
}

void sortStudentNum(){

    
    FILE *fp;
    fp = fopen("studentlist.bin", "r");

    if(!fp){
        printf("File could not be opened.\n");
    }else{
        studentInfo *s, s1;

        fseek(fp, 0, SEEK_END);
        int  numRecords = ftell(fp)/sizeof(studentInfo);
        rewind(fp);

        s = (studentInfo*)calloc(numRecords,sizeof(studentInfo));

        for(int i = 0; i<numRecords; i++){
            fread(&s[i], sizeof(studentInfo), 1, fp);
        }

        for(int i = 0; i < numRecords; i++){
            for(int j = 1; j < numRecords; j++){
                if(s[i].studentNumber < s[j].studentNumber){
                    s1 = s[i];
                    s[i] = s[j];
                    s[j] = s1;
                }
            }
        }

        for(int i = 0; i < numRecords; i++){
            printf("\n\nStudent Number: %d\n", s[i].studentNumber);
            printf("Last Name: %s\n", s[i].lastName);
            printf("First Name: %s\n", s[i].firstName);
            printf("Course: %s\n", s[i].course);
            printf("Year Level: %d\n", s[i].yearLevel);
            printf("Age: %d\n", s[i].age);
            printf("Sex: %s\n", s[i].sex);
            printf("Final Grade: %i\n", s[i].finalGrade);
        }
        
        free(s);
        fclose(fp);

    }

}



void menu(){
    printf("\n1. Enrol a student");
    printf("\n2. Drop a student");
    printf("\n3. Edit a student");
    printf("\n4. Display all students");
    printf("\n5. Filter students by last name");
    printf("\n6. Filter students by student number");
    printf("\n7. Filter students by year level");
    printf("\n8. Filter students by course");
    printf("\n9. Filter students by sex");
    printf("\n10. Filter students by passing");
    printf("\n11. Filter students by failing");
    printf("\n12. Sort students by last name");
    printf("\n13. Filter students by student number");
    printf("\n14. Exit");
    printf("\n\n?\n");
}

void printStudentInfo(struct studentInfo *s){
    printf("\n\nStudent Number: %d\n", s->studentNumber);
    printf("Last Name: %s\n", s->lastName);
    printf("First Name: %s\n", s->firstName);
    printf("Course: %s\n", s->course);
    printf("Year Level: %d\n", s->yearLevel);
    printf("Age: %d\n", s->age);
    printf("Sex: %s\n", s->sex);
    printf("Final Grade: %i\n", s->finalGrade);
}

int verifyStudentNum(int x){

    FILE* fp = fopen("studentlist.bin", "r");
    int ok = 0;

    if(!fp){
        printf("File could not be opened.\n");
    }else{
        
        studentInfo s;

        while(fread(&s, sizeof(studentInfo), 1, fp) && !feof(fp)){
            if(x == s.studentNumber){
                ok = 1;
                return ok;
            }
        }
        fclose(fp);
    }
}

