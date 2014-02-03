#include <sched.h>
#include <linux/unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <limits.h>
#include <algorithm>
#include "simulator.h"
#include <vector>
#include "core_manager.h"
#include "core.h"
#include "network.h"
#include "cache.h"
#include "config.h"

#include "log.h"

CoreManager::CoreManager()
      : m_core_tls(TLS::create())
      , m_thread_type_tls(TLS::create())
      , m_num_registered_sim_threads(0)
      , m_num_registered_core_threads(0)
{
   LOG_PRINT("Starting CoreManager Constructor.");

   for (UInt32 i = 0; i < Config::getSingleton()->getTotalCores(); i++)
   {
      m_cores.push_back(new Core(i));
   }

   LOG_PRINT("Finished CoreManager Constructor.");
}

CoreManager::~CoreManager()
{
   for (std::vector<Core *>::iterator i = m_cores.begin(); i != m_cores.end(); i++)
      delete *i;

   delete m_core_tls;
   delete m_thread_type_tls;
}

std::vector<Core*> *CoreManager::getM_Cores() {
	return &m_cores;
}

void CoreManager::lockMemMaps() {
	memMapsLock.lock();
}

void CoreManager::unlockMemMaps() {
	memMapsLock.unlock();
}


/*************** BLOCK CHOP COMMENTS***************
*
* Contains the different objects for calling different handling mechanisms. 
* Add an object if you have created a new handling mechanism.
* If not used NEED NOT DELETE. DESTRUCTOR TAKES CARE OF IT
*
***************************************************/


bool CoreManager::checkMemAccesses(Core * currentCore) {
	PerformanceModel * currentModel = currentCore->getPerformanceModel();
	std::map<uint64_t, std::set<Operand*>> *memoryAccesses = currentModel->getMemoryAccesses();
	for(std::map<uint64_t, std::set<Operand*>>::iterator it = memoryAccesses->begin(); it != memoryAccesses->end(); it++) 
	{
		for(std::set<Operand*>::iterator it2 = it->second.begin(); it2!= it->second.end(); it2++)
		{	
			if((*it2)->m_direction == Operand::Direction::WRITE) 
			{
				for(unsigned int i = 0; i < m_cores.size(); i++) 
				{
					Core * otherCore = m_cores[i];
					PerformanceModel * otherModel = otherCore->getPerformanceModel();

					ThreadManager *threadManager = Sim()->getThreadManager();
					 Thread *otherThread = otherCore->getThread();
					if(otherThread!=NULL)
					{	
                        			int threadID = otherThread->getId();
						Core::State threadState = threadManager->getThreadState(threadID);	
						if(otherCore->getId() != currentCore->getId() && otherCore->getState() == Core::State::RUNNING  && threadState!= Core::State::STALLED) 
						{
							std::map<uint64_t,std::set<Operand*>> *otherMemoryAccesses = otherModel->getMemoryAccesses();
							if(otherMemoryAccesses->find(it->first) != otherMemoryAccesses->end()) 
							{
								printf("Write by the current core :: Read by different core\n");//<<std::endl;
								return true;
							}//if in
						}//if out
					}	
				}//2nd for loopy
			}
			else if((*it2)->m_direction == Operand::Direction::READ)
			{
				for(unsigned int i = 0; i < m_cores.size(); i++) 
				{
					Core * otherCore = m_cores[i];
					PerformanceModel * otherModel = otherCore->getPerformanceModel();
					
					ThreadManager *threadManager = Sim()->getThreadManager();
					 Thread *otherThread = otherCore->getThread();
                        		if(otherThread!=NULL)
					{
						int threadID = otherThread->getId();
						Core::State threadState = threadManager->getThreadState(threadID);
						if(otherCore->getId() != currentCore->getId() && otherCore->getState() == Core::State::RUNNING && threadState!= Core::State::STALLED) 
						{
							std::map<uint64_t,std::set<Operand*>> *otherMemoryAccesses = otherModel->getMemoryAccesses();
							
							if(otherMemoryAccesses->find(it->first) != otherMemoryAccesses->end())
							{
								std::set<Operand*> *operandSet = &(*otherMemoryAccesses)[it->first];
								if(operandSet->size() != 0)
								{
									for(std::set<Operand*>::iterator it3 = operandSet->begin(); it3 != operandSet->end(); it3++)
									{
										if((*it3)->m_direction == Operand::Direction::WRITE)
										{
											printf("Read by current core :: Write by different core\n");//<<std::endl;
											return true;
										}
									}	 
			
								}//check for Read in current cores and Write in other cores
							}
						}
					}				
				}//iterate through all the other cores

			}//read operands in current cores else if
			
		}//1st for loop	
		
	
	}//main for loop
	return false;

}

void CoreManager::initializeCommId(SInt32 comm_id)
{
   LOG_PRINT("initializeCommId - current core (id) = %p (%d)", getCurrentCore(), getCurrentCoreID());

   core_id_t core_id = getCurrentCoreID();

   LOG_ASSERT_ERROR(core_id != INVALID_CORE_ID, "Unexpected invalid core id : %d", core_id);

   LOG_PRINT("Initializing comm_id: %d to core_id: %d", comm_id, core_id);

   // Broadcast this update to other processes

   Config::getSingleton()->updateCommToCoreMap(comm_id, core_id);

   LOG_PRINT("Finished.");
}

void CoreManager::initializeThread(core_id_t core_id)
{
   m_core_tls->set(m_cores.at(core_id));
   m_thread_type_tls->setInt(APP_THREAD);

   LOG_PRINT("Initialize thread for core %p (%d)", m_cores.at(core_id), m_cores.at(core_id)->getId());
   LOG_ASSERT_ERROR(m_core_tls->get() == (void*)(m_cores.at(core_id)),
                    "TLS appears to be broken. %p != %p", m_core_tls->get(), (void*)(m_cores.at(core_id)));
}

void CoreManager::terminateThread()
{
   LOG_ASSERT_WARNING(m_core_tls->get() != NULL, "Thread not initialized while terminating.");
   m_core_tls->set(NULL);
}

Core *CoreManager::getCoreFromID(core_id_t id)
{
   LOG_ASSERT_ERROR(id < (core_id_t)Config::getSingleton()->getTotalCores(), "Illegal index in getCoreFromID!");
   return m_cores.at(id);
}

core_id_t CoreManager::registerSimThread(ThreadType type)
{
    if (getCurrentCore() != NULL)
    {
        LOG_PRINT_ERROR("registerSimMemThread - Initialized thread twice");
        return getCurrentCore()->getId();
    }

    ScopedLock sl(m_num_registered_threads_lock);

    UInt32 *num_registered_threads = NULL;
    if (type == SIM_THREAD)
       num_registered_threads = &m_num_registered_sim_threads;
    else if (type == CORE_THREAD)
       num_registered_threads = &m_num_registered_core_threads;
    else
       LOG_ASSERT_ERROR(false, "Unknown thread type %d", type);


    LOG_ASSERT_ERROR(*num_registered_threads < Config::getSingleton()->getTotalCores(),
                     "All sim threads already registered. %d > %d",
                     *num_registered_threads+1, Config::getSingleton()->getTotalCores());

    Core *core = m_cores.at(*num_registered_threads);

    m_core_tls->set(core);
    m_thread_type_tls->setInt(type);

    ++(*num_registered_threads);

    return core->getId();
}

bool CoreManager::amiSimThread()
{
    return m_thread_type_tls->getInt() == SIM_THREAD;
}

bool CoreManager::amiCoreThread()
{
    return m_thread_type_tls->getInt() == CORE_THREAD;
}

bool CoreManager::amiUserThread()
{
    return m_thread_type_tls->getInt() == APP_THREAD;
}
