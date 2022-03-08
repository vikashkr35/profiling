#ifndef CACHE_SET_NRU_H
#define CACHE_SET_NRU_H

#include "cache_set.h"

class CacheSetNRU : public CacheSet
{
   public:
      CacheSetNRU(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize);
      ~CacheSetNRU();

      UInt32 getReplacementIndex(CacheCntlr *cntlr, bool isStruc = false, UInt32 set_index = 0, UInt64 num_accesses = 0);  //.... neelam: CHANGE!!! added isStruc for separate cache ways; set_index and num_accesses for DRRIP
      void updateReplacementIndex(UInt32 accessed_index, bool isStruc = false);

   private:
      UInt8* m_lru_bits;
      UInt8  m_num_bits_set;
      UInt8  m_replacement_pointer;
};

#endif /* CACHE_SET_NRU_H */
