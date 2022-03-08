#include "cache_set_lru_s_ns.h"
#include "log.h"
#include "stats.h"

using namespace std;

// Implements LRU replacement for structure and non-structure data, optionally augmented with Query-Based Selection [Jaleel et al., MICRO'10]

CacheSetLRU_S_NS::CacheSetLRU_S_NS(
      CacheBase::cache_t cache_type,
      UInt32 associativity, UInt32 blocksize, CacheSetLRU_S_NS_Info* set_info, UInt8 num_attempts)
   : CacheSet(cache_type, associativity, blocksize)
   , m_num_attempts(num_attempts)
   , m_set_info(set_info)
{
   m_lru_bits = new UInt8[m_associativity];
   for (UInt32 i = 0; i < m_associativity; i++)
      m_lru_bits[i] = i%(m_associativity/2);
}

//................ helper print function ............
void 
CacheSetLRU_S_NS::printStatus(UInt32 accessed_index, bool isStruc)
{
   cout << "index: " << accessed_index << " struc: " << isStruc << endl;
}
//...................................................

CacheSetLRU_S_NS::~CacheSetLRU_S_NS()
{
   delete [] m_lru_bits;
}

UInt32
CacheSetLRU_S_NS::getReplacementIndex(CacheCntlr *cntlr, bool isStruc, UInt32 set_index, UInt64 num_accesses)
{
   // First try to find an invalid block
   UInt32 j = isStruc ? (m_associativity/2):0;
   UInt32 last_associativity = isStruc ? m_associativity : (m_associativity/2);
   for (; j < last_associativity; j++)
   {
      if (!m_cache_block_info_array[j]->isValid())
      {
         // Mark our newly-inserted line as most-recently used
         // cout << "getReplacementIndex(): isStruc: " << isStruc << " index: " << j << endl;
         moveToMRU(j, isStruc);
         if(isStruc)
         {
            assert(j < m_associativity);
            assert(j >= (m_associativity/2));   //assert to make sure that the index for structure data is [(m_associativity/2), m_associativity)
         }
         else{
            assert(j < (m_associativity/2));    //assert to make sure that the index for structure data is [0,(m_associativity/2))
         }
         // printStatus(j, isStruc);
         return j;
      }
   }

   // Make m_num_attemps attempts at evicting the block at LRU position
   for(UInt8 attempt = 0; attempt < m_num_attempts; ++attempt)
   {
      UInt32 index = isStruc ? (m_associativity/2) : 0;
      UInt8 max_bits = 0;
      UInt32 i = isStruc ? (m_associativity/2) : 0;
      for (; i < last_associativity; i++)
      {
         if (m_lru_bits[i] > max_bits && isValidReplacement(i))
         {
            index = i;
            max_bits = m_lru_bits[i];
         }
      }
      LOG_ASSERT_ERROR(index < m_associativity, "Error Finding LRU bits");

      bool qbs_reject = false;
      if (attempt < m_num_attempts - 1)
      {
         LOG_ASSERT_ERROR(cntlr != NULL, "CacheCntlr == NULL, QBS can only be used when cntlr is passed in");
         qbs_reject = cntlr->isInLowerLevelCache(m_cache_block_info_array[index]);
      }

      if (qbs_reject)
      {
         // Block is contained in lower-level cache, and we have more tries remaining.
         // Move this block to MRU and try again
         moveToMRU(index, isStruc);
         cntlr->incrementQBSLookupCost();
         continue;
      }
      else
      {
         // Mark our newly-inserted line as most-recently used
         moveToMRU(index, isStruc);
         m_set_info->incrementAttempt(attempt);
         // cout << "getReplacementIndex(): isStruc: " << isStruc << " index: " << index << endl;
         if(isStruc)
         {
            assert(index < m_associativity);
            assert(index >= (m_associativity/2));  //assert to make sure that the index for structure data is [(m_associativity/2), m_associativity)
         }
         else{
            assert(index < (m_associativity/2));   //assert to make sure that the index for structure data is [0,(m_associativity/2))
         }
         // printStatus(index, isStruc);
         return index;
      }
   }

   LOG_PRINT_ERROR("Should not reach here");
}

void
CacheSetLRU_S_NS::updateReplacementIndex(UInt32 accessed_index, bool isStruc)
{
   m_set_info->increment(m_lru_bits[accessed_index]);
   moveToMRU(accessed_index, isStruc);
}

void
CacheSetLRU_S_NS::moveToMRU(UInt32 accessed_index, bool isStruc)
{
   UInt32 i = isStruc ? (m_associativity/2) : 0;
   UInt32 last_associativity = isStruc ? m_associativity : (m_associativity/2);

   for (; i < last_associativity; i++)
   {
      if (m_lru_bits[i] < m_lru_bits[accessed_index]){
         m_lru_bits[i] ++;
         assert(m_lru_bits[i] < (m_associativity/2)); //assert to make sure lru counter value never exceeds m_associativity/2
      }
   }
   m_lru_bits[accessed_index] = 0;
}

CacheSetLRU_S_NS_Info::CacheSetLRU_S_NS_Info(String name, String cfgname, core_id_t core_id, UInt32 associativity, UInt8 num_attempts)
   : m_associativity(associativity)
   , m_attempts(NULL)
{
   m_access = new UInt64[m_associativity];
   for(UInt32 i = 0; i < m_associativity; ++i)
   {
      m_access[i] = 0;
      registerStatsMetric(name, core_id, String("access-mru-")+itostr(i), &m_access[i]);
   }

   if (num_attempts > 1)
   {
      m_attempts = new UInt64[num_attempts];
      for(UInt32 i = 0; i < num_attempts; ++i)
      {
         m_attempts[i] = 0;
         registerStatsMetric(name, core_id, String("qbs-attempt-")+itostr(i), &m_attempts[i]);
      }
   }
};

CacheSetLRU_S_NS_Info::~CacheSetLRU_S_NS_Info()
{
   delete [] m_access;
   if (m_attempts)
      delete [] m_attempts;
}
