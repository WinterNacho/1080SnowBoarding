#include "Ogre.h"
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreFileSystemLayer.h>
#include "rapidcsv.h"
#include <stdlib.h>

class SnowBoarding : public OgreBites::ApplicationContext, public OgreBites::InputListener {
public:
    SnowBoarding();
    virtual ~SnowBoarding() {};

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void moveCamera(float deltaTime);
	void moveCameraLado(float deltaTime);
	void rotateCamera(float deltaTime);
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
	lightNode->setPosition(0, 0, 20);
	lightNode->attachObject(light);


	// Camera
	Ogre::Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(0.1);
	cam->setAutoAspectRatio(false);
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
	camNode->pitch(Ogre::Degree(-60));
	camNode->setPosition(0, 60, 30);
	camNode->attachObject(cam);

	Ogre::Viewport* viewport = getRenderWindow()->addViewport(cam);
	viewport->setBackgroundColour(Ogre::ColourValue(0.1f, 0.2f, 0.5f));

	// brick
	//Ogre::Entity* brickEntity = scnMgr->createEntity("brick1", "brick.obj");
	//Ogre::SceneNode* brickNode = scnMgr->createSceneNode();
	//
	//scnMgr->getRootSceneNode()->addChild(brickNode);

	//brickNode->setPosition(0,0,0);
	//brickNode->yaw(Ogre::Degree(90));
	//brickNode->attachObject(brickEntity);

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

	rapidcsv::Document doc("C:/dev/arq-motores/1080SnowBoarding_/1080SnowBoarding/assets/map/pista_con_acera.csv");

	std::vector<float> xCoords = doc.GetColumn<float>("x");
	std::vector<float> yCoords = doc.GetColumn<float>("y");
	std::vector<float> zCoords = doc.GetColumn<float>("z");


	for (size_t i = 0; i < xCoords.size(); ++i) {

		std::cout << xCoords[i] << yCoords[i] << zCoords[i] << std::endl;
		ground->position(xCoords[i], yCoords[i], zCoords[i]);
	}
	int numPuntos = xCoords.size();

	// Calcular el número de columnas
	int numColumnas = 0;
	while (numColumnas * (numColumnas + 1) < numPuntos) {
		numColumnas++;
	}

    // Calcular el número de filas
    int numFilas = numPuntos / numColumnas;

	for (int fila = 0; fila < numFilas - 1; ++fila) {
		for (int columna = 0; columna < numColumnas - 1; ++columna) {
			// Índices de los puntos en la cuadrícula
			int v0 = fila * numColumnas + columna;        // Inferior izquierda
			int v1 = v0 + 1;                              // Inferior derecha
			int v2 = v0 + numColumnas;                   // Superior izquierda
			int v3 = v2 + 1;                              // Superior derecha

			// Crear dos triángulos para cada cuadrado
			ground->triangle(v0, v1, v2);  // Triángulo 1 (v0, v1, v2)
			ground->triangle(v1, v3, v2);  // Triángulo 2 (v1, v3, v2)
			ground->triangle(v0, v2, v1);  // Triángulo 1 (v0, v1, v2)
			ground->triangle(v1, v2, v3);  // Triángulo 2 (v1, v3, v2)
		}
	}
	// Definir triángulos automáticamente en función de la anchura de la cuadrícula
	//ground->triangle(0, 1, 2);
	//ground->triangle(0, 2, 3);
	ground->end();

	// Attach the ground to the scene
	Ogre::SceneNode* groundNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	groundNode->attachObject(ground);

}

bool SnowBoarding::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
		getRoot()->queueEndRendering();
	}
	if (evt.keysym.sym == OgreBites::SDLK_UP) {
		moveCamera(-1); // Mueve la cámara hacia adelante
	}
	if (evt.keysym.sym == OgreBites::SDLK_DOWN) {
		moveCamera(1); // Mueve la cámara hacia atrás
	}
	if (evt.keysym.sym == OgreBites::SDLK_LEFT) {
		moveCameraLado(-1); // Mueve la cámara hacia adelante
		//rotateCamera(-30); // Mueve la cámara hacia atrás
	}
	if (evt.keysym.sym == OgreBites::SDLK_RIGHT) {
		moveCameraLado(1); // Mueve la cámara hacia atrás
	}
	if (evt.keysym.sym == 'z') {
		rotateCamera(1); // Mueve la cámara hacia atrás
	}
	if (evt.keysym.sym == 'x') {
		rotateCamera(-1); // Mueve la cámara hacia atrás
	}
	return true;
}

void SnowBoarding::rotateCamera(float direction) {
	// direction es -1 para adelante y 1 para atrás
	// Dirección de rotación (-1 para sentido horario, 1 para antihorario)
	Ogre::Real angle = Ogre::Degree(moveSpeed * direction).valueRadians();

	// Obtener la posición actual de la cámara relativa al origen
	Ogre::Vector3 camPos = camNode->getPosition();

	// Calcular la nueva posición rotada alrededor del eje Y
	Ogre::Real newX = camPos.x * cos(angle) - camPos.z * sin(angle);
	Ogre::Real newZ = camPos.x * sin(angle) + camPos.z * cos(angle);

	// Actualizar la posición de la cámara manteniendo la distancia al origen
	camNode->setPosition(newX, camPos.y, newZ);

	// Asegurar que la cámara siempre mire hacia el origen
	camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
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
