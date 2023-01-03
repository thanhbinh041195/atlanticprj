#pragma once
#include <vector>
#include "cocos2d.h"
#include "Model.h"
#include "Define.h"

using namespace cocos2d;
using namespace std;

class MapManager
{
public:
	static MapManager* getInstance();

	void loadMap(Layer* scene, int id, string name);

	void createMap();
	void addPlayerToMap(Model* player);
	void addItemToMap(Model* item);
	void addMonsterToMap(Model* monster, string name);

	int getId();
	bool getDone();
	void setDone(bool b);
	string getMapName();
	cocos2d::TMXTiledMap* getTiledMap();
	bool isCanMoveToPoint(Vec2 point);
	bool isPointCanSeePoint(Vec2 point1, Vec2 point2);
	std::vector<Rect> getCollisionObjects();
	std::vector<Node*> getCollisionNodes();

private:
	static MapManager* instance;

	int id;
	string mapName;
	bool isDone;
	cocos2d::TMXTiledMap* tiledMap;
	cocos2d::Layer* scene;
	std::vector<Rect> collisionObjects;
	std::vector<Node*> collisionNodes;

	MapManager();
	~MapManager();
};