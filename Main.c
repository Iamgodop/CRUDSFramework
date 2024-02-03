// CRUD Framework

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU "Menu.cfg"
#define FIELDS "Fields.cfg"
#define DATAFILE "Records.dat"
#define FIELDSIZE 30
#define READNEWVALUE(s_buffer) (printf("Enter new Value: ", s_buffer), fgets(s_buffer, FIELDSIZE, stdin))

int create_record();
void menu();
void view_records();
char * get_data(char field[]);
int find_record(char ID[]);
void search_record(char ID[]);
int delete_record(char ID[]);
int update_record(char ID[]);
void find_field_count();
void exit_message()
{
	printf("Thanks for using our application.\n");
}

FILE *fp_records, *fp_fields, *fp_menu;
char s_buffer[FIELDSIZE];
int field_count = 0;

void main()
{
	atexit(exit_message);
	fp_fields = fopen(FIELDS, "r");
	fp_menu = fopen(MENU, "r");
	find_field_count();
	while(1)
	{
		menu();
	}
}

void find_field_count()
{
	while(fgets(s_buffer, FIELDSIZE, fp_fields))
	{
		field_count++;
	}
	rewind(fp_fields);
}
void menu()
{
	char s_buffer[100];
	int d_choice;

	while (fgets(s_buffer, 100, fp_menu))
	{
		printf("%s", s_buffer);
	}
	rewind(fp_menu);
	printf("Enter your choice: ");
	scanf("%d", &d_choice);
	switch (d_choice)
	{
	case 1: if(create_record())
			{
				printf("Succesfully added a new record.\n");
			}
			else
			{
				printf("Failed to insert.\n");
			}
			break; 
	case 2: view_records();
			break;
	case 3: printf("Enter ID to update: ");
			getchar();
			gets(s_buffer);
			if (update_record(s_buffer))
			{
				printf("%s: Succesfully Updated.\n", s_buffer);
			}
			else
			{
				printf("%s: Failed to update.\n", s_buffer);
			}
			break;
	case 4: printf("Enter ID of record to delete: ");
			scanf("%s", s_buffer);
			if (delete_record(s_buffer))
			{
				printf("Succesfully deleted.\n");
			}
			else
			{
				printf("Failed to delete.\n");
			}
			break;
	case 5: printf("Enter ID to search: ");
			getchar();
			gets(s_buffer);
			search_record(s_buffer);
			break;
	case 6: exit(0);
	default: printf("Invalid Choice.\n");
			 break;
	}
}

char * get_data(char field[])
{
	printf("Enter %s: ", field);
	fgets(field, FIELDSIZE, stdin);
	return field;
}

int create_record()
{
	char s_buffer[FIELDSIZE];
	fp_records = fopen(DATAFILE, "a");
	getchar();
	fputs("a\n", fp_records);
	while (fgets(s_buffer, FIELDSIZE, fp_fields))
	{
		s_buffer[strlen(s_buffer) - 1] = '\0'; 
		fputs(get_data(s_buffer), fp_records);
	}
	fclose(fp_records);
	rewind(fp_fields);
	return 1;
}

void view_records()
{
	char s_data_buffer[FIELDSIZE], s_field_buffer[FIELDSIZE], status[2];
	fp_records = fopen(DATAFILE, "r");
	fgets(status, 3, fp_records);
	// getchar();
	printf("----------------------------------------------------------\n");
	while (1)
	{
			if (fgets(s_field_buffer, FIELDSIZE, fp_fields))
			{
				s_field_buffer[strlen(s_field_buffer) - 1] = '\0';
			}
			else
			{
				rewind(fp_fields);
				printf("----------------------------------------------------------\n");
				fgets(status, FIELDSIZE, fp_records);
				continue;
			}
			if (! fgets(s_data_buffer, FIELDSIZE, fp_records))
			{
				break;
			}
			if (status[0] == 'a')
			{
				printf("%s: %s", s_field_buffer, s_data_buffer);
			}
	}
	rewind(fp_fields);
	fclose(fp_records);
}

int find_record(char ID[])
{
	char status[3];
	fp_records = fopen(DATAFILE, "r");
	while (fgets(s_buffer, FIELDSIZE, fp_records))
	{
		s_buffer[strlen(s_buffer) - 1] = '\0';
		if (! strcmp(s_buffer, ID))
		{
			fseek(fp_records, - (5 + strlen(s_buffer)), SEEK_CUR);
			fgets(status, 3, fp_records);
			if (status[0] == 'a')
			{
				int d_record_position = ftell(fp_records);
				fclose(fp_records);
				return (d_record_position);		
			}
			else
			{
				fseek(fp_records, 2 + strlen(s_buffer), SEEK_CUR);
			}
		}
	}
	fclose(fp_records);
	return -1;
}

void search_record(char ID[])
{
	char s_buffer[FIELDSIZE];
	int d_record_position = find_record(ID);
	if (d_record_position != -1)
	{
		printf("----------------------------------------------------------\n");
		fp_records = fopen(DATAFILE, "r");
		fseek(fp_records, d_record_position, SEEK_SET);
		while (fgets(s_buffer, FIELDSIZE, fp_fields))
		{
			s_buffer[strlen(s_buffer) - 1] = '\0';
			printf("%s: ", s_buffer);
			fgets(s_buffer, FIELDSIZE, fp_records);
			printf("%s", s_buffer);
		}
		printf("----------------------------------------------------------\n");
	}
	else
	{
		printf("%s: No Such Record Found!\n", ID);
	}
	rewind(fp_fields);
	fclose(fp_records);
}

int update_record(char ID[])
{
	int d_record_position = find_record(ID), d_counter;
	if (d_record_position != -1)
	{
		fp_records = fopen(DATAFILE, "r+");
		fseek(fp_records, d_record_position, SEEK_SET);
		for (d_counter = 0; d_counter < field_count; d_counter++)
		{
			fgets(s_buffer, FIELDSIZE, fp_records);
		}
		fseek(fp_records, - (strlen(s_buffer) + 1), SEEK_CUR);
		READNEWVALUE(s_buffer);
		fwrite(s_buffer, strlen(s_buffer), 1, fp_records);
		fclose(fp_records);
		return 1;
	}
	else
	{
		return 0;
	}
}

int delete_record(char ID[])
{
	int d_record_position = find_record(ID);
	if (d_record_position != -1)
	{
		fp_records = fopen(DATAFILE, "r+");
		fseek(fp_records, d_record_position - 3, SEEK_SET);
		fwrite("d\n", 2, 1, fp_records);
		fclose(fp_records);
		return 1;
	}
	else
	{
		return 0;
	}
}