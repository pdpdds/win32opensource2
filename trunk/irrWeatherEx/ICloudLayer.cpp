// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// For conditions of distribution and use, see copyright notice in irrWeatherManager.h

#include "ICloudLayer.h"
#include "ICloudSceneNode.h"
#include <IrrlichtDevice.h>
#include <ISceneManager.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;


ICloudLayer::ICloudLayer(irr::IrrlichtDevice* const irrDevice, const vector3df& layerCenter)
{
    device = irrDevice;

    cloudCount = 0;

    static u32 newCloudLayerID;
    newCloudLayerID++;

    uniqueID = newCloudLayerID;

    IsStormCloudLayer = false;

}

void ICloudLayer::addCloud(const SCloudCreationInfo &info, u32 extraSeed)
{
    // create a cloud node
    ICloudSceneNode* clouds = new ICloudSceneNode(
            device->getSceneManager()->getRootSceneNode(), device->getSceneManager(),
                device->getTimer(), info.ID, info.location, info.rotation, vector3df(1,1,1), info.globalParticleSize);

    video::ITexture* txture = 0;
    if(info.textureNames.size() > 0)
    {
        int num = clouds->rand2(info.textureNames.size(), device->getTimer()->getTime()*extraSeed);

        txture = device->getVideoDriver()->getTexture(info.textureNames[num-1].c_str());
    }

    else
        txture = device->getVideoDriver()->getTexture(info.textureName.c_str());

    // set any old random seed
    srand(device->getTimer()->getRealTime() + extraSeed);

    // set the level of details to draw at what distance
    clouds->setLOD(info.LOD);

    // set the maximum detail level when recursing
    clouds->setMaxDepth(info.maxDepth);


    clouds->setMaterialFlag(video::EMF_LIGHTING, true);


    clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // set any old random seed
    srand(rand());

    // call a "here's one I made earlier" function
    clouds->makeRandomCloud(info.numParticles); //60

    clouds->setScale(info.scale);

    clouds->setMaterialTexture(0, txture );

    cloudCount++;
    Clouds.push_back(clouds);
}

ICloudSceneNode* const ICloudLayer::getCloudByIndex(u32 index) const
{
    list<ICloudSceneNode*>::ConstIterator it = Clouds.begin();

    it += index;
    ICloudSceneNode* cloud = 0;
    cloud = (*it);
    if(cloud != 0)
    {
        return cloud;
    }

    return 0;
}


void ICloudLayer::removeCloud(ICloudSceneNode* const cloud, bool removeFromScene)
{
    if(cloud)
    {
        list<ICloudSceneNode*>::Iterator it = Clouds.begin();

        for(; it != Clouds.end(); )
        {
            ICloudSceneNode* co = (*it);
            if(co == cloud)
            {
                if(removeFromScene)
                    co->remove();
                it = Clouds.erase(it);
                cloudCount--;
            }

            else
                it++;
        }
    }
}

void ICloudLayer::removeAllClouds(bool removeFromScene)
{
    list<ICloudSceneNode*>::Iterator it = Clouds.begin();

    for(; it != Clouds.end(); )
    {
        ICloudSceneNode* cloud = (*it);
        if(removeFromScene)
            cloud->remove();
        it = Clouds.erase(it);
        cloudCount--;
    }
}

u32 ICloudLayer::getCloudCount()
{
    return cloudCount;
}

ICloudLayer::~ICloudLayer()
{
    removeAllClouds(true);
}
