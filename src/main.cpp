#include "Ogre.h"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreFileSystemLayer.h>
#include "rapidcsv.h"
#include <stdlib.h>
#include "rider.h"

class SnowBoarding : public OgreBites::ApplicationContext, public OgreBites::InputListener {
public:
    SnowBoarding();
    virtual ~SnowBoarding() {};

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void moveCamera(float deltaTime);
	void moveCameraLado(float deltaTime);
	void rotateCamera(float deltaTime);
	void rotateCameraWow(float deltaTime);
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
	Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setDirection(0, 1, -1);
	lightNode->setPosition(0, 10, -10);
	lightNode->attachObject(light);


	// Camera
	Ogre::Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(0.1);
	cam->setAutoAspectRatio(false);
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->yaw(Ogre::Degree(180));
	camNode->pitch(Ogre::Degree(-15));
	camNode->setPosition(0, 250, -40);
	//camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
	camNode->attachObject(cam);

	Ogre::Viewport* viewport = getRenderWindow()->addViewport(cam);
	viewport->setBackgroundColour(Ogre::ColourValue(0.1f, 0.2f, 0.5f));

	// player
	Ogre::Entity* playerEntity = scnMgr->createEntity("player", "brick.obj");
	Ogre::SceneNode* playerNode = scnMgr->createSceneNode();
	Rider* player = new Rider(playerNode, camNode);
	
	scnMgr->getRootSceneNode()->addChild(playerNode);
	player->setPosition(Ogre::Vector3(0,0,0));
	playerNode->pitch(Ogre::Degree(90));

	playerNode->attachObject(playerEntity);

	addInputListener(player);
	root->addFrameListener(player);

	// map
	// Crear un material con una textura
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("GroundMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("grass.jpg");
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Crear el objeto ManualObject para el suelo
	Ogre::ManualObject* ground = scnMgr->createManualObject("Ground");
	// Define the material
	ground->begin("GroundMaterial", Ogre::RenderOperation::OT_TRIANGLE_LIST); // Cambia "BaseWhite" por "GroundMaterial"

	//// Add vertices
	//ground->position(0, 0, 0); // Vertex 0
	//ground->position(0, 0, 5); // Vertex 2
	//ground->position(5, 2, 5); // Vertex 2
	//ground->position(5, 0, 0); // Vertex 3

	////// Define texture coordinates (optional)
	//ground->textureCoord(0, 0);
	//ground->textureCoord(1, 0);
	//ground->textureCoord(1, 1);
	//ground->textureCoord(0, 1);

	//// Define the triangles (two triangles to form a quad)
	//ground->triangle(0, 1, 2);
	//ground->triangle(0, 2, 3);

	rapidcsv::Document doc("C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/map/pista_con_acera_gaussiana_compuesta.csv");

	std::vector<float> xCoords = doc.GetColumn<float>("x");
	std::vector<float> yCoords = doc.GetColumn<float>("y");
	std::vector<float> zCoords = doc.GetColumn<float>("z");


	for (size_t i = 0; i < xCoords.size(); ++i) {

		std::cout << xCoords[i] << yCoords[i] << zCoords[i] << std::endl;
		ground->position(xCoords[i], yCoords[i], zCoords[i]);
	}
	int numVertices = xCoords.size();

	int numColumnas = 287;
	int numFilas = 7;
		ground->triangle(0, 7, 1);
	ground->triangle(1, 7, 8);
	for (int i = 0; i < numVertices; i++) {
		if ((i+1) % numFilas == 0)
			continue;
		ground->triangle(i, i+numFilas, i+1);
		ground->triangle(i+1, i+numFilas, i+numFilas+1);
	}

	ground->end();

	// Attach the ground to the scene
	Ogre::SceneNode* groundNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(ground);

}

bool SnowBoarding::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
		getRoot()->queueEndRendering();
	}
	//if (evt.keysym.sym == OgreBites::SDLK_UP) {
	//	moveCamera(-1); // Mueve la cámara hacia adelante
	//}
	//if (evt.keysym.sym == OgreBites::SDLK_DOWN) {
	//	moveCamera(1); // Mueve la cámara hacia atrás
	//}
	//if (evt.keysym.sym == OgreBites::SDLK_LEFT) {
	//	moveCameraLado(-1); // Mueve la cámara hacia adelante
	//	//rotateCamera(-30); // Mueve la cámara hacia atrás
	//}
	//if (evt.keysym.sym == OgreBites::SDLK_RIGHT) {
	//	moveCameraLado(1); // Mueve la cámara hacia atrás
	//}
	//if (evt.keysym.sym == 'z') {
	//	rotateCamera(1); // Mueve la cámara hacia atrás
	//}
	//if (evt.keysym.sym == 'x') {
	//	rotateCamera(-1); // Mueve la cámara hacia atrás
	//}
	//if (evt.keysym.sym == 'c') {
	//	rotateCameraWow(1); // Mueve la cámara hacia atrás
	//}
	//if (evt.keysym.sym == 'v') {
	//	rotateCameraWow(-1); // Mueve la cámara hacia atrás
	//}
	return true;
}
void SnowBoarding::rotateCameraWow(float direction) {
	camNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(direction * moveSpeed), Ogre::Node::TS_WORLD);
}

void SnowBoarding::rotateCamera(float direction) {
	camNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(direction * moveSpeed),Ogre::Node::TS_WORLD);
}
void SnowBoarding::moveCamera(float direction) {
	// direction es -1 para adelante y 1 para atrás
	camNode->translate(0, 0, moveSpeed * direction);
}
void SnowBoarding::moveCameraLado(float direction) {
	// direction es -1 para adelante y 1 para atrás
	camNode->translate(moveSpeed * direction, 0, 0);
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
