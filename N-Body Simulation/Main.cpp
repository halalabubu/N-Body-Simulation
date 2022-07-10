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
#define PCOUNT 100
#define GRIDCOUNT 4096 //2^12

const std::string FRAGLOC = "resource/particles.frag";


void InitParticles(Particle* particles);

//float G_CONSTANT = 0.001f;

int main()
{
	int VERTICAL_RESOLUTION = 1080;
	int HORIZONTAL_RESOLUTION = 1920;
	float ASPECT_RATIO = 16 / 9;
	float DELTA = 5.0f / 1000.0f;// 
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
	
	sf::Shader *shader;
	sf::Uint8* pixelValues;
	Particle* particles;
	Pixel* pixel;
	Grid* grid;

	cudaMallocManaged(&shader, sizeof(sf::Shader));
	cudaMallocManaged(&pixelValues, pixelCount * 4 * sizeof(sf::Uint8));
	cudaMallocManaged(&particles, PCOUNT * sizeof(Particle));
	cudaMallocManaged(&pixel, pixelCount * sizeof(Pixel));
	cudaMallocManaged(&grid, GRIDCOUNT * sizeof(Grid));

	//sets the texture to all black 0x00,0x00,0x00,0xFF	RGBA
	setTextureColor << <NUMBLOCKS, BLOCKSIZE >> > (pixelCount * 4, pixelValues);
	cudaDeviceSynchronize();
	texture.update(pixelValues);
	texture.setSmooth(true);

	//init particle positions
	InitParticles(particles);

	//shader declared and now exists on managed memory
	shader = new sf::Shader;
	shader->loadFromFile(FRAGLOC,sf::Shader::Fragment);

	
	

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

		


		placeIntoPixel <<<NUMBLOCKS, BLOCKSIZE >> > (PCOUNT, WIDTH, particles, pixel);
		cudaDeviceSynchronize();
		updateTexture << <NUMBLOCKS, BLOCKSIZE >> > (pixelCount, pixelValues, pixel);
		cudaDeviceSynchronize();
		//texture.update(pixelValues);


		//window.draw(sprite,shader);
		window.draw(sprite);

		window.display();
		window.clear();



	}





	cudaFree(pixelValues);
	cudaFree(particles);
	cudaFree(pixel);
	cudaFree(grid);
	return 0;
}
void InitParticles(Particle* particles) {

	for (size_t i = 0; i < PCOUNT; i++)
	{
		particles[i].mass = 1;
		particles[i].velocity.x = 0;
		particles[i].velocity.y = 0;

		particles[i].pos.x = rand() % (WIDTH - 40) + 20;
		particles[i].pos.y = rand() % (HEIGHT - 40) + 20;
	}


}