#include<stdio.h>
#include<stdlib.h>
#include"employee_server.h"

/*Here in void main() the user will be asked to login or register if he/she is a new employee*/
void main()
{
    int choice1;
    printf("\n........WELCOME TO EMPLOYEE LEAVE MANAGEMENT SYSTEM........\n");
    get_time();
    printf("\nEnter 1 to login\nEnter 2 if you are a new employee\n");
    scanf("%d",&choice1);
    if(choice1==1) emp_login();
    else if(choice1==2) emp_register();
    else
    {
        printf("\nWrong input\n\n");
        exit(0);
    }
}
