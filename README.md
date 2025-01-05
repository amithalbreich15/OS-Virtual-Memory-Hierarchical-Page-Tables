This project involves implementing a virtual memory management system using hierarchical page tables, simulating physical memory. It is part of an operating system exercise focused on understanding how virtual addresses are mapped to physical addresses through paging and the challenges involved in efficiently handling memory mapping for processes.

### Project Description:

In the given task, you're required to design and implement a virtual memory interface that utilizes hierarchical page tables for address translation. This hierarchical approach is more memory-efficient than a flat page table by using a tree structure of smaller tables, allowing the mapping of virtual memory addresses to physical memory frames. 

The project requires you to implement the functions in `VirtualMemory.h`, using the physical memory API from `PhysicalMemory.h` and following the constraints mentioned (like avoiding dynamic memory allocation and global variables). You will simulate address translations, swapping in and out pages from physical memory, and managing hierarchical page tables to handle memory efficiently. The implementation needs to support different memory sizes and tree depths, with the address translation involving multiple steps based on the page tables' depth.

You'll be tasked with ensuring that pages are loaded, swapped in, or evicted correctly when needed, according to the defined rules, such as finding unused frames or handling page faults. The solution must be implemented with a focus on memory efficiency and correctness, and the project will be tested with varying memory configurations and tree depths.
1. **Virtual Memory System**: Mapping virtual addresses to physical memory using page tables.
2. **Hierarchical Page Tables**: A tree structure that reduces memory consumption by dividing the address space into layers.
3. **Physical Memory API**: A simulated physical memory API to interact with the physical memory, including reading and writing data.
4. **Page Fault Handling**: Managing the swapping in and evicting of pages as needed.
5. **Memory Constraints**: Implementing the solution without using dynamic memory allocation or global variables.

### Assignment Requirements:
- Implement the functions in `VirtualMemory.h` using the specified hierarchical page table design.
- Ensure the implementation works with various memory sizes and page configurations.
- Avoid using global variables or dynamic memory allocation (e.g., no STL containers).
- Create a Makefile to build the library `libVirtualMemory.a`.
- Submit the source files, a README, and the Makefile.

Skills gained: Hands-on experience with memory management techniques such as paging, page fault handling, and memory efficiency in the context of virtual memory.
