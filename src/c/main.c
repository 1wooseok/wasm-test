#include <assert.h>
#include "pomoku.h"
#include <stdio.h>

void test_init_game(void);
void test_horizontal_chain(void);
void test_combined_chains(void);
void test_swap_row(void);
void test_insert_row(void);
void test_insert_row_before_full(void);
void test_insert_column_before_full(void);
void test_insert_column_out_of_range(void);
void test_remove_row(void);
void test_remove_row_before_min(void);
void test_remove_column_before_min(void);
void test_insert_row_out_of_range(void);
void test_copy_row(void);

int main(void)
{
    test_init_game();

    test_insert_column_out_of_range();

    test_insert_row_out_of_range();

    test_remove_column_before_min();

    test_remove_row_before_min();

    test_insert_column_before_full();

    test_insert_row_before_full();

    assert(is_placeable(0, 0) == TRUE);
    assert(is_placeable(14, 14) == TRUE);
    assert(is_placeable(15, 15) == FALSE);

    assert(place_stone(COLOR_BLACK, 0, 0) == TRUE);
    assert(is_placeable(0, 0) == FALSE);
    assert(get_color(0, 0) == 0);
    assert(place_stone(COLOR_WHITE, 0, 0) == FALSE);
    assert(place_stone(COLOR_BLACK, 0, 0) == FALSE);

    test_horizontal_chain();
    test_combined_chains();

    test_swap_row();
    test_insert_row();
    test_remove_row();
    test_copy_row();

    

    return 0;
}

void test_init_game(void)
{
    init_game();

    assert(get_row_count() == 15);
    assert(get_column_count() == 15);
    assert(get_score(5) == -1);
    assert(get_score(COLOR_BLACK) == 0);
    assert(get_score(COLOR_WHITE) == 0);
}

void test_horizontal_chain(void)
{
    test_init_game();

    place_stone(COLOR_BLACK, 0, 0);
    place_stone(COLOR_BLACK, 0, 1);
    place_stone(COLOR_BLACK, 0, 2);
    place_stone(COLOR_BLACK, 0, 3);
    place_stone(COLOR_BLACK, 0, 4);

    assert(get_score(COLOR_BLACK) == 1);

    place_stone(COLOR_BLACK, 0, 5);

    assert(get_score(COLOR_BLACK) == 3);

    place_stone(COLOR_BLACK, 0, 7);
    assert(get_score(COLOR_BLACK) == 3);

    place_stone(COLOR_BLACK, 0, 6);
    assert(get_score(COLOR_BLACK) == 7);

    assert(get_score(COLOR_WHITE) == 0);
}

void test_combined_chains(void)
{
    test_init_game();

    place_stone(COLOR_BLACK, 0, 0);
    place_stone(COLOR_BLACK, 0, 1);
    place_stone(COLOR_BLACK, 0, 2);
    place_stone(COLOR_BLACK, 0, 4);
    place_stone(COLOR_BLACK, 0, 5);

    place_stone(COLOR_BLACK, 1, 3);
    place_stone(COLOR_BLACK, 2, 3);
    place_stone(COLOR_BLACK, 3, 3);
    place_stone(COLOR_BLACK, 4, 3);

    assert(get_score(COLOR_BLACK) == 0);

    place_stone(COLOR_BLACK, 0, 3);

    assert(get_score(COLOR_BLACK) == 3);
}

void test_swap_row(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 6; i++) {
        place_stone(COLOR_BLACK, 0, i);
    }

    assert(get_score(COLOR_BLACK) == 3);

    assert(swap_rows(COLOR_BLACK, 0, 4) == TRUE);
    assert(get_score(COLOR_BLACK) == 1);

    for (i = 0; i < 6; i++) {
        assert(is_placeable(0, i) == TRUE);
        assert(is_placeable(4, i) == FALSE);
    }
}

void test_insert_row(void)
{
    unsigned int i;

    test_init_game();

    place_stone(COLOR_BLACK, 2, 0);
    place_stone(COLOR_BLACK, 2, 1);
    place_stone(COLOR_BLACK, 2, 2);
    place_stone(COLOR_BLACK, 2, 3);
    place_stone(COLOR_BLACK, 2, 4);
    place_stone(COLOR_BLACK, 2, 5);

    assert(get_score(COLOR_BLACK) == 3);

    assert(insert_row(COLOR_BLACK, 2) == TRUE);
    assert(get_score(COLOR_BLACK) == 0);
    assert(get_row_count() == 16);

    for (i = 0; i < 6; i++) {
        assert(is_placeable(2, i) == TRUE);
        assert(is_placeable(3, i) == FALSE);
    }
}

void test_insert_row_out_of_range(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 15; ++i) {
        place_stone(COLOR_BLACK, 14, i);
    }

    assert(insert_row(COLOR_BLACK, 15) == TRUE);
    
    assert(get_row_count() == 16);

    assert(insert_row(COLOR_BLACK, 16) == TRUE);
    assert(insert_row(COLOR_BLACK, 17) == TRUE);
    assert(insert_row(COLOR_BLACK, 18) == TRUE);
    assert(insert_row(COLOR_BLACK, 19) == TRUE);

    assert(get_row_count() == 20);
    assert(insert_row(COLOR_BLACK, 20) == FALSE);

    assert(get_row_count() == 20);
}

void test_insert_row_before_full(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 15; ++i) {
        place_stone(COLOR_BLACK, 14, i);
    }
    
    assert(insert_row(COLOR_BLACK, 14) == TRUE);
    assert(insert_row(COLOR_BLACK, 14) == TRUE);
    assert(insert_row(COLOR_BLACK, 14) == TRUE);
    assert(insert_row(COLOR_BLACK, 14) == TRUE);

    assert(get_row_count() == 19);

    assert(insert_row(COLOR_BLACK, 14) == TRUE);

    assert(get_row_count() == 20);

    assert(insert_row(COLOR_BLACK, 14) == FALSE);
}

void test_insert_column_before_full(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 15; ++i) {
        place_stone(COLOR_BLACK, i, 14);
    }
    
    assert(insert_column(COLOR_BLACK, 14) == TRUE);
    assert(insert_column(COLOR_BLACK, 14) == TRUE);
    assert(insert_column(COLOR_BLACK, 14) == TRUE);
    assert(insert_column(COLOR_BLACK, 14) == TRUE);

    assert(get_column_count() == 19);

    assert(insert_column(COLOR_BLACK, 14) == TRUE);

    assert(get_column_count() == 20);

    assert(insert_column(COLOR_BLACK, 14) == FALSE);
}

void test_insert_column_out_of_range(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 15; ++i) {
        place_stone(COLOR_BLACK, i, 14);
    }

    assert(insert_column(COLOR_BLACK, 15) == TRUE);
    
    assert(get_column_count() == 16);

    assert(insert_column(COLOR_BLACK, 17) == FALSE);

    assert(insert_column(COLOR_BLACK, 16) == TRUE);
    assert(insert_column(COLOR_BLACK, 17) == TRUE);
    assert(insert_column(COLOR_BLACK, 18) == TRUE);
    assert(insert_column(COLOR_BLACK, 19) == TRUE);

    assert(get_column_count() == 20);
    assert(insert_column(COLOR_BLACK, 20) == FALSE);

    assert(get_column_count() == 20);
}

void test_remove_row(void)
{
    unsigned int i;

    test_init_game();

    place_stone(COLOR_BLACK, 2, 0);
    place_stone(COLOR_BLACK, 2, 1);
    place_stone(COLOR_BLACK, 2, 2);
    place_stone(COLOR_BLACK, 2, 3);
    place_stone(COLOR_BLACK, 2, 4);
    place_stone(COLOR_BLACK, 2, 5);

    assert(get_score(COLOR_BLACK) == 3);
    assert(remove_row(COLOR_BLACK, 2) == TRUE);
    assert(get_score(COLOR_BLACK) == 0);

    for (i = 0; i < get_column_count(); i++) {
        assert(is_placeable(2, i) == TRUE);
    }
}

void test_remove_row_before_min(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 15; ++i) {
        place_stone(COLOR_BLACK, 2, i);
        place_stone(COLOR_BLACK, 3, i);
    }

    assert(remove_row(COLOR_BLACK, 4) == TRUE);
    assert(remove_row(COLOR_BLACK, 4) == TRUE);
    assert(remove_row(COLOR_BLACK, 4) == TRUE);
    assert(remove_row(COLOR_BLACK, 4) == TRUE);

    assert(get_row_count() == 11);

    assert(remove_row(COLOR_BLACK, 4) == TRUE);
    assert(get_row_count() == 10);

    assert(remove_row(COLOR_BLACK, 4) == FALSE);
    assert(get_row_count() == 10);
}

void test_remove_column_before_min(void)
{
    unsigned int i;

    test_init_game();

    for (i = 0; i < 15; ++i) {
        place_stone(COLOR_BLACK, 2, i);
        place_stone(COLOR_BLACK, 3, i);
    }

    assert(remove_column(COLOR_BLACK, 1) == TRUE);
    assert(remove_column(COLOR_BLACK, 1) == TRUE);
    assert(remove_column(COLOR_BLACK, 1) == TRUE);
    assert(remove_column(COLOR_BLACK, 1) == TRUE);

    assert(get_column_count() == 11);

    assert(remove_column(COLOR_BLACK, 1) == TRUE);
    assert(get_column_count() == 10);


    assert(remove_column(COLOR_BLACK, 1) == FALSE);
    assert(get_column_count() == 10);
}

void test_copy_row(void)
{
    unsigned int i;

    test_init_game();

    place_stone(COLOR_BLACK, 2, 0);
    place_stone(COLOR_BLACK, 2, 1);
    place_stone(COLOR_BLACK, 2, 2);
    place_stone(COLOR_BLACK, 2, 3);
    place_stone(COLOR_BLACK, 2, 4);
    place_stone(COLOR_BLACK, 2, 5);
    place_stone(COLOR_BLACK, 2, 6);

    place_stone(COLOR_WHITE, 2, 7);
    place_stone(COLOR_WHITE, 2, 8);

    assert(get_score(COLOR_BLACK) == 6);
    assert(copy_row(COLOR_BLACK, 2, 4) == TRUE);
    assert(get_score(COLOR_BLACK) == 2);

    for (i = 0; i < get_column_count(); i++) {
        assert(get_color(4, i) == get_color(2, i));
    }
}
