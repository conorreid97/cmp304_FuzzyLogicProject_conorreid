#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "GameObject.h"


using namespace std;
using namespace sf;

class Car : public GameObject
{
public: 
	Car();
	~Car();

	void Update(float dt);
	const void Render(RenderWindow* window);

private:
	Sprite car;

	RenderWindow* window;

};

