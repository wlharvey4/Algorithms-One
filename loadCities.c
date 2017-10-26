/*
 * loadCities.c
 * 2017-10-25
 *
 * Version 0.1_a
 *
 * Load usa115475_cities.txt and usa115475.tsp
 * format for use by Traveling Salesman Problem
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define CITIES "TSP/usa115475.tsp"
#define CITIES_NAMES "TSP/usa115475_cities_b.txt"
#define READ "r"
#define BUFSIZE 100

extern int errno;

struct City {
  char *name;
  char *state;
  double x;
  double y;
};

/********* PROTOTYPES ***********/

/* Find the first line at which cities begins,
 * i.e., skip over header lines */
void
findFirstCity(FILE *);

/* Get the next city line; returns EOF
 * on eof or 1 on successful read */
int
getCity(FILE *, char *);

/* Given a line containing city coordinates,
 * and a line containing a city and state,
 * fill a City struct */
void
fillCity(char *, char *, struct City *);

/* Given a line, extract its coordinates */
double
getCoord(char *);

/* Print a city's name, state, and x and y coordinates */
void
printCityInfo(struct City *);

/********************************/

int
main(int argc, char *argv[]) {
  /* open the two files for reading */
  FILE *cities = fopen(CITIES, READ);
  FILE *cities_names = fopen(CITIES_NAMES, READ);
  if (cities == NULL || cities_names == NULL) {
    fprintf(stderr, "Failed to open cities or cities_names\n");
    exit(EXIT_FAILURE);
  }

  /* position the file pointers to the beginning of the data */
  findFirstCity(cities);
  findFirstCity(cities_names);

  /* read city data and place the city's name, state, and
   * coordinates in a structure */
  char city[BUFSIZE];
  char city_name[BUFSIZE];
  int result;

  // get a city line
  while ((result = getCity(cities, city)) == 1) {
    // get a city name line
    result = getCity(cities_names, city_name);
    if (result != 1) {
      fprintf(stderr, "Error reading cities_names\n");
      exit(EXIT_FAILURE);
    }
  
    struct City *cityStr = malloc(sizeof(struct City));
    fillCity(city, city_name, cityStr);
    printCityInfo(cityStr);
  }

  if (result == -1)
    fprintf(stderr, "Success\n");
  else
    fprintf(stderr, "Error\n");

} /***************************************************/



void
findFirstCity(FILE *fp) {
  char line[BUFSIZE];
  char *rl;
  char c;
  int rc;

  while (1) {
    clearerr(fp);
    c = getc(fp);
    if (c == EOF) {
      if (ferror(fp))
        fprintf(stderr, "Error %d\n", errno);
      else if (feof(fp))
        fprintf(stderr, "Reached eof too early\n");
        
      fprintf(stderr, "Error trying to find the first city (getc)\n");
      exit(EXIT_FAILURE);
    }
    if (isspace(c))
      continue;
    if (isdigit(c)) {
      ungetc(c, fp);
      return;
    }
    /* putchar(c); */
    rl = fgets(line, BUFSIZE, fp);
    /* printf("%s", line); */
    if (rl == NULL) {
      fprintf(stderr, "Error trying to find the first city (fgets)\n");
      exit(EXIT_FAILURE);
    }
  }
}

int
getCity(FILE *fp, char *city) {
  char *rl = fgets(city, BUFSIZE, fp);
  if (rl == NULL) {
    if (feof(fp)) {
      return EOF;
    }
    if (ferror(fp)) {
      fprintf(stderr, "Error reading a city line: %d\n", errno);
      exit(EXIT_FAILURE);
    }
    fprintf(stderr, "Some strange error\n");
    exit(EXIT_FAILURE);
  }
  return 1;
}

void
fillCity(char *city, char *city_name, struct City * city_str) {
  int c, l;
  double x1, y1;
  if ((c = sscanf(city, "%d %lf %lf\n", &l, &x1, &y1)) != 3) {
    fprintf(stderr, "Failed to read three numbers in city\n");
    exit(EXIT_FAILURE);
  }

  char name[BUFSIZE], county[BUFSIZE], state[2];
  double x2, y2;

  if ((c = sscanf(city_name, "%lf %lf %s %s %s", &x2, &y2, name, county, state)) != 5) {
    fprintf(stderr, "Failed to read five numbers in city names; read %d %s\n", c, name);
    exit(EXIT_FAILURE);
  }

  if (fabs(x1 - x2) > .01) {
    fprintf(stderr, "Different coordinates in line %d, city and state: %s, %s\n", l, name, state);
  }

  char *n = malloc(strlen(name));
  char *s = malloc(2);
  strcpy(n, name);
  strcpy(s, state);

  city_str->x = x1;
  city_str->y = y1;
  city_str->name = n;
  city_str->state = s;
}

void
printCityInfo(struct City *city) {
  printf("%s, %s (%f, %f)\n", city->name, city->state, city->x, city->y);
}
