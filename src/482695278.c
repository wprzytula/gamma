#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "gamma.h"
#include <stdbool.h>
#include <string.h>


int main() {

/*
scenario: test_random_actions
uuid: 482695278
*/
/*
random actions, total chaos
*/
gamma_t* board = gamma_new(23, 15, 4, 44);
assert( board != NULL );


assert( gamma_move(board, 1, 13, 11) == 1 );
assert( gamma_move(board, 2, 20, 13) == 1 );
assert( gamma_move(board, 3, 13, 8) == 1 );
assert( gamma_move(board, 4, 9, 2) == 1 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 8, 17) == 0 );
assert( gamma_move(board, 1, 16, 10) == 1 );
assert( gamma_move(board, 2, 1, 20) == 0 );
assert( gamma_move(board, 2, 21, 2) == 1 );
assert( gamma_move(board, 3, 11, 20) == 0 );
assert( gamma_move(board, 1, 2, 12) == 1 );
assert( gamma_move(board, 2, 5, 18) == 0 );
assert( gamma_free_fields(board, 2) == 338 );
assert( gamma_move(board, 3, 4, 17) == 0 );
assert( gamma_busy_fields(board, 3) == 1 );
assert( gamma_golden_move(board, 3, 8, 13) == 0 );
assert( gamma_move(board, 4, 0, 8) == 1 );
assert( gamma_move(board, 1, 13, 12) == 1 );
assert( gamma_move(board, 1, 16, 8) == 1 );
assert( gamma_move(board, 2, 3, 8) == 1 );
assert( gamma_move(board, 2, 3, 3) == 1 );
assert( gamma_move(board, 3, 8, 17) == 0 );
assert( gamma_move(board, 3, 20, 12) == 1 );
assert( gamma_move(board, 4, 3, 19) == 0 );
assert( gamma_move(board, 4, 0, 8) == 0 );
assert( gamma_move(board, 1, 0, 13) == 1 );
assert( gamma_move(board, 1, 4, 5) == 1 );
assert( gamma_move(board, 2, 1, 1) == 1 );
assert( gamma_move(board, 3, 20, 3) == 1 );
assert( gamma_move(board, 3, 11, 4) == 1 );
assert( gamma_move(board, 4, 4, 21) == 0 );


char* board326004568 = gamma_board(board);
assert( board326004568 != NULL );
assert( strcmp(board326004568, 
".......................\n"
"1...................2..\n"
"..1..........1......3..\n"
".............1.........\n"
"................1......\n"
".......................\n"
"4..2.........3..1......\n"
".......................\n"
".......................\n"
"....1..................\n"
"...........3...........\n"
"...2................3..\n"
".........4...........2.\n"
".2.....................\n"
".......................\n") == 0);
free(board326004568);
board326004568 = NULL;
assert( gamma_move(board, 1, 13, 11) == 0 );
assert( gamma_move(board, 1, 16, 3) == 1 );
assert( gamma_busy_fields(board, 1) == 8 );
assert( gamma_free_fields(board, 1) == 326 );
assert( gamma_move(board, 2, 1, 2) == 1 );
assert( gamma_move(board, 2, 14, 2) == 1 );


char* board262294050 = gamma_board(board);
assert( board262294050 != NULL );
assert( strcmp(board262294050, 
".......................\n"
"1...................2..\n"
"..1..........1......3..\n"
".............1.........\n"
"................1......\n"
".......................\n"
"4..2.........3..1......\n"
".......................\n"
".......................\n"
"....1..................\n"
"...........3...........\n"
"...2............1...3..\n"
".2.......4....2......2.\n"
".2.....................\n"
".......................\n") == 0);
free(board262294050);
board262294050 = NULL;
assert( gamma_move(board, 3, 7, 2) == 1 );
assert( gamma_move(board, 3, 8, 13) == 1 );
assert( gamma_move(board, 4, 1, 3) == 1 );
assert( gamma_move(board, 4, 10, 14) == 1 );


char* board905395436 = gamma_board(board);
assert( board905395436 != NULL );
assert( strcmp(board905395436, 
"..........4............\n"
"1.......3...........2..\n"
"..1..........1......3..\n"
".............1.........\n"
"................1......\n"
".......................\n"
"4..2.........3..1......\n"
".......................\n"
".......................\n"
"....1..................\n"
"...........3...........\n"
".4.2............1...3..\n"
".2.....3.4....2......2.\n"
".2.....................\n"
".......................\n") == 0);
free(board905395436);
board905395436 = NULL;
assert( gamma_move(board, 1, 1, 15) == 0 );
assert( gamma_move(board, 1, 12, 8) == 1 );
assert( gamma_move(board, 2, 4, 2) == 1 );
assert( gamma_busy_fields(board, 2) == 8 );
assert( gamma_move(board, 3, 3, 19) == 0 );
assert( gamma_move(board, 3, 1, 0) == 1 );
assert( gamma_free_fields(board, 3) == 317 );
assert( gamma_move(board, 4, 12, 14) == 1 );
assert( gamma_move(board, 4, 8, 2) == 1 );
assert( gamma_move(board, 1, 5, 0) == 1 );
assert( gamma_move(board, 2, 3, 9) == 1 );
assert( gamma_move(board, 2, 5, 0) == 0 );
assert( gamma_golden_move(board, 2, 14, 10) == 0 );
assert( gamma_move(board, 3, 10, 15) == 0 );
assert( gamma_move(board, 3, 20, 6) == 1 );
assert( gamma_move(board, 4, 12, 22) == 0 );
assert( gamma_move(board, 1, 14, 5) == 1 );
assert( gamma_move(board, 2, 2, 11) == 1 );
assert( gamma_busy_fields(board, 2) == 10 );
assert( gamma_golden_possible(board, 2) == 1 );
assert( gamma_move(board, 3, 7, 1) == 1 );
assert( gamma_move(board, 3, 15, 14) == 1 );
assert( gamma_move(board, 4, 10, 4) == 1 );
assert( gamma_move(board, 4, 13, 1) == 1 );
assert( gamma_move(board, 2, 10, 9) == 1 );
assert( gamma_move(board, 2, 16, 12) == 1 );
assert( gamma_golden_possible(board, 2) == 1 );
assert( gamma_golden_move(board, 2, 3, 1) == 0 );
assert( gamma_move(board, 3, 0, 4) == 1 );
assert( gamma_move(board, 3, 3, 12) == 1 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 7, 16) == 0 );
assert( gamma_busy_fields(board, 4) == 8 );
assert( gamma_move(board, 1, 13, 6) == 1 );
assert( gamma_move(board, 1, 18, 9) == 1 );
assert( gamma_move(board, 2, 5, 1) == 1 );
assert( gamma_golden_possible(board, 2) == 1 );
assert( gamma_move(board, 3, 1, 4) == 1 );
assert( gamma_move(board, 3, 0, 8) == 0 );
assert( gamma_move(board, 4, 10, 10) == 1 );
assert( gamma_move(board, 4, 10, 12) == 1 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 8, 10) == 1 );
assert( gamma_move(board, 1, 16, 14) == 1 );
assert( gamma_move(board, 2, 13, 17) == 0 );
assert( gamma_move(board, 3, 9, 3) == 1 );
assert( gamma_move(board, 4, 13, 2) == 1 );
assert( gamma_move(board, 4, 4, 14) == 1 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 10, 13) == 1 );
assert( gamma_move(board, 1, 10, 3) == 1 );
assert( gamma_move(board, 2, 10, 12) == 0 );
assert( gamma_move(board, 2, 18, 8) == 1 );
assert( gamma_move(board, 3, 2, 16) == 0 );
assert( gamma_move(board, 3, 21, 1) == 1 );
assert( gamma_move(board, 4, 5, 12) == 1 );
assert( gamma_move(board, 1, 3, 19) == 0 );
assert( gamma_move(board, 2, 15, 13) == 1 );
assert( gamma_golden_move(board, 2, 4, 0) == 0 );
assert( gamma_move(board, 3, 1, 22) == 0 );
assert( gamma_move(board, 3, 8, 3) == 1 );
assert( gamma_free_fields(board, 3) == 284 );
assert( gamma_golden_move(board, 3, 2, 4) == 0 );
assert( gamma_move(board, 4, 9, 1) == 1 );
assert( gamma_move(board, 1, 11, 7) == 1 );
assert( gamma_move(board, 1, 16, 0) == 1 );
assert( gamma_golden_move(board, 1, 14, 15) == 0 );
assert( gamma_move(board, 2, 12, 7) == 1 );
assert( gamma_move(board, 2, 0, 14) == 1 );
assert( gamma_move(board, 3, 5, 10) == 1 );
assert( gamma_move(board, 3, 7, 10) == 1 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 4, 6) == 1 );
assert( gamma_move(board, 4, 10, 10) == 0 );
assert( gamma_move(board, 1, 4, 4) == 1 );
assert( gamma_move(board, 1, 1, 0) == 0 );
assert( gamma_move(board, 2, 16, 6) == 1 );


char* board753383446 = gamma_board(board);
assert( board753383446 != NULL );
assert( strcmp(board753383446, 
"2...4.....4.4..31......\n"
"1.......3.1....2....2..\n"
"..13.4....4..1..2...3..\n"
"..2..........1.........\n"
".....3.31.4.....1......\n"
"...2......2.......1....\n"
"4..2........13..1.2....\n"
"...........12..........\n"
"....4........1..2...3..\n"
"....1.........1........\n"
"33..1.....43...........\n"
".4.2....331.....1...3..\n"
".2..2..344...42......2.\n"
".2...2.3.4...4.......3.\n"
".3...1..........1......\n") == 0);
free(board753383446);
board753383446 = NULL;
assert( gamma_move(board, 3, 2, 1) == 1 );
assert( gamma_busy_fields(board, 3) == 19 );
assert( gamma_move(board, 4, 7, 10) == 0 );
assert( gamma_golden_move(board, 4, 12, 2) == 0 );
assert( gamma_move(board, 1, 4, 5) == 0 );
assert( gamma_move(board, 1, 5, 1) == 0 );
assert( gamma_move(board, 2, 21, 13) == 1 );
assert( gamma_move(board, 2, 1, 12) == 1 );
assert( gamma_move(board, 3, 4, 4) == 0 );
assert( gamma_move(board, 3, 9, 12) == 1 );
assert( gamma_move(board, 4, 6, 19) == 0 );
assert( gamma_move(board, 1, 11, 11) == 1 );
assert( gamma_move(board, 2, 11, 0) == 1 );
assert( gamma_move(board, 2, 9, 2) == 0 );
assert( gamma_move(board, 3, 16, 2) == 1 );
assert( gamma_move(board, 3, 8, 7) == 1 );
assert( gamma_move(board, 4, 11, 13) == 1 );


char* board612490298 = gamma_board(board);
assert( board612490298 != NULL );
assert( strcmp(board612490298, 
"2...4.....4.4..31......\n"
"1.......3.14...2....22.\n"
".213.4...34..1..2...3..\n"
"..2........1.1.........\n"
".....3.31.4.....1......\n"
"...2......2.......1....\n"
"4..2........13..1.2....\n"
"........3..12..........\n"
"....4........1..2...3..\n"
"....1.........1........\n"
"33..1.....43...........\n"
".4.2....331.....1...3..\n"
".2..2..344...42.3....2.\n"
".23..2.3.4...4.......3.\n"
".3...1.....2....1......\n") == 0);
free(board612490298);
board612490298 = NULL;
assert( gamma_move(board, 1, 11, 6) == 1 );
assert( gamma_move(board, 1, 1, 11) == 1 );
assert( gamma_golden_move(board, 1, 2, 13) == 0 );
assert( gamma_move(board, 2, 22, 0) == 1 );
assert( gamma_golden_possible(board, 2) == 1 );
assert( gamma_move(board, 3, 3, 21) == 0 );
assert( gamma_move(board, 3, 21, 11) == 1 );
assert( gamma_busy_fields(board, 3) == 23 );
assert( gamma_move(board, 4, 5, 2) == 1 );
assert( gamma_move(board, 1, 6, 1) == 1 );
assert( gamma_move(board, 1, 8, 12) == 1 );
assert( gamma_golden_possible(board, 1) == 1 );
assert( gamma_move(board, 2, 5, 18) == 0 );
assert( gamma_move(board, 3, 8, 15) == 0 );
assert( gamma_move(board, 4, 7, 21) == 0 );
assert( gamma_move(board, 1, 9, 6) == 1 );
assert( gamma_move(board, 1, 21, 8) == 1 );
assert( gamma_move(board, 2, 15, 12) == 1 );
assert( gamma_busy_fields(board, 2) == 23 );
assert( gamma_move(board, 3, 16, 5) == 1 );
assert( gamma_move(board, 3, 14, 13) == 1 );
assert( gamma_golden_move(board, 3, 10, 16) == 0 );
assert( gamma_move(board, 4, 7, 6) == 1 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 13, 16) == 0 );
assert( gamma_move(board, 2, 4, 3) == 1 );
assert( gamma_move(board, 2, 16, 13) == 1 );


char* board862171768 = gamma_board(board);
assert( board862171768 != NULL );
assert( strcmp(board862171768, 
"2...4.....4.4..31......\n"
"1.......3.14..322...22.\n"
".213.4..134..1.22...3..\n"
".12........1.1.......3.\n"
".....3.31.4.....1......\n"
"...2......2.......1....\n"
"4..2........13..1.2..1.\n"
"........3..12..........\n"
"....4..4.1.1.1..2...3..\n"
"....1.........1.3......\n"
"33..1.....43...........\n"
".4.22...331.....1...3..\n"
".2..24.344...42.3....2.\n"
".23..213.4...4.......3.\n"
".3...1.....2....1.....2\n") == 0);
free(board862171768);
board862171768 = NULL;
assert( gamma_move(board, 3, 13, 6) == 0 );
assert( gamma_move(board, 4, 2, 6) == 1 );
assert( gamma_move(board, 1, 2, 2) == 1 );
assert( gamma_move(board, 2, 13, 5) == 1 );
assert( gamma_move(board, 3, 3, 12) == 0 );
assert( gamma_move(board, 3, 22, 5) == 1 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 18, 10) == 1 );
assert( gamma_move(board, 1, 14, 20) == 0 );
assert( gamma_move(board, 2, 13, 18) == 0 );
assert( gamma_busy_fields(board, 3) == 26 );
assert( gamma_move(board, 4, 3, 12) == 0 );
assert( gamma_move(board, 4, 10, 6) == 1 );
assert( gamma_move(board, 1, 10, 11) == 1 );
assert( gamma_move(board, 1, 0, 10) == 1 );
assert( gamma_busy_fields(board, 1) == 30 );
assert( gamma_move(board, 2, 0, 13) == 0 );
assert( gamma_move(board, 2, 14, 0) == 1 );
assert( gamma_free_fields(board, 2) == 241 );


char* board601289008 = gamma_board(board);
assert( board601289008 != NULL );
assert( strcmp(board601289008, 
"2...4.....4.4..31......\n"
"1.......3.14..322...22.\n"
".213.4..134..1.22...3..\n"
".12.......11.1.......3.\n"
"1....3.31.4.....1.4....\n"
"...2......2.......1....\n"
"4..2........13..1.2..1.\n"
"........3..12..........\n"
"..4.4..4.141.1..2...3..\n"
"....1........21.3.....3\n"
"33..1.....43...........\n"
".4.22...331.....1...3..\n"
".21.24.344...42.3....2.\n"
".23..213.4...4.......3.\n"
".3...1.....2..2.1.....2\n") == 0);
free(board601289008);
board601289008 = NULL;
assert( gamma_move(board, 3, 10, 4) == 0 );
assert( gamma_move(board, 3, 20, 13) == 0 );
assert( gamma_free_fields(board, 3) == 241 );
assert( gamma_move(board, 4, 5, 14) == 1 );
assert( gamma_move(board, 1, 8, 17) == 0 );
assert( gamma_move(board, 2, 13, 18) == 0 );
assert( gamma_move(board, 3, 11, 18) == 0 );
assert( gamma_move(board, 4, 10, 1) == 1 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 3, 13) == 1 );
assert( gamma_move(board, 1, 6, 6) == 1 );
assert( gamma_free_fields(board, 1) == 237 );
assert( gamma_move(board, 3, 10, 1) == 0 );
assert( gamma_move(board, 4, 11, 14) == 1 );
assert( gamma_move(board, 4, 0, 4) == 0 );
assert( gamma_busy_fields(board, 4) == 24 );
assert( gamma_move(board, 1, 22, 8) == 1 );
assert( gamma_move(board, 1, 18, 8) == 0 );
assert( gamma_move(board, 2, 2, 11) == 0 );
assert( gamma_move(board, 2, 22, 5) == 0 );
assert( gamma_free_fields(board, 2) == 235 );
assert( gamma_move(board, 3, 18, 9) == 0 );
assert( gamma_move(board, 4, 9, 15) == 0 );
assert( gamma_move(board, 4, 1, 9) == 1 );
assert( gamma_move(board, 1, 8, 4) == 1 );
assert( gamma_free_fields(board, 1) == 233 );
assert( gamma_move(board, 2, 20, 10) == 1 );
assert( gamma_move(board, 2, 17, 2) == 1 );
assert( gamma_busy_fields(board, 2) == 29 );
assert( gamma_move(board, 3, 1, 20) == 0 );
assert( gamma_move(board, 3, 8, 6) == 1 );
assert( gamma_busy_fields(board, 3) == 27 );
assert( gamma_move(board, 4, 4, 12) == 1 );
assert( gamma_move(board, 4, 8, 12) == 0 );
assert( gamma_golden_move(board, 4, 6, 11) == 0 );
assert( gamma_move(board, 1, 20, 14) == 1 );


char* board790108340 = gamma_board(board);
assert( board790108340 != NULL );
assert( strcmp(board790108340, 
"2...44....444..31...1..\n"
"1..1....3.14..322...22.\n"
".21344..134..1.22...3..\n"
".12.......11.1.......3.\n"
"1....3.31.4.....1.4.2..\n"
".4.2......2.......1....\n"
"4..2........13..1.2..11\n"
"........3..12..........\n"
"..4.4.143141.1..2...3..\n"
"....1........21.3.....3\n"
"33..1...1.43...........\n"
".4.22...331.....1...3..\n"
".21.24.344...42.32...2.\n"
".23..213.44..4.......3.\n"
".3...1.....2..2.1.....2\n") == 0);
free(board790108340);
board790108340 = NULL;
assert( gamma_move(board, 2, 8, 19) == 0 );


char* board475848698 = gamma_board(board);
assert( board475848698 != NULL );
assert( strcmp(board475848698, 
"2...44....444..31...1..\n"
"1..1....3.14..322...22.\n"
".21344..134..1.22...3..\n"
".12.......11.1.......3.\n"
"1....3.31.4.....1.4.2..\n"
".4.2......2.......1....\n"
"4..2........13..1.2..11\n"
"........3..12..........\n"
"..4.4.143141.1..2...3..\n"
"....1........21.3.....3\n"
"33..1...1.43...........\n"
".4.22...331.....1...3..\n"
".21.24.344...42.32...2.\n"
".23..213.44..4.......3.\n"
".3...1.....2..2.1.....2\n") == 0);
free(board475848698);
board475848698 = NULL;
assert( gamma_move(board, 3, 15, 8) == 1 );
assert( gamma_move(board, 3, 16, 13) == 0 );
assert( gamma_move(board, 4, 8, 5) == 1 );
assert( gamma_move(board, 1, 9, 17) == 0 );
assert( gamma_move(board, 1, 18, 4) == 1 );
assert( gamma_move(board, 2, 10, 15) == 0 );
assert( gamma_move(board, 2, 21, 11) == 0 );
assert( gamma_busy_fields(board, 2) == 29 );
assert( gamma_move(board, 3, 4, 6) == 0 );
assert( gamma_move(board, 3, 9, 6) == 0 );
assert( gamma_busy_fields(board, 3) == 28 );
assert( gamma_move(board, 4, 2, 15) == 0 );
assert( gamma_move(board, 4, 20, 2) == 1 );
assert( gamma_move(board, 1, 12, 14) == 0 );
assert( gamma_move(board, 1, 14, 4) == 1 );
assert( gamma_move(board, 2, 6, 2) == 1 );
assert( gamma_free_fields(board, 2) == 222 );
assert( gamma_golden_possible(board, 2) == 1 );
assert( gamma_move(board, 3, 13, 9) == 1 );
assert( gamma_move(board, 3, 14, 8) == 1 );
assert( gamma_move(board, 4, 1, 4) == 0 );
assert( gamma_move(board, 4, 3, 3) == 0 );
assert( gamma_move(board, 1, 18, 5) == 1 );
assert( gamma_move(board, 1, 8, 3) == 0 );
assert( gamma_move(board, 2, 0, 10) == 0 );
assert( gamma_move(board, 2, 17, 1) == 1 );
assert( gamma_busy_fields(board, 2) == 31 );
assert( gamma_move(board, 4, 18, 7) == 1 );
assert( gamma_move(board, 1, 0, 10) == 0 );
assert( gamma_move(board, 1, 3, 9) == 0 );
assert( gamma_move(board, 2, 15, 6) == 1 );
assert( gamma_busy_fields(board, 2) == 32 );
assert( gamma_move(board, 3, 5, 5) == 1 );
assert( gamma_move(board, 3, 9, 13) == 1 );
assert( gamma_busy_fields(board, 3) == 32 );
assert( gamma_move(board, 4, 18, 10) == 0 );
assert( gamma_move(board, 4, 3, 3) == 0 );
assert( gamma_move(board, 1, 9, 22) == 0 );
assert( gamma_busy_fields(board, 1) == 38 );
assert( gamma_move(board, 2, 11, 9) == 1 );
assert( gamma_move(board, 2, 13, 9) == 0 );
assert( gamma_golden_move(board, 2, 8, 15) == 0 );
assert( gamma_move(board, 3, 6, 12) == 1 );
assert( gamma_move(board, 3, 3, 7) == 1 );
assert( gamma_move(board, 4, 20, 8) == 1 );
assert( gamma_move(board, 4, 20, 11) == 1 );
assert( gamma_move(board, 1, 9, 12) == 0 );
assert( gamma_move(board, 2, 5, 20) == 0 );
assert( gamma_move(board, 2, 18, 8) == 0 );
assert( gamma_move(board, 3, 8, 2) == 0 );
assert( gamma_move(board, 3, 10, 11) == 0 );
assert( gamma_move(board, 4, 4, 19) == 0 );
assert( gamma_move(board, 1, 11, 6) == 0 );
assert( gamma_golden_possible(board, 1) == 1 );
assert( gamma_move(board, 2, 7, 5) == 1 );
assert( gamma_move(board, 2, 20, 12) == 0 );
assert( gamma_move(board, 3, 13, 2) == 0 );
assert( gamma_move(board, 4, 15, 4) == 1 );
assert( gamma_move(board, 1, 7, 16) == 0 );
assert( gamma_golden_possible(board, 1) == 1 );
assert( gamma_move(board, 2, 11, 5) == 1 );
assert( gamma_free_fields(board, 2) == 206 );
assert( gamma_move(board, 3, 6, 22) == 0 );
assert( gamma_move(board, 3, 4, 6) == 0 );
assert( gamma_move(board, 4, 2, 15) == 0 );
assert( gamma_move(board, 1, 5, 20) == 0 );
assert( gamma_move(board, 2, 11, 4) == 0 );
assert( gamma_move(board, 3, 11, 22) == 0 );
assert( gamma_move(board, 4, 1, 7) == 1 );
assert( gamma_move(board, 1, 14, 18) == 0 );
assert( gamma_move(board, 1, 9, 0) == 1 );
assert( gamma_move(board, 2, 8, 1) == 1 );
assert( gamma_move(board, 3, 12, 7) == 0 );
assert( gamma_move(board, 3, 2, 8) == 1 );
assert( gamma_busy_fields(board, 3) == 35 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 5, 2) == 0 );
assert( gamma_move(board, 4, 14, 5) == 0 );
assert( gamma_move(board, 1, 0, 9) == 1 );
assert( gamma_move(board, 1, 19, 11) == 1 );
assert( gamma_move(board, 2, 10, 1) == 0 );
assert( gamma_move(board, 2, 16, 13) == 0 );
assert( gamma_move(board, 3, 2, 6) == 0 );
assert( gamma_move(board, 3, 5, 8) == 1 );
assert( gamma_busy_fields(board, 4) == 33 );
assert( gamma_move(board, 1, 9, 13) == 0 );
assert( gamma_busy_fields(board, 1) == 41 );
assert( gamma_move(board, 2, 0, 10) == 0 );


char* board134165639 = gamma_board(board);
assert( board134165639 != NULL );
assert( strcmp(board134165639, 
"2...44....444..31...1..\n"
"1..1....3314..322...22.\n"
".213443.134..1.22...3..\n"
".12.......11.1.....143.\n"
"1....3.31.4.....1.4.2..\n"
"14.2......22.3....1....\n"
"4.32.3......13331.2.411\n"
".4.3....3..12.....4....\n"
"..4.4.143141.1.22...3..\n"
"....13.24..2.21.3.1...3\n"
"33..1...1.43..14..1....\n"
".4.22...331.....1...3..\n"
".21.242344...42.32..42.\n"
".23..213244..4...2...3.\n"
".3...1...1.2..2.1.....2\n") == 0);
free(board134165639);
board134165639 = NULL;
assert( gamma_move(board, 3, 3, 7) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 9, 0) == 0 );
assert( gamma_move(board, 1, 1, 8) == 1 );
assert( gamma_move(board, 2, 0, 18) == 0 );
assert( gamma_move(board, 2, 21, 5) == 1 );
assert( gamma_free_fields(board, 2) == 197 );
assert( gamma_move(board, 3, 0, 11) == 1 );
assert( gamma_move(board, 3, 0, 3) == 1 );
assert( gamma_move(board, 4, 2, 10) == 1 );
assert( gamma_move(board, 1, 5, 10) == 0 );
assert( gamma_free_fields(board, 1) == 194 );
assert( gamma_move(board, 2, 6, 0) == 1 );
assert( gamma_move(board, 2, 10, 8) == 1 );
assert( gamma_busy_fields(board, 2) == 39 );
assert( gamma_move(board, 3, 6, 3) == 1 );
assert( gamma_busy_fields(board, 4) == 34 );
assert( gamma_move(board, 1, 14, 14) == 1 );
assert( gamma_move(board, 1, 4, 9) == 1 );
assert( gamma_move(board, 2, 10, 6) == 0 );
assert( gamma_golden_possible(board, 2) == 1 );
assert( gamma_move(board, 3, 4, 4) == 0 );
assert( gamma_move(board, 3, 20, 6) == 0 );
assert( gamma_busy_fields(board, 3) == 39 );
assert( gamma_move(board, 4, 10, 9) == 0 );
assert( gamma_move(board, 4, 9, 1) == 0 );
assert( gamma_move(board, 1, 0, 10) == 0 );
assert( gamma_move(board, 1, 7, 2) == 0 );
assert( gamma_move(board, 2, 0, 20) == 0 );
assert( gamma_move(board, 3, 1, 0) == 0 );
assert( gamma_move(board, 3, 13, 11) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 10, 21) == 0 );
assert( gamma_move(board, 1, 6, 14) == 1 );
assert( gamma_free_fields(board, 1) == 188 );
assert( gamma_golden_move(board, 1, 13, 8) == 1 );
assert( gamma_move(board, 2, 13, 13) == 1 );
assert( gamma_move(board, 2, 4, 1) == 1 );
assert( gamma_golden_move(board, 2, 13, 11) == 1 );
assert( gamma_move(board, 3, 3, 17) == 0 );
assert( gamma_move(board, 4, 5, 9) == 1 );
assert( gamma_move(board, 1, 16, 11) == 1 );
assert( gamma_move(board, 2, 8, 8) == 1 );
assert( gamma_move(board, 4, 8, 11) == 1 );
assert( gamma_move(board, 1, 15, 9) == 1 );
assert( gamma_move(board, 2, 2, 0) == 1 );
assert( gamma_move(board, 2, 17, 1) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 7, 9) == 1 );
assert( gamma_move(board, 4, 22, 11) == 1 );
assert( gamma_move(board, 4, 0, 0) == 1 );
assert( gamma_move(board, 1, 12, 18) == 0 );
assert( gamma_move(board, 2, 14, 7) == 1 );
assert( gamma_move(board, 2, 0, 11) == 0 );
assert( gamma_move(board, 3, 13, 22) == 0 );
assert( gamma_move(board, 4, 14, 13) == 0 );
assert( gamma_move(board, 4, 21, 1) == 0 );
assert( gamma_busy_fields(board, 1) == 47 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 11, 14) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 3, 21) == 0 );
assert( gamma_move(board, 3, 4, 12) == 0 );
assert( gamma_move(board, 4, 4, 9) == 0 );
assert( gamma_move(board, 1, 21, 4) == 1 );
assert( gamma_move(board, 1, 9, 2) == 0 );
assert( gamma_golden_move(board, 2, 13, 11) == 0 );
assert( gamma_move(board, 4, 5, 19) == 0 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 6, 14) == 0 );
assert( gamma_move(board, 1, 16, 4) == 1 );
assert( gamma_busy_fields(board, 1) == 49 );
assert( gamma_move(board, 2, 4, 9) == 0 );
assert( gamma_move(board, 3, 14, 3) == 1 );
assert( gamma_move(board, 4, 3, 15) == 0 );
assert( gamma_free_fields(board, 4) == 173 );
assert( gamma_move(board, 1, 8, 17) == 0 );
assert( gamma_move(board, 2, 19, 13) == 1 );
assert( gamma_move(board, 2, 0, 9) == 0 );
assert( gamma_move(board, 3, 14, 9) == 1 );
assert( gamma_busy_fields(board, 3) == 41 );
assert( gamma_move(board, 4, 17, 5) == 1 );
assert( gamma_free_fields(board, 4) == 170 );
assert( gamma_move(board, 1, 9, 21) == 0 );
assert( gamma_move(board, 1, 2, 6) == 0 );
assert( gamma_free_fields(board, 1) == 170 );
assert( gamma_move(board, 2, 10, 4) == 0 );
assert( gamma_move(board, 4, 5, 15) == 0 );
assert( gamma_move(board, 4, 6, 3) == 0 );
assert( gamma_move(board, 1, 9, 22) == 0 );
assert( gamma_move(board, 1, 14, 13) == 0 );
assert( gamma_busy_fields(board, 1) == 49 );
assert( gamma_move(board, 2, 21, 4) == 0 );
assert( gamma_move(board, 3, 14, 7) == 0 );
assert( gamma_golden_move(board, 3, 12, 5) == 0 );
assert( gamma_move(board, 4, 3, 12) == 0 );
assert( gamma_move(board, 1, 11, 1) == 1 );
assert( gamma_move(board, 2, 6, 3) == 0 );
assert( gamma_move(board, 3, 15, 0) == 1 );
assert( gamma_move(board, 3, 16, 3) == 0 );
assert( gamma_free_fields(board, 3) == 168 );
assert( gamma_move(board, 4, 14, 14) == 0 );
assert( gamma_move(board, 4, 18, 11) == 1 );
assert( gamma_move(board, 1, 6, 17) == 0 );
assert( gamma_move(board, 2, 14, 21) == 0 );
assert( gamma_move(board, 2, 13, 10) == 1 );
assert( gamma_move(board, 3, 12, 7) == 0 );
assert( gamma_move(board, 4, 17, 14) == 1 );
assert( gamma_move(board, 4, 13, 0) == 1 );
assert( gamma_move(board, 1, 6, 0) == 0 );
assert( gamma_move(board, 2, 5, 9) == 0 );
assert( gamma_move(board, 3, 10, 22) == 0 );
assert( gamma_move(board, 3, 9, 11) == 1 );
assert( gamma_move(board, 4, 19, 7) == 1 );
assert( gamma_move(board, 1, 7, 7) == 1 );
assert( gamma_move(board, 1, 22, 4) == 1 );
assert( gamma_move(board, 2, 8, 6) == 0 );
assert( gamma_move(board, 2, 2, 6) == 0 );
assert( gamma_move(board, 3, 5, 15) == 0 );
assert( gamma_free_fields(board, 3) == 160 );
assert( gamma_move(board, 4, 8, 7) == 0 );
assert( gamma_move(board, 1, 0, 10) == 0 );
assert( gamma_move(board, 1, 12, 6) == 1 );
assert( gamma_busy_fields(board, 1) == 53 );
assert( gamma_move(board, 3, 0, 11) == 0 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 22, 1) == 1 );
assert( gamma_move(board, 2, 9, 9) == 1 );
assert( gamma_golden_move(board, 2, 3, 0) == 0 );
assert( gamma_move(board, 3, 0, 10) == 0 );
assert( gamma_move(board, 3, 6, 4) == 1 );
assert( gamma_move(board, 1, 3, 17) == 0 );
assert( gamma_move(board, 2, 0, 12) == 1 );
assert( gamma_move(board, 2, 3, 9) == 0 );
assert( gamma_move(board, 3, 8, 3) == 0 );
assert( gamma_move(board, 3, 15, 9) == 0 );
assert( gamma_golden_move(board, 4, 2, 17) == 0 );
assert( gamma_move(board, 1, 16, 0) == 0 );
assert( gamma_golden_move(board, 1, 4, 0) == 0 );
assert( gamma_move(board, 2, 22, 2) == 1 );
assert( gamma_move(board, 2, 18, 0) == 1 );
assert( gamma_move(board, 3, 3, 5) == 1 );


char* board395826200 = gamma_board(board);
assert( board395826200 != NULL );
assert( strcmp(board395826200, 
"2...441...444.1314..1..\n"
"1..1....3314.2322..222.\n"
"2213443.134..1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.4..2..1.4.2..\n"
"14.214.3.222.331..1....\n"
"4132.3..2.2.11331.2.411\n"
".4.3...13..12.2...44...\n"
"..4.4.14314111.22...3..\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1...3..\n"
".21.242344...42.32..422\n"
".23.22132441.4...2...31\n"
"432..12..1.2.4231.2...2\n") == 0);
free(board395826200);
board395826200 = NULL;
assert( gamma_move(board, 1, 11, 12) == 1 );
assert( gamma_move(board, 1, 19, 11) == 0 );
assert( gamma_move(board, 2, 6, 17) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 6, 22) == 0 );
assert( gamma_move(board, 3, 1, 11) == 0 );
assert( gamma_move(board, 4, 0, 10) == 0 );
assert( gamma_free_fields(board, 4) == 151 );
assert( gamma_move(board, 1, 4, 0) == 1 );


char* board299163889 = gamma_board(board);
assert( board299163889 != NULL );
assert( strcmp(board299163889, 
"2...441...444.1314..1..\n"
"1..1....3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.4..2..1.4.2..\n"
"14.214.3.222.331..1....\n"
"4132.3..2.2.11331.2.411\n"
".4.3...13..12.2...44...\n"
"..4.4.14314111.22...3..\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1...3..\n"
".21.242344...42.32..422\n"
".23.22132441.4...2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board299163889);
board299163889 = NULL;
assert( gamma_move(board, 2, 10, 3) == 0 );
assert( gamma_move(board, 2, 15, 1) == 1 );
assert( gamma_move(board, 3, 14, 3) == 0 );
assert( gamma_move(board, 4, 0, 12) == 0 );
assert( gamma_move(board, 4, 19, 3) == 1 );
assert( gamma_move(board, 1, 0, 9) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 10, 21) == 0 );
assert( gamma_move(board, 3, 11, 5) == 0 );
assert( gamma_move(board, 4, 10, 1) == 0 );
assert( gamma_move(board, 4, 20, 3) == 0 );
assert( gamma_free_fields(board, 4) == 148 );
assert( gamma_move(board, 1, 7, 15) == 0 );
assert( gamma_move(board, 2, 3, 2) == 1 );
assert( gamma_busy_fields(board, 2) == 53 );
assert( gamma_move(board, 3, 20, 9) == 1 );
assert( gamma_busy_fields(board, 3) == 46 );
assert( gamma_move(board, 4, 11, 10) == 1 );
assert( gamma_move(board, 4, 17, 10) == 1 );
assert( gamma_move(board, 1, 4, 13) == 1 );
assert( gamma_move(board, 1, 22, 0) == 0 );


char* board200697173 = gamma_board(board);
assert( board200697173 != NULL );
assert( strcmp(board200697173, 
"2...441...444.1314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".4.3...13..12.2...44...\n"
"..4.4.14314111.22...3..\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board200697173);
board200697173 = NULL;
assert( gamma_move(board, 2, 0, 20) == 0 );
assert( gamma_move(board, 2, 12, 14) == 0 );
assert( gamma_move(board, 3, 11, 12) == 0 );
assert( gamma_move(board, 4, 8, 11) == 0 );
assert( gamma_move(board, 4, 5, 10) == 0 );
assert( gamma_move(board, 1, 3, 13) == 0 );
assert( gamma_busy_fields(board, 1) == 57 );
assert( gamma_move(board, 2, 22, 6) == 1 );
assert( gamma_move(board, 3, 13, 11) == 0 );
assert( gamma_move(board, 4, 3, 14) == 1 );
assert( gamma_move(board, 4, 9, 2) == 0 );


char* board792833029 = gamma_board(board);
assert( board792833029 != NULL );
assert( strcmp(board792833029, 
"2..4441...444.1314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".4.3...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board792833029);
board792833029 = NULL;
assert( gamma_move(board, 1, 11, 17) == 0 );
assert( gamma_move(board, 2, 10, 12) == 0 );
assert( gamma_move(board, 3, 7, 15) == 0 );


char* board109272576 = gamma_board(board);
assert( board109272576 != NULL );
assert( strcmp(board109272576, 
"2..4441...444.1314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".4.3...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board109272576);
board109272576 = NULL;
assert( gamma_move(board, 4, 12, 8) == 0 );
assert( gamma_free_fields(board, 4) == 141 );
assert( gamma_move(board, 1, 6, 1) == 0 );
assert( gamma_move(board, 2, 2, 7) == 1 );
assert( gamma_free_fields(board, 2) == 140 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 7, 17) == 0 );
assert( gamma_move(board, 4, 9, 2) == 0 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_free_fields(board, 1) == 140 );


char* board721948812 = gamma_board(board);
assert( board721948812 != NULL );
assert( strcmp(board721948812, 
"2..4441...444.1314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board721948812);
board721948812 = NULL;
assert( gamma_move(board, 2, 4, 17) == 0 );
assert( gamma_move(board, 3, 0, 20) == 0 );
assert( gamma_free_fields(board, 3) == 140 );
assert( gamma_move(board, 4, 14, 5) == 0 );
assert( gamma_golden_possible(board, 4) == 1 );


char* board654616654 = gamma_board(board);
assert( board654616654 != NULL );
assert( strcmp(board654616654, 
"2..4441...444.1314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...3.1..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board654616654);
board654616654 = NULL;
assert( gamma_move(board, 1, 13, 12) == 0 );
assert( gamma_move(board, 1, 15, 3) == 1 );
assert( gamma_move(board, 2, 11, 5) == 0 );
assert( gamma_free_fields(board, 3) == 139 );
assert( gamma_move(board, 4, 8, 7) == 0 );
assert( gamma_move(board, 4, 15, 3) == 0 );
assert( gamma_free_fields(board, 4) == 139 );
assert( gamma_golden_possible(board, 4) == 1 );


char* board153984665 = gamma_board(board);
assert( board153984665 != NULL );
assert( strcmp(board153984665, 
"2..4441...444.1314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312.....4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board153984665);
board153984665 = NULL;
assert( gamma_move(board, 1, 17, 2) == 0 );
assert( gamma_busy_fields(board, 1) == 58 );
assert( gamma_move(board, 2, 14, 13) == 0 );
assert( gamma_move(board, 2, 6, 11) == 1 );
assert( gamma_golden_move(board, 2, 2, 2) == 0 );
assert( gamma_move(board, 4, 2, 12) == 0 );
assert( gamma_move(board, 1, 5, 10) == 0 );
assert( gamma_move(board, 1, 19, 7) == 0 );
assert( gamma_move(board, 2, 3, 12) == 0 );
assert( gamma_move(board, 3, 0, 8) == 0 );
assert( gamma_move(board, 3, 9, 13) == 0 );
assert( gamma_move(board, 4, 1, 0) == 0 );
assert( gamma_move(board, 4, 15, 4) == 0 );
assert( gamma_free_fields(board, 4) == 138 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 17, 1) == 0 );
assert( gamma_busy_fields(board, 1) == 58 );
assert( gamma_move(board, 2, 4, 13) == 0 );
assert( gamma_move(board, 2, 16, 5) == 0 );
assert( gamma_move(board, 3, 3, 22) == 0 );
assert( gamma_move(board, 3, 15, 3) == 0 );
assert( gamma_move(board, 4, 10, 1) == 0 );
assert( gamma_move(board, 4, 1, 7) == 0 );
assert( gamma_move(board, 1, 2, 0) == 0 );
assert( gamma_move(board, 1, 11, 12) == 0 );
assert( gamma_golden_move(board, 1, 13, 13) == 0 );
assert( gamma_move(board, 3, 8, 11) == 0 );
assert( gamma_move(board, 4, 7, 9) == 0 );
assert( gamma_move(board, 1, 6, 6) == 0 );
assert( gamma_move(board, 2, 5, 1) == 0 );
assert( gamma_move(board, 3, 6, 0) == 0 );
assert( gamma_move(board, 4, 10, 9) == 0 );
assert( gamma_move(board, 4, 13, 14) == 1 );


char* board216049510 = gamma_board(board);
assert( board216049510 != NULL );
assert( strcmp(board216049510, 
"2..4441...44441314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312...2.4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board216049510);
board216049510 = NULL;
assert( gamma_busy_fields(board, 1) == 58 );


char* board860104744 = gamma_board(board);
assert( board860104744 != NULL );
assert( strcmp(board860104744, 
"2..4441...44441314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312...2.4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board860104744);
board860104744 = NULL;
assert( gamma_move(board, 2, 1, 20) == 0 );
assert( gamma_move(board, 2, 16, 11) == 0 );
assert( gamma_move(board, 3, 6, 3) == 0 );
assert( gamma_move(board, 4, 7, 9) == 0 );
assert( gamma_move(board, 1, 11, 7) == 0 );


char* board461855660 = gamma_board(board);
assert( board461855660 != NULL );
assert( strcmp(board461855660, 
"2..4441...44441314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22...3..\n"
"312...2.4311.2..1.41434\n"
"1.4..3.31.44.2..144.2..\n"
"14.214.3.222.331..1.3..\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.331...311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.112..1.2.4231.2...2\n") == 0);
free(board461855660);
board461855660 = NULL;
assert( gamma_move(board, 2, 0, 12) == 0 );
assert( gamma_move(board, 3, 0, 3) == 0 );
assert( gamma_move(board, 3, 7, 0) == 1 );
assert( gamma_move(board, 4, 6, 19) == 0 );
assert( gamma_move(board, 1, 18, 12) == 1 );
assert( gamma_move(board, 2, 12, 14) == 0 );
assert( gamma_move(board, 2, 22, 10) == 1 );
assert( gamma_move(board, 3, 13, 13) == 0 );
assert( gamma_move(board, 3, 8, 5) == 0 );
assert( gamma_move(board, 4, 14, 22) == 0 );
assert( gamma_move(board, 1, 11, 3) == 1 );
assert( gamma_move(board, 1, 22, 9) == 1 );
assert( gamma_free_fields(board, 1) == 132 );


char* board441797100 = gamma_board(board);
assert( board441797100 != NULL );
assert( strcmp(board441797100, 
"2..4441...44441314..1..\n"
"1..11...3314.2322..222.\n"
"2213443.1341.1.22.1.3..\n"
"312...2.4311.2..1.41434\n"
"1.4..3.31.44.2..144.2.2\n"
"14.214.3.222.331..1.3.1\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2...44...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.3.1.43..141.1..11\n"
"34.22.3.3311..311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.2...31\n"
"432.1123.1.2.4231.2...2\n") == 0);
free(board441797100);
board441797100 = NULL;
assert( gamma_move(board, 2, 8, 4) == 0 );
assert( gamma_move(board, 2, 6, 12) == 0 );
assert( gamma_free_fields(board, 2) == 132 );
assert( gamma_move(board, 3, 13, 6) == 0 );
assert( gamma_move(board, 4, 9, 16) == 0 );
assert( gamma_golden_move(board, 4, 12, 20) == 0 );
assert( gamma_move(board, 1, 18, 13) == 1 );
assert( gamma_move(board, 2, 7, 16) == 0 );
assert( gamma_move(board, 3, 20, 13) == 0 );
assert( gamma_move(board, 3, 14, 0) == 0 );
assert( gamma_move(board, 4, 5, 13) == 1 );
assert( gamma_free_fields(board, 4) == 130 );
assert( gamma_move(board, 1, 8, 4) == 0 );
assert( gamma_move(board, 1, 12, 0) == 1 );
assert( gamma_free_fields(board, 1) == 129 );
assert( gamma_move(board, 2, 8, 5) == 0 );
assert( gamma_move(board, 3, 6, 3) == 0 );
assert( gamma_move(board, 4, 11, 14) == 0 );
assert( gamma_free_fields(board, 4) == 129 );
assert( gamma_golden_possible(board, 4) == 1 );
assert( gamma_move(board, 1, 12, 21) == 0 );
assert( gamma_move(board, 1, 19, 3) == 0 );
assert( gamma_golden_move(board, 1, 9, 14) == 0 );
assert( gamma_move(board, 2, 4, 19) == 0 );
assert( gamma_move(board, 3, 18, 1) == 1 );
assert( gamma_move(board, 3, 10, 3) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 10, 0) == 1 );
assert( gamma_move(board, 1, 0, 11) == 0 );
assert( gamma_move(board, 2, 13, 1) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 4, 17) == 0 );
assert( gamma_move(board, 3, 9, 4) == 1 );
assert( gamma_move(board, 4, 12, 21) == 0 );
assert( gamma_busy_fields(board, 4) == 50 );
assert( gamma_move(board, 1, 7, 4) == 1 );
assert( gamma_busy_fields(board, 1) == 64 );
assert( gamma_move(board, 2, 9, 12) == 0 );
assert( gamma_move(board, 2, 6, 11) == 0 );
assert( gamma_move(board, 3, 10, 6) == 0 );
assert( gamma_move(board, 4, 2, 18) == 0 );
assert( gamma_move(board, 1, 18, 7) == 0 );
assert( gamma_move(board, 1, 17, 7) == 1 );
assert( gamma_move(board, 2, 12, 17) == 0 );
assert( gamma_move(board, 3, 12, 14) == 0 );
assert( gamma_busy_fields(board, 3) == 49 );


char* board559630595 = gamma_board(board);
assert( board559630595 != NULL );
assert( strcmp(board559630595, 
"2..4441...44441314..1..\n"
"1..114..3314.2322.1222.\n"
"2213443.1341.1.22.1.3..\n"
"312...2.4311.2..1.41434\n"
"1.4..3.31.44.2..144.2.2\n"
"14.214.3.222.331..1.3.1\n"
"4132.3..2.2.11331.2.411\n"
".423...13..12.2..144...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.341..23\n"
"33..1.311343..141.1..11\n"
"34.22.3.3311..311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.23..31\n"
"432.1123.14214231.2...2\n") == 0);
free(board559630595);
board559630595 = NULL;
assert( gamma_move(board, 4, 4, 17) == 0 );
assert( gamma_move(board, 1, 5, 10) == 0 );
assert( gamma_move(board, 2, 15, 14) == 0 );
assert( gamma_move(board, 3, 3, 11) == 1 );
assert( gamma_move(board, 4, 22, 9) == 0 );
assert( gamma_move(board, 1, 5, 19) == 0 );
assert( gamma_move(board, 2, 5, 7) == 1 );
assert( gamma_move(board, 3, 19, 5) == 1 );
assert( gamma_move(board, 4, 4, 10) == 1 );
assert( gamma_move(board, 1, 16, 7) == 1 );
assert( gamma_move(board, 2, 11, 5) == 0 );
assert( gamma_busy_fields(board, 2) == 58 );
assert( gamma_move(board, 3, 10, 21) == 0 );
assert( gamma_move(board, 3, 18, 4) == 0 );
assert( gamma_golden_move(board, 3, 6, 15) == 0 );
assert( gamma_move(board, 4, 17, 12) == 1 );
assert( gamma_move(board, 1, 5, 14) == 0 );
assert( gamma_move(board, 1, 5, 10) == 0 );
assert( gamma_free_fields(board, 1) == 118 );
assert( gamma_golden_move(board, 1, 2, 22) == 0 );
assert( gamma_move(board, 2, 0, 3) == 0 );
assert( gamma_move(board, 2, 6, 14) == 0 );
assert( gamma_move(board, 3, 10, 14) == 0 );
assert( gamma_move(board, 3, 10, 9) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 9, 16) == 0 );
assert( gamma_move(board, 4, 4, 2) == 0 );
assert( gamma_golden_move(board, 4, 14, 4) == 1 );


char* board470569354 = gamma_board(board);
assert( board470569354 != NULL );
assert( strcmp(board470569354, 
"2..4441...44441314..1..\n"
"1..114..3314.2322.1222.\n"
"2213443.1341.1.2241.3..\n"
"3123..2.4311.2..1.41434\n"
"1.4.43.31.44.2..144.2.2\n"
"14.214.3.222.331..1.3.1\n"
"4132.3..2.2.11331.2.411\n"
".423.2.13..12.2.1144...\n"
"..4.4.14314111.22...3.2\n"
"...313.24..2.21.3413.23\n"
"33..1.311343..441.1..11\n"
"34.22.3.3311..311..43..\n"
".212242344...42.32..422\n"
".23.22132441.4.2.23..31\n"
"432.1123.14214231.2...2\n") == 0);
free(board470569354);
board470569354 = NULL;
assert( gamma_move(board, 1, 11, 14) == 0 );
assert( gamma_move(board, 2, 9, 21) == 0 );
assert( gamma_move(board, 2, 6, 2) == 0 );
assert( gamma_move(board, 3, 8, 11) == 0 );
assert( gamma_free_fields(board, 4) == 118 );
assert( gamma_move(board, 1, 3, 5) == 0 );
assert( gamma_move(board, 1, 6, 4) == 0 );
assert( gamma_move(board, 2, 11, 7) == 0 );
assert( gamma_move(board, 2, 14, 1) == 1 );
assert( gamma_move(board, 3, 9, 5) == 1 );
assert( gamma_move(board, 4, 19, 14) == 1 );
assert( gamma_move(board, 1, 16, 3) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 11, 4) == 0 );
assert( gamma_move(board, 2, 13, 5) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 5, 0) == 0 );
assert( gamma_move(board, 3, 15, 9) == 0 );
assert( gamma_move(board, 4, 0, 17) == 0 );
assert( gamma_move(board, 2, 9, 6) == 0 );
assert( gamma_busy_fields(board, 2) == 59 );
assert( gamma_move(board, 3, 16, 5) == 0 );
assert( gamma_move(board, 3, 12, 5) == 1 );
assert( gamma_move(board, 4, 1, 16) == 0 );
assert( gamma_move(board, 4, 20, 5) == 1 );
assert( gamma_move(board, 1, 2, 12) == 0 );
assert( gamma_move(board, 1, 3, 6) == 1 );
assert( gamma_move(board, 2, 4, 5) == 0 );
assert( gamma_move(board, 3, 0, 11) == 0 );
assert( gamma_move(board, 3, 20, 3) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 18, 5) == 0 );
assert( gamma_move(board, 4, 14, 11) == 1 );
assert( gamma_move(board, 1, 3, 21) == 0 );
assert( gamma_move(board, 1, 19, 9) == 1 );
assert( gamma_move(board, 2, 4, 13) == 0 );
assert( gamma_move(board, 2, 15, 2) == 1 );
assert( gamma_move(board, 3, 8, 7) == 0 );
assert( gamma_free_fields(board, 3) == 109 );
assert( gamma_move(board, 4, 8, 4) == 0 );
assert( gamma_move(board, 4, 4, 7) == 1 );
assert( gamma_free_fields(board, 4) == 108 );
assert( gamma_move(board, 1, 9, 12) == 0 );
assert( gamma_move(board, 1, 1, 1) == 0 );


char* board897465378 = gamma_board(board);
assert( board897465378 != NULL );
assert( strcmp(board897465378, 
"2..4441...44441314.41..\n"
"1..114..3314.2322.1222.\n"
"2213443.1341.1.2241.3..\n"
"3123..2.4311.24.1.41434\n"
"1.4.43.31.44.2..144.2.2\n"
"14.214.3.222.331..113.1\n"
"4132.3..2.2.11331.2.411\n"
".42342.13..12.2.1144...\n"
"..414.14314111.22...3.2\n"
"...313.243.2321.3413423\n"
"33..1.311343..441.1..11\n"
"34.22.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.23..31\n"
"432.1123.14214231.2...2\n") == 0);
free(board897465378);
board897465378 = NULL;
assert( gamma_move(board, 2, 12, 21) == 0 );
assert( gamma_free_fields(board, 2) == 108 );
assert( gamma_move(board, 3, 4, 13) == 0 );
assert( gamma_move(board, 3, 19, 9) == 0 );
assert( gamma_move(board, 4, 2, 2) == 0 );
assert( gamma_move(board, 1, 0, 21) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 19, 1) == 1 );
assert( gamma_move(board, 2, 8, 6) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 21, 5) == 0 );
assert( gamma_move(board, 4, 12, 22) == 0 );
assert( gamma_move(board, 4, 1, 0) == 0 );
assert( gamma_move(board, 1, 11, 15) == 0 );
assert( gamma_move(board, 1, 18, 13) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 14, 21) == 0 );
assert( gamma_move(board, 4, 18, 0) == 0 );
assert( gamma_move(board, 1, 11, 8) == 1 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 5, 1) == 0 );
assert( gamma_move(board, 2, 15, 3) == 0 );
assert( gamma_move(board, 3, 22, 0) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );


char* board171281101 = gamma_board(board);
assert( board171281101 != NULL );
assert( strcmp(board171281101, 
"2..4441...44441314.41..\n"
"1..114..3314.2322.1222.\n"
"2213443.1341.1.2241.3..\n"
"3123..2.4311.24.1.41434\n"
"1.4.43.31.44.2..144.2.2\n"
"14.214.3.222.331..113.1\n"
"4132.3..2.2111331.2.411\n"
".42342.13..12.2.1144...\n"
"..414.14314111.22...3.2\n"
"...313.243.2321.3413423\n"
"33..1.311343..441.1..11\n"
"34.22.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2...2\n") == 0);
free(board171281101);
board171281101 = NULL;
assert( gamma_move(board, 4, 9, 16) == 0 );
assert( gamma_busy_fields(board, 4) == 57 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 16, 7) == 0 );
assert( gamma_move(board, 2, 2, 2) == 0 );
assert( gamma_move(board, 3, 13, 6) == 0 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 1, 3) == 0 );
assert( gamma_move(board, 1, 12, 22) == 0 );
assert( gamma_move(board, 2, 3, 14) == 0 );
assert( gamma_move(board, 2, 0, 12) == 0 );
assert( gamma_move(board, 3, 7, 15) == 0 );
assert( gamma_move(board, 4, 19, 4) == 1 );
assert( gamma_move(board, 1, 13, 7) == 1 );
assert( gamma_move(board, 2, 10, 14) == 0 );


char* board678829099 = gamma_board(board);
assert( board678829099 != NULL );
assert( strcmp(board678829099, 
"2..4441...44441314.41..\n"
"1..114..3314.2322.1222.\n"
"2213443.1341.1.2241.3..\n"
"3123..2.4311.24.1.41434\n"
"1.4.43.31.44.2..144.2.2\n"
"14.214.3.222.331..113.1\n"
"4132.3..2.2111331.2.411\n"
".42342.13..1212.1144...\n"
"..414.14314111.22...3.2\n"
"...313.243.2321.3413423\n"
"33..1.311343..441.14.11\n"
"34.22.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2...2\n") == 0);
free(board678829099);
board678829099 = NULL;
assert( gamma_move(board, 4, 7, 15) == 0 );
assert( gamma_move(board, 1, 4, 17) == 0 );
assert( gamma_move(board, 1, 3, 10) == 1 );
assert( gamma_move(board, 2, 15, 2) == 0 );
assert( gamma_move(board, 3, 11, 0) == 0 );
assert( gamma_move(board, 3, 0, 13) == 0 );
assert( gamma_move(board, 4, 1, 11) == 0 );
assert( gamma_move(board, 1, 14, 14) == 0 );
assert( gamma_move(board, 1, 10, 9) == 0 );
assert( gamma_move(board, 2, 12, 10) == 1 );
assert( gamma_move(board, 2, 7, 7) == 0 );
assert( gamma_move(board, 3, 10, 1) == 0 );
assert( gamma_move(board, 4, 0, 3) == 0 );
assert( gamma_move(board, 4, 12, 10) == 0 );
assert( gamma_move(board, 1, 0, 19) == 0 );
assert( gamma_busy_fields(board, 1) == 70 );
assert( gamma_move(board, 2, 14, 7) == 0 );
assert( gamma_move(board, 3, 5, 2) == 0 );
assert( gamma_free_fields(board, 3) == 102 );
assert( gamma_move(board, 4, 1, 12) == 0 );
assert( gamma_move(board, 4, 2, 3) == 1 );
assert( gamma_move(board, 1, 14, 22) == 0 );
assert( gamma_move(board, 1, 13, 1) == 0 );
assert( gamma_move(board, 2, 6, 19) == 0 );
assert( gamma_move(board, 3, 11, 7) == 0 );


char* board225157052 = gamma_board(board);
assert( board225157052 != NULL );
assert( strcmp(board225157052, 
"2..4441...44441314.41..\n"
"1..114..3314.2322.1222.\n"
"2213443.1341.1.2241.3..\n"
"3123..2.4311.24.1.41434\n"
"1.4143.31.4422..144.2.2\n"
"14.214.3.222.331..113.1\n"
"4132.3..2.2111331.2.411\n"
".42342.13..1212.1144...\n"
"..414.14314111.22...3.2\n"
"...313.243.2321.3413423\n"
"33..1.311343..441.14.11\n"
"34422.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2...2\n") == 0);
free(board225157052);
board225157052 = NULL;
assert( gamma_move(board, 4, 10, 14) == 0 );
assert( gamma_move(board, 1, 10, 15) == 0 );
assert( gamma_move(board, 1, 12, 9) == 1 );
assert( gamma_move(board, 2, 21, 10) == 1 );
assert( gamma_busy_fields(board, 2) == 63 );
assert( gamma_move(board, 3, 1, 14) == 1 );
assert( gamma_move(board, 4, 10, 6) == 0 );
assert( gamma_move(board, 4, 7, 8) == 1 );
assert( gamma_move(board, 1, 6, 17) == 0 );
assert( gamma_move(board, 2, 8, 4) == 0 );
assert( gamma_move(board, 2, 17, 4) == 1 );
assert( gamma_move(board, 3, 8, 4) == 0 );
assert( gamma_move(board, 4, 12, 14) == 0 );
assert( gamma_move(board, 4, 2, 2) == 0 );
assert( gamma_move(board, 1, 10, 9) == 0 );
assert( gamma_busy_fields(board, 1) == 71 );
assert( gamma_move(board, 2, 7, 9) == 0 );
assert( gamma_move(board, 2, 14, 4) == 0 );
assert( gamma_move(board, 3, 11, 7) == 0 );
assert( gamma_move(board, 3, 14, 4) == 0 );
board225157052 = gamma_board(board);
puts(board225157052);
free(board225157052);
board225157052 = NULL;
assert( gamma_move(board, 1, 21, 12) == 0 );
assert( gamma_move(board, 2, 12, 7) == 0 );
assert( gamma_move(board, 2, 6, 5) == 1 );
assert( gamma_move(board, 3, 6, 5) == 0 );
assert( gamma_move(board, 3, 1, 13) == 1 );
assert( gamma_move(board, 4, 6, 5) == 0 );
assert( gamma_move(board, 1, 2, 19) == 0 );
assert( gamma_move(board, 1, 21, 7) == 1 );
assert( gamma_move(board, 2, 21, 0) == 1 );
assert( gamma_move(board, 3, 15, 0) == 0 );
assert( gamma_move(board, 3, 9, 13) == 0 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 6, 1) == 0 );
assert( gamma_move(board, 1, 0, 9) == 0 );
assert( gamma_free_fields(board, 1) == 52 );


char* board317345662 = gamma_board(board);
assert( board317345662 != NULL );
assert( strcmp(board317345662, 
"23.4441...44441314.41..\n"
"13.114..3314.2322.1222.\n"
"2213443.1341.1.2241.3..\n"
"3123..2.4311.24.1.41434\n"
"1.4143.31.4422..144.222\n"
"14.214.3.2221331..113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..1212.1144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34422.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2..22\n") == 0);
free(board317345662);
board317345662 = NULL;
assert( gamma_move(board, 2, 7, 22) == 0 );
assert( gamma_move(board, 2, 17, 11) == 1 );
assert( gamma_move(board, 3, 2, 18) == 0 );
assert( gamma_busy_fields(board, 3) == 55 );
assert( gamma_free_fields(board, 3) == 91 );
assert( gamma_golden_possible(board, 3) == 1 );
assert( gamma_move(board, 4, 6, 0) == 0 );
assert( gamma_move(board, 4, 3, 3) == 0 );
assert( gamma_move(board, 1, 13, 0) == 0 );
assert( gamma_busy_fields(board, 1) == 72 );
assert( gamma_move(board, 2, 7, 15) == 0 );
assert( gamma_move(board, 2, 3, 11) == 0 );
assert( gamma_move(board, 3, 20, 0) == 1 );
assert( gamma_busy_fields(board, 3) == 56 );
assert( gamma_move(board, 4, 13, 17) == 0 );
assert( gamma_move(board, 1, 1, 12) == 0 );
assert( gamma_move(board, 2, 5, 15) == 0 );
assert( gamma_move(board, 3, 14, 2) == 0 );
assert( gamma_move(board, 3, 15, 7) == 1 );
assert( gamma_busy_fields(board, 3) == 57 );
assert( gamma_move(board, 1, 5, 10) == 0 );
assert( gamma_move(board, 1, 5, 7) == 0 );
assert( gamma_move(board, 2, 4, 0) == 0 );
assert( gamma_busy_fields(board, 2) == 67 );
assert( gamma_move(board, 3, 3, 5) == 0 );
assert( gamma_golden_move(board, 3, 2, 3) == 1 );
assert( gamma_move(board, 4, 3, 5) == 0 );
assert( gamma_move(board, 4, 11, 8) == 0 );
assert( gamma_move(board, 1, 7, 0) == 0 );
assert( gamma_move(board, 1, 12, 12) == 1 );
assert( gamma_move(board, 2, 6, 19) == 0 );
assert( gamma_move(board, 2, 8, 5) == 0 );
assert( gamma_move(board, 3, 13, 12) == 0 );


char* board256163670 = gamma_board(board);
assert( board256163670 != NULL );
assert( strcmp(board256163670, 
"23.4441...44441314.41..\n"
"13.114..3314.2322.1222.\n"
"2213443.134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"1.4143.31.4422..144.222\n"
"14.214.3.2221331..113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board256163670);
board256163670 = NULL;
assert( gamma_move(board, 4, 11, 15) == 0 );
assert( gamma_move(board, 4, 17, 13) == 1 );
assert( gamma_free_fields(board, 4) == 87 );
assert( gamma_move(board, 1, 6, 21) == 0 );
assert( gamma_move(board, 2, 4, 12) == 0 );
assert( gamma_move(board, 2, 19, 11) == 0 );
assert( gamma_move(board, 3, 6, 14) == 0 );
assert( gamma_move(board, 4, 5, 1) == 0 );
assert( gamma_move(board, 1, 14, 10) == 1 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 5, 10) == 0 );
assert( gamma_move(board, 2, 17, 4) == 0 );
assert( gamma_move(board, 3, 8, 19) == 0 );
assert( gamma_golden_move(board, 3, 5, 7) == 0 );
assert( gamma_move(board, 4, 2, 2) == 0 );
assert( gamma_move(board, 4, 11, 13) == 0 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 8, 19) == 0 );
assert( gamma_free_fields(board, 1) == 49 );
assert( gamma_move(board, 2, 7, 12) == 1 );
assert( gamma_move(board, 2, 1, 10) == 1 );
assert( gamma_move(board, 3, 15, 10) == 1 );
assert( gamma_move(board, 3, 16, 9) == 1 );
assert( gamma_free_fields(board, 3) == 82 );
assert( gamma_move(board, 4, 14, 2) == 0 );
assert( gamma_move(board, 4, 4, 6) == 0 );
assert( gamma_move(board, 1, 6, 0) == 0 );
assert( gamma_move(board, 1, 18, 1) == 0 );
assert( gamma_move(board, 3, 3, 22) == 0 );
assert( gamma_move(board, 4, 14, 2) == 0 );
assert( gamma_move(board, 4, 1, 12) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 1, 20) == 0 );
assert( gamma_move(board, 2, 7, 10) == 0 );
assert( gamma_move(board, 3, 2, 12) == 0 );
assert( gamma_move(board, 3, 21, 5) == 0 );
assert( gamma_move(board, 4, 0, 0) == 0 );
assert( gamma_move(board, 2, 12, 22) == 0 );
assert( gamma_move(board, 2, 2, 12) == 0 );
assert( gamma_move(board, 3, 3, 13) == 0 );
assert( gamma_move(board, 3, 5, 9) == 0 );
assert( gamma_move(board, 4, 13, 6) == 0 );
assert( gamma_move(board, 4, 7, 0) == 0 );
assert( gamma_move(board, 1, 6, 18) == 0 );
assert( gamma_move(board, 1, 10, 1) == 0 );
assert( gamma_free_fields(board, 1) == 45 );


char* board394595402 = gamma_board(board);
assert( board394595402 != NULL );
assert( strcmp(board394595402, 
"23.4441...44441314.41..\n"
"13.114..3314.232241222.\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board394595402);
board394595402 = NULL;
assert( gamma_move(board, 2, 3, 18) == 0 );
assert( gamma_move(board, 2, 9, 4) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_free_fields(board, 3) == 82 );
assert( gamma_move(board, 4, 15, 8) == 0 );


char* board690667589 = gamma_board(board);
assert( board690667589 != NULL );
assert( strcmp(board690667589, 
"23.4441...44441314.41..\n"
"13.114..3314.232241222.\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board690667589);
board690667589 = NULL;
assert( gamma_move(board, 1, 3, 18) == 0 );
assert( gamma_busy_fields(board, 1) == 74 );
assert( gamma_move(board, 2, 14, 8) == 0 );
assert( gamma_move(board, 2, 15, 12) == 0 );
assert( gamma_move(board, 3, 2, 12) == 0 );
assert( gamma_move(board, 3, 18, 11) == 0 );
assert( gamma_move(board, 4, 2, 7) == 0 );
assert( gamma_free_fields(board, 4) == 82 );
assert( gamma_move(board, 1, 11, 13) == 0 );
assert( gamma_move(board, 2, 9, 2) == 0 );
assert( gamma_move(board, 2, 9, 6) == 0 );
assert( gamma_move(board, 3, 15, 4) == 0 );
assert( gamma_move(board, 3, 5, 14) == 0 );
assert( gamma_move(board, 4, 5, 10) == 0 );
assert( gamma_move(board, 4, 5, 0) == 0 );
assert( gamma_move(board, 1, 14, 7) == 0 );
assert( gamma_move(board, 2, 14, 5) == 0 );
assert( gamma_move(board, 2, 14, 10) == 0 );
assert( gamma_move(board, 3, 4, 12) == 0 );
assert( gamma_move(board, 3, 13, 9) == 0 );
assert( gamma_move(board, 4, 10, 6) == 0 );
assert( gamma_move(board, 1, 7, 10) == 0 );
assert( gamma_move(board, 2, 10, 6) == 0 );
assert( gamma_move(board, 2, 22, 13) == 1 );


char* board898473317 = gamma_board(board);
assert( board898473317 != NULL );
assert( strcmp(board898473317, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board898473317);
board898473317 = NULL;
assert( gamma_move(board, 3, 14, 18) == 0 );


char* board312391606 = gamma_board(board);
assert( board312391606 != NULL );
assert( strcmp(board312391606, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board312391606);
board312391606 = NULL;
assert( gamma_move(board, 4, 12, 21) == 0 );
assert( gamma_move(board, 4, 2, 0) == 0 );
assert( gamma_free_fields(board, 4) == 81 );
assert( gamma_move(board, 1, 2, 11) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 3, 7) == 0 );
assert( gamma_move(board, 2, 5, 1) == 0 );
assert( gamma_move(board, 3, 11, 5) == 0 );
assert( gamma_golden_possible(board, 3) == 0 );
assert( gamma_move(board, 4, 12, 14) == 0 );
assert( gamma_move(board, 4, 22, 1) == 0 );
assert( gamma_free_fields(board, 4) == 81 );
assert( gamma_move(board, 1, 3, 21) == 0 );
assert( gamma_move(board, 2, 9, 17) == 0 );
assert( gamma_move(board, 2, 9, 9) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 2, 18) == 0 );
assert( gamma_move(board, 3, 1, 10) == 0 );
assert( gamma_move(board, 4, 7, 10) == 0 );


char* board253308720 = gamma_board(board);
assert( board253308720 != NULL );
assert( strcmp(board253308720, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.13..121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311..311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board253308720);
board253308720 = NULL;
assert( gamma_move(board, 1, 3, 12) == 0 );
assert( gamma_move(board, 2, 0, 12) == 0 );
assert( gamma_move(board, 2, 19, 9) == 0 );
assert( gamma_move(board, 3, 2, 10) == 0 );
assert( gamma_move(board, 3, 2, 8) == 0 );
assert( gamma_busy_fields(board, 3) == 60 );
assert( gamma_move(board, 4, 18, 9) == 0 );
assert( gamma_move(board, 1, 0, 8) == 0 );
assert( gamma_move(board, 1, 3, 2) == 0 );
assert( gamma_move(board, 2, 4, 4) == 0 );
assert( gamma_move(board, 3, 2, 11) == 0 );
assert( gamma_move(board, 4, 14, 22) == 0 );
assert( gamma_golden_move(board, 4, 3, 9) == 0 );
assert( gamma_move(board, 1, 2, 18) == 0 );
assert( gamma_move(board, 1, 12, 5) == 0 );
assert( gamma_free_fields(board, 1) == 45 );
assert( gamma_golden_move(board, 1, 7, 4) == 0 );
assert( gamma_move(board, 2, 1, 3) == 0 );
assert( gamma_move(board, 2, 6, 11) == 0 );
assert( gamma_busy_fields(board, 2) == 70 );
assert( gamma_free_fields(board, 2) == 81 );
assert( gamma_move(board, 3, 14, 8) == 0 );
assert( gamma_move(board, 4, 0, 11) == 0 );
assert( gamma_move(board, 4, 9, 7) == 1 );
assert( gamma_move(board, 1, 10, 10) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 12, 14) == 0 );
assert( gamma_move(board, 4, 6, 21) == 0 );
assert( gamma_move(board, 1, 1, 12) == 0 );
assert( gamma_move(board, 2, 5, 0) == 0 );
assert( gamma_move(board, 3, 5, 0) == 0 );
assert( gamma_move(board, 4, 4, 2) == 0 );
assert( gamma_move(board, 1, 18, 9) == 0 );
assert( gamma_move(board, 1, 1, 11) == 0 );
assert( gamma_move(board, 2, 4, 12) == 0 );
assert( gamma_move(board, 3, 9, 21) == 0 );
assert( gamma_golden_move(board, 3, 2, 8) == 0 );
assert( gamma_move(board, 4, 13, 3) == 1 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 6, 5) == 0 );
assert( gamma_move(board, 1, 10, 6) == 0 );


char* board881533663 = gamma_board(board);
assert( board881533663 != NULL );
assert( strcmp(board881533663, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board881533663);
board881533663 = NULL;
assert( gamma_move(board, 2, 4, 20) == 0 );
assert( gamma_move(board, 2, 11, 5) == 0 );
assert( gamma_move(board, 3, 9, 5) == 0 );
assert( gamma_busy_fields(board, 3) == 60 );
assert( gamma_move(board, 4, 9, 17) == 0 );
assert( gamma_move(board, 1, 4, 5) == 0 );
assert( gamma_free_fields(board, 1) == 44 );
assert( gamma_move(board, 2, 10, 6) == 0 );
assert( gamma_move(board, 2, 7, 9) == 0 );
assert( gamma_move(board, 3, 3, 12) == 0 );
assert( gamma_move(board, 4, 3, 18) == 0 );


char* board754013161 = gamma_board(board);
assert( board754013161 != NULL );
assert( strcmp(board754013161, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board754013161);
board754013161 = NULL;
assert( gamma_move(board, 1, 11, 4) == 0 );
assert( gamma_golden_move(board, 1, 2, 4) == 0 );
assert( gamma_move(board, 2, 12, 21) == 0 );
assert( gamma_move(board, 2, 3, 13) == 0 );
assert( gamma_move(board, 3, 14, 10) == 0 );
assert( gamma_free_fields(board, 3) == 79 );
assert( gamma_golden_possible(board, 3) == 0 );
assert( gamma_move(board, 4, 6, 19) == 0 );
assert( gamma_move(board, 4, 3, 11) == 0 );
assert( gamma_move(board, 1, 2, 19) == 0 );
assert( gamma_move(board, 2, 11, 13) == 0 );


char* board794268839 = gamma_board(board);
assert( board794268839 != NULL );
assert( strcmp(board794268839, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..2.4311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3.22213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.22...3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
".212242344...42232..422\n"
".23.22132441.422.232.31\n"
"432.1123.14214231.2.322\n") == 0);
free(board794268839);
board794268839 = NULL;
assert( gamma_move(board, 3, 5, 0) == 0 );
assert( gamma_move(board, 4, 8, 4) == 0 );
assert( gamma_golden_move(board, 4, 8, 13) == 0 );
assert( gamma_move(board, 1, 5, 7) == 0 );
assert( gamma_move(board, 1, 3, 1) == 0 );
assert( gamma_move(board, 2, 12, 21) == 0 );
assert( gamma_move(board, 3, 14, 22) == 0 );
assert( gamma_move(board, 3, 16, 5) == 0 );
assert( gamma_golden_move(board, 3, 14, 3) == 0 );
assert( gamma_move(board, 4, 6, 14) == 0 );
assert( gamma_move(board, 4, 8, 8) == 0 );
assert( gamma_busy_fields(board, 4) == 62 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 13, 7) == 0 );
assert( gamma_move(board, 1, 8, 9) == 1 );
assert( gamma_move(board, 2, 3, 12) == 0 );
assert( gamma_move(board, 2, 22, 5) == 0 );
assert( gamma_move(board, 3, 10, 6) == 0 );
assert( gamma_free_fields(board, 3) == 78 );
assert( gamma_move(board, 4, 0, 8) == 0 );
assert( gamma_move(board, 4, 3, 6) == 0 );
assert( gamma_move(board, 1, 3, 18) == 0 );
assert( gamma_move(board, 2, 2, 11) == 0 );
assert( gamma_move(board, 3, 2, 0) == 0 );
assert( gamma_move(board, 3, 9, 13) == 0 );
assert( gamma_golden_possible(board, 3) == 0 );
assert( gamma_move(board, 4, 20, 6) == 0 );
assert( gamma_move(board, 4, 6, 11) == 0 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 12, 19) == 0 );
assert( gamma_move(board, 1, 5, 13) == 0 );
assert( gamma_move(board, 2, 1, 16) == 0 );
assert( gamma_move(board, 2, 0, 2) == 1 );
assert( gamma_move(board, 3, 8, 6) == 0 );
assert( gamma_move(board, 2, 11, 12) == 0 );
assert( gamma_move(board, 3, 17, 10) == 0 );
assert( gamma_move(board, 3, 0, 4) == 0 );
assert( gamma_move(board, 4, 16, 8) == 0 );
assert( gamma_move(board, 4, 4, 13) == 0 );
assert( gamma_move(board, 1, 3, 18) == 0 );
assert( gamma_move(board, 1, 6, 5) == 0 );
assert( gamma_move(board, 2, 12, 9) == 0 );
assert( gamma_move(board, 3, 12, 22) == 0 );
assert( gamma_move(board, 4, 3, 7) == 0 );
assert( gamma_move(board, 4, 10, 1) == 0 );
assert( gamma_golden_move(board, 4, 12, 7) == 0 );
assert( gamma_move(board, 1, 15, 13) == 0 );
assert( gamma_busy_fields(board, 1) == 75 );
assert( gamma_move(board, 2, 13, 6) == 0 );
assert( gamma_move(board, 3, 2, 11) == 0 );
assert( gamma_move(board, 4, 9, 21) == 0 );
assert( gamma_move(board, 1, 14, 9) == 0 );
assert( gamma_move(board, 1, 22, 13) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 14, 2) == 0 );
assert( gamma_free_fields(board, 2) == 77 );
assert( gamma_move(board, 3, 3, 5) == 0 );
assert( gamma_move(board, 4, 0, 8) == 0 );
assert( gamma_move(board, 1, 8, 4) == 0 );
assert( gamma_move(board, 1, 14, 3) == 0 );
assert( gamma_move(board, 2, 12, 21) == 0 );
assert( gamma_move(board, 2, 16, 4) == 0 );
assert( gamma_move(board, 3, 11, 2) == 1 );
assert( gamma_golden_possible(board, 3) == 0 );
assert( gamma_move(board, 4, 8, 5) == 0 );
assert( gamma_move(board, 4, 7, 11) == 1 );
assert( gamma_free_fields(board, 4) == 75 );
assert( gamma_move(board, 1, 13, 12) == 0 );
assert( gamma_move(board, 1, 13, 10) == 0 );
assert( gamma_move(board, 2, 7, 12) == 0 );
assert( gamma_move(board, 2, 1, 8) == 0 );
assert( gamma_busy_fields(board, 2) == 71 );
assert( gamma_move(board, 3, 7, 6) == 0 );
assert( gamma_busy_fields(board, 3) == 61 );
assert( gamma_move(board, 4, 13, 2) == 0 );
assert( gamma_move(board, 1, 6, 1) == 0 );
assert( gamma_move(board, 2, 21, 8) == 0 );
assert( gamma_move(board, 2, 14, 0) == 0 );
assert( gamma_move(board, 3, 9, 17) == 0 );
assert( gamma_move(board, 3, 4, 2) == 0 );
assert( gamma_move(board, 4, 13, 7) == 0 );
assert( gamma_move(board, 4, 9, 5) == 0 );
assert( gamma_free_fields(board, 1) == 42 );
assert( gamma_move(board, 2, 2, 19) == 0 );
assert( gamma_move(board, 3, 4, 3) == 0 );
assert( gamma_move(board, 3, 16, 0) == 0 );
assert( gamma_move(board, 4, 5, 0) == 0 );
assert( gamma_move(board, 1, 0, 17) == 0 );
assert( gamma_move(board, 2, 10, 19) == 0 );
assert( gamma_move(board, 2, 9, 5) == 0 );
assert( gamma_move(board, 4, 8, 6) == 0 );
assert( gamma_move(board, 1, 4, 10) == 0 );
assert( gamma_move(board, 1, 15, 0) == 0 );
assert( gamma_move(board, 2, 10, 6) == 0 );
assert( gamma_move(board, 2, 16, 3) == 0 );
assert( gamma_move(board, 3, 21, 4) == 0 );
assert( gamma_move(board, 3, 7, 5) == 0 );
assert( gamma_move(board, 4, 17, 10) == 0 );
assert( gamma_move(board, 4, 8, 0) == 1 );
assert( gamma_busy_fields(board, 4) == 64 );
assert( gamma_move(board, 1, 9, 17) == 0 );
assert( gamma_move(board, 1, 17, 6) == 1 );
assert( gamma_move(board, 2, 4, 20) == 0 );
assert( gamma_move(board, 2, 21, 11) == 0 );
assert( gamma_move(board, 3, 17, 11) == 0 );
assert( gamma_move(board, 4, 18, 13) == 0 );


char* board947985197 = gamma_board(board);
assert( board947985197 != NULL );
assert( strcmp(board947985197, 
"23.4441...44441314.41..\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..244311.24.1241434\n"
"124143.31.442213144.222\n"
"14.214.3122213313.113.1\n"
"4132.3.42.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.221..3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
"2212242344.3.42232..422\n"
".23.22132441.422.232.31\n"
"432.1123414214231.2.322\n") == 0);
free(board947985197);
board947985197 = NULL;
assert( gamma_move(board, 2, 13, 2) == 0 );
assert( gamma_move(board, 2, 9, 10) == 1 );
assert( gamma_move(board, 3, 2, 2) == 0 );
assert( gamma_move(board, 3, 6, 8) == 1 );
assert( gamma_move(board, 4, 14, 18) == 0 );
assert( gamma_move(board, 4, 14, 5) == 0 );
assert( gamma_move(board, 1, 6, 21) == 0 );
assert( gamma_move(board, 1, 8, 10) == 0 );
assert( gamma_move(board, 2, 7, 6) == 0 );
assert( gamma_move(board, 2, 10, 9) == 0 );
assert( gamma_busy_fields(board, 2) == 72 );
assert( gamma_move(board, 4, 2, 12) == 0 );
assert( gamma_move(board, 4, 13, 7) == 0 );
assert( gamma_free_fields(board, 2) == 71 );
assert( gamma_move(board, 3, 9, 6) == 0 );
assert( gamma_move(board, 3, 20, 8) == 0 );
assert( gamma_free_fields(board, 3) == 71 );
assert( gamma_move(board, 4, 15, 8) == 0 );
assert( gamma_busy_fields(board, 4) == 64 );
assert( gamma_move(board, 1, 21, 14) == 1 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 0, 13) == 0 );
assert( gamma_move(board, 3, 2, 10) == 0 );
assert( gamma_busy_fields(board, 3) == 62 );
assert( gamma_move(board, 4, 4, 3) == 0 );
assert( gamma_free_fields(board, 4) == 70 );
assert( gamma_move(board, 1, 11, 1) == 0 );
assert( gamma_move(board, 2, 1, 12) == 0 );
assert( gamma_move(board, 3, 11, 15) == 0 );
assert( gamma_move(board, 3, 6, 8) == 0 );
assert( gamma_move(board, 4, 9, 13) == 0 );
assert( gamma_move(board, 1, 9, 3) == 0 );
assert( gamma_move(board, 1, 4, 4) == 0 );
assert( gamma_busy_fields(board, 1) == 77 );
assert( gamma_move(board, 2, 2, 18) == 0 );
assert( gamma_move(board, 2, 12, 1) == 1 );
assert( gamma_move(board, 3, 22, 10) == 0 );
assert( gamma_move(board, 4, 8, 10) == 0 );
assert( gamma_move(board, 1, 7, 0) == 0 );


char* board204060720 = gamma_board(board);
assert( board204060720 != NULL );
assert( strcmp(board204060720, 
"23.4441...44441314.411.\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..244311.24.1241434\n"
"124143.312442213144.222\n"
"14.214.3122213313.113.1\n"
"4132.3342.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.221..3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
"2212242344.3.42232..422\n"
".23.221324412422.232.31\n"
"432.1123414214231.2.322\n") == 0);
free(board204060720);
board204060720 = NULL;
assert( gamma_move(board, 2, 1, 7) == 0 );
assert( gamma_move(board, 3, 3, 17) == 0 );
assert( gamma_move(board, 3, 22, 9) == 0 );
assert( gamma_busy_fields(board, 3) == 62 );
assert( gamma_move(board, 4, 1, 3) == 0 );
assert( gamma_move(board, 1, 21, 5) == 0 );
assert( gamma_move(board, 2, 3, 17) == 0 );
assert( gamma_busy_fields(board, 2) == 73 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 3, 9, 21) == 0 );
assert( gamma_move(board, 3, 13, 1) == 0 );
assert( gamma_move(board, 4, 6, 1) == 0 );
assert( gamma_free_fields(board, 4) == 69 );
assert( gamma_move(board, 1, 13, 6) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 3, 7, 12) == 0 );
assert( gamma_move(board, 4, 3, 9) == 0 );
assert( gamma_free_fields(board, 4) == 69 );


char* board617481129 = gamma_board(board);
assert( board617481129 != NULL );
assert( strcmp(board617481129, 
"23.4441...44441314.411.\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..244311.24.1241434\n"
"124143.312442213144.222\n"
"14.214.3122213313.113.1\n"
"4132.3342.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.221..3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
"2212242344.3.42232..422\n"
".23.221324412422.232.31\n"
"432.1123414214231.2.322\n") == 0);
free(board617481129);
board617481129 = NULL;
assert( gamma_move(board, 1, 2, 19) == 0 );
assert( gamma_move(board, 1, 1, 6) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 6, 14) == 0 );
assert( gamma_move(board, 3, 3, 7) == 0 );
assert( gamma_busy_fields(board, 3) == 62 );
assert( gamma_move(board, 4, 20, 6) == 0 );


char* board741441919 = gamma_board(board);
assert( board741441919 != NULL );
assert( strcmp(board741441919, 
"23.4441...44441314.411.\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..244311.24.1241434\n"
"124143.312442213144.222\n"
"14.214.3122213313.113.1\n"
"4132.3342.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.221..3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
"2212242344.3.42232..422\n"
".23.221324412422.232.31\n"
"432.1123414214231.2.322\n") == 0);
free(board741441919);
board741441919 = NULL;
assert( gamma_move(board, 1, 20, 5) == 0 );
assert( gamma_move(board, 1, 9, 0) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_move(board, 2, 4, 12) == 0 );
assert( gamma_move(board, 2, 16, 0) == 0 );
assert( gamma_golden_possible(board, 2) == 0 );
assert( gamma_move(board, 4, 14, 22) == 0 );
assert( gamma_golden_possible(board, 4) == 0 );
assert( gamma_move(board, 1, 11, 5) == 0 );


char* board537323290 = gamma_board(board);
assert( board537323290 != NULL );
assert( strcmp(board537323290, 
"23.4441...44441314.411.\n"
"13.114..3314.2322412222\n"
"22134432134111.2241.3..\n"
"3123..244311.24.1241434\n"
"124143.312442213144.222\n"
"14.214.3122213313.113.1\n"
"4132.3342.2111331.2.411\n"
".42342.134.121231144.1.\n"
"..414.14314111.221..3.2\n"
"...3132243.2321.3413423\n"
"33..1.311343..441214.11\n"
"34322.3.3311.4311..43..\n"
"2212242344.3.42232..422\n"
".23.221324412422.232.31\n"
"432.1123414214231.2.322\n") == 0);
free(board537323290);
board537323290 = NULL;
assert( gamma_move(board, 2, 7, 5) == 0 );
assert( gamma_move(board, 3, 18, 10) == 0 );
assert( gamma_move(board, 4, 7, 20) == 0 );
assert( gamma_move(board, 4, 17, 3) == 1 );
assert( gamma_move(board, 1, 7, 22) == 0 );
assert( gamma_move(board, 1, 15, 11) == 1 );
assert( gamma_move(board, 2, 7, 22) == 0 );
assert( gamma_golden_possible(board, 3) == 0 );
assert( gamma_move(board, 4, 8, 17) == 0 );
assert( gamma_free_fields(board, 4) == 67 );
assert( gamma_move(board, 1, 8, 17) == 0 );
assert( gamma_move(board, 2, 14, 18) == 0 );
assert( gamma_move(board, 3, 0, 17) == 0 );
assert( gamma_move(board, 3, 13, 3) == 0 );
assert( gamma_move(board, 4, 5, 0) == 0 );
assert( gamma_move(board, 4, 6, 12) == 0 );
assert( gamma_move(board, 1, 6, 0) == 0 );
assert( gamma_golden_possible(board, 1) == 0 );
assert( gamma_golden_move(board, 2, 13, 3) == 0 );


gamma_delete(board);

    return 0;
}
