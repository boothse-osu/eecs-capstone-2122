
#include "csv_parser.h"

//Dynamic memory handling for adding points to the printing path
//Returns current size of the printing path
void add_point(struct Path* path, struct Point point) {

	//If there isn't enough memory, we need to add more
	if ((path->size) + 1 > path->cap) {
		//Otherwise double the cap
		struct Point* tmp = (struct Point*)realloc(path->points, sizeof(struct Point) * path->cap * 2);
		if (tmp != NULL) {
			path->points = tmp;
			path->cap = 2 * path->cap;
		}
		else {
			printf("Unexpected error: Failed to allocate memory\n");
			exit(EXIT_FAILURE);
		}
	}

	path->points[path->size] = point;
	path->size++;
}

//CSV parser function. Returns 0 on success 1 on failure 
int csv_parse(struct Path* path, char* filepath) {

	FILE* fp;
	fp = fopen(filepath, "r");

	if (fp == NULL) {
		perror("File opening error");
		return 1;
	}

	//Go through all of the lines of the file and process them individually
	char buffer[CSVBUFFSIZE];
	fgets(buffer, CSVBUFFSIZE, fp);

	int linecount = 0;

	while (fgets(buffer, CSVBUFFSIZE, fp) != NULL) {

		//printf("Token: %s\n", buffer);

		//Ignore header line
		if (linecount != 0) {

			//Break the string into tokens and put the tokens into Point structs

			const char* delims = ",\n";
			char* token = strtok(buffer, delims);
			int token_count = 0;

			struct Point temptok;

			while (token != NULL) {

				//printf("%s\n", token);

				switch (token_count) {

				case 0: temptok.x = (float)atof(token); break;

				case 1: temptok.y = (float)atof(token); break;

				case 2: temptok.z = (float)atof(token); break;

				case 3: temptok.theta = (float)atof(token); break;

				case 4: temptok.phi = (float)atof(token); break;

				case 5: temptok.extrusion = atoi(token); break;
				}

				token = strtok(NULL, delims);
				token_count++;
			}

			add_point(path, temptok);
		}

		linecount++;
	}

	fclose(fp);

	return 0;
}