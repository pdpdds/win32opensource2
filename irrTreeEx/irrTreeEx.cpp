/*
Written by Asger Feldthaus

February 2007
*/

#include "stdafx.h"
#include <irrlicht.h>

#include "CTreeGenerator.h"
#include "CBillboardGroupSceneNode.h"
#include "CTreeSceneNode.h"
#include <time.h>
#include <iostream>

#pragma comment(lib, "Irrlicht.lib")

using namespace irr;
using namespace scene;
using namespace video;

class CEventReceiver : public IEventReceiver
{
public:
	CEventReceiver()
	{
		for (s32 i = 0; i<256; i++)
		{
			Keys[i] = false;
			TappedKeys[i] = false;
		}
	}

	bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_KEY_INPUT_EVENT)
		{
			Keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
			TappedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}
		return false;
	}

	void reset()
	{
		for (s32 i = 0; i<256; i++)
			TappedKeys[i] = false;
	}

	bool Keys[256];
	bool TappedKeys[256];
};

ISceneManager* manager = 0;
video::IVideoDriver* driver = 0;

CTreeGenerator* generator = 0;

CTreeSceneNode* tree = 0;

video::ITexture* billTexture = 0;
video::ITexture* leafTexture = 0;

struct STreeDesignFiles
{
	const c8* DesignFile;
	const c8* TreeTextureFile;
	const c8* LeafTextureFile;
	const c8* BillTextureFile;
};

struct STreeDesign
{
	CTreeGenerator* Generator;
	video::ITexture* TreeTexture;
	video::ITexture* LeafTexture;
	video::ITexture* BillTexture;
};

const s32 NUM_TREE_DESIGNS = 4;

const STreeDesignFiles treeDesignFiles[NUM_TREE_DESIGNS] = {
	{ "./trees/Oak.xml", "./textures/OakBark.png", "./textures/OakLeaf.png", "./textures/OakBillboard.png" },
	{ "./trees/Aspen.xml", "./textures/AspenBark.png", "./textures/AspenLeaf.png", "./textures/AspenBillboard.png" },
	{ "./trees/Pine.xml", "./textures/PineBark.png", "./textures/PineLeaf.png", "./textures/PineBillboard.png" },
	{ "./trees/Willow.xml", "./textures/WillowBark.png", "./textures/WillowLeaf.png", "./textures/WillowBillboard.png" }
};

STreeDesign treeDesigns[NUM_TREE_DESIGNS];

s32 currentTreeDesign = 0;

core::vector3df lightDir;

s32 seed = 0;

video::E_MATERIAL_TYPE leafMaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

bool lightsEnabled = true;

void generateNewTree()
{
	tree->setup(treeDesigns[currentTreeDesign].Generator, seed, treeDesigns[currentTreeDesign].BillTexture);

	tree->getLeafNode()->getMaterial(0).TextureLayer[0].AnisotropicFilter = true;
	tree->getLeafNode()->getMaterial(0).TextureLayer[0].BilinearFilter = false;

	tree->getLeafNode()->setMaterialTexture(0, treeDesigns[currentTreeDesign].LeafTexture);
	tree->getLeafNode()->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

	tree->setMaterialTexture(0, treeDesigns[currentTreeDesign].TreeTexture);

	if (lightsEnabled)
		tree->getLeafNode()->applyVertexShadows(lightDir, 1.0f, 0.25f);

	tree->getLeafNode()->setMaterialType(leafMaterialType);
}

int main()
{
	srand((u32)time(0)); // This is to generate random seeds.

	IrrlichtDevice* device = createDevice(video::EDT_OPENGL);

	CEventReceiver receiver;
	device->setEventReceiver(&receiver);

	manager = device->getSceneManager();
	driver = device->getVideoDriver();

	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	//
	// Load tree designs
	//
	for (s32 i = 0; i<NUM_TREE_DESIGNS; i++)
	{
		treeDesigns[i].Generator = new CTreeGenerator(manager);

		io::IXMLReader* xml = device->getFileSystem()->createXMLReader(treeDesignFiles[i].DesignFile);

		treeDesigns[i].Generator->loadFromXML(xml);

		xml->drop();

		treeDesigns[i].TreeTexture = driver->getTexture(treeDesignFiles[i].TreeTextureFile);

		treeDesigns[i].LeafTexture = driver->getTexture(treeDesignFiles[i].LeafTextureFile);

		treeDesigns[i].BillTexture = driver->getTexture(treeDesignFiles[i].BillTextureFile);
	}

	//
	// Load leaf shader
	//
	leafMaterialType = (video::E_MATERIAL_TYPE) driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		"./shaders/leaves.vert",
		"main",
		EVST_VS_2_0,
		"./shaders/leaves.frag",
		"main",
		EPST_PS_2_0,
		0,
		EMT_TRANSPARENT_ALPHA_CHANNEL,
		0);



	//
	// Tree scene node
	//    
	tree = new CTreeSceneNode(manager->getRootSceneNode(), manager);
	tree->drop();

	tree->setMaterialFlag(video::EMF_LIGHTING, lightsEnabled);


	//
	// Camera
	//
	scene::ICameraSceneNode* camera = manager->addCameraSceneNodeFPS(0, 100, 100);

	camera->setPosition(core::vector3df(23.4f, 233.4f, -150.9f));


	//
	// Light
	//
	scene::ILightSceneNode* light = manager->addLightSceneNode(0, core::vector3df(100, 100, 100), video::SColorf(1, 1, 1, 1), 10000.0f);

	light->getLightData().AmbientColor.set(0.25f, 0.25f, 0.25f, 0.25f);

	lightDir = core::vector3df(-1, -1, -1);
	lightDir.normalize();


	generateNewTree();

	//
	// Interface
	//
	guienv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255, 200, 255, 255));

	guienv->addStaticText(L"By Asger Feldthaus", core::rect<s32>(10, 40, 100, 60));

	guienv->addStaticText(L"F6: Toggle lighting", core::rect<s32>(10, 260, 100, 280));
	guienv->addStaticText(L"F7: Previous tree design", core::rect<s32>(10, 280, 100, 300));
	guienv->addStaticText(L"F8: Next tree design", core::rect<s32>(10, 300, 100, 320));
	guienv->addStaticText(L"F9: Generate new tree", core::rect<s32>(10, 320, 100, 340));

	//
	// Run loop
	//
	while (device->run())
	{
		//
		// Render
		//
		driver->beginScene(true, true, video::SColor(0, 40, 40, 40));

		manager->drawAll();
		guienv->drawAll();

		driver->endScene();


		//
		// Window caption
		//
		wchar_t caption[60];

		s32 tri = driver->getPrimitiveCountDrawn();

		swprintf(&caption[0], 60, L"FPS: %i, Triangles: %i, Vertices: %i", driver->getFPS(), tri, tree->getVertexCount());

		device->setWindowCaption(&caption[0]);


		//
		// Controls
		//
		if (receiver.TappedKeys[KEY_F6])
		{
			lightsEnabled = !lightsEnabled;
			tree->setMaterialFlag(video::EMF_LIGHTING, lightsEnabled);
			if (lightsEnabled)
				tree->getLeafNode()->applyVertexShadows(lightDir, 1.0f, 0.25f);
			else
				tree->getLeafNode()->resetVertexShadows();
		}
		if (receiver.TappedKeys[KEY_F7])
		{
			currentTreeDesign--;
			if (currentTreeDesign < 0)
				currentTreeDesign += NUM_TREE_DESIGNS;
			generateNewTree();
		}
		if (receiver.TappedKeys[KEY_F8])
		{
			currentTreeDesign = (currentTreeDesign + 1) % NUM_TREE_DESIGNS;
			generateNewTree();
		}
		if (receiver.TappedKeys[KEY_F9])
		{
			seed = rand();
			generateNewTree();
		}
		if (receiver.TappedKeys[KEY_ESCAPE])
		{
			device->closeDevice();
		}

		receiver.reset();
	}

	//
	// Clean up
	//
	for (s32 i = 0; i<NUM_TREE_DESIGNS; i++)
	{
		treeDesigns[i].Generator->drop();
	}

	device->drop();

	return 0;
}
