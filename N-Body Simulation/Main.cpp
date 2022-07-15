#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>


#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Kernal.cuh"

#include<iostream>



#define WIDTH 1024
#define HEIGHT 1024
#define PCOUNT 600
#define GRIDCOUNT 4096 //2^12

__managed__ float DELTA = 5.0f / 100000000000.0f;
const float GRIDSIZE = 1024;
const int LEVELS = 6;
const int CURRENT_LEVEL = 0;
const std::string FRAGLOC = "resource/particles.frag";
int calculationMode = 1;//0 is barnes hut, 1 is naive

void InitParticles(Particle* particles);
void uniformParticles(Particle* particles);
void drawGrid(Node* node, sf::RenderWindow& window);
void buildTreeConnection(NodeList* nList);

//float G_CONSTANT = 0.001f;

__host__ int main()
{
	int VERTICAL_RESOLUTION = 1080;
	int HORIZONTAL_RESOLUTION = 1920;
	float ASPECT_RATIO = 16 / 9;
	float DELTA = 5.0f / 1000.0f;// 
	bool debugGrid = true;
	//float DELTA = 1.0f;

	int pixelCount = WIDTH * HEIGHT;
	const int BLOCKSIZE = 256;
	const int NUMBLOCKS = (pixelCount + BLOCKSIZE - 1) / BLOCKSIZE;


	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "N-Body Simulation");
	window.setFramerateLimit(144);

	//texture
	sf::Texture texture;
	sf::Sprite sprite;
	//texture.loadFromFile("resource/test.png");
	texture.create(WIDTH, HEIGHT);
	sprite.setTexture(texture);
	sprite.setOrigin(sf::Vector2f(0, 0));

	Particle* particles;
	//Node* node;
	NodeList* nList;

	cudaMallocManaged(&particles, PCOUNT * sizeof(Particle));
	//cudaMallocManaged(&node, sizeof(Node));
	//cudaMallocManaged(&nList, sizeof(NodeList));
	cudaMallocManaged(&nList, sizeof(NodeList));


	//node = new Node;
	nList->level0.startIndex = 0;
	nList->level0.endIndex = PCOUNT - 1;


	//init particle positions
	InitParticles(particles);
	//uniformParticles(particles);

	//shader declared and now exists on managed memory
	//shader = new sf::Shader;
	//shader->loadFromFile(FRAGLOC,sf::Shader::Fragment);


	sf::VertexArray triangles(sf::Points, PCOUNT);
	for (size_t i = 0; i < PCOUNT; i++)
	{
		triangles[i].color = sf::Color::White;
	}


	//for (size_t i = 0; i < PCOUNT; i+=3)
	//{
	//	int demo = rand() % WIDTH;
	//	int demo2 = rand() % WIDTH;
	//	triangles[i].position = sf::Vector2f(0+demo, 0 + demo2);
	//}

	buildTreeConnection(nList);


	int he;
	he = 1;
	he = 1;
	he = 1;

	//let values propagate
	for (size_t i = 0; i < 6; i++)
		buildTree << <1, 1 >> > (particles, &nList->level0, nList, CURRENT_LEVEL, LEVELS, GRIDSIZE, 0, 0);

	cudaDeviceSynchronize();
	std::cout << nList->level0.totalMass;
	std::cout << std::endl;
	//buildTree << <1, 1 >> > (particles, &nList->level0, nList, CURRENT_LEVEL, LEVELS, GRIDSIZE, 0, 0);
	//cudaDeviceSynchronize();
	//for (size_t i = 0; i < PCOUNT; i++)
	//{
	//	std::cout << "Y Value: " << particles[i].pos.y << std::endl;
	//}


	sf::View camera(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT));

	// run the program as long as the window is open
	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool zoomIn = false;
	bool zoomOut = false;





	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				moveUp = true;
			else
				moveUp = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				moveDown = true;
			else
				moveDown = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				moveLeft = true;
			else
				moveLeft = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				moveRight = true;
			else
				moveRight = false;
			//std::cout << sf::Event::MouseWheelScrollEvent::delta;


			//event.mouseWheelScroll.delta
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				zoomOut = true;
			else
				zoomOut = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				zoomIn = true;
			else
				zoomIn = false;


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		{
			if (moveUp)
				camera.move(0.f, -1.f);
			if (moveDown)
				camera.move(0.f, 1.f);
			if (moveLeft)
				camera.move(-1.f, 0.f);
			if (moveRight)
				camera.move(1.f, 0.f);
			if (zoomIn)
				camera.zoom(0.99f);
			if (zoomOut)
				camera.zoom(1.01f);
		}
		window.setView(camera);




		//placeIntoPixel <<<NUMBLOCKS, BLOCKSIZE >> > (PCOUNT, WIDTH, particles, pixel);
		//cudaDeviceSynchronize();
		//updateTexture << <NUMBLOCKS, BLOCKSIZE >> > (pixelCount, pixelValues, pixel);
		//cudaDeviceSynchronize();
		//texture.update(pixelValues);
		for (size_t i = 0; i < PCOUNT; i++)
		{
			triangles[i].position.x = particles[i].pos.x;
			triangles[i].position.y = particles[i].pos.y;
		}
		buildTree << <1, 1 >> > (particles, &nList->level0, nList, CURRENT_LEVEL, LEVELS, GRIDSIZE, 0, 0);
		cudaDeviceSynchronize();
		if (debugGrid)
		{
			drawGrid(&nList->level0, window);
			for (size_t i = 0; i < pow(4, 1); i++)
				drawGrid(&nList->level1[i], window);
			for (size_t i = 0; i < pow(4, 2); i++)
				drawGrid(&nList->level2[i], window);
			for (size_t i = 0; i < pow(4, 3); i++)
				drawGrid(&nList->level3[i], window);
			for (size_t i = 0; i < pow(4, 4); i++)
				drawGrid(&nList->level4[i], window);
			for (size_t i = 0; i < pow(4, 5); i++)
				drawGrid(&nList->level5[i], window);
			for (size_t i = 0; i < pow(4, 6); i++)
				drawGrid(&nList->level6[i], window);

			//std::cout << particles[0].velocity.x << "	" << particles[0].velocity.y;
			//std::cout << std::endl;
		}

		setVelSetPos << <80, 128 >> > (particles, nList, DELTA);
		cudaDeviceSynchronize();


		//window.draw(sprite,shader);
		//window.draw(sprite);
		window.draw(triangles);
		window.display();
		window.clear();



	}




	cudaFree(nList);
	cudaFree(particles);

	return 0;
}
void InitParticles(Particle* particles) {

	for (size_t i = 0; i < PCOUNT; i++)
	{
		particles[i].mass = 1;
		particles[i].velocity.x = 0;
		particles[i].velocity.y = 0;
		int randint = rand() % 1000;
		float decimal = randint / 1000.0f;

		particles[i].pos.x = rand() % (WIDTH - 500) + decimal + 250;
		particles[i].pos.y = rand() % (HEIGHT - 500) + decimal + 250;
	}

	//particles[0].mass = 1;
	//particles[0].pos.x = 400;
	//particles[0].pos.y = 400;
	//particles[1].mass = 1;
	//particles[1].pos.x = 410;
	//particles[1].pos.y = 400;
}

__host__ void drawGrid(Node* node, sf::RenderWindow& window) {



	//hard coded in kernal.cu FIX, line 22
	if (node->totalMass < 10)
		return;


	sf::Vertex line[4];
	line[0].position = sf::Vector2f(node->mins.x + node->width / 2, node->mins.y);
	line[1].position = sf::Vector2f(node->mins.x + node->width / 2, node->mins.y + node->width);

	line[2].position = sf::Vector2f(node->mins.x, node->mins.y + +node->width / 2);
	line[3].position = sf::Vector2f(node->mins.x + node->width, node->mins.y + +node->width / 2);

	window.draw(line, 4, sf::Lines);
	//for (size_t i = 0; i < 4; i++)
	//{
	//	if (node->quadrants[i]->totalMass != 0)
	//	{
	//		drawGrid(node->quadrants[i], window);
	//	}
	//}




}
// not nec but makes the tree way easier to work with
void buildTreeConnection(NodeList* nList) {

	nList->level0.parent = nullptr;
	for (size_t j = 0; j < 4; j++)
		nList->level1[j].parent = &nList->level0;
	for (size_t j = 0; j < pow(4, 2); j += 4)
		for (size_t i = 0; i < 4; i++)
			nList->level2[j + i].parent = &nList->level1[i];
	for (size_t j = 0; j < pow(4, 3); j += 4)
		for (size_t i = 0; i < 4; i++)
			nList->level3[j + i].parent = &nList->level2[i];
	for (size_t j = 0; j < pow(4, 4); j += 4)
		for (size_t i = 0; i < 4; i++)
			nList->level4[j + i].parent = &nList->level3[i];
	for (size_t j = 0; j < pow(4, 5); j += 4)
		for (size_t i = 0; i < 4; i++)
			nList->level5[j + i].parent = &nList->level4[i];
	for (size_t j = 0; j < pow(4, 6); j += 4)
		for (size_t i = 0; i < 4; i++)
			nList->level6[j + i].parent = &nList->level5[i];


	//level 1
	int currentLevel = 1;
	for (size_t i = 0; i < pow(4, currentLevel); i++)
		nList->level0.quadrants[i] = &nList->level1[i];


	currentLevel++;
	//level 2

	for (size_t j = 0; j < pow(4, currentLevel); j++)
		for (size_t i = 0; i < 4; i++)
			nList->level1[j].quadrants[i] = &nList->level2[j * 4 + i];


	currentLevel++;
	//level 3
	for (size_t j = 0; j < pow(4, currentLevel); j++)
		for (size_t i = 0; i < 4; i++)
			nList->level2[j].quadrants[i] = &nList->level3[j * 4 + i];




	currentLevel++;
	//level 4
	for (size_t j = 0; j < pow(4, currentLevel); j++)
	{

		for (size_t i = 0; i < 4; i++)
		{
			nList->level3[j].quadrants[i] = &nList->level4[j * 4 + i];
		}
	}
	currentLevel++;
	//level 5
	for (size_t j = 0; j < pow(4, currentLevel); j++)
	{

		for (size_t i = 0; i < 4; i++)
		{
			nList->level4[j].quadrants[i] = &nList->level5[j * 4 + i];
		}
	}
	currentLevel++;
	//level 6
	for (size_t j = 0; j < pow(4, currentLevel); j++)
	{

		for (size_t i = 0; i < 4; i++)
		{
			nList->level5[j].quadrants[i] = &nList->level6[j * 4 + i];
		}
	}

}


void uniformParticles(Particle* particles)
{
	float length = sqrt(1000);
	int plength = sqrt(PCOUNT);
	float pSpace = 900 / plength;
	

	for (size_t i = 0; i < plength; i++)
	{
		for (size_t j = 0; j < plength+1; j++)
		{
			if (i + j * plength > PCOUNT)
				return;

			particles[i+j*plength].pos.x = i*pSpace+20;
			particles[i+j*plength].pos.y = j*pSpace+20;
		}

	}
	for (size_t i = 0; i < PCOUNT; i++)
	{
		particles[PCOUNT].velocity.x = 0;
		particles[PCOUNT].velocity.y = 0;
	}


}