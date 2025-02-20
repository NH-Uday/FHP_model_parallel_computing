#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>

//int width = 30;
//int height = 10;
//double lspacing = 0.1;
const int x_nodes = 300;
const int y_nodes = 100;
FILE *fp;
//int local_width = 50;

//int grid[x_nodes*y_nodes * 6];

void grid_init(int x_nodes, int y_nodes, int *grid) {
    srand(time(NULL));
    for (int i = 0; i < x_nodes * y_nodes * 6; i++) 
    {
        if (i % 6 == 0) 
        {
            if (rand() % 100 < 5) 
            {
                for (int j = 0; j < 6; j++)
                {
                    grid[i + rand() % 6] = 1;
                }
            }
        }
    }
}

void print_grid(int *grid, int x_nodes, int y_nodes) {
    for (int i = 0; i < x_nodes * y_nodes * 6; i+=6) {
        for (int j = 0; j < 6; j++) {
            fprintf(fp,"%d",grid[i + j]);
        }
        fprintf(fp,"\n");
    }
}

void print_grid2(int *grid, int x_nodes, int y_nodes,char *filename) {
    fp=fopen(filename,"a");


    for (int i = 0; i < x_nodes * y_nodes * 6; i+=6) {
        for (int j = 0; j < 6; j++) {
            fprintf(fp,"%d",grid[i + j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void collision(int *grid, int x_nodes, int y_nodes) 
{
    srand(time(NULL));
    for (int i = 0; i < x_nodes * y_nodes * 6; i += 6) 
    {
        if (i % (y_nodes * 6) != 0 && i > (y_nodes*6) && (i+6) % (y_nodes * 6) != 0) //Applying collision leaving 1st row, 1st column, last column
        {
            for (int j = 0; j < 6; j++) 
            {
                int dir1 = j;
                int dir2 = (j + 3) % 6;
                
                int dir3 = j;
                int dir4 = (j+2) % 6;
                int dir5 = (j+4) % 6;

                //Condition 1: one back to back

                if ((grid[i + dir1] == 1) && (grid[i + dir2] == 1) && !(grid[i + (dir1 + 1)%6] + grid[i + (dir1 + 2)%6] + grid[i + (dir1 + 4)%6] + grid[i + (dir1 + 5)%6])) 
                {
                    //printf("condition 1\n");
                    int coin_flip = rand() % 2;
                    grid[i + dir1] = 0;
                    grid[i + dir2] = 0;

                    if (coin_flip)
                    {
                        grid[(i + (dir1 + 1)%6)] = 1;
                        grid[(i + (dir2 + 1)%6)] = 1;
                    }
                    else
                    {
                        grid[(i + (dir1 + 5)%6)] = 1;
                        grid[(i + (dir2 + 5)%6)] = 1;
                    }
                    break;
                }

                // Condition 2: Three occupied directions Mercedes

                else if (grid[i + dir3] == 1 && grid[i + dir4] == 1 && grid[i + dir5] == 1 && grid[i + (dir3 + 1)%6] == 0 && grid[i + (dir4 + 1)%6] == 0 && grid[i + (dir5 + 1)%6] == 0)
                {
                    //printf("condition 2\n");
                    grid[i + (dir3) % 6] = 0;
                    grid[i + (dir4) % 6] = 0;
                    grid[i + (dir5) % 6] = 0;
                    grid[i + (dir3 + 1) % 6] = 1;
                    grid[i + (dir4 + 1) % 6] = 1;
                    grid[i + (dir5 + 1) % 6] = 1;
                    break;
                }

                //Condition 3 : A node with four occupied directions

                else if ((grid[i + dir1] == 1) && (grid[i + dir2] == 1) && grid[i + (dir1 + 1)%6] == 1 && (grid[i + (dir2 + 1)%6] == 1) && !(grid[i + (dir1 + 2)%6] + grid[i + (dir1 + 5)%6]))
                {
                    //printf("condition 3\n");
                    int coin_flip = rand() % 2;
                    grid[i + dir1] = 0;
                    grid[i + dir2] = 0;
                    grid[i + (dir1 + 1)%6] = 0;
                    grid[i + (dir2 + 1)%6] = 0;

                    if (coin_flip)
                    {
                        grid[(i + (dir1 + 1)%6)] = 1;
                        grid[(i + (dir2 + 1)%6)] = 1;
                        grid[(i + (dir1 + 2)%6)] = 1;
                        grid[(i + (dir2 + 2)%6)] = 1;

                    }
                    else
                    {
                        grid[(i + (dir1 + 5)%6)] = 1;
                        grid[(i + (dir2 + 5)%6)] = 1;
                        grid[(i + (dir1 + 6)%6)] = 1;
                        grid[(i + (dir2 + 6)%6)] = 1;
                    }
                    break;
                }

                // Condition 4 : A node with three occupied directions, of which two are back-to-back directions and the third occupied direction

                else if ((grid[i + dir1] == 1) && (grid[i + dir2] == 1) && (grid[i + (dir1 + 1)%6] + grid[i + (dir1 + 2)%6] + grid[i + (dir1 + 4)%6] + grid[i + (dir1 + 5)%6]) == 1 && grid[i + (dir2 + 1)%6] == 1) 
                {
                    //printf("condition 4.1\n");
                    grid[i + dir1] = 0;
                    grid[i + dir2] = 0;

                    grid[(i + (dir1 + 2)%6)] = 1;
                    grid[(i + (dir2 + 2)%6)] = 1;
                    
                    break;
                }

                else if ((grid[i + dir1] == 1) && (grid[i + dir2] == 1) && (grid[i + (dir1 + 1)%6] + grid[i + (dir1 + 2)%6] + grid[i + (dir1 + 4)%6] + grid[i + (dir1 + 5)%6]) == 1 && grid[i + (dir2 + 5)%6] == 1) 
                {
                    //printf("condition 4.2\n");
                    grid[i + dir1] = 0;
                    grid[i + dir2] = 0;

                    grid[(i + (dir1 + 1)%6)] = 1;
                    grid[(i + (dir2 + 1)%6)] = 1;
                    
                    break;
                }
            }
        }
    }
}

void slip(int *grid, int x_nodes, int y_nodes)
{
    for (int i = 0; i < x_nodes * y_nodes * 6; i+=6) 
    {  
        // slip
        if (i % (y_nodes*6) == 0 || (i+6) % (y_nodes*6) == 0)
        {
            for (int j = 0; j < 6; j++)
            {
               grid[i+j] = 0;
            }
        }
    } 
}

void dirichlet_in(int *grid, int x_nodes, int y_nodes)
{
    for (int i = 0; i < x_nodes * y_nodes * 6; i+=6) 
    {  

        // Dirichlet

        // for top row
        if (i < y_nodes * 6)
        { 
            // filling 3,4,5 indexed leg in the node randomly

            if (rand() % 2 == 1)
            {
                grid[i+3] = 1;
            }

            if (rand() % 2 == 1)
            {
                grid[i+4] = 1;
            }

            if (rand() % 2 == 1)
            {
                grid[i+5] = 1;
            }
        }
    } 
}

void dirichlet_out(int *grid, int x_nodes, int y_nodes)
{
    for (int i = 0; i < x_nodes * y_nodes * 6; i+=6) 
    {  
        // dirichlet for bottom row

        if (i > (x_nodes*y_nodes*6 - y_nodes*6))
        {
            for (int j = 0; j < 6; j++)
            {
               grid[i+j] = 0;
            }
        }
    } 
}

void streaming(int *grid, int x_nodes, int y_nodes)
{
    int neb;
    int newGrid[x_nodes * y_nodes * 6];

    // Initializing newGrid to 0
    for (int i = 0; i < x_nodes * y_nodes * 6; i++)
    {
        newGrid[i] = 0;
    }

    //main streaming function
    for (int i = 0; i < x_nodes * y_nodes * 6; i+=6) 
    {
        // skipping the slip and bottom dirichlet
        if (i % (y_nodes*6) && (i+6) % (y_nodes * 6) != 0 && i < (x_nodes*y_nodes*6 - y_nodes*6))
        {
            // Looping through the legs
            for (int j = 0; j < 6; j++)
            {
                // Checking if any of the legs are 1
                if (grid[i + j] == 1)
                {
                    // checking odd or even as the neighbors are different for even and odd
                    if ((i / 6) % 2 == 1)
                    {
                        // figuring out the correct neighbor to change
                        switch (j)
                        {
                        case 0:  
                            neb = i - 6;
                            // Handling the left boundary slip condition
                            if (neb % (y_nodes*6) == 0)
                            {
                                grid[i + j] = 0;
                                newGrid[i + 2] = 1;
                            }
                            break;

                        case 1:
                            neb = i - (y_nodes * 6);
                            break;
                        
                        case 2:
                            neb = i + 6;
                            break;

                        case 3:
                            neb = i + (y_nodes+1)*6;
                            break;

                        case 4:
                            neb = i + (y_nodes * 6);
                            break;

                        case 5:
                            neb = i + (y_nodes-1)*6;
                            // Handling the left boundary slip condition
                            if (neb % (y_nodes*6) == 0)
                            {
                                grid[i + j] = 0;
                                newGrid[i + 3] = 1;
                            }
                            break;
                        
                        default:
                            break;
                        }
                        
                        
                    }
                    else
                    {
                        switch (j)
                        {
                        case 0:  
                            neb = i - ((y_nodes+1) * 6);
                            break;

                        case 1:
                            neb = i - (y_nodes * 6);
                            break;
                        
                        case 2:
                            neb = i - ((y_nodes-1)*6);
                            // Handling the right boundary slip condition
                            if ((neb+6) % (y_nodes*6) == 0)
                            {
                                grid[i + j] = 0;
                                newGrid[i] = 1;

                            }
                            break;

                        case 3:
                            neb = i + 6;
                            // Handling the right boundary slip condition
                            if ((neb+6) % (y_nodes*6) == 0)
                            {
                                grid[i + j] = 0;
                                newGrid[i + 5] = 1;
                            }

                            break;

                        case 4:
                            neb = i + (y_nodes * 6);
                            break;

                        case 5:
                            neb = i - 6;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    // filling the new grid
                    newGrid[i + j] = 0;
                    newGrid[neb + j] = 1;
                    //printf("neb+j:%d filled\n",neb+j);
                } 
            }
        }
    }

    memcpy(grid, newGrid, sizeof(newGrid));
}

void temp_streaming_2layers(int *grid, int *temp, int *upper_layer, int *lower_layer, int x_nodes, int y_nodes){
    for (int i = 0; i < y_nodes*6; i++){
        temp[i] = upper_layer[i];
    }
    for (int j = 0; j < x_nodes*y_nodes*6; j++){
        temp[j+(y_nodes*6)] = grid[j];
    }
    for (int k = 0; k < y_nodes*6; k++){
        temp[k+x_nodes*y_nodes*6+(y_nodes*6)] = lower_layer[k];
    }
    streaming(temp, x_nodes+2, y_nodes);

    for (int m = 0; m<x_nodes*y_nodes*6; m++){
        grid[m] = temp[m+(y_nodes*6)];
    }
}

void temp_streaming_upper_layer(int *grid, int *temp, int *upper_layer, int x_nodes, int y_nodes){
    for (int i = 0; i < y_nodes*6; i++){
        temp[i] = upper_layer[i];
    }
    for (int j = 0; j < x_nodes*y_nodes*6; j++){
        temp[j+(y_nodes*6)] = grid[j];
    }
    streaming(temp, x_nodes+1, y_nodes);

    for (int m = 0; m<x_nodes*y_nodes*6; m++){
        grid[m] = temp[m+(y_nodes*6)];
    }
}

void temp_streaming_lower_layer(int *grid, int *temp, int *lower_layer, int x_nodes, int y_nodes){
    for (int j = 0; j < x_nodes*y_nodes*6; j++){
        temp[j] = grid[j];
    }
    for (int k = 0; k < y_nodes*6; k++){
        temp[k+x_nodes*y_nodes*6] = lower_layer[k];
    }
    streaming(temp, x_nodes+1, y_nodes);

    for (int m = 0; m<x_nodes*y_nodes*6; m++){
        grid[m] = temp[m];
    }
}

int main() {

    int rank, numprocs;
    srand(time(NULL));

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    int local_width = x_nodes / numprocs;

    int local_grid[local_width * y_nodes * 6];
    int temp_grid[local_width * y_nodes * 6];
    grid_init(local_width, y_nodes, local_grid);

    int upper_layer[y_nodes*6], lower_layer[y_nodes*6];

    // for (int i =0; i<y_nodes*6; i++){
    //     upper_layer[i] = 0;
    //     lower_layer[i] = 0;
    // }


    if (rank == 0){
        clock_t start_time, end_time;
        double total_time;
        int temp[(local_width * y_nodes * 6) + (y_nodes*6)];

        start_time = clock(); // Record the start time
        
        dirichlet_in(local_grid, local_width, y_nodes);
        slip(local_grid, local_width, y_nodes);


        for (int t=0; t < 100; t++)
        {
            collision(local_grid, local_width, y_nodes);
            if (numprocs != 1){
                MPI_Send(&local_grid[y_nodes*6*(local_width-1)], y_nodes*6, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
                MPI_Recv(lower_layer, y_nodes*6, MPI_INT, (rank + 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


                temp_streaming_lower_layer(local_grid, temp, lower_layer, local_width, y_nodes);

                //streaming(temp, local_width, y_nodes);
                //print_grid2(grid, local_width, y_nodes);

                dirichlet_in(local_grid, local_width, y_nodes);
                slip(local_grid, local_width, y_nodes);
                // print_grid(grid, local_width, y_nodes);
            }
            else{
                streaming(local_grid, local_width, y_nodes);
                //print_grid2(grid, local_width, y_nodes);

                dirichlet_in(local_grid, local_width, y_nodes);
                slip(local_grid, local_width, y_nodes);
                dirichlet_out(local_grid, local_width, y_nodes);
                // print_grid(grid, local_width, y_nodes);
            }

            //saving result in a file
            char filename[50]; // Buffer to store the formatted filename
            sprintf(filename, "output%d.txt", t); 
            fp = fopen(filename, "w+"); 

            fprintf(fp,"%d\n",x_nodes);
            fprintf(fp,"%d\n",y_nodes);
		    fclose(fp);
            print_grid2(local_grid,local_width,y_nodes,filename);
            MPI_Barrier(MPI_COMM_WORLD);
            for (int n=1;n<numprocs;n++)
                {
                    MPI_Recv(temp_grid, local_width*y_nodes*6, MPI_INT, n, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
                    print_grid2(temp_grid,local_width,y_nodes,filename);
            
                }
            MPI_Barrier(MPI_COMM_WORLD);
        }
        end_time = clock(); // Record the end time

        total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calculate total time in seconds
        printf("Numprocs = %d, Total time taken: %.6f seconds\n", numprocs, total_time);
    }

    else if (rank!=numprocs-1){
        //clock_t start_time, end_time;
        //double total_time;

        //start_time = clock(); // Record the start time
        slip(local_grid, local_width, y_nodes);
        int temp[local_width * y_nodes * 6 + (y_nodes*6)+(y_nodes*6)];

        for (int t=0; t < 1000; t++)
        {
            collision(local_grid, local_width, y_nodes);

            MPI_Send(&local_grid[y_nodes*6*(local_width-1)], y_nodes*6, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
            MPI_Recv(upper_layer, y_nodes*6, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(local_grid, y_nodes*6, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
            MPI_Recv(lower_layer, y_nodes*6, MPI_INT, (rank + 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


            temp_streaming_2layers(local_grid, temp, upper_layer, lower_layer, local_width, y_nodes);

            //streaming(temp, local_width, y_nodes);
            //print_grid2(grid, local_width, y_nodes);

            slip(local_grid, local_width, y_nodes);
            // print_grid(grid, local_width, y_nodes);

            //sending to 0 to save the result
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Send(local_grid, local_width*y_nodes*6, MPI_INT, 0, 7, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);

        }
        //end_time = clock(); // Record the end time

        //total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calculate total time in seconds
        //printf("Rank = %d, Total time taken: %.6f seconds\n", rank, total_time);
    }
    else{
        //clock_t start_time, end_time;
        //double total_time;

        //start_time = clock(); // Record the start time
        slip(local_grid, local_width, y_nodes);
        dirichlet_out(local_grid, local_width, y_nodes);
        int temp[local_width * y_nodes * 6 + (y_nodes*6)];

        for (int t=0; t < 1000; t++)
        {
            collision(local_grid, local_width, y_nodes);
            MPI_Recv(upper_layer, y_nodes*6, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(local_grid, y_nodes*6, MPI_INT, rank-1, 1, MPI_COMM_WORLD);


            temp_streaming_upper_layer(local_grid, temp, upper_layer, local_width, y_nodes);

            //streaming(temp, local_width, y_nodes);
            //print_grid2(grid, local_width, y_nodes);

            slip(local_grid, local_width, y_nodes);
            dirichlet_out(local_grid, local_width, y_nodes);
            // print_grid(grid, local_width, y_nodes);

            //sending to 0 to save the result
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Send(local_grid, local_width*y_nodes*6, MPI_INT, 0, 7, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);
        }
        //end_time = clock(); // Record the end time

        //total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Calculate total time in seconds
        //printf("Rank = %d, Total time taken: %.6f seconds\n", rank, total_time);
        
    }


    // MPI_Barrier(MPI_COMM_WORLD);
    // if (rank != 0){
	// 	MPI_Send(local_grid, local_width*y_nodes*6, MPI_INT, 0, 7, MPI_COMM_WORLD);
	// }
    // else {
    //     fp=fopen("output1.txt","w+");
    //     fprintf(fp,"%d\n",x_nodes);
    //     fprintf(fp,"%d\n",y_nodes);
	// 	fclose(fp);
    //     print_grid2(local_grid,local_width,y_nodes);

    //      for (int n=1;n<numprocs;n++)
    //       {
    //          MPI_Recv(local_grid, local_width*y_nodes*6, MPI_INT, n, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    //          print_grid2(local_grid,local_width,y_nodes);
            
    //       }
    // }

    MPI_Finalize();

    return 0;
}
