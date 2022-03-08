#ifndef CACHE_SET_LRU_S_NS_H
#define CACHE_SET_LRU_S_NS_H

#include "cache_set.h"

class CacheSetLRU_S_NS_Info : public CacheSetInfo
{
   public:
      CacheSetLRU_S_NS_Info(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts);
      virtual ~CacheSetLRU_S_NS_Info();
      void increment(UInt32 index)
      {
         LOG_ASSERT_ERROR(index < m_associativity, "Index(%d) >= Associativity(%d)", index, m_associativity);
         ++m_access[index];
      }
      void incrementAttempt(UInt8 attempt)
      {
         if (m_attempts)
            ++m_attempts[attempt];
         else
            LOG_ASSERT_ERROR(attempt == 0, "No place to store attempt# histogram but attempt != 0");
      }
   private:
      const UInt32 m_associativity;
      UInt64* m_access;
      UInt64* m_attempts;
};

class CacheSetLRU_S_NS : public CacheSet
{
   public:
      CacheSetLRU_S_NS(CacheBase::cache_t cache_type,
            UInt32 associativity, UInt32 blocksize, CacheSetLRU_S_NS_Info* set_info, UInt8 num_attempts);
      virtual ~CacheSetLRU_S_NS();

      virtual UInt32 getReplacementIndex(CacheCntlr *cntlr, bool isStruc=false, UInt32 set_index = 0, UInt64 num_accesses = 0);   //.... neelam: CHANGE!!! added isStruc for separate cache ways; set_index and num_accesses for DRRIP
      void updateReplacementIndex(UInt32 accessed_index, bool isStruc=false); //neelam: CHANGE!!! added isStruc

   protected:
      const UInt8 m_num_attempts;
      UInt8* m_lru_bits;
      CacheSetLRU_S_NS_Info* m_set_info;
      void moveToMRU(UInt32 accessed_index, bool isStruc=false);
      //............... helper print function ..................
      void printStatus(UInt32 accesses_index, bool isStruc);
      //........................................................
};

#endif /* CACHE_SET_LRU_H */
