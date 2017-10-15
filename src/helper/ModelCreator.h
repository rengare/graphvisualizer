#ifndef MODEL_CREATOR
#define MODEL_CREATOR

#include <vector>

#include "../config/AppConfig.h"
#include "../graphic/GraphicsStructure.h"
#include "../models/IModel.h"
#include "../models/FR/FRModelCpu.h"
#include "../models/FR/FRModel.h"
#include "../models/random/RandomModel.h"

namespace ModelCreator
{
static IModel *GetModelByType(ModelData *data, AppConfig *config, int type)
{
    switch (type)
    {
    case 0:
        return new FRModel(config, data->nodeData, data->edgeData, data->fromToConnections);
    case 1:
        return new FRModelCpu(config, data->nodeData, data->edgeData, data->fromToConnections);
    case 2:
        return new RandomModel(config, data->nodeData, data->edgeData, data->fromToConnections);
    }

}
};

#endif