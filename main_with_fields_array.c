// CRUD Framework

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LENGTH 30
#define DATAFILE "Records.dat"
#define MENU "Menu.cfg"
#define FIELDS "Fields.cfg"
#define MAX_LENGTH 100
#define READID() printf("Enter ID: "), fgets(s_buffer, LENGTH, stdin)
#define READVALUE() (printf("Enter %s: ", cptr_fields[counter]), fgets(s_buffer, LENGTH, stdin))
#define PRINT_SEPERATOR() printf("--------------------------------------------------------\n");
#define READ_NEW_VALUE(s_new_value) printf("Enter new value of %s: ", cptr_fields[counter - 1]), gets(s_new_value)
#define PRINTID() (ID[strlen(ID) - 1] = '\0', printf("%s: ", ID))
#define MAX(a, b) ((a > b )? (a) : (b))

void menu();
void get_fields();
int create_record();
int delete_record(char ID[]);
int search_record(char ID[]);
int update_record(char ID[]);
int get_record_position(char ID[]);
void view_all_records();
void get_messages();

FILE *fp_records, *fp_fields, *fp_menu;
int fields_count = 0, counter;
char s_buffer[MAX_LENGTH], **cptr_fields, s_messages[7][MAX_LENGTH];

void main()
{
	fp_fields = fopen(FIELDS, "r");
	fp_menu = fopen(MENU, "r");
	get_fields();
	get_messages();
	while (1)
	{
		menu();
	}
}

void get_messages()
{
	FILE *fp_messages = fopen("messages.cfg", "r");
	for (counter = 0;counter < 7;counter++)
	{
		fgets(s_messages[counter], MAX_LENGTH, fp_messages);
	}
}

void get_fields()
{
	while (fgets(s_buffer, LENGTH, fp_fields))
	{
		fields_count++;
	}
	rewind(fp_fields);

	cptr_fields = malloc(sizeof(char*) * fields_count);
	for (counter = 0;counter < fields_count; counter ++)
	{
		cptr_fields[counter] = malloc(LENGTH);
		fgets(cptr_fields[counter], LENGTH, fp_fields);
		cptr_fields[counter][strlen(cptr_fields[counter]) - 1] = '\0';
	}
	rewind(fp_fields);
}

void menu()
{
	int d_choice;

	while (fgets(s_buffer, MAX_LENGTH, fp_menu))
	{
		printf("%s", s_buffer);
	}
	rewind(fp_menu);

	printf("Enter your choice: ");
	scanf("%d", &d_choice);
	getchar();

	switch(d_choice)
	{
	case 1: if (create_record())
			{
				printf("%s", s_messages[0]);
			}
			else
			{
				printf("%s", s_messages[1]);
			}
			break;
	case 2: view_all_records();
			break;
	case 3: READID();
			if (update_record(s_buffer))
			{
				printf("%s", s_messages[2]);
			}
			else
			{
				printf("%s", s_messages[3]);
			}
			break;
	case 4: READID();
			if (delete_record(s_buffer))
			{
				printf("%s", s_messages[4]);
			}
			else
			{
				printf("%s", s_messages[5]);
			}
			break;
	case 5: READID();
			search_record(s_buffer);
			break;
	case 6: exit(0);
			break;
	default: printf("Enter a valid option.\n");
			 break;
	}
}

int create_record()
{
	// getchar();
	fp_records = fopen(DATAFILE, "a");
	fputs("a\n", fp_records);
	for (counter = 0;counter < fields_count;counter++)
	{
		READVALUE();
		fputs(s_buffer, fp_records);
	}
	fclose(fp_records);
	return 1;
}

void print_record()
{
	PRINT_SEPERATOR();
	for (counter = 0;counter < fields_count;counter++)
	{
		fgets(s_buffer, LENGTH, fp_records);
		printf("%s: %s", cptr_fields[counter], s_buffer);
	}
	PRINT_SEPERATOR();
}

void view_all_records()
{
	fp_records = fopen(DATAFILE, "r");
	char status[3];
	fgets(status, 3, fp_records);
	while (! feof(fp_records))
	{
		if (status[0] == 'a')
		{
			print_record();
			fgets(status, 3, fp_records);
		}
		else
		{
			for (counter = 0;counter < fields_count;counter++)
			{
				fgets(s_buffer, LENGTH, fp_records);
			}
			fgets(status, 3, fp_records);
		}
	}
	fclose(fp_records);
}

int get_record_position(char ID[])
{
	char buffer[LENGTH], status[3];
	fp_records = fopen(DATAFILE, "r");
	counter = 0;
	while (fgets(buffer, LENGTH, fp_records))
	{
		if (! strcmp(buffer, ID))
		{
			if (counter == 1)
			{
				fseek(fp_records, 0 - (strlen(buffer) + 3), SEEK_CUR);
			}
			else
			{
				fseek(fp_records, 0 - (strlen(buffer) + 4), SEEK_CUR);
			}
			fgets(status, LENGTH, fp_records);
			if (status[0] == 'a')
			{
				int d_record_position = ftell(fp_records);
				fclose(fp_records);
				return d_record_position;
			}
			else
			{
				fseek(fp_records, strlen(buffer), SEEK_CUR);
			}
		}
		counter++;
	}
	fclose(fp_records);
	return -1;
}

int search_record(char ID[])
{
	int d_record_position = get_record_position(ID);
	if (d_record_position != -1)
	{
		fopen(DATAFILE, "r");
		fseek(fp_records, d_record_position + 1, SEEK_SET);
		print_record();
		fclose(fp_records);
	}
	else
	{
		PRINTID();
		printf("%s", s_messages[6]);
	}
}

int update_record(char ID[])
{
	int d_record_position = get_record_position(ID);
	char s_new_value[LENGTH];
	if (d_record_position != -1)
	{
		fp_records = fopen(DATAFILE, "r+");
		fseek(fp_records, d_record_position, SEEK_SET);
		for (counter = 0; counter < fields_count; counter++)
		{
			fgets(s_buffer, LENGTH, fp_records);
		}
		fseek(fp_records, - (strlen(s_buffer) + 1), SEEK_CUR);
		READ_NEW_VALUE(s_new_value);
		fwrite(s_new_value, MAX(strlen(s_buffer), strlen(s_new_value)), 1, fp_records);
		fclose(fp_records);
		PRINTID();
		return 1;
	}
	else
	{
		PRINTID();
		return 0;
	}
}

int delete_record(char ID[])
{
	int d_record_position = get_record_position(ID);
	if (d_record_position != -1)
	{
		fp_records = fopen(DATAFILE, "r+");
		fseek(fp_records, d_record_position - 3, SEEK_SET);
		fwrite("d\n", 2, 1, fp_records);
		fclose(fp_records);
		PRINTID();
		return 1;
	}
	else
	{
		PRINTID();
		return 0;
	}
}