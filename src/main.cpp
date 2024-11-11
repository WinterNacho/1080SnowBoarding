#include "Ogre.h"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreFileSystemLayer.h>
#include "rapidcsv.h"
#include <stdlib.h>
#include "rider.h"
#include "ramp.h"
#include "utils.h"

class SnowBoarding : public OgreBites::ApplicationContext, public OgreBites::InputListener {
public:
    SnowBoarding();
    virtual ~SnowBoarding() {};

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
private:
	Ogre::SceneNode* camNode; // Almacena el nodo de la cámara
	float moveSpeed; // Velocidad de movimiento de la cámara
};

SnowBoarding::SnowBoarding() : OgreBites::ApplicationContext("SnowBoarding"), moveSpeed(5.0f) {}

void SnowBoarding::setup() {

	// InputListener
	OgreBites::ApplicationContext::setup();
	addInputListener(this);

	// Pointer to the root
	Ogre::Root* root = getRoot();
	Ogre::SceneManager* scnMgr = root->createSceneManager();

	// Register the scene with the RTShaderSystem
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// light
	Ogre::Light* light = scnMgr->createLight("MainLight");
	light->setType(Ogre::Light::LightTypes::LT_POINT);
	light->setCastShadows(true);

	// Ajusta la intensidad de la luz
	light->setDiffuseColour(0.3f, 0.3f, 0.3f); // Color difuso ligeramente menor
	light->setSpecularColour(0.1f, 0.1f, 0.1f); // Color especular (blanco brillante)
	light->setPowerScale(5.0f); // Aumenta la intensidad de la luz


	Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setDirection(0, -1, 0);
	lightNode->setPosition(10, 40, -10);
	lightNode->attachObject(light);

	// Camera
	Ogre::Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(0.1);
	cam->setAutoAspectRatio(false);
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->yaw(Ogre::Degree(180));
	camNode->pitch(Ogre::Degree(-15));
	camNode->setPosition(0, 250, -40);
	camNode->attachObject(cam);

	Ogre::Viewport* viewport = getRenderWindow()->addViewport(cam);
	viewport->setBackgroundColour(Ogre::ColourValue(0.1f, 0.2f, 0.5f));
	
	// player
	Ogre::Entity* playerEntity = scnMgr->createEntity("player", "brick.obj");
	Ogre::SceneNode* playerNode = scnMgr->createSceneNode();
	Rider* player = new Rider(playerNode, camNode);
	
	scnMgr->getRootSceneNode()->addChild(playerNode);
	player->setPosition(Ogre::Vector3(10,0,0));
	playerNode->pitch(Ogre::Degree(90));
	playerNode->scale(Ogre::Vector3(0.5, 0.5, 0.5));
	playerNode->attachObject(playerEntity);

	addInputListener(player);
	root->addFrameListener(player);

	// Map
	// Crear un material con una textura
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("GroundMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("grass.jpg");
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Ground
	Ogre::ManualObject* ground = scnMgr->createManualObject("Ground");
	ground->begin("GroundMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);


	rapidcsv::Document doc("C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/map/map.csv");

	std::vector<float> xCoords = doc.GetColumn<float>("x");
	std::vector<float> yCoords = doc.GetColumn<float>("y");
	std::vector<float> zCoords = doc.GetColumn<float>("z");

	int numColumnas = 120;
	int numFilas = 8;

	for (size_t i = 0; i < xCoords.size(); ++i) {
		ground->position(xCoords[i], yCoords[i], zCoords[i]);
		float u = (float)(i % numFilas) / (numFilas - 1);
		float v = (float)(i / numColumnas) / (numColumnas - 1);
		ground->textureCoord(u, v);
	}
	int numVertices = xCoords.size();

	for (int i = 0; i < numVertices; i++) {
		if (((i + 1) % numFilas == 0) or (i > numVertices - numFilas-1)) {
			continue;
		}
		ground->triangle(i, i+numFilas, i+1);
		ground->triangle(i+1, i+numFilas, i+numFilas+1);
		std::cout << i << " " << i + numFilas << " " << i + 1 << std::endl;
		std::cout << i+1 << " " << i + numFilas << " " << i + numFilas + 1 << std::endl;
	}

	ground->end();

	// Attach the ground to the scene
	Ogre::SceneNode* groundNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(ground);

	scnMgr->setSkyBox(true, "skybox", 100);


	// ramp
	Ogre::Entity* ramp1Entity = scnMgr->createEntity("ramp1", "ramp.obj");
	Ogre::SceneNode* ramp1Node = scnMgr->createSceneNode();
	ramp1Node->yaw(Ogre::Degree(90));
	ramp1Node->pitch(Ogre::Degree(-90));
	Ramp* ramp1 = new Ramp(ramp1Node, player, Ogre::Vector3(10, altura(10, 70), 70));
	ramp1->setup();
	scnMgr->getRootSceneNode()->addChild(ramp1Node);
	//player->setPosition(Ogre::Vector3(10, 0, 0));
	//playerNode->pitch(Ogre::Degree(90));
	//playerNode->scale(Ogre::Vector3(0.5, 0.5, 0.5));
	ramp1Node->attachObject(ramp1Entity);

	//addInputListener(player);
	root->addFrameListener(ramp1);

}

bool SnowBoarding::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
		getRoot()->queueEndRendering();
	}
	return true;
}

int main() {
	try {
		SnowBoarding app;
		app.initApp();
		app.getRoot()->startRendering();
		app.closeApp();
	}
	catch (Ogre::Exception& e) {
		Ogre::LogManager::getSingleton().logMessage(e.what());
	}
    return 0;
}
