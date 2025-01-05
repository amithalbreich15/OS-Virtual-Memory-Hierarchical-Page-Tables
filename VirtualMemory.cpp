//
// Created by yuval on 6/10/2023.
//

#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include "TreeSearch.h"
#include <cmath>
#include <algorithm>
#include <cassert>

#define NUM_ROWS PAGE_SIZE
#define LIB_SUCCESS 1
#define LIB_FAILURE 0


uint64_t get_offset(uint64_t virtualAddress, int i){
  /*
   * Calculates the offset of the given virtual address in the page table of
   * the i-th level
   */
  int first_block_size = OFFSET_WIDTH;
  int remainder = VIRTUAL_ADDRESS_WIDTH % OFFSET_WIDTH;
  if (remainder != 0){
      first_block_size = remainder;
  }
  int shift_amount = VIRTUAL_ADDRESS_WIDTH - first_block_size - i*OFFSET_WIDTH;
  return (virtualAddress >> shift_amount) & (NUM_ROWS-1);
}


void calculate_dist (uint64_t cur_addr, uint64_t parent, int cur_offset,
                     TreeSearch *tree_search, uint64_t virtual_addr)
{
  /*
   * Calculates cyclical distance in virtual space between page to be
   * swapped in and current page. If distance is maximal, replaces the
   * relevant values in given TreeSearch object. Assumes that current page
   * is a real page (and not a page table)
   */
  long dist = std::abs(
      (long)(tree_search->swapped_in_virtual_addr) - (long)virtual_addr);
  long cyclical_dist = std::min((long)NUM_PAGES - dist, dist);
  if (cyclical_dist > tree_search->max_dist){
    tree_search->max_dist = cyclical_dist;
    tree_search->swapped_out_p = cur_addr;
    tree_search->swapped_out_p_parent = parent;
    tree_search->swapped_out_p_offset = cur_offset;
    tree_search->swapped_out_virtual_addr = virtual_addr;
  }
}


void tree_traversal(uint64_t cur_addr, int cur_level, uint64_t parent,
int cur_offset, uint64_t cur_virtual_addr, TreeSearch *tree_search){
  /*
   * Traverses the tree of the page tables and searches for:
   * 1. Empty page table
   * 2. Unused frame (by calculating the max frame address)
   * 3. Non-table page with maximal cyclic distance to given address
   * If any match is found, relevant values are modified through provided
   * TreeSearch object.
   */

  // case 2
  if (cur_addr > tree_search->max_frame){
    tree_search->max_frame = cur_addr;
  }

  uint64_t virtual_addr = (cur_virtual_addr << OFFSET_WIDTH) + cur_offset;

  // case 3
  if (cur_level == TABLES_DEPTH+1){
    calculate_dist (cur_addr, parent, cur_offset, tree_search, virtual_addr);
    return;
  }

  // explore existing child tables
  bool is_empty = true;
  for (int i=0; i<NUM_ROWS; i++){
    word_t next_addr;
    PMread (cur_addr*PAGE_SIZE + i, &next_addr);
    if (next_addr != 0){
      is_empty = false;
      tree_traversal (next_addr, cur_level+1, cur_addr, i, virtual_addr,
                      tree_search);
    }
  }

  // case 1
  if (is_empty && cur_addr != tree_search->parent_table && cur_level != 1){
    tree_search->empty_table = cur_addr;
    tree_search->empty_table_parent = parent;
    tree_search->empty_table_offset = cur_offset;
  }
}


uint64_t find_new_slot(uint64_t swapped_in_virtual_addr, uint64_t parent_table){
  /*
   * Searches for an available frame, and if none is available evicts the
   * page with largest cyclical distance from given page (which is different
   * than given parent table) and returns its physical address.
   */
  TreeSearch tree_search(swapped_in_virtual_addr, parent_table);
  uint64_t cur_addr = 0;
  int cur_level = 1;
  uint64_t parent = -1;
  int cur_offset = 0;
  uint64_t cur_virtual_addr = 0;

  tree_traversal (cur_addr, cur_level, parent, cur_offset, cur_virtual_addr,
                  &tree_search);

  // case 1 - empty table was found
  if (tree_search.empty_table != (uint64_t)-1){
    assert(tree_search.empty_table_offset >= 0);
    assert(tree_search.empty_table_parent != (uint64_t)-1);
    PMwrite(tree_search.empty_table_parent*PAGE_SIZE + tree_search.empty_table_offset,
            (word_t)0);
    return tree_search.empty_table;
  }

  // case 2 - unused table was found
  if (tree_search.max_frame < NUM_FRAMES-1){
    return tree_search.max_frame + 1;
  }

  // case 3 - swap is required
  assert(tree_search.swapped_out_virtual_addr != (uint64_t)-1);
  assert(tree_search.swapped_out_p_parent != (uint64_t)-1);
  assert(tree_search.swapped_out_p_offset >= 0);
  PMevict (tree_search.swapped_out_p, tree_search.swapped_out_virtual_addr);
  PMwrite(tree_search.swapped_out_p_parent*PAGE_SIZE + tree_search.swapped_out_p_offset,
          (word_t)0);
  return tree_search.swapped_out_p;
}


void init_table(uint64_t table_addr){
  /*
   * Initializes a given physical address with zeros in all entries
   */
  for (int i=0; i<NUM_ROWS; i++){
    PMwrite(table_addr*PAGE_SIZE + i, (word_t)0);
  }
}


uint64_t find_frame(uint64_t virtualAddress){
  /*
   * Translates the given virtual address to a physical address. If the
   * given page is not mapped to physical memory, an avialable frame is
   * found and the page is restored to physical memory. If needed, ancestors
   * in the page table tree are created and initialized.
   */
  uint64_t cur_addr = 0;
  uint64_t page_index = virtualAddress >> OFFSET_WIDTH;
  for (int i=0; i<TABLES_DEPTH; i++){
    uint64_t offset = get_offset (virtualAddress, i);
    word_t next_addr_word;
    PMread (cur_addr*PAGE_SIZE + offset, &next_addr_word);
    uint64_t next_addr = next_addr_word;
    if (next_addr == 0){
      next_addr = find_new_slot(page_index, cur_addr);
      if (i == TABLES_DEPTH-1){
        PMrestore (next_addr, page_index);
      }
      else{
        init_table (next_addr);
      }
      PMwrite (cur_addr*PAGE_SIZE + offset, (word_t)next_addr);
    }
    cur_addr = next_addr;
  }
  uint64_t final_offset = virtualAddress & (PAGE_SIZE-1);
  return cur_addr*PAGE_SIZE + final_offset;
}


void VMinitialize(){
  init_table(0);
}


int VMread(uint64_t virtualAddress, word_t* value){
  if (virtualAddress >= VIRTUAL_MEMORY_SIZE){
    return LIB_FAILURE;
  }
  auto frame_addr = find_frame(virtualAddress);
  PMread (frame_addr, value);
  return LIB_SUCCESS;
}


int VMwrite(uint64_t virtualAddress, word_t value){
  if (virtualAddress >= VIRTUAL_MEMORY_SIZE){
    return LIB_FAILURE;
  }
  auto frame_addr = find_frame(virtualAddress);
  PMwrite (frame_addr, value);
  return LIB_SUCCESS;
}
