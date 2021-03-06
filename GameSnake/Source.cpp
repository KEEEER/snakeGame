#include <iostream>
#include <random>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
using namespace sf;
using namespace std;

sf::Texture t, food, head, t_purple;
sf::SoundBuffer getFoodSound;
sf::Sound foodSound;
sf::SoundBuffer WhatTheFuckIsThisFlashOoooo;
sf::Sound Dead;

bool eatFood = false;
int windowMaxX = 600;
int windowMaxY = 600;
int stepX = 30;
int stepY = 30;
int soundStep = 10;
bool restart = false;
bool jumped = false;

struct posXY {
	int posX;
	int posY;
	posXY(int X, int Y) :posX(X), posY(Y){};
	void setXY(int X, int Y) {
		posX = X;
		posY = Y;
	}
	void printXY() {
		cout << "pos: " << posX << "  " << posY << endl;
	}
}dir(0, 0), sweetFood(0, 0);

void drawFood(RenderWindow &window, posXY &sweetFood , Sprite &foodS) {
	foodS.setTexture(food);
	foodS.setPosition(sweetFood.posX, sweetFood.posY);
	window.draw(foodS);
}

void drawSnake(RenderWindow &window, vector<Sprite> &s) {
	s[0].setTexture(head);
	window.draw(s[0]);
	for (int i = 1; i < s.size(); i++) {
		
		window.draw(s[i]);
	}
	window.display();
}
void moveSnake(int& snakeSize, vector<Sprite> &s, vector<posXY> &body, int valX, int valY, string clickStr) {
	for (int i = snakeSize-1; i > 0 ; i--) {
		s[i].setTexture(t);
		s[i].setPosition(sf::Vector2f(body[i - 1].posX, body[i - 1].posY));
		body[i].posX = body[i - 1].posX;
		body[i].posY = body[i - 1].posY;
	}
	if (body[0].posX + valX >= windowMaxX) body[0].posX = 0 - abs(valX);
	if (body[0].posX + valX < 0) body[0].posX = windowMaxX + abs(valX);
	if (body[0].posY + valY >= windowMaxY) body[0].posY = 0 - abs(valY);
	if (body[0].posY + valY < 0) body[0].posY = windowMaxY + abs(valY);	

	s[0].setPosition(sf::Vector2f(body[0].posX + valX, body[0].posY + valY));
	body[0].posX += valX;
	body[0].posY += valY;
	if (body[0].posX == sweetFood.posX && body[0].posY == sweetFood.posY && !jumped) {
		Sprite temp;
		body.push_back(posXY(body[s.size() - 1].posX - valX, body[s.size() - 1].posY - valY));
		s.push_back(temp);	
		s[s.size() - 1].setPosition(sf::Vector2f(body[s.size() - 1].posX , body[s.size() - 1].posY));
		snakeSize++;
		eatFood = true;
		foodSound.play();
	}
	for (int i = 1; i < snakeSize; i++) {
		if (body[0].posX == body[i].posX && body[0].posY == body[i].posY ) {
			if (jumped) {
				s[i].setTexture(t_purple);
			}
			else {
				Dead.play();
				restart = true;
			}
		}
	}
}

int main() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> distX(1.0, windowMaxX / stepX);
	std::uniform_real_distribution<double> distY(1.0, windowMaxY / stepY);
	//while (true) {
		restart = false;

		int snakeSize = 1;

		RenderWindow window(VideoMode(windowMaxX, windowMaxY), "why that ice bird comes again?");
		vector<Sprite> s(snakeSize);
		Sprite foodS;

		vector<posXY> body(snakeSize , posXY(0,0));

		sweetFood.setXY((int)distX(mt) * stepX, (int)distY(mt) * stepY);

		string preEvent = "Queen";
	
		t.loadFromFile("./graphics/square_small.png");
		food.loadFromFile("./graphics/icebird_small.png");
		head.loadFromFile("./graphics/character_small.png");
		t_purple.loadFromFile("./graphics/baby_small.jpg");

		if (!getFoodSound.loadFromFile("./sounds/A.ogg")) cout << "loadFailed" << endl;
		foodSound.setBuffer(getFoodSound);
		foodSound.setVolume(50);
		if (!WhatTheFuckIsThisFlashOoooo.loadFromFile("./sounds/OOOOO.ogg")) cout << "loadFailed" << endl;
		Dead.setBuffer(WhatTheFuckIsThisFlashOoooo);
		Dead.setVolume(35);

		while (window.isOpen()) {
			Event e;
			while (window.pollEvent(e)) {
				jumped = false;
				if (e.type == Event::Closed) {
					window.close();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					cout << "A pressed" << endl;
					jumped = true;
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					foodSound.setVolume(foodSound.getVolume() + soundStep);
					Dead.setVolume(Dead.getVolume() + soundStep);
					cout << foodSound.getVolume();
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
					foodSound.setVolume(foodSound.getVolume() - soundStep);
					Dead.setVolume(Dead.getVolume() - soundStep);
					cout << foodSound.getVolume();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && preEvent != "R") {
					if (preEvent == "L") continue;
					dir.setXY(stepX, 0);
					preEvent = "R";
					cout << "R pressed" << endl;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && preEvent != "L") {
					if (preEvent == "R") continue;
					dir.setXY(-stepX, 0);
					preEvent = "L";
					cout << "L pressed" << endl;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && preEvent != "U") {
					if (preEvent == "D") continue;
					dir.setXY(0, -stepY);
					preEvent = "U";
					cout << "U pressed" << endl;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && preEvent != "D") {
					if (preEvent == "U") continue;
					dir.setXY(0, stepY);
					preEvent = "D";
					cout << "D pressed" << endl;
				}

			}
			moveSnake(snakeSize, s, body, dir.posX, dir.posY, preEvent);
			if (eatFood) {
				eatFood = false;
				sweetFood.setXY((int)distX(mt) * stepX, (int)distY(mt) * stepY);
			}
			window.clear(Color::White);
			drawFood(window, sweetFood, foodS);
			drawSnake(window, s);
			sf::sleep(sf::milliseconds(75));
			if (restart) {
				sf::sleep(sf::milliseconds(3600));
				break;
			}
		}
	//}
	
	return 0;
}
