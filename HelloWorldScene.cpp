#include "HelloWorldScene.h"
#include "time.h"
#include "windows.h"

USING_NS_CC;

int foodX;
int foodY;
int dir = 2;
int score = 0;
int size = 0;
int array[1200][2];
boolean hasSnake[40][30];
int speed = 100;
boolean gameOver = false;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = HelloWorld::create();

	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	for (int i = 0; i < 1200; i++) {
		array[i][0] = 0;
		array[i][1] = 0;
	}

	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 30; j++) {
			hasSnake[i][j] = false;
		}
	}

	srand(time(NULL));
	foodX = rand() % 40;
	foodY = rand() % 30 + 1;

	while (foodX == 20 && foodY == 15) {
		srand(time(NULL));
		foodX = rand() % 40;
		foodY = rand() % 30 + 1;
	}

	foodX = 16 * foodX;
	foodY = 16 * foodY;

	label = Label::createWithSystemFont(std::to_string(score), "Arial", 40);
	bg = Sprite::create("background.png");
	sprite = Sprite::create("food.png");
	newSnake[0] = Sprite::create("snake.png");
	newSnake[0]->setAnchorPoint(Vec2(0, 1));
	label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	sprite->setAnchorPoint(Vec2(0,1));
	bg->setPosition(320, 240);
	sprite->setPosition(foodX,foodY);
	newSnake[0]->setPosition(320, 240);
	this->addChild(label, 1);
	this->addChild(bg, 0);
	this->addChild(sprite, 0);
	this->addChild(newSnake[0], 0);

	this->scheduleUpdate();

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)->void {
		Vector<FiniteTimeAction*> actions;
		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			if (dir != 2) {
				dir = 1;
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			if (dir != 1) {
				dir = 2;
			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			if (dir != 3) {
				dir = 0;
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			if (dir != 0) {
				dir = 3;
			}
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, newSnake[0]);

	return true;
}

void HelloWorld::update(float delta) {
	if (gameOver) {
		Sleep(1000);
		exit(1);
	} else {
		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 30; j++) {
				hasSnake[i][j] = false;
			}
		}
		auto oldPos = newSnake[0]->getPosition();
		auto position = newSnake[0]->getPosition();
		array[0][0] = position.x;
		array[0][1] = position.y;
		for (int i = 0; i <= size; i++) {
			hasSnake[array[i][0]/16][array[i][1]/16] = true;
		}
		switch (dir) {
		case 0:
			position.y += 16;
			if (position.y > 480) {
				position.y = 16;
			}
			break;
		case 1:
			position.x -= 16;
			if (position.x < 0) {
				position.x = 624;
			}
			break;
		case 2:
			position.x += 16;
			if (position.x > 624) {
				position.x = 0;
			}
			break;
		case 3:
			position.y -= 16;
			if (position.y < 16) {
				position.y = 480;
			}
			break;
		}
		if (position.x == foodX && position.y == foodY) {
			score++;
			size++;
			speed--;
			if (speed < 0) {
				speed = 0;
			}
			array[size][0] = array[size - 1][0];
			array[size][1] = array[size - 1][1];
			newSnake[size] = Sprite::create("snake.png");
			newSnake[size]->setAnchorPoint(Vec2(0, 1));
			newSnake[size]->setPosition(array[size][0], array[size][1]);
			this->addChild(newSnake[size]);
			this->removeChild(label);
			label = Label::createWithSystemFont(std::to_string(score), "Arial", 40);
			label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
			this->addChild(label);
			srand(time(NULL));
			foodX = rand() % 40;
			foodY = rand() % 30 + 1;
			while (hasSnake[foodX][foodY]) {
				srand(time(NULL));
				foodX = rand() % 40;
				foodY = rand() % 30 + 1;
			}
			foodX = 16 * foodX;
			foodY = 16 * foodY;
			sprite->setPosition(foodX, foodY);
		}
		for (int i = size; i > 0; i--) {
			array[i][0] = array[i - 1][0];
			array[i][1] = array[i - 1][1];
			newSnake[i]->setPosition(array[i][0], array[i][1]);
		}
		array[0][0] = position.x;
		array[0][1] = position.y;
		newSnake[0]->setPosition(position.x, position.y);
		for (int i = 1; i <= size; i++) {
			if (array[0][0] == array[i][0] && array[0][1] == array[i][1]) {
				gameOver = true;
				this->removeAllChildren();
				label = Label::createWithSystemFont("Game over. Your score is " + std::to_string(score) + ".", "Arial", 40);
				label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
				this->addChild(label);
			}
		}
		Sleep(speed);
	}
}