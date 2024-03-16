#ifndef POMOKU_H
#include "color.h"

#define POMOKU_H

#define TRUE 1
#define FALSE 0

/* rule */
#define MIN_CONNECTION_COUNT_GET_SCORE 5

/* skill */
#define SWAP_SKILL_POINT 2
#define INSERT_SKILL_POINT 3
#define REMOVE_SKILL_POINT 3
#define COPY_SKILL_POINT 4

/* board */
#define BOARD_SIZE 15
#define MAX_BOARD_SIZE 20
#define MIN_BOARD_SIZE 10
#define INITIAL_CELL_VALUE -1

int (*get_board(void))[MAX_BOARD_SIZE];

void init_game(void);

unsigned int get_row_count(void);

unsigned int get_column_count(void);

int get_score(const color_t color);

int get_color(const unsigned int row, const unsigned int col);

int is_placeable(const unsigned int row, const unsigned int col);

int place_stone(const color_t color, const unsigned int row, const unsigned int col);

/* special moves */
int insert_row(const color_t color, const unsigned int row);

int insert_column(const color_t color, const unsigned int col);

int remove_row(const color_t color, const unsigned int row);

int remove_column(const color_t color, const unsigned int col);

int swap_rows(const color_t color, const unsigned int row0, const unsigned int row1);

int swap_columns(const color_t color, const unsigned int col0, const unsigned int col1);

int copy_row(const color_t color, const unsigned int src, const unsigned int dst);

int copy_column(const color_t color, const unsigned int src, const unsigned int dst);

/* speical helper */
unsigned int calc_score(const color_t color, const unsigned int row, const unsigned int col);

/* helper */
void add_score_by_color(const color_t color, unsigned int score);

void reduce_score_by_color(const color_t color, unsigned int score);

int is_valid_row_col(const unsigned int row, const unsigned int col);

#endif /* POMOKU_H */
