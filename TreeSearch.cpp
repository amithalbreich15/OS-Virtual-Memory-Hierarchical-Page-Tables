//
// Created by yuval on 6/16/2023.
//
#include "TreeSearch.h"

TreeSearch::TreeSearch(uint64_t swapped_in_v, uint64_t parent_t):
swapped_in_virtual_addr(swapped_in_v), parent_table(parent_t), max_frame(0),
empty_table(-1), empty_table_parent(-1), empty_table_offset(-1),
max_dist(-1), swapped_out_p(1), swapped_out_p_parent(-1),
swapped_out_p_offset(-1), swapped_out_virtual_addr(-1){}
