// irrgrassEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <irrlicht.h>
#include "TKGrassSceneNode.h"
#include "TKGrassSceneNode_ShaderCB.h"

using namespace irr;using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

int main(int argc, char** argv)
{
	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, true, 0);

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100, 0.1);

	ISceneNode* ground = smgr->addCubeSceneNode();
	ground->setScale(vector3df(20, 0.01, 20));
	ground->setMaterialFlag(EMF_LIGHTING, false);
	ground->setMaterialTexture(0, driver->getTexture("../media/ground.jpg"));
	ground->getMaterial(0).getTextureMatrix(0).setScale(vector3df(50, 50, 50));


	TKGrassShaderCallBack* grassShaderCB = new TKGrassShaderCallBack();

	s32 materialGrass = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		"../media/grass.vert", "vertexMain", video::EVST_VS_1_1,
		"../media/grass.frag", "pixelMain", video::EPST_PS_1_1,
		grassShaderCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);


	for (int i = 0; i<20; i++)
	for (int j = 0; j<20; j++)
	{
		TKGrassSceneNode* temp = new TKGrassSceneNode(smgr->getRootSceneNode(), smgr, -1, materialGrass, 25);
		temp->setPosition(vector3df(i + (rand() % 10)*0.1, 0, j + (rand() % 10)*0.1));
	}


	while (device->run())
	{
		driver->beginScene(true, true, SColor(0, 0, 0, 0));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}
