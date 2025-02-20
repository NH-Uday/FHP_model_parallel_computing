#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

int width = 30;
int height = 10;
double lspacing = 0.1;
const int x_nodes = 300;
const int y_nodes = 100;
FILE *fp;

// Initialize grid
void grid_init(int x_nodes, int y_nodes, int *grid) {
    srand(time(NULL));
    for (int i = 0; i < x_nodes * y_nodes * 6; i++) {
        grid[i] = 0;  // Initialize to zero
        if (i % 6 == 0 && rand() % 100 < 5) {
            for (int j = 0; j < 6; j++) {
                grid[i + (rand() % 6)] = 1;
            }
        }
    }
}

// Print grid to a file
void print_grid(int *grid, int x_nodes, int y_nodes) {
    fp = fopen("output_serial.txt", "a");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    fprintf(fp, "%d\n", x_nodes);
    fprintf(fp, "%d\n", y_nodes);

    for (int i = 0; i < x_nodes * y_nodes * 6; i += 6) {
        for (int j = 0; j < 6; j++) {
            fprintf(fp, "%d", grid[i + j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Collision function with boundary checks
void collision(int *grid, int x_nodes, int y_nodes) {
    for (int i = 0; i < x_nodes * y_nodes * 6; i += 6) {
        for (int j = 0; j < 6; j++) {
            int dir1 = j;
            int dir2 = (j + 3) % 6;

            // Ensure index validity
            if (i >= 0 && i + dir1 < x_nodes * y_nodes * 6 && i + dir2 < x_nodes * y_nodes * 6) {
                if (grid[i + dir1] == 1 && grid[i + dir2] == 1) {
                    grid[i + dir1] = 0;
                    grid[i + dir2] = 0;
                    grid[i + (rand() % 6)] = 1;
                    break;
                }
            }
        }
    }
}

// Streaming function with boundary checks
void streaming(int *grid, int x_nodes, int y_nodes) {
    int *newGrid = (int *)calloc(x_nodes * y_nodes * 6, sizeof(int));
    if (!newGrid) {
        perror("Memory allocation failed");
        exit(1);
    }

    for (int i = 0; i < x_nodes * y_nodes * 6; i += 6) {
        for (int j = 0; j < 6; j++) {
            if (grid[i + j] == 1) {
                int new_idx = (i + j + 6) % (x_nodes * y_nodes * 6);
                if (new_idx < x_nodes * y_nodes * 6) {
                    newGrid[new_idx] = 1;
                }
            }
        }
    }
    memcpy(grid, newGrid, x_nodes * y_nodes * 6 * sizeof(int));
    free(newGrid);
}

// Main function
int main() {
    // Allocate memory for the grid
    int *grid = (int *)calloc(x_nodes * y_nodes * 6, sizeof(int));
    if (!grid) {
        perror("Memory allocation failed");
        return 1;
    }

    // Initialize the grid
    grid_init(x_nodes, y_nodes, grid);

    clock_t start_time = clock();
    for (int t = 0; t < 1000; t++) {
        collision(grid, x_nodes, y_nodes);
        streaming(grid, x_nodes, y_nodes);
        print_grid(grid, x_nodes, y_nodes);
    }
    clock_t end_time = clock();

    printf("Total time taken: %.6f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    free(grid);
    return 0;
}

