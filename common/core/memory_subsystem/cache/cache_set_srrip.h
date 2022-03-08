#ifndef CACHE_SET_SRRIP_H
#define CACHE_SET_SRRIP_H

#include "cache_set.h"
#include "cache_set_lru.h"

class CacheSetSRRIP : public CacheSet
{
   public:
      CacheSetSRRIP(String cfgname, core_id_t core_id,
            CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetInfoLRU* set_info, UInt8 num_attempts);
      ~CacheSetSRRIP();

      UInt32 getReplacementIndex(CacheCntlr *cntlr, bool isStruc = false, UInt32 set_index = 0, UInt64 num_accesses = 0);  //.... neelam: CHANGE!!! added isStruc for separate cache ways; set_index and num_accesses for DRRIP
      void updateReplacementIndex(UInt32 accessed_index, bool isStruc = false);

   private:
      const UInt8 m_rrip_numbits;
      const UInt8 m_rrip_max;
      const UInt8 m_rrip_insert;
      const UInt8 m_num_attempts;
      UInt8* m_rrip_bits;
      UInt8  m_replacement_pointer;
      CacheSetInfoLRU* m_set_info;
};

#endif /* CACHE_SET_H */
