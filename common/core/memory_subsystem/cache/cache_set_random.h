#ifndef CACHE_SET_RANDOM_H
#define CACHE_SET_RANDOM_H

#include "cache_set.h"

class CacheSetRandom : public CacheSet
{
   public:
      CacheSetRandom(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize);
      ~CacheSetRandom();

      UInt32 getReplacementIndex(CacheCntlr *cntlr, bool isStruc = false, UInt32 set_index = 0, UInt64 num_accesses = 0);  //.... neelam: CHANGE!!! added isStruc for separate cache ways; set_index and num_accesses for DRRIP
      void updateReplacementIndex(UInt32 accessed_index, bool isStruc = false);

   private:
      Random m_rand;
};

#endif /* CACHE_SET_RANDOM_H */
