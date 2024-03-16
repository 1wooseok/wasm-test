#include "pomoku.h"
#include <assert.h>
#include <stdio.h>
#include <emscripten/emscripten.h>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE static int s_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE] = { 0, };
/* HACK*/
EXTERN EMSCRIPTEN_KEEPALIVE static unsigned int s_curr_row_size = 0;
EXTERN EMSCRIPTEN_KEEPALIVE static unsigned int s_curr_col_size = 0;

EXTERN EMSCRIPTEN_KEEPALIVE static unsigned int s_black_score = 0;
EXTERN EMSCRIPTEN_KEEPALIVE static unsigned int s_white_score = 0;

EXTERN EMSCRIPTEN_KEEPALIVE int (*get_board(void))[MAX_BOARD_SIZE]
{
    return s_board;
}

EXTERN EMSCRIPTEN_KEEPALIVE void init_game(void)
{
    size_t i;
    size_t j;
    for (i = 0; i < MAX_BOARD_SIZE; ++i) {
        for (j = 0; j < MAX_BOARD_SIZE; ++j) {
            s_board[i][j] = INITIAL_CELL_VALUE;
        }
    }

    s_curr_row_size = BOARD_SIZE;
    s_curr_col_size = BOARD_SIZE;

    s_black_score = 0;
    s_white_score = 0;
}

EXTERN EMSCRIPTEN_KEEPALIVE unsigned int get_row_count(void)
{
    return s_curr_row_size;
}

EXTERN EMSCRIPTEN_KEEPALIVE unsigned int get_column_count(void)
{
    return s_curr_col_size;
}

EXTERN EMSCRIPTEN_KEEPALIVE int get_score(const color_t color)
{
    switch (color) {
    case COLOR_BLACK:
        return s_black_score;
    case COLOR_WHITE:
        return s_white_score;
    default:
        return -1;
    }
}

EXTERN EMSCRIPTEN_KEEPALIVE int get_color(const unsigned int row, const unsigned int col)
{
    if (is_valid_row_col(row, col) == FALSE) {
        return -1;
    }

    switch (s_board[row][col]) {
    case COLOR_BLACK:
        return 0;
    case COLOR_WHITE:
        return 1;
    default:
        return -1;
    }
}

EXTERN EMSCRIPTEN_KEEPALIVE int is_placeable(const unsigned int row, const unsigned int col)
{
    if (is_valid_row_col(row, col) == FALSE) {
        return FALSE;
    }

    return s_board[row][col] == INITIAL_CELL_VALUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int place_stone(const color_t color, const unsigned int row, const unsigned int col)
{
    if ((color != COLOR_BLACK) && (color != COLOR_WHITE)) {
        return FALSE;
    }

    if (is_placeable(row, col) == FALSE) {
        return FALSE;
    }

    s_board[row][col] = color;

    add_score_by_color(color, calc_score(color, row, col));

    return TRUE;
}

/*
  special moves
*/

EXTERN EMSCRIPTEN_KEEPALIVE int insert_row(const color_t color, const unsigned int row)
{
    size_t i;
    size_t j;
    unsigned int row_count = get_row_count();
    unsigned int column_count = get_column_count();

    if (row > row_count) {
        return FALSE;
    }

    if (row_count == MAX_BOARD_SIZE) {
        return FALSE;
    }

    if (get_score(color) < INSERT_SKILL_POINT) {
        return FALSE;
    }

    ++s_curr_row_size;
    row_count = get_row_count();
    for (i = row_count - 1; i > row; --i) {
        for (j = 0; j < column_count; ++j) {
            s_board[i][j] = s_board[i - 1][j];
        }
    }

    for (j = 0; j < column_count; ++j) {
        s_board[row][j] = INITIAL_CELL_VALUE;
    }

    reduce_score_by_color(color, INSERT_SKILL_POINT);
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int insert_column(const color_t color, const unsigned int col)
{
    size_t i;
    size_t j;
    unsigned int row_count = get_row_count();
    unsigned int column_count = get_column_count();

    if (col > column_count) {
        return FALSE;
    }

    if (column_count == MAX_BOARD_SIZE) {
        return FALSE;
    }

    if (get_score(color) < INSERT_SKILL_POINT) {
        return FALSE;
    }

    ++s_curr_col_size;
    column_count = get_column_count();
    for (i = 0; i < row_count; ++i) {
        for (j = column_count - 1; j > col; --j) { /* memory */
            s_board[i][j] = s_board[i][j - 1];
        }
    }

    for (i = 0; i < row_count; ++i) {
        s_board[i][col] = INITIAL_CELL_VALUE;
    }

    reduce_score_by_color(color, INSERT_SKILL_POINT);
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int remove_row(const color_t color, const unsigned int row)
{
    size_t i;
    size_t j;
    unsigned int row_count = get_row_count();
    unsigned int column_count = get_column_count();

    if (is_valid_row_col(row, 0) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < REMOVE_SKILL_POINT) {
        return FALSE;
    }

    if (row_count == MIN_BOARD_SIZE) {
        return FALSE;
    }

    for (i = row; i < row_count - 1; ++i) {
        for (j = 0; j < column_count; ++j) {
            s_board[i][j] = s_board[i + 1][j];
        }
    }

    for (j = 0; j < column_count; ++j) {
        s_board[row_count - 1][j] = INITIAL_CELL_VALUE;
    }

    reduce_score_by_color(color, REMOVE_SKILL_POINT);
    --s_curr_row_size;
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int remove_column(const color_t color, const unsigned int col)
{
    size_t i;
    size_t j;
    unsigned int row_count = get_row_count();
    unsigned int column_count = get_column_count();

    if (is_valid_row_col(0, col) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < REMOVE_SKILL_POINT) {
        return FALSE;
    }

    if (column_count == MIN_BOARD_SIZE) {
        return FALSE;
    }

    for (i = 0; i < row_count; ++i) {
        for (j = col; j < column_count - 1; ++j) {
            s_board[i][j] = s_board[i][j + 1];
        }
    }

    for (i = 0; i < row_count; ++i) {
        s_board[i][column_count - 1] = INITIAL_CELL_VALUE;
    }

    reduce_score_by_color(color, REMOVE_SKILL_POINT);
    --s_curr_col_size;
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int swap_rows(const color_t color, const unsigned int row0, const unsigned int row1)
{
    int j;
    int column_count = (int)get_column_count();

    if (is_valid_row_col(row0, 0) == FALSE) {
        return FALSE;
    }

    if (is_valid_row_col(row1, 0) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < SWAP_SKILL_POINT) {
        return FALSE;
    }

    if (row0 == row1) {
        reduce_score_by_color(color, SWAP_SKILL_POINT);
        return TRUE;
    }

    for (j = 0; j < column_count; ++j) {
        int temp = s_board[row0][j];
        s_board[row0][j] = s_board[row1][j];
        s_board[row1][j] = temp;
    }

    reduce_score_by_color(color, SWAP_SKILL_POINT);
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int swap_columns(const color_t color, const unsigned int col0, const unsigned int col1)
{
    int i;
    int row_count = (int)get_row_count();

    if (is_valid_row_col(0, col0) == FALSE) {
        return FALSE;
    }

    if (is_valid_row_col(0, col1) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < SWAP_SKILL_POINT) {
        return FALSE;
    }

    if (col0 == col1) {
        reduce_score_by_color(color, SWAP_SKILL_POINT);
        return TRUE;
    }

    for (i = 0; i < row_count; ++i) {
        int temp = s_board[i][col0];
        s_board[i][col0] = s_board[i][col1];
        s_board[i][col1] = temp;
    }

    reduce_score_by_color(color, SWAP_SKILL_POINT);
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int copy_row(const color_t color, const unsigned int src, const unsigned int dst)
{
    int j;
    int column_count = (int)get_column_count();

    if (is_valid_row_col(src, 0) == FALSE) {
        return FALSE;
    }

    if (is_valid_row_col(dst, 0) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < COPY_SKILL_POINT) {
        return FALSE;
    }

    if (src == dst) {
        reduce_score_by_color(color, COPY_SKILL_POINT);
        return TRUE;
    }

    for (j = 0; j < column_count; ++j) {
        s_board[dst][j] = s_board[src][j];
    }

    reduce_score_by_color(color, COPY_SKILL_POINT);
    return TRUE;
}

EXTERN EMSCRIPTEN_KEEPALIVE int copy_column(const color_t color, const unsigned int src, const unsigned int dst)
{
    int i;
    int row_count = (int)get_row_count();

    if (is_valid_row_col(0, src) == FALSE) {
        return FALSE;
    }

    if (is_valid_row_col(0, dst) == FALSE) {
        return FALSE;
    }

    if (get_score(color) < COPY_SKILL_POINT) {
        return FALSE;
    }

    if (src == dst) {
        reduce_score_by_color(color, COPY_SKILL_POINT);
        return TRUE;
    }

    for (i = 0; i < row_count; ++i) {
        s_board[i][dst] = s_board[i][src];
    }

    reduce_score_by_color(color, COPY_SKILL_POINT);
    return TRUE;
}

/*
  helper
*/
EXTERN EMSCRIPTEN_KEEPALIVE void add_score_by_color(const color_t color, unsigned int score)
{
    switch (color) {
    case COLOR_BLACK:
        s_black_score += score;
        break;
    case COLOR_WHITE:
        s_white_score += score;
        break;
    default:
        assert(FALSE);
    }
}

EXTERN EMSCRIPTEN_KEEPALIVE void reduce_score_by_color(const color_t color, unsigned int score)
{
    switch (color) {
    case COLOR_BLACK:
        s_black_score -= score;
        break;
    case COLOR_WHITE:
        s_white_score -= score;
        break;
    default:
        assert(FALSE);
    }
}


EXTERN EMSCRIPTEN_KEEPALIVE int is_valid_row_col(const unsigned int row, const unsigned int col)
{
    return row < get_row_count() && col < get_column_count();
}
/*
  special helper
*/
EXTERN EMSCRIPTEN_KEEPALIVE unsigned int calc_score(const color_t color, const unsigned int row, const unsigned int col)
{
    unsigned int score = 0;

    unsigned int connection_count[4] = { 0, };
    const size_t ROW = 0;
    const size_t COLUMN = 1;
    const size_t DOWNWARD_DIAGONAL = 2;
    const size_t UPWARD_DIAGONAL = 3;

    const unsigned int row_count = get_row_count();
    const unsigned int column_count = get_column_count();
    int i;
    int j;

    /* DIRECTION_ROW: */
    {
        /* 왼쪽 <- (현재) */
        for (i = col; i >= 0; --i) {
            if (s_board[row][i] != (int)color) {
                break;
            }

            ++connection_count[ROW];
        }

        /* (현재 + 1) -> 오른쪽 */
        for (i = col + 1; i < column_count; ++i) {
            if (s_board[row][i] != (int)color) {
                break;
            }

            ++connection_count[ROW];
        }
    }

    /* DIRECTION_COLUMN */
    {
        /* 위쪽 이동 */
        for (i = row; i >= 0; --i) {
            if (s_board[i][col] != (int)color) {
                break;
            }

            ++connection_count[COLUMN];
        }

        /* 아래쪽 이동 */
        for (i = row + 1; i < row_count; ++i) {
            if (s_board[i][col] != (int)color) {
                break;
            }

            ++connection_count[COLUMN];
        }
    }

    /* DIRECTION_DOWNWARD_DIAGONAL: */
    {
        /* 놓은곳 위치부터 왼쪽 위로 이동*/
        i = row;
        j = col;
        while (i >= 0 && j >= 0) {
            if (s_board[i][j] != (int)color) {
                break;
            }

            ++connection_count[DOWNWARD_DIAGONAL];

            --i;
            --j;
        }

        /* 놓은곳 다음 위치 부터 오른쪽 아래로 이동*/
        i = row + 1;
        j = col + 1;
        while (i < row_count && j < column_count) {
            if (s_board[i][j] != (int)color) {
                break;
            }

            ++connection_count[DOWNWARD_DIAGONAL];

            ++i;
            ++j;
        }
    }

    /* DIRECTION_UPWARD_DIAGONAL: */
    {
        /* 놓은곳 위치부터 오른쪽 위로 이동*/
        i = row;
        j = col;
        while (i >= 0 && j < column_count) {
            if (s_board[i][j] != (int)color) {
                break;
            }

            ++connection_count[UPWARD_DIAGONAL];

            --i;
            ++j;
        }

        /* 놓은곳 다음위치 부터 왼쪽 아래로 이동*/
        i = row + 1;
        j = col - 1;
        while (i < row_count && j >= 0) {
            if (s_board[i][j] != (int)color) {
                break;
            }

            ++connection_count[UPWARD_DIAGONAL];

            ++i;
            --j;
        }
    }

    for (i = 0; i < 4; ++i) {
        if (connection_count[i] < MIN_CONNECTION_COUNT_GET_SCORE) {
            continue;
        }

        score += connection_count[i] - (MIN_CONNECTION_COUNT_GET_SCORE - 1);
    }

    return score;
}
