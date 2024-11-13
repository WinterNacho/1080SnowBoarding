#include "Ogre.h"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreFileSystemLayer.h>
#include "rapidcsv.h"
#include <stdlib.h>
#include "rider.h"
#include "ramp.h"
#include "tree.h"
#include "utils.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>


class SnowBoarding : public OgreBites::ApplicationContext, public OgreBites::InputListener {
public:
    SnowBoarding();
    virtual ~SnowBoarding() {};

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	Ogre::SceneNode* camNode; 
	Ogre::SceneNode* RiderNode;
	float moveSpeed; 
	float elapsedTime = 0.0f;
	float gameTimeLimit = 30.0f;

	bool soundPlayed = false; 
	float waitStartTime = 0.0f; 
	float soundDuration = 3.0f;     
	bool gameOver = false;

	// miniaudio variables
	ma_engine engine;  // Audio engine
	ma_sound music;
	ma_sound collisionSound;
	ma_sound rampSound;
	ma_sound winSound;
	ma_sound loseSound;

};

SnowBoarding::SnowBoarding() : OgreBites::ApplicationContext("SnowBoarding"), moveSpeed(5.0f) {}

void SnowBoarding::setup() {

	// InputListener
	OgreBites::ApplicationContext::setup();
	addInputListener(this);

	// Pointer to the root
	Ogre::Root* root = getRoot();
	Ogre::SceneManager* scnMgr = root->createSceneManager();
	scnMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

	// Register the scene with the RTShaderSystem
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// Initialize miniaudio engine
	ma_engine_init(NULL, &engine);
	ma_sound_init_from_file(&engine, "C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/audio/silly-pups-in-snow.mp3", 0, NULL, NULL, &music);
	ma_sound_init_from_file(&engine, "C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/audio/mixkit-short-explosion.wav", 0, NULL, NULL, &collisionSound);
	ma_sound_init_from_file(&engine, "C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/audio/mixkit-retro-game-notification.wav", 0, NULL, NULL, &rampSound);
	ma_sound_init_from_file(&engine, "C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/audio/mixkit-cheering-crowd-loud-whistle.wav", 0, NULL, NULL, &winSound);
	ma_sound_init_from_file(&engine, "C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/audio/mixkit-player-losing-or-failing.wav", 0, NULL, NULL, &loseSound);
	// Play the music

	ma_sound_start(&music);
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
	lightNode->setPosition(-20, 40, 0);
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
	Ogre::Entity* playerEntity = scnMgr->createEntity("player", "Penguin.obj");
	//playerEntity->setMaterialName("Penguin");
	Ogre::SceneNode* playerNode = scnMgr->createSceneNode();
	RiderNode = playerNode;
	Rider* player = new Rider(playerNode, camNode, &collisionSound, &rampSound);
	
	scnMgr->getRootSceneNode()->addChild(playerNode);
	player->setPosition(Ogre::Vector3(10,0,0));
	playerNode->scale(Ogre::Vector3(0.04, 0.04, 0.04));
	playerNode->attachObject(playerEntity);

	addInputListener(player);
	root->addFrameListener(player);

	// Map
	// Crear un material con una textura
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("GroundMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("snow.jpg");
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Ground
	Ogre::ManualObject* ground = scnMgr->createManualObject("Ground");
	ground->begin("GroundMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	ground->setCastShadows(false);
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
	}

	ground->end();

	// Attach the ground to the scene
	Ogre::SceneNode* groundNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(ground);

	scnMgr->setSkyBox(true, "SnowBoarding/Skybox", 2000);

	// ramp
	Ogre::Entity* ramps[5];
	Ogre::SceneNode* rampssNodes[5];
	Ramp* rampsActors[5];
	std::srand(std::time(0));
	for (int i = 0; i < 4; i++) {
		float posX = 2.0 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 16.0));
		ramps[i] = scnMgr->createEntity("Ramp" + Ogre::StringConverter::toString(i), "ramp.obj");
		ramps[i]->setMaterialName("Basics/Red");
		rampssNodes[i] = scnMgr->getRootSceneNode()->createChildSceneNode("Ramp" + Ogre::StringConverter::toString(i) + "Node");
		rampssNodes[i]->yaw(Ogre::Degree(90));
		rampssNodes[i]->pitch(Ogre::Degree(-90));
		rampsActors[i] = new Ramp(rampssNodes[i], player, Ogre::Vector3(posX, altura(posX, 100 + i * 230)-1, 100 + i * 230));
		rampsActors[i]->setup();
		rampssNodes[i]->attachObject(ramps[i]);
		root->addFrameListener(rampsActors[i]);
	}

	// Tree
	
	Ogre::Entity* trees[20];
	Ogre::SceneNode* treesNodes[20];

	Tree* treesActors[20];
	for (int i = 0; i < 20; i++) {
		float posX = 2.0 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 16.0));
		trees[i] = scnMgr->createEntity("Tree" + Ogre::StringConverter::toString(i), "tree.obj");
		treesNodes[i] = scnMgr->getRootSceneNode()->createChildSceneNode("Tree" + Ogre::StringConverter::toString(i) + "Node");
		treesNodes[i]->scale(Ogre::Vector3(0.5, 0.5, 0.5));
		treesActors[i] = new Tree(treesNodes[i], player, Ogre::Vector3(posX, altura(posX, 150 + i * 50) + 2, 150+i*50));
		treesActors[i]->setup();
		treesNodes[i]->attachObject(trees[i]);
		root->addFrameListener(treesActors[i]);
	}
	
}

bool SnowBoarding::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
		getRoot()->queueEndRendering();
	}
	return true;
}

bool SnowBoarding::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	elapsedTime += evt.timeSinceLastFrame;

	// Calcula el tiempo restante
	float remainingTime = gameTimeLimit - elapsedTime;

	// Caso de perder
	if (remainingTime <= 0.0f && !soundPlayed) {
		std::cout << "PERDISTE" << std::endl;
		ma_sound_stop(&music);
		ma_sound_start(&loseSound);

		// Marca el inicio del temporizador para esperar el sonido
		waitStartTime = elapsedTime;
		soundPlayed = true;  // Marca que el sonido de perder ha sido reproducido
	}

	// Caso de ganar
	else if (RiderNode->getPosition().z >= 1100 && !soundPlayed) {
		std::cout << "GANASTE" << std::endl;
		ma_sound_start(&winSound);

		// Marca el inicio del temporizador para esperar el sonido
		waitStartTime = elapsedTime;
		soundPlayed = true;  // Marca que el sonido de ganar ha sido reproducido
	}

	// Si el sonido ha sido reproducido, espera a que termine antes de finalizar
	if (soundPlayed && (elapsedTime - waitStartTime) >= soundDuration) {
		std::cout << "Terminando el juego..." << std::endl;
		gameOver = true;  // Marca el fin del juego
		return false;  // Detiene el ciclo de renderizado
	}

	// Muestra el tiempo restante en caso de que no se haya ganado ni perdido
	//std::cout << "Tiempo restante: " << remainingTime << " segundos" << std::endl;
	return OgreBites::ApplicationContext::frameRenderingQueued(evt);
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
