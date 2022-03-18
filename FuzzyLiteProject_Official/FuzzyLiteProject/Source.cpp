#include <fl/Headers.h>
#include <SFML/Graphics.hpp>
#include "Input.h"
#include <iostream>

using namespace std;
using namespace fl;
using namespace sf;




/*
	________TO DO_________

	-- GET INPUT TO MOVE TANK 
	-- LINK THE FUZZY SYSTEM WITH THE INPUT
	-- SETUP OTHER CLASSES
*/




int main()
{
	Engine* engine = new Engine();
	//Engine* engine = FisImporter().fromFile("Example.fis");

	RenderWindow window(sf::VideoMode(640, 480), "CMP303 - Prediction");

	window.setFramerateLimit(60);
	Input input;
	Clock clock;

	// Background 
	Texture groundTex;
	Texture carTex;
	Texture lineTex;
	Texture lineTex2;
	Sprite ground;
	Sprite car;
	Sprite line;
	Sprite line2;
	groundTex.loadFromFile("Assets/BackgroundTex.png");
	carTex.loadFromFile("Assets/CarTex.png");
	lineTex.loadFromFile("Assets/LineTex.png");
	lineTex2.loadFromFile("Assets/LineTex.png");
	// init ground
	groundTex.setRepeated(true);
	ground.setTexture(groundTex);
	ground.setTextureRect(IntRect(0, 0, 640, 480));

	// init car
	carTex.setRepeated(false);
	car.setPosition(320, 240);
	car.setTexture(carTex);
	//ground.setTextureRect(IntRect(0, 0, 0, 0));


	// init line
	lineTex.setRepeated(true);
	line.setTexture(lineTex);
	line.setPosition(320, 300);
	lineTex2.setRepeated(true);
	line2.setTexture(lineTex2);
	line2.setPosition(320, -1235);
	//Initialise font and text
	Font montserrat;
	Text debugText;

	//montserrat.loadFromFile("Graphics/TextFont.ttf");
	//debugText.setFont(montserrat);
	//debugText.setOutlineColor(sf::Color::Black);
	//debugText.setOutlineThickness(1.f);

	InputVariable* obstacle = new InputVariable;
	InputVariable* speed = new InputVariable;
	engine->addInputVariable(obstacle);
	engine->addInputVariable(speed);

	obstacle->setName("obstacle");
	obstacle->setRange(0.000, 1.000);
	speed->setName("speed");
	speed->setRange(0.000, 1.000);

	obstacle->addTerm(new Ramp("left", 1.000, 0.000));
	obstacle->addTerm(new Ramp("right", 0.000, 1.000));

	speed->addTerm(new Ramp("moving_left", 0.400, 1.000));
	speed->addTerm(new Triangle("none", 0.200, 1.000, 0.200));
	speed->addTerm(new Ramp("moving_right", 0.400, 1.000));

	// creating output variable
	OutputVariable* mSteer = new OutputVariable;
	engine->addOutputVariable(mSteer);
	mSteer->setName("mSteer");
	mSteer->setRange(0.000, 1.000);
	mSteer->setAggregation(new Maximum);
	mSteer->setDefuzzifier(new Centroid);
	mSteer->setDefaultValue(fl::nan);
	mSteer->addTerm(new Ramp("left", 1.000, 0.000));
	mSteer->addTerm(new Ramp("right", 0.000, 1.000));
	mSteer->addTerm(new Ramp("moving_left", 1.000, 0.400));
	mSteer->addTerm(new Triangle("none", 0.200, 1.000, 0.200));
	mSteer->addTerm(new Ramp("moving_right", 0.400, 1.000));


	// make the rules
	RuleBlock* mamdani = new RuleBlock;
	mamdani->setName("mamdani");
	mamdani->setConjunction(new AlgebraicProduct);
	mamdani->setDisjunction(new Maximum);
	mamdani->setImplication(new AlgebraicProduct);
	mamdani->setActivation(new General);
	mamdani->addRule(Rule::parse("if obstacle is left or speed is moving_left then mSteer is right", engine));
	mamdani->addRule(Rule::parse("if obstacle is right or speed is moving_right then mSteer is left", engine));
	mamdani->addRule(Rule::parse("if speed is none then mSteer is none", engine));
	engine->addRuleBlock(mamdani);

	float gameSpeed;
	gameSpeed = 1.0f;
	float dt = clock.restart().asSeconds() * gameSpeed;

	bool isSteeringLeft = false;
	//while (1)
	//{
	//	std::string input = "";
	//	std::cin >> input;
	//	
	//	std::stringstream ss(input);
	//	float number = 0.f;
	//	ss >> number;
	//	number -= 0.5f;

	//	obstacle->setValue(number);
	//	engine->process();
	//	float outP = (float)mSteer->getValue() - 0.5;
	//	std::cout << "obstacle.input = " << number <<
	//		" => steer.output = " <<outP << std::endl;

	//}

	float steerAmount;
	steerAmount = 0.0f;
	float number = 0.f;

	Vector2f newPos = Vector2f(0.0f, 0.0f);
	Vector2f newPos2 = Vector2f(0.0f, 0.0f);

	float output = 0.f;
	//while (1) {

	//	std::cin >> input;
	//	std::stringstream ss(input);
	//}

	//float number = 0.f;
	//ss >> number;



	//move road continously
	Vector2f velocity;
	velocity = { 0,0 };
	velocity.y = 500 * dt;
	while (1) {

		while (window.isOpen()) {
			Event event;

			// setup input etc
			while (window.pollEvent(event)) {
				switch (event.type) {
				case Event::Closed:
					cout << "Game Closed\n";


					window.close();
					break;
				case Event::Resized:
					window.setView(View(FloatRect(0.0f, 0.0f, (float)event.size.width, (float)event.size.height)));
					break;
				case Event::KeyPressed:
					input.setKeyDown(event.key.code);
					break;
				case Event::KeyReleased:
					input.setKeyUp(event.key.code);
					break;
				case Event::MouseMoved:
					input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
					break;
				case Event::MouseButtonPressed:
					if (event.mouseButton.button == Mouse::Left) {
						input.setMouseLDown(true);
					}
					else if (event.mouseButton.button == Mouse::Right) {
						input.setMouseRDown(true);
					}
					break;
				case Event::MouseButtonReleased:
					if (event.mouseButton.button == Mouse::Left) {
						input.setMouseLDown(false);
					}
					else if (event.mouseButton.button == Mouse::Right) {
						input.setMouseRDown(false);
					}
					break;
				default:
					// dont handle anymore events
					break;
				}
			}


			////float dt = clock.restart().asSeconds() * gameSpeed;



		/*	if (input.isKeyDown(Keyboard::W)) {
			}*/

			//output = (float)mSteer->getValue();

			// input
			if (input.isKeyDown(Keyboard::A)) {
				isSteeringLeft = true;
				// moves the roads to the side to look like the car is steering away from road

				newPos = line.getPosition() + Vector2f(50.0, 0.0) * dt;
				newPos2 = line2.getPosition() + Vector2f(50.0, 0.0) * dt;
				//newPos = car.getPosition() + Vector2f(50.0, 0.0) * dt;
				//car.setPosition(newPos);
				line.setPosition(newPos);
				line2.setPosition(newPos2);
				steerAmount += 0.1f * dt;
				car.setRotation(-30.0f);
				//steerAmount += 0.1f;
				/**/
			}
			else if (output >= 0.6){
				isSteeringLeft = false;
				car.setRotation(30.0f);
				if (output <= 0.66663 || line.getPosition().x >= 320) {
					newPos = line.getPosition() - Vector2f(30.0, 0.0) * dt;
					//newPos = Vector2f(line.getPosition().x, output - 0.6 * dt) ;
					line.setPosition(newPos);
					newPos2 = line2.getPosition() - Vector2f(30.0, 0.0) * dt;
					//newPos2 = Vector2f(line.getPosition().x, output - 0.6 * dt);
					line2.setPosition(newPos2);
					steerAmount -= 0.1f * dt;
				}

			}
			// Rotate car depending on which way its moving
			if (car.getPosition().x > 320) {
				car.setRotation(30.0f);
			}
			if (line.getPosition().x <= 320) {
				car.setRotation(0.0f);
			}

			//

			// reset the roads when they go off screen
			cout << "line 1 pos" << line.getPosition().x << line.getPosition().y << endl;
			if (line.getPosition().y >= 1000.0) {
				line.setPosition(line.getPosition().x, -2050);
			}
			if (line2.getPosition().y >= 1000.0) {
				line2.setPosition(line2.getPosition().x, -2050);
			}

			line.move(velocity);
			line2.move(velocity);

			//steerAmount >> number;
			//number -= 0.5;
			obstacle->setValue(steerAmount);
			engine->process();
			output = (float)mSteer->getValue()/* - 0.4*/;
			std::cout << "obstacle.input = " << steerAmount <<
				" => steer.output = " <<output << std::endl;

		
			// Render to screen
			window.clear();

			window.draw(ground);
			window.draw(line);
			window.draw(line2);
			window.draw(car);


			window.display();

		}
	}
	delete engine;
	return 0;
}


