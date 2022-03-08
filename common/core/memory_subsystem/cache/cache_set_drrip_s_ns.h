#ifndef CACHE_SET_DRRIP_H
#define CACHE_SET_DRRIP_H

#include "cache_set.h"
#include "cache_set_lru.h"
class CacheSetDRRIP_S_NS : public CacheSet
{
   public:
      CacheSetDRRIP_S_NS(String cfgname, core_id_t core_id,
            CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetInfoLRU* set_info, UInt8 num_attempts);
      ~CacheSetDRRIP_S_NS();

      UInt32 getReplacementIndex(CacheCntlr *cntlr, bool isStruc, UInt32 set_index, UInt64 num_accesses);
      void updateReplacementIndex(UInt32 accessed_index, bool isStruc);
      int check_leader(UInt32 set_id);
   private:
      const UInt8 m_rrip_numbits;
      const UInt8 m_rrip_max;
      const UInt8 m_rrip_insert;
      const UInt8 m_num_attempts;
      UInt8* m_rrip_bits;
      UInt8  m_replacement_pointer;
      CacheSetInfoLRU* m_set_info;
};

#endif 
