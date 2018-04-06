#include <time.h>
#include <stdlib.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"


// initialize h, explored
struct heap h;
struct heap explored;

void initialize_ai(){
	heap_init(&h);
	heap_init(&explored);
}

/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth,
    propagation_t propagation, int *num_generated_node, int *num_expanded_node){

	//FILL IN THE GRAPH ALGORITHM
	// creat new node
	node_t *curr_node;
	node_t *first_node;
	if((first_node = malloc(sizeof(node_t))) == NULL) {
		printf("failed malloc new node\n");
	}
	first_node->priority = 0;
	first_node->depth = 0;
	first_node->num_childs = 0;
	memcpy(first_node->board, board, sizeof(uint8_t)*SIZE*SIZE);
	first_node->parent = NULL;

	uint32_t score[SIZE] = {0,0,0,0};		// to store the max score in max, or add each score in in avg
	int numscore[SIZE] = {0,0,0,0};		// to store num of score added in, only used in avg

	heap_push(&h, first_node);

	while (h.count != 0) {

		curr_node = heap_delete(&h);
		*num_expanded_node += 1;
		heap_push(&explored, curr_node);

		if(curr_node->depth < max_depth) {
			move_t action;
			for (action=0; action<=3; action++) {
				node_t* new_node = initialize_new_node(curr_node, board, action);

				*num_generated_node += 1;
				if (execute_move_t(new_node->board, &(new_node->priority), action)){
					// add frontier and propagate
					curr_node->num_childs += 1;
					addRandom(new_node->board);
					heap_push(&h, new_node);

					propagate_to_first(new_node, score, numscore, propagation);
				}else{
					// free new node
					free(new_node);
					new_node = NULL;
				}
			}
		}

	}

	// free memory
	emptyPQ(&explored);
	emptyPQ(&h);
	return choose_best_action(score, numscore, propagation);
}

node_t* initialize_new_node(node_t *curr_node, uint8_t board[SIZE][SIZE],move_t action) {
	node_t *new_node;
	if((new_node = (node_t*) malloc(sizeof(node_t))) == NULL) {
		printf("failed malloc new node\n");
	}
	new_node->priority = curr_node->priority;
	new_node->depth = curr_node->depth +1;
	new_node->num_childs = 0;
	new_node->move = action;
	memcpy(new_node->board, board, sizeof(uint8_t)*SIZE*SIZE);
	new_node->parent = curr_node; // the parent here is the current board.
	return new_node;
}

void propagate_to_first(node_t* node, uint32_t score[SIZE], int numscore[SIZE], propagation_t propagation) {

	// to fint it's root
	uint32_t node_score = node->priority;
	while (node->depth > 1){
		node = node->parent;
	}

	if (propagation == 0){ // max
		if (node_score >= score[node->move]){
			score[node->move] = node_score;
		}
	}else{ // avg
		score[node->move] += node_score;
		numscore[node->move] += 1;
	}
}

move_t choose_best_action(uint32_t score[SIZE], int numscore[SIZE], propagation_t propagation) {
	move_t direction;
	move_t best_action = rand() % 4;

	if (propagation == 1){ // avg: total / num of score
		for(direction=0; direction <= 3; direction++) {
			if (numscore[direction] != 0){
				score[direction] = score[direction] / numscore[direction];
			}
		}
	}
	// find the max
	for(direction=0; direction <= 3; direction++) {
		if (score[direction] > score[best_action]) {
			best_action = direction;
		}
	}
	return best_action;
}
