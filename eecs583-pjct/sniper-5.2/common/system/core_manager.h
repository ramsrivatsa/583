#ifndef CORE_MANAGER_H
#define CORE_MANAGER_H

#include "fixed_types.h"
#include "tls.h"
#include "lock.h"
#include "log.h"
#include "core.h"
#include "performance_model.h"
#include "dynamic_instruction_info.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <mutex>

struct DynamicInstructionInfo;
class Core;

class CoreManager
{
   public:
      CoreManager();
      ~CoreManager();

      enum ThreadType {
          INVALID,
          APP_THREAD,   // Application (Pin) thread
          CORE_THREAD,  // Core (Performance model) thread
          SIM_THREAD    // Simulator (Network model) thread
      };

      std::vector<Core *> * getM_Cores();
      bool checkMemAccesses(Core * core);
      void initializeCommId(SInt32 comm_id);
      void initializeThread(core_id_t core_id);
      void terminateThread();
      core_id_t registerSimThread(ThreadType type);

      core_id_t getCurrentCoreID(int threadIndex = -1) // id of currently active core (or INVALID_CORE_ID)
      {
         Core *core = getCurrentCore(threadIndex);
         if (!core)
             return INVALID_CORE_ID;
         else
             return core->getId();
      }
      Core *getCurrentCore(int threadIndex = -1)
      {
          return m_core_tls->getPtr<Core>(threadIndex);
      }

      Core *getCoreFromID(core_id_t core_id);

      bool amiUserThread();
      bool amiCoreThread();
      bool amiSimThread();

      void lockMemMaps();
      void unlockMemMaps();

   private:

      UInt32 *tid_map;
      TLS *m_core_tls;
      TLS *m_thread_type_tls;

      UInt32 m_num_registered_sim_threads;
      UInt32 m_num_registered_core_threads;
      Lock m_num_registered_threads_lock;

      std::vector<Core*> m_cores;
      std::mutex memMapsLock;
};

#endif
