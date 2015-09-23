#include "graph.h"
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <omp.h>
#define NUM_OF_CELLS  3969 // 63*63 // 136192
//#define NUM_OF_CELLS  100 // 63*63 // 136192
#define NUM_OF_MONTHS 1404+10
#define NUM_OF_WEEKS  832 // 16*52	
#define NUM_OF_YEARS  16 // 1990-2005	
#define YEAR_MAX      2005

#define PARALLEL

using namespace std;

float Data[NUM_OF_YEARS][NUM_OF_WEEKS][NUM_OF_CELLS];
float buffer[NUM_OF_CELLS];
float averages[NUM_OF_CELLS];
float getCorrelation(int xpos, int ypos);
void readData(int yearMax);

int main(int argc, char const *argv[]) {
	graph G(NUM_OF_CELLS);
	

	#ifdef PARALLEL
    printf("Number of Threads = %d\n", omp_get_num_threads());
	omp_set_num_threads(omp_get_num_procs()*3/2); 
    printf("Number of Processors = %d\n", omp_get_num_procs());
    printf("Number of Threads = %d\n", omp_get_num_threads());
    #endif

	printf("reading data\n");
	FILE *fp;
	const char base[] = "./subregion/";

	// Read ALL data to Data[][][]
	for (int year = 1990; year <= YEAR_MAX; ++year) 
	{
		char yearStr[5] = {};

		for (int week = 1; week <= 52; ++week)	
		{
			char weekStr[40];
			char path[100] = {};
			strcat(path, base);		
			sprintf(weekStr, "%4d/Beaufort_Sea_diffw%02dy%4d+landmask", year, week, year);
			strcat(path, weekStr);
			//printf("%s\n", path);

			// read data from file
			fp = fopen(path, "rb");
			
			if(fp == NULL)
			{
				printf("\nError, file not found\n"); 
				return 1;
			}

			fread(buffer, sizeof(float), NUM_OF_CELLS, fp);
			memcpy(Data[year - 1990][week - 1], buffer, NUM_OF_CELLS * sizeof(float));

			memset(buffer, 0, NUM_OF_CELLS);
			fclose(fp);
		}	
	}

	FILE * avgOut = fopen("avg.dat", "rb");
	
	if(avgOut == NULL)
	{
		printf("\nError, file not found\n"); 
		return 1;
	}

	fread(averages, sizeof(float), NUM_OF_CELLS, avgOut); 

	//for(int i=0; i<NUM_OF_CELLS; i++)
	//	printf("%d = %f\n", i, averages[i]);

	// Calculations start here

	#ifdef PARALLEL
	omp_lock_t lock;
	omp_init_lock(&lock);	
	

	#pragma omp parallel for
	#endif
	for (int i = 0; i < NUM_OF_CELLS; ++i) 
	{
		if(omp_get_thread_num() == 0)
			#ifdef PARALLEL
			printf("Progress: %d/%d\n", i, NUM_OF_CELLS/omp_get_num_threads());
			#else
			printf("Progress: %d/%d\n", i, NUM_OF_CELLS);	
			#endif

		for (int j = i + 1; j < NUM_OF_CELLS; ++j) 
		{ 
			float r;
			r = getCorrelation(i, j);
			// print correlation
	        // printf("(%d, %d): %f\n", i, j, r);
			if (r > 0.9) 
			{
				#ifdef PARALLEL
				omp_set_lock(&lock);
				#endif
				G.addEdge(i, j);
				#ifdef PARALLEL
				omp_unset_lock(&lock);
				#endif
			}
		}
	}

	#ifdef PARALLEL
	omp_destroy_lock(&lock);	
	#endif	
	
	G.printGraph();	

	return 0;
}

// USE LATER. NEED MODIFICATION
void readData(int yearMax) 
{
	printf("reading data\n");
	FILE *fp;
	float buffer[10];
	const char base[] = "./subregion/";

	// Read ALL data to Data[][][]
	for (int year = 1990; year <= yearMax ; ++year) 
	{
		char yearStr[5] = {};

		for (int week = 1; week <= 52; ++week)	
		{
			char weekStr[40];
			char path[100] = {};
			strcat(path, base);		
			sprintf(weekStr, "%4d/Beaufort_Sea_diffw%02dy%4d+landmask", year, week, year);
			strcat(path, weekStr);
			// printf("%s\n", path);

			// read data from file
			fp = fopen(path, "rb");

			fread(buffer, sizeof(float), NUM_OF_CELLS, fp);
			memcpy(Data[year - 1990][week - 1], buffer, NUM_OF_CELLS * sizeof(float));

			memset(buffer, 0, NUM_OF_CELLS);
			// count++;
			fclose(fp);
		}	
	}
}


float getCorrelation(int xpos, int ypos) {
	float xmean = 0;
	float ymean = 0;


	float X[NUM_OF_WEEKS];
	float Y[NUM_OF_WEEKS];

	// read from Data[][][] to X[] and Y[]
	for (int j = 0; j <= (YEAR_MAX - 1990); ++j) 
	{
		for (int i = 0; i < 52; ++i) 
		{
			X[j*52 + i] = Data[j][i][xpos];
			Y[j*52 + i] = Data[j][i][ypos];

			xmean += X[j*52 + i];
			ymean += Y[j*52 + i];
		}
	}

 	xmean /= (YEAR_MAX - 1990 + 1) * 52;
 	ymean /= (YEAR_MAX - 1990 + 1) * 52;

	// Sxx, Syy
	float Sxx = 0, Syy = 0, Sxy = 0;
	for (int i = 0; i < 52 * (YEAR_MAX - 1990 + 1); ++i) 
	{
		Sxx += (X[i] - xmean) * (X[i] - xmean);
		Syy += (Y[i] - ymean) * (Y[i] - ymean);
		Sxy += (X[i] - xmean) * (Y[i] - ymean);
	}	

	// printf("%f, %f, %f\n", Sxx, Syy, Sxy);
	float r;
	Sxy = Sxy > 0 ? Sxy : -Sxy;
	if (Sxx == 0 || Syy == 0) 
	{
		r = 0;
	} 
	else 
	{
		r = Sxy / sqrt(Sxx * Syy);
	}

	// r = %f
	// printf("%f\n", r);

	// memset(X, 0, ARRAYS);
	// memset(Y, 0, ARRAYS);

	return r;
}
