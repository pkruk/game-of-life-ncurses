#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <ncurses.h>

using namespace std;

int MAX_SIZE = 56; //50 + 6 for our 'borders'
int start = 3;
int finish = 3;

int rule(int array[3][3]) {
  /*
    Filter function, which apply proper rules for our Game of life
   */
  int result = 0; 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 1 and j == 1)
	continue;
      result += array[i][j];
    }
  }//liczymy ilu ma sasiadow
  if (array[1][1] == 0 and result == 3) //if is dead, and have 3 neighbours, brick zombie back
    return 1;
  if (array[1][1] == 1 and (result == 2 or result == 3)) //will stay alive if have 2 or 3 neighbours
    return 1;
  return 0;
}

void calculate(int **array, int size ) {
  // c+11, will set it to zero.
  int **tmp = new int *[size]; //tmp table for keeping next step
  int rules[3][3]; //subarray, our filter
  for (int i = 0; i < size; tmp[i] = new int [size], i++);//inicjalizacja na zera
  for (int i = start; i < size - finish; i++ ) {
    for (int j = start; j < size - finish; j++) {
      //calculate subarray
      for (int k = i-1; k <= i+1; k++) {
	for (int l = j-1; l <= j+1; l++) {
	  rules[k-i+1][l-j+1] = array[k][l]; //crazy indexes, because we indexing from zero
	}
      }
      tmp[i][j] = rule(rules); //set a rule
    }
  }
 memcpy(array, tmp, size * size);
}

void randomize (int **array, int size ) {
  //randomize contenr of array
  for (int i = start; i < size - finish ; i++){
    for (int j = start; j < size - finish; j++ )
      array[i][j] = rand() % 2;
  }
}

void print_array(int **array, int size) {
  //console print
  for (int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      if ( array[i][j] == 0)
	std::cout << ".";
      else
	std::cout << "X";
    }
    std::cout << "\n";
  }
}

void ncurses(int **array, int size) {
  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_YELLOW);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      attron(COLOR_PAIR(rand()%3 + 1));
      if (array[i][j] == 0)
	mvaddch(i,j, 10);
      else
	mvaddch(i,j, 46);
    }
  }
  refresh();
}


int main() {
  srand((unsigned) time(0));
  int **tmp = new int *[MAX_SIZE];
  for (int i = 0; i < MAX_SIZE; tmp[i] = new int[MAX_SIZE], i++);
  randomize(tmp, MAX_SIZE);
  initscr();
  start_color();
  curs_set(0);
  while (true) {
    ncurses(tmp, MAX_SIZE);
    calculate(tmp, MAX_SIZE);
    ncurses(tmp, MAX_SIZE);
    cin.get();
  }
  endwin();
  return 0;
}
