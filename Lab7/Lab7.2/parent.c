#include "parent.h"

struct record rec = {0, 0, 0, 0};

FILE *file;

void rec_init();
void rec_fputs();
void cat();
void show_record(int);
void modify_record(int);

int main()
{
    rec.number = 0;
    int choice = 1;
    while (1)
    {
        printf("  [1] :             push the record into the file;\n");
        printf("  [2] :             show the records from the file;\n");
        printf("  [3x]:             show the record Nx;\n");
        printf("  [4x]:             modify the record Nx;\n");
        printf("  [5] :             clean the file;\n");
        printf("If you want to quit the program, choose another variant.\n");
        printf("Your choice:        ");
        rewind(stdin);
        scanf("%d", &choice);
        if (choice == 1)
        {
            if ((file = fopen("file.txt", "r")) == NULL)
            {
                printf("File wasn't open\n");
                return 0;
            }
            rewind(file);
            char symbol = 0;
            while ((symbol = getc(file)) != EOF)
            {
                if (symbol == 'N' && (symbol = getc(file)) > 48 && symbol < 58)
                {
                    rec.number = symbol - 48;
                }
            }
            fclose(file);
            rec.number++;
            printf("\nPush the record N%d into the file\n\n", rec.number);
            rec_init();
            rec_fputs();
        }
        else if (choice == 2)
        {
            printf("\nShow the records from the file\n\n");
            cat();
        }
        else if (choice >= 31 && choice < 40)
        {
            int number = choice - 30;
            printf("\nShow the record N%d\n\n", number);
            show_record(number);
        }
        else if (choice >= 41 && choice < 50)
        {
            int number = choice - 40;
            printf("\nModify the record N%d\n\n", number);
            modify_record(number);
        }
        else if (choice == 5)
        {
            printf("\nClean the file\n\n");
            if ((file = fopen("file.txt", "w")) == NULL)
            {
                printf("File wasn't open\n");
                return 0;
            }
            fclose(file);
            rec.number = 1;
        }
        else if (choice == 0)
        {
            printf("\nQuit\n\n");
            break;
        }
        choice = 0;
    }
    exit(0);
}

void rec_init()
{
    rec.name[0] = '\n';
    printf("Enter a student name:          ");
    rewind(stdin);
    for (int i = 0; i < 40; i++)
    {
        scanf("%c", &rec.name[i]);
        if (rec.name[i] == '.')
        {
            rec.name[i] = ' ';
            break;
        }
    }
    rec.name[0] = ' ';
    printf("Enter a year:                  ");
    while (1)
    {
        rewind(stdin);
        scanf("%d", &rec.year);
        if (rec.year < 2000 || rec.year > 2005)
        {
            printf("The year is incorrect\n");
        }
        else
            break;
    }
    printf("Enter a number of semester:    ");
    while (1)
    {
        rewind(stdin);
        scanf("%d", &rec.semester);
        if (rec.semester < 1 || rec.semester > 8)
        {
            printf("The semester is incorrect\n");
        }
        else
            break;
    }
}

void rec_fputs()
{
    if ((file = fopen("file.txt", "a")) == NULL)
    {
        printf("File wasn't open\n");
        return;
    }
    fpos_t pos;
    fgetpos(file, &pos);
    struct flock fLock;
    fLock.l_type = F_WRLCK;
    fLock.l_whence = SEEK_CUR;
    fLock.l_start = 0;
    fLock.l_len = 0;
    if (fcntl(fileno(file), F_SETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(rec_fputs));
        return;
    }
    fprintf(file, "N%d name:%syear: %d semester: %d\n",
            rec.number,
            rec.name,
            rec.year,
            rec.semester);
    fsetpos(file, &pos);
    fLock.l_type = F_UNLCK;
    if (fcntl(fileno(file), F_SETLKW, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(rec_fputs));
        return;
    }
    rec.number++;
    fclose(file);
}

void cat()
{
    if ((file = fopen("file.txt", "r")) == NULL)
    {
        printf("File wasn't open\n");
        return;
    }
    struct flock fLock;
    fLock.l_type = F_RDLCK;
    fLock.l_whence = SEEK_SET;
    fLock.l_start = 0;
    fLock.l_len = 0;
    if (fcntl(fileno(file), F_SETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(cat));
        return;
    }
    rewind(file);
    char symbol = 0;
    while ((symbol = getc(file)) != EOF)
    {
        printf("%c", symbol);
    }
    printf("\n");
    fLock.l_type = F_UNLCK;
    if (fcntl(fileno(file), F_SETLKW, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(cat));
        return;
    }
    fclose(file);
}

void show_record(int number)
{
    if ((file = fopen("file.txt", "r")) == NULL)
    {
        printf("File wasn't open\n");
        return;
    }
    rewind(file);
    char symbol = 0;
    fpos_t pos;
    while ((symbol = getc(file)) != EOF)
    {
        if (symbol == 'N' && getc(file) == number + 48)
        {
            fseek(file, -2, 1);
            fgetpos(file, &pos);
            break;
        }
    }
    struct flock fLock;
    fLock.l_type = F_RDLCK;
    fLock.l_whence = SEEK_CUR;
    fLock.l_start = 0;
    fLock.l_len = 0;
    if (fcntl(fileno(file), F_SETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(show_record));
        return;
    }
    while ((symbol = getc(file)) != EOF)
    {
        printf("%c", symbol);
        if (symbol == '\n')
        {
            break;
        }
    }
    printf("\n");
    fsetpos(file, &pos);
    fLock.l_type = F_UNLCK;
    if (fcntl(fileno(file), F_SETLKW, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(show_record));
        return;
    }
    fclose(file);
}

void modify_record(int number)
{
    show_record(number);
    if ((file = fopen("file.txt", "r+")) == NULL)
    {
        printf("File wasn't open\n");
        return;
    }
    struct flock fLock;
    fLock.l_type = F_WRLCK;
    fLock.l_whence = SEEK_SET;
    fLock.l_start = 0;
    fLock.l_len = 0;
    if (fcntl(fileno(file), F_SETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(modify_record));
        return;
    }
    char g = 0;
    printf("Enter a modify version of record N%d\n", number);
    rec_init();
    rec.number = number;
    int size = 0;
    while (getc(file) != EOF)
        size++;
    char *str = NULL;
    str = (char *)malloc(size * sizeof(char));
    char symbol = 0;
    int index = 0;
    fpos_t pos1, pos2;
    rewind(file);
    while ((symbol = getc(file)) != EOF)
    {
        if (symbol == 'N' && (g = getc(file)) == number + 48)
        {
            str[index] = '_';
            index++;
            fseek(file, -2, 1);
            fgetpos(file, &pos1);
            while (getc(file) != '\n')
                ;
        }
        else
        {
            fseek(file, -2, 1);
            while ((symbol = getc(file)) != '\n')
            {
                str[index++] = symbol;
            }
            str[index++] = symbol;
            continue;
        }
        while ((symbol = getc(file)) != EOF)
        {
            str[index++] = symbol;
        }
        fclose(file);
        if ((file = fopen("file.txt", "w")) == NULL)
        {
            printf("File wasn't open\n");
            return;
        }
        for (index = 0; str[index] != '_'; index++)
        {
            fprintf(file, "%c", str[index]);
        }
        fprintf(file, "N%d name:%syear: %d semester: %d\n",
                rec.number,
                rec.name,
                rec.year,
                rec.semester);
        for (index++; index < strlen(str); index++)
        {
            fprintf(file, "%c", str[index]);
        }
        fLock.l_type = F_UNLCK;
        if (fcntl(fileno(file), F_SETLKW, &fLock) == -1)
        {
            ERROR_HANDLER("fcntl", nameof(modify_record));
            return;
        }
    }
    fclose(file);
    free(str);
}

// ДОРАБОТАТЬ ATEXIT В 4, 5, 7 ЛАБАХ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!