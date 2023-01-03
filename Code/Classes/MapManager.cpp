#include "MapManager.h"


MapManager* MapManager::instance = 0;

MapManager* MapManager::getInstance()
{
	if (!instance)
	{
		instance = new MapManager();
	}
	return instance;
}

void MapManager::loadMap(Layer* scene, int id, string name)
{
	this->scene = scene;
	this->id = id;
	this->mapName = name;
	this->createMap();
}

void MapManager::createMap()
{
	if (!this->scene)
	{
		cocos2d::log("Map is null");
	}
	else
	{
		this->collisionNodes.clear();
		this->collisionObjects.clear();
		this->tiledMap = cocos2d::TMXTiledMap::create(this->mapName);

		auto visibleSize = Director::getInstance()->getVisibleSize();
		scene->addChild(this->tiledMap, 0, "map");

		int width = this->tiledMap->getMapSize().width;
		int height = this->tiledMap->getMapSize().height;

		auto mapSize = this->tiledMap->getMapSize();
		auto tileSize = this->tiledMap->getTileSize();
		auto objectGroup = this->tiledMap->getObjectGroup("ColliderMap");
		if (!objectGroup)
		{
			cocos2d::log("objectGroup is null");
		}
		else
		{
			for (auto object : objectGroup->getObjects())
			{
				auto value = object.asValueMap();
				auto x = value.at("x").asFloat();
				auto y = value.at("y").asFloat();
				auto name = value.at("name").asString();
				auto width = value.at("width").asFloat();
				auto height = value.at("height").asFloat();

				auto physicsBody2D = cocos2d::PhysicsBody::createBox(cocos2d::Size(width, height), PhysicsMaterial(10000.0f, 0.0f, 1.0f));
				physicsBody2D->setMass(50);
			
				physicsBody2D->setCollisionBitmask(Model::BITMASK_GROUND);
		
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				auto node = Node::create();
				node->setPhysicsBody(physicsBody2D);
				node->setPosition(x + width / 2, y + height / 2);
				scene->addChild(node);

				this->collisionNodes.push_back(node);
				this->collisionObjects.push_back(Rect(x, y, width, height));
			}
		}
		//Tele
		auto teleGroup = this->tiledMap->getObjectGroup("Teleport");
		if (!teleGroup)
		{
			cocos2d::log("teleGroup is null");
		}
		else
		{
			for (auto object : teleGroup->getObjects())
			{
				auto value = object.asValueMap();
				auto x = value.at("x").asFloat();
				auto y = value.at("y").asFloat();
				auto width = value.at("width").asFloat();
				auto height = value.at("height").asFloat();

				auto physicsBody2D = cocos2d::PhysicsBody::createBox(cocos2d::Size(width, height), PhysicsMaterial(10000.0f, 0.0f, 1.0f));
				physicsBody2D->setMass(50);
				physicsBody2D->setCollisionBitmask(Model::BITMASK_TELEPORT);
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				auto node = Node::create();
				node->setPhysicsBody(physicsBody2D);
				node->setPosition(x + width / 2, y + height / 2);
				scene->addChild(node);
			}
		}

		// TalkNPC
		auto talkObject = this->tiledMap->getObjectGroup("TalkNPC");
		if (!talkObject)
		{
			cocos2d::log("talkObject is null");
		}
		else
		{
			for (auto object : talkObject->getObjects())
			{
				auto value = object.asValueMap();
				auto x = value.at("x").asFloat();
				auto y = value.at("y").asFloat();
				auto width = value.at("width").asFloat();
				auto height = value.at("height").asFloat();
				auto physicsBody2D = cocos2d::PhysicsBody::createCircle(width / 2, PhysicsMaterial(10000.0f, 0.0f, 1.0f));

				Sprite* sprite;
				string name = object.asValueMap().at("name").asString();
				if (name == "TalkNPC_Keeper")
				{
					physicsBody2D->setCollisionBitmask(Model::BITMASK_TalkNPC_Keeper);
					sprite = Sprite::create("NPC/NPC1_13.png");
				}
				else if (name == "TalkNPC_Medicine")
				{
					physicsBody2D->setCollisionBitmask(Model::BITMASK_TalkNPC_Medicine);
					sprite = Sprite::create("NPC/NPC1_60.png");
				}
				else if (name == "TalkNPC_ArmorSeller")
				{
					physicsBody2D->setCollisionBitmask(Model::BITMASK_TalkNPC_ArmorSeller);
					sprite = Sprite::create("NPC/NPC1_58.png");
				}
				else if (name == "TalkNPC_ArmsSeller")
				{
					physicsBody2D->setCollisionBitmask(Model::BITMASK_TalkNPC_ArmsSeller);
					sprite = Sprite::create("NPC/NPC1_21.png");
				}

				physicsBody2D->setMass(50);
				physicsBody2D->setContactTestBitmask(true);
				physicsBody2D->setDynamic(false);
				physicsBody2D->setRotationEnable(false);
				physicsBody2D->setPositionOffset(Vec2(0, -50));

				sprite->setScale(0.8);
				sprite->setPhysicsBody(physicsBody2D);
				sprite->setPosition(x + width / 2, y + height / 2 + 50);

				scene->addChild(sprite);
			}
		}

		//Tree
		auto treeGroup = this->tiledMap->getLayer("Tree");
		if (!treeGroup)
		{
			cocos2d::log("treeGroup is null");
		}
		else
		{
			treeGroup->setGlobalZOrder(6);
			for (int y = 0; y < mapSize.height; y++)
			{
				for (int x = 0; x < mapSize.width; x++)
				{
					Sprite *sp = treeGroup->getTileAt(Vec2(x, y));
					if (sp)
					{
						sp->setOpacity(200);
					}
				}
			}
		}

		auto treeUpGroup = this->tiledMap->getLayer("Tree_up");
		if (!treeUpGroup)
		{
			cocos2d::log("treeUpGroup is null");
		}
		else
		{
			treeUpGroup->setGlobalZOrder(6);
			for (int y = 0; y < mapSize.height; y++)
			{
				for (int x = 0; x < mapSize.width; x++)
				{
					Sprite *sp = treeUpGroup->getTileAt(Vec2(x, y));
					if (sp)
					{
						sp->setOpacity(200);
					}
				}
			}
		}

		auto street = this->tiledMap->getLayer("Street_up");
		if (!street)
		{
			cocos2d::log("street is null");
		}
		else
		{
			street->setGlobalZOrder(6);
			for (int y = 0; y < mapSize.height; y++)
			{
				for (int x = 0; x < mapSize.width; x++)
				{
					Sprite *sp = street->getTileAt(Vec2(x, y));
					if (sp)
					{
						sp->setOpacity(200);
					}
				}
			}
		}
	}
}


int MapManager::getId()
{
	return this->id;
}

bool MapManager::getDone()
{
	return isDone;
}

void MapManager::setDone(bool b)
{
	isDone = b;
}

string MapManager::getMapName()
{
	return this->mapName;
}

TMXTiledMap* MapManager::getTiledMap()
{
	return this->tiledMap;
}


std::vector<Rect> MapManager::getCollisionObjects()
{
	return this->collisionObjects;
}

std::vector<Node*> MapManager::getCollisionNodes()
{
	return this->collisionNodes;
}

MapManager::MapManager()
{
}

MapManager::~MapManager()
{

}