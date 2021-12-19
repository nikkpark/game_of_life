#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define GRN "\e[0;32m"
#define RED "\e[0;31m"
#define NC "\e[0m"
void begin(int rows, int columns, float chance);
char* create(int rows, int columns, float chance);
double getRandomDoubleInRange(double min, double max);
void display(int rows, int columns, char* simulation);
char* step(int rows, int columns, char* prevSimulation);
int countNeighbourhood(int rows, int columns, int x, int y, char* simulation);

int main(int argc, char* argv[])
{
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    float chance = atof(argv[3]);

    if (rows <= 0 || columns <= 0) {
        printf("Rows and columns must be positive integers.\n");
        return -1;
    } else if (chance > 1 && chance < 0) {
        printf("Chance must be higher than 0 but lower than 1.\n");
        return -1;
    } else {
        rows += 2;
        columns += 2;
        begin(rows, columns, chance);
        return 0;
    }
}

void display(int rows, int columns, char* simulation)
{
    //printf("\n\n\n\n\n\n\n");
    printf("\033c");
    
    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < columns - 1; x++) {
            if (*(simulation + y * columns + x) == '#') {
                printf(GRN "%c ", *(simulation + y*columns + x));
            } else {
                printf(RED "%c ", *(simulation + y*columns + x));
            }

        }
        printf("\n");
    }
}

void begin(int rows, int columns, float chance)
{
    //printf("Start!\n");

    char* simulation = create(rows, columns, chance);
   
    if (simulation == NULL) {
        return;
    }
    display(rows, columns, simulation);

    while(1) {
        char* newSim = step(rows, columns, simulation);
        if (newSim == NULL) {
            return;
        }

        free(simulation);
        simulation = newSim;
        display(rows, columns, simulation);
        usleep(150000);
    }

}

int countNeighbourhood(int rows, int columns, int x, int y, char* simulation)
{
    int count = 0;
    int pivot = y * columns + x;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            char c = *(simulation + pivot + (i * columns) + j);
            
            if (c == '#') {
                count++;
            }
        }
    }
    return count;
}

char* step(int rows, int columns, char* prevSimulation)
{
    char* steppedSimulation = calloc(rows*columns, sizeof(char));
    if (steppedSimulation == NULL) {
        return NULL;
    }

    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < columns - 1; x++) {
            int live = countNeighbourhood(rows, columns, x, y, prevSimulation);
            char cell = *(prevSimulation + y * columns + x);

            if (cell == '#') {
                live--;
            }
            *(steppedSimulation + y * columns + x) = cell;
            
            if (live < 2) {
                *(steppedSimulation + y * columns + x) = '.';
            } else if ((live == 2 || live == 3) && cell == '#') {
                *(steppedSimulation + y * columns + x) = '#';
            } else if (live > 3 && cell == '#') {
                *(steppedSimulation + y * columns + x) = '.';
            } else if (live == 3 && cell == '.') {
                *(steppedSimulation + y * columns + x) = '#';
            }
        }
     }
     return steppedSimulation;
}

double getRandomDoubleInRange(double min, double max)
{
    return ((double)rand()/RAND_MAX)*(max-min)+min;
}

char* create(int rows, int columns, float chance)
{
    char* simulation = (char*)calloc(rows*columns, sizeof(char));

    if (simulation == NULL) {
        return NULL;
    }

    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < columns - 1; x++) {
            if (getRandomDoubleInRange(0.0, 1.0) <= chance) {
                *(simulation + y*columns + x) = '#';
            }
            else {
                *(simulation + y*columns + x) = '.';
            }
        }
    }
    return simulation;
}
