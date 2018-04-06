#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"


void initialize_ai();

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation, int *num_generated_node, int *num_expanded_node);
// MY function
void propagate_to_first(node_t* node, uint32_t score[], int numscore[], propagation_t propagation);
node_t* initialize_new_node(node_t *cur_node, uint8_t board[SIZE][SIZE], move_t action);
move_t choose_best_action(uint32_t score[4], int numscore[4], propagation_t propagation);
#endif
                         