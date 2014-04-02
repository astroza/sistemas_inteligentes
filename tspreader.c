/* F.A.A. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void print_matrix(int *matrix, int dimension)
{
	int i, j;
	puts("MATRIX: ");
	for(i = 0; i < dimension; i++) {
		for(j = 0; j < dimension; j++)
			printf("%d ", matrix[i*dimension + j]);
		printf("\n");
	}
}

int cost(int x, int y, int *matrix, int dimension)
{
	if(y <= x)
		return matrix[x*dimension + y];
	else
		return matrix[y*dimension + x];
}

void print_sol(int *travel, int dimension)
{
	int i;
	printf("TRAVEL: ");
	for(i = 0; i < dimension; i++)
		printf("%d ", travel[i]);
	printf("\n");
}

void gen_neighbor(int *travel, int dimension)
{
        int i;
        int x, y;
        int t;

        for(i = 0; i < dimension*5; i++) {
                x = rand()%dimension;
                y = rand()%dimension;
                t = travel[x];
                travel[x] = travel[y];
                travel[y] = t;
        }
}

void gen_sol(int *travel, int dimension)
{
	int i;

	for(i = 0; i < dimension; i++)
		travel[i] = i;

	gen_neighbor(travel, dimension);
}

int sol_cost(int *travel, int *matrix, int dimension)
{
	int total_cost = 0;
	int i;

	for(i = 1; i < dimension; i++)
		total_cost += cost(travel[i-1], travel[i], matrix, dimension);

	total_cost += cost(travel[0], travel[dimension-1], matrix, dimension);

	return total_cost;
}

int main(int argc, char **argv)
{
	char buffer[1024];
	char *key, *value;
	int step = 0;
	int *data;
	int column = 0;
	int row = 0;
	int dimension;
	int v, v_best;
	int *travel;
	int *best_travel;
	int i;
	int iterations;

	if(argc < 2)
		return 0;

	iterations = strtol(argv[1], NULL, 10);
	//printf("+ Iterations: %d\n", iterations);

	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		switch(step) {
			case 0:
				key = strtok(buffer, ":");
				value = strtok(NULL, ":");
				if(strstr(key, "EDGE_WEIGHT_SECTION") != NULL)
					step++;
				else if(strcmp(key, "DIMENSION") == 0) {
					dimension = strtol(value, NULL, 10);
					//printf("+ Setting dimension to %d\n", dimension);
					data = malloc(sizeof(int) * dimension*dimension);
					memset(data, 0, sizeof(int) * dimension*dimension);
					travel = malloc(sizeof(int) * dimension);
					best_travel = malloc(sizeof(int) * dimension);
					memset(travel, 0, sizeof(int) * dimension);
				}
				break;
			case 1:
				for(value = strtok(buffer, " "); value != NULL; value = strtok(NULL, " ")) {
					v = strtol(value, NULL, 10);
					data[row*dimension + column] = v;
					if(v == 0) {
						row++;
						column = 0;
					} else
						column++;
				}
		}
	}
	srand(time(NULL));
	//print_matrix(data, dimension);

	gen_sol(travel, dimension);

	v_best = INT_MAX;
	for(i = 0; i < iterations; i++) {
		//print_sol(travel, dimension);
		v = sol_cost(travel, data, dimension);
		if(v_best > v) {
			memcpy(best_travel, travel, sizeof(int)*dimension);
			v_best = v;
		}
		printf("%d %d\n", i, v_best);
		gen_neighbor(travel, dimension);
	}

	//printf("THE BEST:\n");
	//print_sol(travel, dimension);
	//printf("BEST COST: %d\n", v_best);
	free(data);

	return 0;
}
