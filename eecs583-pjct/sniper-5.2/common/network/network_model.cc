#include <cassert>

#include "network.h"
#include "network_types.h"

#include "network_model_magic.h"
#include "network_model_emesh_hop_counter.h"
#include "network_model_emesh_hop_by_hop.h"
#include "network_model_bus.h"
#include "log.h"

NetworkModel::NetworkModel(Network *network) :
   _network(network)
{}

NetworkModel*
NetworkModel::createModel(Network *net, UInt32 model_type, EStaticNetwork net_type)
{
   switch (model_type)
   {
   case NETWORK_MAGIC:
      return new NetworkModelMagic(net);

   case NETWORK_EMESH_HOP_COUNTER:
      return new NetworkModelEMeshHopCounter(net);

   case NETWORK_EMESH_HOP_BY_HOP:
      return new NetworkModelEMeshHopByHop(net, net_type);

   case NETWORK_BUS:
      return new NetworkModelBus(net, net_type);

   default:
      assert(false);
      return NULL;
   }
}

UInt32
NetworkModel::parseNetworkType(String str)
{
   if (str == "magic")
      return NETWORK_MAGIC;
   else if (str == "emesh_hop_counter")
      return NETWORK_EMESH_HOP_COUNTER;
   else if (str == "emesh_hop_by_hop")
      return NETWORK_EMESH_HOP_BY_HOP;
   else if (str == "bus")
      return NETWORK_BUS;
   else
      return (UInt32)-1;
}

std::pair<bool,SInt32>
NetworkModel::computeCoreCountConstraints(UInt32 network_type, SInt32 core_count)
{
   switch (network_type)
   {
      case NETWORK_MAGIC:
      case NETWORK_EMESH_HOP_COUNTER:
      case NETWORK_BUS:
         return std::make_pair(false,core_count);

      case NETWORK_EMESH_HOP_BY_HOP:
         return NetworkModelEMeshHopByHop::computeCoreCountConstraints(core_count);

      default:
         LOG_PRINT_ERROR("Unrecognized network type(%u)", network_type);
         return std::make_pair(false,-1);
   }
}

std::pair<bool, std::vector<core_id_t> >
NetworkModel::computeMemoryControllerPositions(UInt32 network_type, SInt32 num_memory_controllers, SInt32 core_count)
{
   switch (network_type)
   {
      case NETWORK_MAGIC:
      case NETWORK_EMESH_HOP_COUNTER:
      case NETWORK_BUS:
         {
            SInt32 spacing_between_memory_controllers = core_count / num_memory_controllers;
            std::vector<core_id_t> core_list_with_memory_controllers;
            for (core_id_t i = 0; i < num_memory_controllers; i++)
            {
               assert((i*spacing_between_memory_controllers) < core_count);
               core_list_with_memory_controllers.push_back(i * spacing_between_memory_controllers);
            }

            return std::make_pair(false, core_list_with_memory_controllers);
         }

      case NETWORK_EMESH_HOP_BY_HOP:
         return NetworkModelEMeshHopByHop::computeMemoryControllerPositions(num_memory_controllers, core_count);

      default:
         LOG_PRINT_ERROR("Unrecognized network type(%u)", network_type);
         return std::make_pair(false, std::vector<core_id_t>());
   }
}
