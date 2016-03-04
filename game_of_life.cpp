#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <ncurses.h>

using namespace std;

int MAX_SIZE = 56; //50 + 6 na otoczke wokol tablicy, w ten sposob dbam o niewykraczanie poza obszar przy liczeniu i-3, i+3
int start = 3;
int finish = 3;

int rule(int array[3][3]) {
  /*
    Funkcja przyjmuje tablice 3x3, na ktorej srodku (na pozycji (1,1))
    znajduje sie nasz piksel, dla ktorego liczymy sasiedztwo, ktore
    decyduje o tym czy ma przezyc
   */
  int result = 0; 
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 1 and j == 1)
	continue;
      result += array[i][j];
    }
  }//liczymy ilu ma sasiadow
  if (array[1][1] == 0 and result == 3) //jesli martwy i ma 3 sasiadow to wraca jako zombie
    return 1;
  if (array[1][1] == 1 and (result == 2 or result == 3)) //jest zywy i ma 2 lub 3 sasiadow przezywa
    return 1;
  return 0;
}

void calculate(int **array, int size ) {
  // w c++ 11, tablice beda wyzerowane na zero
  int **tmp = new int *[size]; //tablica tymczasowa do przechowywania poprzednich wyliczonych rezultatów
  int rules[3][3]; //nasza podtablica sasiadow dla punktu 
  for (int i = 0; i < size; tmp[i] = new int [size], i++);//inicjalizacja na zera
  for (int i = start; i < size - finish; i++ ) {
    for (int j = start; j < size - finish; j++) {
      //obliczanie zasad dla naszej podtablicy 
      for (int k = i-1; k <= i+1; k++) {
	for (int l = j-1; l <= j+1; l++) {
	  rules[k-i+1][l-j+1] = array[k][l]; //indeksy sa dziwne, ale po prostu, chcemy indeksowac od zera, a mamy spory zapas.
	}
      }
      tmp[i][j] = rule(rules); //przypisanie wyniku reguly
    }
  }
  for (int i = 0; i < size ; i++)
    for (int j = 0; j < size; j++)
      array[i][j] = tmp[i][j]; //przepisanie tablicy kolejnej na terazniejsza

}

void randomize (int **array, int size ) {
  //wylosowanie liczb odpowiednio
  for (int i = start; i < size - finish ; i++){
    for (int j = start; j < size - finish; j++ )
      array[i][j] = rand() % 2;
  }
}

void print_array(int **array, int size) {
  //wyprintowanie tablicy, lacznie z 'okienkiem'
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
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
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
  curs_set(0);
  while (true) {
    ncurses(tmp, MAX_SIZE);
    //std::cout << "---------------------------------------------------------------\n";
    calculate(tmp, MAX_SIZE);
    ncurses(tmp, MAX_SIZE);
    //std::cout << "---------------------------------------------------------------\n";
    cin.get();
  }
  endwin();
  return 0;
}
