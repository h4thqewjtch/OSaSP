#include "parent.h"

struct record
{
    int number;
    char name[40];
    int year;
    int semester;
} rec;

int size = sizeof(rec);

int fd = 0;

void rec_init();
void cat();
void show_record(int);
void modify_record(int);

int main()
{
    if ((fd = open("file.txt", O_RDWR)) == -1)
    {
        ERROR_HANDLER("open", nameof(main));
        return -1;
    }
    rec.number = 0;
    int choice = 1;
    while (1)
    {
        printf("  [1] :             show the records from the file;\n");
        printf("  [2x]:             show the record Nx;\n");
        printf("  [3x]:             modify the record Nx;\n");
        printf("If you want to quit the program, choose another variant.\n");
        printf("Your choice:        ");
        rewind(stdin);
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("\nShow the records from the file\n\n");
            cat();
        }
        else if (choice >= 20 && choice < 30)
        {
            int number = choice - 20;
            printf("\nShow the record N%d\n\n", number);
            show_record(number);
        }
        else if (choice >= 30 && choice < 40)
        {
            int number = choice - 30;
            printf("\nModify the record N%d\n\n", number);
            modify_record(number);
        }
        else if (choice == 0)
        {
            printf("\nQuit\n\n");
            break;
        }
        choice = 0;
    }
    close(fd);
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

void cat()
{
    struct flock fLock;
    fLock.l_type = F_RDLCK;
    fLock.l_whence = SEEK_SET;
    fLock.l_start = 0;
    fLock.l_len = 0;
    if (fcntl(fd, F_SETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(cat));
        return;
    }
    // lseek(fd, 0, SEEK_SET);
    for (int i = 0; i < 10; i++)
    {
        lseek(fd, i * size, SEEK_SET);
        if (read(fd, &rec, sizeof(rec)) == -1)
        {
            ERROR_HANDLER("read", nameof(show_record));
            return;
        }
        printf("N%d name: %s year: %d semester: %d",
               rec.number,
               rec.name,
               rec.year,
               rec.semester);
        printf("\n");
    }
    printf("\n");
    fLock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(cat));
        return;
    }
}

void show_record(int number)
{
    // lseek(fd, 0, SEEK_SET);
    struct flock fLock;
    fLock.l_type = F_WRLCK;
    fLock.l_whence = SEEK_SET;
    fLock.l_start = number * size;
    fLock.l_len = size;
    if (fcntl(fd, F_GETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(show_record));
        return;
    }
    lseek(fd, number * size, SEEK_SET);
    while (1)
    {
        if (fLock.l_type == F_UNLCK)
        {
            if (read(fd, &rec, sizeof(rec)) == -1)
            {
                ERROR_HANDLER("read", nameof(show_record));
                return;
            }
            break;
        }
    }
    printf("N%d name: %s year: %d semester: %d",
           rec.number,
           rec.name,
           rec.year,
           rec.semester);
    printf("\n\n");
    fLock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(show_record));
        return;
    }
}

void modify_record(int number)
{
    show_record(number);
    // lseek(fd, 0, SEEK_SET);
    struct flock fLock;
    fLock.l_type = F_WRLCK;
    fLock.l_whence = SEEK_CUR;
    fLock.l_start = number * size;
    fLock.l_len = size;
    if (fcntl(fd, F_SETLK, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(modify_record));
        return;
    }
    printf("Enter a modify version of record N%d\n", number);
    rec_init();
    rec.number = number;
    lseek(fd, number * size, SEEK_SET);
    if (write(fd, &rec, sizeof(rec)) == -1)
    {
        ERROR_HANDLER("write", nameof(main));
        return;
    }
    fLock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &fLock) == -1)
    {
        ERROR_HANDLER("fcntl", nameof(modify_record));
        return;
    }
}

// ДОРАБОТАТЬ ATEXIT В 4, 5, 7 ЛАБАХ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!