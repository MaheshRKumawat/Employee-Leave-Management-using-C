#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"employee_server.h"


Emp e_login,e_temp,e_new;
//e_login is for the user to login
//e_temp is used to temporarily store some vales of e_login 
//e_temp is also used while registering a new user
//e_new is used for the new employee to register




/*This function will show time and also check 
whether new year has come so that it can 
reset all employee's leaves*/
void get_time()
{
    int this_year;
    time_t t;
    time(&t);
    printf("\t\t%s",ctime(&t));
    time_t s;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);
    int current_year = current_time->tm_year + 1900;
    if(current_year>this_year)
    {
        reset();
        this_year=current_year;
    }
}



/*This function will reset all the employee's
casual,medical and earned leaves to 20 each
if new year comes*/
void reset()
{
    FILE *fp,*fp1;
    fp=fopen("Leave_Record.txt","rb");
    fp1=fopen("Temporary_file.txt","wb");
    fseek(fp, 0L, SEEK_SET);
    while(fread(&e_temp,sizeof(Emp),1,fp))
    {
        e_temp.x=20;
        e_temp.y=20;
        e_temp.z=20;
        fwrite(&e_temp,sizeof(Emp),1,fp1);
        fflush(stdin);
    }
    fclose(fp);  
    fclose(fp1);
    remove("Leave_Record.txt");
    rename("Temporary_file.txt","Leave_Record.txt");
    printf("All employees leaves have been reset\n");
}




/*This function does two task based on user's option
it will either show the user the number of leaves available in each
(or)
the user will apply for leave and it will show the result to the user
using the leave_decide function
It will then change the number of leaves available in the file accordingly */
void leave_management(int c)
{
    int i;
    FILE *fp;
    if((fp=fopen("Leave_Record.txt","rb"))==NULL) printf("\nThere is some ERROR. Try again\n");
    else
    {
        fseek(fp, 0L, SEEK_SET);
        for(i=1;i<=c;i++)
        {
            fread(&e_login,sizeof(Emp),1,fp);
        }
        /*temporary structure that is e_temp will be used so that it can store
        the necessary changes and can be added to the temporary file*/
        e_temp=e_login;

        int choice2,choice3;
        printf("Enter 1 to check the number of leaves available \nEnter 2 to apply for leave\n");
        scanf("%d",&choice2);
        if(choice2==1)
        {
            printf("\nCasual Leaves: %d\n",e_login.x);
            printf("Medical Leaves: %d\n",e_login.y);
            printf("Earned Leaves: %d\n\n",e_login.z);
        }
        else if(choice2==2)
        {
            printf("\nEnter 1 to apply for casual leave\nEnter 2 to apply for medical leave\nEnter 3 to apply for earned leaves\n");
            scanf("%d",&choice3);
            if(choice3==1) e_temp.x=leave_decide(e_login.x);
            else if(choice3==2) e_temp.y=leave_decide(e_login.y);
            else if(choice3==3) e_temp.z=leave_decide(e_login.z);
            else 
            {
                printf("\nWrong Input\n\n");
                exit(0);
            }
            i=1;
            FILE *fp1;
            if((fp1=fopen("Temporary_file.txt","wb"))==NULL) printf("\nThere is some ERROR. Try again\n");
            else
            {
                fseek(fp, 0L, SEEK_SET);
                while(fread(&e_login,sizeof(Emp),1,fp))
                {
                    if(i==c)
                    {
                        fwrite(&e_temp,sizeof(Emp),1,fp1);
                        //Replacing that particular line with temporary structure
                    }
                    else 
                    {
                        fwrite(&e_login,sizeof(Emp),1,fp1);  
                    }
                    i++;
                    fflush(stdin);
                }
                fclose(fp);  //Closing the files
                fclose(fp1);   
                remove("Leave_Record.txt");  //removing this file
                rename("Temporary_file.txt","Leave_Record.txt"); //renaming the file as Leave_Record1.txt
            }
        }
        else printf("\nWrong Input\n\n");
    }
}




/*This function will take w as parameter
where w is the number of remaining days where
the employee can take particular leave that is
either in casual or medical or earned*/
/*It will accept the number of leave days and then based
on w it will give output*/
int leave_decide(int w)
{
    int leave_days,choice4=0;
    printf("\nEnter the number of days you want a leave: ");
    scanf("%d",&leave_days);
    if(w<=0)
    {
        printf("\nSorry you cannot get any more leaves\n\n");
        return w;
    }
    else if( w>0 && (w-leave_days)<0 )
    {
        printf("Sorry you can have only %d days of leave\n",w);
        printf("Enter 1 to apply for %d days of leave or 2 to cancel the leave: ",w);
        scanf("%d",&choice4);
        if(choice4==1)
        {
            printf("Ok %d days of leave has been granted\nThank You\n\n",w);
            return 0;
        }
        else if(choice4==2)
        {
            printf("Ok Leave Application has been cancelled\nThank You\n\n");
            return w;
        }
        else
        {
            printf("\nWrong input\n\n");
            exit(0);
        }
    }
    else 
    {
        printf("Enter 1 to apply for %d days of leave or 2 to cancel the leave: ",leave_days);
        scanf("%d",&choice4);
        if(choice4==1)
        {
            printf("Ok %d days of leave has been granted\nThank You\n\n",leave_days);
            return (w-leave_days);
        }
        else if(choice4==2)
        {
            printf("Ok Leave Application has been cancelled\nThank You\n\n");
            return w;
        }
        else
        {
            printf("Wrong input\n\n");
            exit(0);
        }
    }
}




/*This function will allow the user to login
with his/her login id */
void emp_login()
{
    FILE *fpcheck;
    fpcheck=fopen("Leave_Record.txt","rb");
    int login=0,count=0,check_id;
    printf("Enter your Employee ID: ");
    scanf("%d",&check_id);
    while(fread(&e_login,sizeof(Emp),1,fpcheck) && login!=1)
    {
        ++count;
        if(e_login.id==check_id)
        {
            printf("\nHello %s %s you have successfully logged in\n",e_login.firstname,e_login.lastname);
            fclose(fpcheck);
            leave_management(count); 
            //count is given to locate at which particular line the user's information is there. 
            login=1;
            break;
        }
    }
    if(login==0) 
    {
        printf("\nSorry but there is no employee with this id\n");
    }
}




/*This function is used for the new user to register
if there is no record of his/her in the file*/
/*Employee id will be given to the user which will be
increemented by 1 from the previous user
Example: if the last user in the file has employee id 
as 10010 then the new user employee id will be given as 10011 */
void emp_register()
{
    printf("\nOk first you have to register yourself\n");
    printf("Enter your first name: ");
    scanf("%s",e_new.firstname);
    printf("Enter your last name: ");
    scanf("%s",e_new.lastname);
    FILE *fp_register;
    if((fp_register=fopen("Leave_Record.txt","ab+"))==NULL) printf("\nThere is some ERROR. Try again\n");
    else
    {
    fseek(fp_register, 0L, SEEK_SET);
    while(fread(&e_temp,sizeof(Emp),1,fp_register));
    e_new.id=e_temp.id+1;
    e_new.x=20;
    e_new.y=20;
    e_new.z=20;
    /* Each employee will be given 20 casual leaves,
    20 medical leaves and 20 earned leaves */
    fwrite(&e_new,sizeof(Emp),1,fp_register);
    printf("Congratulations you have registered yourself\n");
    printf("\nYour id is %d\n",e_new.id);
    printf("You have:\n20 casual leaves\n20 medical leaves\n20 earned leaves\n");
    printf("Thank You\n");
    }
    fclose(fp_register);
}