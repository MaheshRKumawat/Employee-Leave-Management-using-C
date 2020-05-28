#define MAX_LIMIT 50

void get_time();
void reset();
int leave_decide(int w);
void leave_management(int c);
void emp_login();
void emp_register();

typedef struct Employee
{
    char firstname[MAX_LIMIT],lastname[MAX_LIMIT];
    int id;
    int x,y,z;
}Emp;

