//
// Created by yuval on 6/16/2023.
//

#ifndef _TREESEARCH_H_
#define _TREESEARCH_H_

//
// Created by yuval on 6/16/2023.
//
#include <cstdint>

class TreeSearch{
 public:
  uint64_t swapped_in_virtual_addr; // page index of page to be swapped in
  uint64_t parent_table; // most recently created page table
  uint64_t max_frame; // maximal index of occupied frame
  uint64_t empty_table; // physical address of empty frame
  uint64_t empty_table_parent; // physical address of empty frame's parent
  int empty_table_offset; // offset of the empty table in its parent table
  long max_dist; // maximal cyclical distance from current page
  uint64_t swapped_out_p; // physical address of page to be swapped out
  uint64_t swapped_out_p_parent; // physical address of the parent of the
  // page to be swapped out
  int swapped_out_p_offset; // offset of the page to be swapped out in its
  // parent table
  uint64_t swapped_out_virtual_addr; // page index of page to be swapped out
  TreeSearch(uint64_t swapped_in_v, uint64_t parent_t);
};


#endif //_TREESEARCH_H_
