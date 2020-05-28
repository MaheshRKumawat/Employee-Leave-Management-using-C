a.out: employee_client.o employee_server.o
		gcc employee_client.o employee_server.o
employee_server.o: employee_server.c employee_server.h
		gcc -c employee_server.c
employee_client.o: employee_client.c employee_server.c
		gcc -c employee_client.c