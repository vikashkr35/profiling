* Branch table contains profiling implementation using 6 counter. This is earlier implementation.
* Branch next_version contains implementation using 12 counter. This is the last implementation.  
* 
* result link-https://docs.google.com/spreadsheets/d/1kRDLraF-WmhtKuCIVBHbLXUUYhqlBc_GVbvZDvsWyHw/edit?usp=sharing
# profiling
There are 3 branches.
branch function_way
  1.Used a function accessSingleLine for checking the avilability of property data in cache.
  2.Addedd counter for stats in file profiling/common/core/memory_subsystem/parametric_dram_directory_msi/cache_cntlr.cc at line no 671.
branch Table 
  1.Used a edge table to make entry of edge data. When property data access comes it will calculate edge data first and then check the table and then increment the
    corresponding counter. 
  2..Addedd counter for stats in file profiling/common/core/memory_subsystem/parametric_dram_directory_msi/cache_cntlr.cc at line no 669.

