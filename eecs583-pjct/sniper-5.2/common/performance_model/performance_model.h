#ifndef PERFORMANCE_MODEL_H
#define PERFORMANCE_MODEL_H
// This class represents the actual performance model for a given core

#include "dvfs_manager.h"
#include "thread_manager.h"
#include "thread.h"
#include "instruction.h"
#include "basic_block.h"
#include "fixed_types.h"
#include "mt_circular_queue.h"
#include "lock.h"
#include "dynamic_instruction_info.h"
#include "subsecond_time.h"
#include <set>
#include <queue>
#include <map>
#include <stdio.h>
#include <math.h>
#include <utility>
#include <iostream>
#include "sim_api.h"
#include "chopper_handler.h"
#include "interpret_handler.h"
#include "adaptive_retry_interpret_handler.h"
#include "adaptive_retry_chopper_handler.h"
#include "retry_handler.h"
#include <mutex>
#include "mem_count_handler.h"
#include "probability_handler.h"

// Forward Decls
typedef uintptr_t IntPtr;
struct DynamicInstructionInfo;
class ChopperHandler;
class InterpretHandler;
class AdaptiveRetryInterpretHandler;
class AdaptiveRetryChopperHandler;
class MemCountHandler;
class ProbabilityHandler;
class Core;
class BranchPredictor;
class FastforwardPerformanceModel;
class InstructionTracer;

class PerformanceModel
{
public:
   // code for 583 eecs
   std::map<uint64_t, std::set<Operand*>> *getMemoryAccesses();
   std::queue<Instruction *> *getMemoryInstructions();
   ChopperHandler *getChopperHandler();
   AdaptiveRetryChopperHandler *getAdaptiveChopperHandler();
   InterpretHandler *getInterpretHandler();
   AdaptiveRetryInterpretHandler *getAdaptiveInterpretHandler();
   MemCountHandler *getMemCountHandler();
   ProbabilityHandler *getProbabilityHandler();
     // code of reecs 583


   static const SubsecondTime DyninsninfoNotAvailable() { return SubsecondTime::MaxTime(); }

   PerformanceModel(Core* core);
   virtual ~PerformanceModel();
   void queueDynamicInstruction(Instruction *i);
   void queueBasicBlock(BasicBlock *basic_block);
   void handleIdleInstruction(Instruction *instruction);
   void commit();
   void resetMemMap();
   void iterate();
   SubsecondTime getCost();
   virtual void synchronize();

   UInt64 getInstructionCount() const { return m_instruction_count; }

   SubsecondTime getElapsedTime() const { return m_elapsed_time.getElapsedTime(); }
   SubsecondTime getNonIdleElapsedTime() const { return getElapsedTime() - m_idle_elapsed_time.getElapsedTime(); }

   void countInstructions(IntPtr address, UInt32 count);
   void pushDynamicInstructionInfo(DynamicInstructionInfo &i);
   void popDynamicInstructionInfo();
   DynamicInstructionInfo* getDynamicInstructionInfo(const Instruction &instruction, bool exec_loads = true);

   static PerformanceModel *create(Core* core);

   BranchPredictor *getBranchPredictor() { return m_bp; }
   BranchPredictor const* getConstBranchPredictor() const { return m_bp; }

   FastforwardPerformanceModel *getFastforwardPerformanceModel() { return m_fastforward_model; }
   FastforwardPerformanceModel const* getFastforwardPerformanceModel() const { return m_fastforward_model; }

   virtual void barrierEnter() { }
   virtual void barrierExit() { }

   void disable();
   void enable();
   bool isEnabled() { return m_enabled; }
   void setHold(bool hold) { m_hold = hold; }

   bool isFastForward() { return m_fastforward; }
   void setFastForward(bool fastforward, bool detailed_sync = true)
   {
      m_fastforward = fastforward;
      m_detailed_sync = detailed_sync;
      // Fastforward performance model has controlled time for a while, now let the detailed model know time has advanced
      if (fastforward == false)
         notifyElapsedTimeUpdate();
   }

   int getNumMemReads();
   int getNumMemWrites();
   int getTotalMemAccesses();

   int getCommitCount();

protected:
   friend class SpawnInstruction;
   friend class FastforwardPerformanceModel;

   void setElapsedTime(SubsecondTime time);
   void incrementElapsedTime(SubsecondTime time) { m_elapsed_time.addLatency(time); }
   void incrementIdleElapsedTime(SubsecondTime time);

   #ifdef ENABLE_PERF_MODEL_OWN_THREAD
      typedef MTCircularQueue<DynamicInstructionInfo> DynamicInstructionInfoQueue;
      typedef MTCircularQueue<BasicBlock *> BasicBlockQueue;
   #else
      typedef CircularQueue<DynamicInstructionInfo> DynamicInstructionInfoQueue;
      typedef CircularQueue<BasicBlock *> BasicBlockQueue;
   #endif

   Core* getCore() { return m_core; }

private:

   DynamicInstructionInfo* getDynamicInstructionInfo();
   ChopperHandler* chopperHandler;
   AdaptiveRetryChopperHandler* aChopperHandler;
   InterpretHandler* interpretHandler;
   AdaptiveRetryInterpretHandler* aInterpretHandler;
   MemCountHandler *memCountHandler;
   ProbabilityHandler *probabilityHandler;

   // Simulate a single instruction
   virtual bool handleInstruction(Instruction const* instruction) = 0;

   // When time is jumped ahead outside of control of the performance model (synchronization instructions, etc.)
   // notify it here. This may be used to synchronize internal time or to flush various instruction queues
   virtual void notifyElapsedTimeUpdate() {}

   Core* m_core;

   bool m_enabled;

   bool m_fastforward;
   FastforwardPerformanceModel* m_fastforward_model;
   bool m_detailed_sync;

   bool m_hold;


protected:
   UInt64 m_instruction_count;

   ComponentTime m_elapsed_time;
private:
   ComponentTime m_idle_elapsed_time;

   SubsecondTime m_cpiStartTime;
   // CPI components for Sync and Recv instructions
   SubsecondTime m_cpiSyncFutex;
   SubsecondTime m_cpiSyncPthreadMutex;
   SubsecondTime m_cpiSyncPthreadCond;
   SubsecondTime m_cpiSyncPthreadBarrier;
   SubsecondTime m_cpiSyncJoin;
   SubsecondTime m_cpiSyncPause;
   SubsecondTime m_cpiSyncSleep;
   SubsecondTime m_cpiSyncUnscheduled;
   SubsecondTime m_cpiSyncDvfsTransition;
   SubsecondTime m_cpiRecv;

   BasicBlockQueue m_basic_block_queue;
   DynamicInstructionInfoQueue m_dynamic_info_queue;

   UInt32 m_current_ins_index;

   BranchPredictor *m_bp;

   InstructionTracer *m_instruction_tracer;
    
    std::map<uint64_t, std::set<Operand*>> memoryAccesses;
    int count;
    SubsecondTime totalExecCost;

    int commitCount;

    std::queue<Instruction *> memoryInstructions;
    // int regionSize; // needs to be initialized
};

#endif
