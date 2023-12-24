#include "hidden_singles.h"
#include <stdbool.h>
#include <stdlib.h>

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box) return total number of unique value
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values) {
    int totalValues = 0;
    int hiddensingleArray[BOARD_SIZE]={0};

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (p_cells[i]->num_candidates > 1) {
            int *candidates = get_candidates(p_cells[i]);
            for (int j = 0; j < p_cells[i]->num_candidates; ++j) {
                int candidate = candidates[j];
                hiddensingleArray[candidate - 1]++;
            }
            free(candidates);
        }
    }
    //Save the hidden single values 
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (hiddensingleArray[i] == 1) {
                hidden_single_values[totalValues++]=i + 1;
        }   
    }

    return totalValues;  
}
// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter) {
    int hidden_single_values[BOARD_SIZE];
    for (int i = 0; i < find_hidden_single_values(p_cells, hidden_single_values); ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
           if (is_candidate(p_cells[j], hidden_single_values[i])) {
                HiddenSingle container;
                container.p_cell = p_cells[j];
                container.value = hidden_single_values[i];
                p_hidden_singles[(*p_counter)++] = container;
            }
        }
    }
}

// find hidden single cells in a board, return the number of hidden single cells
int hidden_singles(SudokuBoard *p_board) {
    HiddenSingle hidden_singles[BOARD_SIZE*BOARD_SIZE];
    int counter = 0;
    int repeat = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &counter);
        find_hidden_single(p_board->p_cols[i], hidden_singles, &counter);
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &counter);
    }

    repeat = counter;
    for (int i = 0; i < counter; ++i) {
        int notRepeat = 0;
        Cell *hidden_single_cell = hidden_singles[i].p_cell;
        int *candidates = get_candidates(hidden_single_cell);
        int num_candidates = hidden_single_cell->num_candidates;

        for (int j = 0; j < num_candidates; ++j) {
            if (candidates[j] != hidden_singles[i].value) {
                unset_candidate(hidden_single_cell, candidates[j]);
                notRepeat = 1;
            }
        }
        repeat -= notRepeat;
        free(candidates);
    }

    return counter - repeat;
}

