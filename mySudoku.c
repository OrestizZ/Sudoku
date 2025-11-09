#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SIZE 9

void start(void);
void printSudoku(int sudoku[][SIZE]);
void help(int *spot1, int *spot2, int nums[], int sudoku[][SIZE]);
void terminate(int sudoku[][SIZE]);

int checkArguments(int argc, char **argv);
int solve(int zeros, int nums[], int sudoku[][SIZE]);
int scramble(int sudoku[][SIZE]);
int getPlayerInput(char *playerInput, int sudoku[][SIZE], int *spot1, int *spot2, int *value);
int playerInputFormatValidation(char *playerInput);
int playerInputSpotValidation(char *playerInput, int sudoku[][SIZE], int *spot1, int *spot2);

bool sudokuValidation(int nums[], int sudoku[][SIZE]);

int main(int argc, char **argv)
{
    int checkValue = 0;

    checkValue = checkArguments(argc, argv);

    if (checkValue == 1)
        start();

    return 0;
}

int checkArguments(int argc, char **argv)
{
    // checks if arguments passed are right
    // returns 0 if the 'help' option was selected or if there was an error in argument format
    // returns 1 if the 'start game' option was selected
    // returns 2 if the 'load sudoku' option was selected
    if (argc > 3)
    {
        printf("Too many arguments! Type ./mySudoku -h for help.");
        return 0;
    }
    else if (argc < 2)
    {
        printf("Too few arguments! Type ./mySudoku -h for help.");
        return 0;
    }

    if (argv[1][0] == '-' && argv[1][2] == '\0')
    {
        if (argv[1][1] == 'h')
        {
            if (argc > 2)
            {
                printf("Unknown argument: %s\n", argv[2]);
                printf("Type ./mySudoku -h for help.\n");
                return 0;
            }

            printf("Type ./mySudoku -n to start playing.\n");
            printf("Type ./mySudoku -l 'name-of-file' to load an existing sudoku file.\n");
            printf("Type ./mySudoku -h for help.\n");
            return 0;
        }
        else if (argv[1][1] == 'n')
        {
            if (argc > 2)
            {
                printf("Unknown argument: %s\n", argv[2]);
                printf("Type ./mySudoku -h for help.\n");
                return 0;
            }
            return 1;
        }
        else if (argv[1][1] == 'l')
        {
            if (argc < 3)
            {
                printf("You have to include the name of the file you want to load.\n");
                printf("Type ./mySudoku -h for help.\n");
                return 0;
            }

            printf("load file");
            return 2;
        }
        else
        {
            printf("Unknown argument: %s\n", argv[1]);
            printf("Type ./mySudoku -h for help.\n");
            return 0;
        }
    }
    else
    {
        printf("Unknown argument: %s\n", argv[1]);
        printf("Type ./mySudoku -h for help.\n");
        return 0;
    }

    return 0;
}

void start(void)
{
    int sudoku[SIZE][SIZE], solved[SIZE][SIZE];
    int nums[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int a, b, a1, b1, poss = 0, index, zeroCount = 0, zeroCountConst, tries = 0;
    bool valid;

    FILE *fh_input;

    fh_input = fopen("solved.sud", "r");

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fscanf(fh_input, "%d", &sudoku[i][j]);
            solved[i][j] = sudoku[i][j];
        }
    }

    fclose(fh_input);

    srand(time(NULL));

    zeroCount = scramble(sudoku);

    zeroCountConst = zeroCount;

    printf("\nWelcome to Sudoku!\n");
    printf("Type: \n");
    printf("(2,3)=4 to put 4 in cell (2,3)\n");
    printf("(2,3)=0 to get all possibilities of the cell (2,3)\n");
    printf("(2,3)=-1 to see the solution of the cell (2,3)\n");
    printf("(0,0)=0 to see the solution of the entire sudoku\n");
    printf("(-1,-1)=-1 to terminate\n\n");

    printSudoku(sudoku);

    printf("\n");

    int playerOption, *spot1, *spot2, *value, tempSpot1, tempSpot2, tempValue;

    spot1 = malloc(sizeof(int));
    spot2 = malloc(sizeof(int));
    value = malloc(sizeof(int));

    char playerInput[11];

    do
    {
        playerOption = getPlayerInput(playerInput, sudoku, spot1, spot2, value);

        switch (playerOption)
        {
        case 1:
            if (solved[*spot1][*spot2] == *value)
            {
                sudoku[*spot1][*spot2] = *value;
                zeroCount--;
            }
            else
                printf("That is the wrong number! Try again\n");
            break;
        case 2:
            while (zeroCount && tries <= zeroCountConst)
            {
                zeroCount = solve(zeroCount, nums, sudoku);
                tries++;
            }
            if (zeroCount)
                printf("This sudoku has more than one solutions. Therefore it can't be solved logically.\n");
            break;
        case 3:
            printf("Possible solutions for this cell are: ");
            help(spot1, spot2, nums, sudoku);
            printf("\n");
            break;
        case 4:
            terminate(sudoku);
            break;
        case 5:
            printf("The correct solution for (%d,%d) is %d!\n", *spot1 + 1, *spot2 + 1, solved[*spot1][*spot2]);
            break;
        default:
            break;
        }

        printSudoku(sudoku);
        if (!zeroCount)
        {
            free(spot1);
            free(spot2);
            free(value);
            exit(0);
        }
        else
            tries = zeroCountConst - zeroCount;

    } while (playerOption);
}

int scramble(int sudoku[][SIZE])
{
    int hidden, hiddenCount, i, j;

    hidden = 25 + rand() % 16;

    hiddenCount = hidden;

    while (hiddenCount)
    {
        i = rand() % 9;
        j = rand() % 9;

        if (sudoku[i][j] != 0)
        {
            sudoku[i][j] = 0;
            hiddenCount--;
        }
    }

    return hidden;
}

int solve(int zeros, int nums[], int sudoku[][SIZE])
{
    int a, b, a1, b1, index, poss = 0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (sudoku[i][j] == 0)
            {
                poss = 0;

                for (int k = 0; k < SIZE; k++)
                    if (sudoku[k][j] != 0)
                        nums[sudoku[k][j] - 1] = 0;

                for (int k = 0; k < SIZE; k++)
                    if (sudoku[i][k] != 0)
                        nums[sudoku[i][k] - 1] = 0;

                a = i / 3 * 3;
                b = j / 3 * 3;

                a1 = a;
                b1 = b;

                for (a = a1; a < a1 + 3; a++)
                    for (b = b1; b < b1 + 3; b++)
                        if (sudoku[a][b] != 0)
                            nums[sudoku[a][b] - 1] = 0;

                for (int k = 0; k < SIZE; k++)
                {
                    if (nums[k] != 0)
                    {
                        poss++;

                        if (poss == 1)
                            index = k;
                        else if (poss > 1)
                            break;
                    }
                }
                if (poss == 1)
                {
                    sudoku[i][j] = nums[index];
                    zeros--;
                }

                for (int k = 0; k < SIZE; k++)
                    nums[k] = k + 1;
            }
        }
    }

    return zeros;
}

bool sudokuValidation(int nums[], int sudoku[][SIZE])
{
    int comp[9];
    int temp;

    // checks if every row of sud matches nums

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            comp[j] = sudoku[i][j];
        }
        for (int k = 0; k < 9; k++)
        {
            for (int l = k + 1; l < 9; l++)
            {
                if (comp[k] > comp[l])
                {
                    temp = comp[k];
                    comp[k] = comp[l];
                    comp[l] = temp;
                }
            }
        }
        for (int j = 0; j < 9; j++)
            if (comp[j] != nums[j])
                return false;
    }

    // checks if every column of sud matches nums

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            comp[j] = sudoku[j][i];
        for (int k = 0; k < 9; k++)
        {
            for (int l = k + 1; l < 9; l++)
            {
                if (comp[k] > comp[l])
                {
                    temp = comp[k];
                    comp[k] = comp[l];
                    comp[l] = temp;
                }
            }
        }
        for (int j = 0; j < 9; j++)
            if (comp[j] != nums[j])
                return false;
    }

    // checks if every 3x3 box of sud matches nums

    int o = 0;
    int p = 3;
    int c = 0;
    int v = 3;
    int w = 0;

    for (int x = 0; x < 9; x++)
    {
        if (x == 3 || x == 6)
        {
            c += 3;
            v += 3;
            o = 0;
            p = 3;
        }
        w = 0;
        for (int i = c; i < v; i++)
        {
            for (int j = o; j < p; j++)
            {
                comp[w] = sudoku[i][j];
                w++;
            }
            if (i == 2 || i == 5 || i == 8)
            {
                o += 3;
                p += 3;
            }
        }
        for (int k = 0; k < 9; k++)
        {
            for (int l = k + 1; l < 9; l++)
            {
                if (comp[k] > comp[l])
                {
                    temp = comp[k];
                    comp[k] = comp[l];
                    comp[l] = temp;
                }
            }
        }
        for (int j = 0; j < 9; j++)
            if (comp[j] != nums[j])
                return false;
    }

    return true;
}

void printSudoku(int sudoku[][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        if (i == 3 || i == 6)
        {
            for (int k = 0; k < 21; k++)
                printf("-");
            printf("\n");
        }
        for (int j = 0; j < SIZE; j++)
        {
            if (j == 3 || j == 6)
                printf("| ");

            if (sudoku[i][j] != 0)
                printf("%d ", sudoku[i][j]);
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

int getPlayerInput(char *playerInput, int sudoku[][SIZE], int *spot1, int *spot2, int *value)
{
    int playerInputFormatValid, playerInputSpotValid;
    do
    {
        fgets(playerInput, 15, stdin);
        playerInput[strcspn(playerInput, "\n")] = 0;
        playerInputFormatValid = playerInputFormatValidation(playerInput);
        playerInputSpotValid = playerInputSpotValidation(playerInput, sudoku, spot1, spot2);

        if (!playerInputFormatValid)
            printf("Invalid option. Try again.\n", playerInput);

        else if ((playerInputFormatValid == 1 || playerInputFormatValid == 3 || playerInputFormatValid == 5) && !playerInputSpotValid)
            printf("This space is already occupied. Try a different one.\n");

        else if ((playerInputFormatValid == 1 || playerInputFormatValid == 3 || playerInputFormatValid == 5) && playerInputSpotValid)
            break;

        else if (playerInputFormatValid == 2 || playerInputFormatValid == 4)
            break;

    } while (true);

    if (playerInputFormatValid == 1)
        *value = (playerInput[6] - '0');

    return playerInputFormatValid;
}

int playerInputFormatValidation(char *playerInput)
{
    if (playerInput[0] == '(' && (playerInput[1] >= '1' && playerInput[1] <= '9') && playerInput[2] == ',' && (playerInput[3] >= '1' && playerInput[3] <= '9') && playerInput[4] == ')' && playerInput[5] == '=' && (playerInput[6] >= '1' && playerInput[6] <= '9') && playerInput[7] == '\0')
        return 1;
    else if (playerInput[0] == '(' && playerInput[1] == '0' && playerInput[2] == ',' && playerInput[3] == '0' && playerInput[4] == ')' && playerInput[5] == '=' && playerInput[6] == '0' && playerInput[7] == '\0')
        return 2;
    else if (playerInput[0] == '(' && (playerInput[1] >= '1' && playerInput[1] <= '9') && playerInput[2] == ',' && (playerInput[3] >= '1' && playerInput[3] <= '9') && playerInput[4] == ')' && playerInput[5] == '=' && playerInput[6] == '0' && playerInput[7] == '\0')
        return 3;
    else if (playerInput[0] == '(' && playerInput[1] == '-' && playerInput[2] == '1' && playerInput[3] == ',' && playerInput[4] == '-' && playerInput[5] == '1' && playerInput[6] == ')' && playerInput[7] == '=' && playerInput[8] == '-' && playerInput[9] == '1' && playerInput[10] == '\0')
        return 4;
    else if (playerInput[0] == '(' && (playerInput[1] >= '1' && playerInput[1] <= '9') && playerInput[2] == ',' && (playerInput[3] >= '1' && playerInput[3] <= '9') && playerInput[4] == ')' && playerInput[5] == '=' && playerInput[6] == '-' && playerInput[7] == '1' && playerInput[8] == '\0')
        return 5;

    return 0;
}

int playerInputSpotValidation(char *playerInput, int sudoku[][SIZE], int *spot1, int *spot2)
{
    *spot1 = (playerInput[1] - '0') - 1;
    *spot2 = (playerInput[3] - '0') - 1;

    if (!sudoku[*spot1][*spot2])
        return 1;

    return 0;
}

void terminate(int sudoku[][SIZE])
{
    // int saveProgress;
    // printf("\nDo you want to save your progress in your current puzzle? (type 1 for yes): \n");
    // scanf("%d", &saveProgress);

    // if (saveProgress == 1)
    //     exit(0);
    // else
    //     exit(0);

    exit(0);
}

void help(int *spot1, int *spot2, int nums[], int sudoku[][SIZE])
{
    int a, b, a1, b1;

    if (sudoku[*spot1][*spot2] == 0)
    {
        for (int k = 0; k < SIZE; k++)
            if (sudoku[k][*spot2] != 0)
                nums[sudoku[k][*spot2] - 1] = 0;

        for (int k = 0; k < SIZE; k++)
            if (sudoku[*spot1][k] != 0)
                nums[sudoku[*spot1][k] - 1] = 0;

        a = *spot1 / 3 * 3;
        b = *spot2 / 3 * 3;

        a1 = a;
        b1 = b;

        for (a = a1; a < a1 + 3; a++)
            for (b = b1; b < b1 + 3; b++)
                if (sudoku[a][b] != 0)
                    nums[sudoku[a][b] - 1] = 0;
    }

    for (int i = 0; i < SIZE; i++)
        if (nums[i])
            printf("%d ", nums[i]);

    for (int i = 0; i < SIZE; i++)
        nums[i] = i + 1;
}
