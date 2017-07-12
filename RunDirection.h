#ifndef RUN_DIRECTION
#define RUN_DIRECTION

#include <BWAPI.h>
#include "VariableDeclarations.h"

BWAPI::Position RunAway(BWAPI::Unit runner, BWAPI::Unit runnee)
{
	if (runner == nullptr || runnee == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}
	int x1 = runner->getPosition().x;
	int y1 = runner->getPosition().y;
	int x2 = runnee->getPosition().x;
	int y2 = runnee->getPosition().y;

	//if (runner->isFlying())
	//{
	runner->move(BWAPI::Position(2 * x1 - x2, 2 * y1 - y2));
	return BWAPI::Position(2 * x1 - x2, 2 * y1 - y2);
	//}
	//else
	//{
	//int dist = (int)sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	//int x3 = x1 + (32 / dist)*(x1 - x2);
	//int y3 = y1 + (32 / dist)*(y1 - y2);
	//if (Broodwar->isWalkable(x3/8,y3/8))
	//{
	//	runner->move(BWAPI::Position(x3,y3));
	//	return BWAPI::Position(x3,y3);
	//}
	//else
	//{
	//	runner->move(BWTA::getNearestBaseLocation(runner->getTileBWAPI::Position())->getPosition());
	//	return BWTA::getNearestBaseLocation(runner->getTileBWAPI::Position())->getPosition();
	//}


	//int dist = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
	//int x3 = x1 + (32 / dist)*(x1 - x2);
	//int y3 = y1 + (32 / dist)*(y1 - y2);
	//for (int i = ; i < 10; i = -1.2 * i)
	//{
	//	if ()
	//	{
	//		runner->move(BWAPI::Position());
	//		return BWAPI::Position();
	//	}
	//}



	//}

	//Broodwar->drawLineMap(x1, y1, x2, y2, Colors::Orange);
	//Broodwar->drawLineMap(x1, y1, 2 * x1 - x2, 2 * y1 - y2, Colors::Purple);
	//Broodwar->drawCircle(CoordinateType::Map, 2 * x1 - x2, 2 * y1 - y2, 10, Colors::Purple, true);
}

BWAPI::Position RunAway(BWAPI::Unitset runner, BWAPI::Unit runnee)
{
	if (runner.size() < 1 || runnee == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}
	int x1 = runner.getPosition().x;
	int y1 = runner.getPosition().y;
	int x2 = runnee->getPosition().x;
	int y2 = runnee->getPosition().y;

	runner.move(BWAPI::Position(2 * x1 - x2, 2 * y1 - y2));
	return BWAPI::Position(2 * x1 - x2, 2 * y1 - y2);
}

BWAPI::Position RunAway(BWAPI::Unitset runner, BWAPI::Position runnee)
{
	if (runner.size() < 1)
	{
		return BWAPI::Position(-1, -1);
	}
	int x1 = runner.getPosition().x;
	int y1 = runner.getPosition().y;
	int x2 = runnee.x;
	int y2 = runnee.y;

	runner.move(BWAPI::Position(2 * x1 - x2, 2 * y1 - y2));
	return BWAPI::Position(2 * x1 - x2, 2 * y1 - y2);
}

BWAPI::Position RunAway(BWAPI::Unit runner, BWAPI::Position runnee)
{
	if (runner == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}
	int x1 = runner->getPosition().x;
	int y1 = runner->getPosition().y;
	int x2 = runnee.x;
	int y2 = runnee.y;

	runner->move(BWAPI::Position(2 * x1 - x2, 2 * y1 - y2));
	return BWAPI::Position(2 * x1 - x2, 2 * y1 - y2);
}


BWAPI::Position RunTowards(BWAPI::Unit runner, BWAPI::Unit runnee)
{
	if (runner == nullptr || runnee == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}
	int x1 = runner->getPosition().x;
	int y1 = runner->getPosition().y;
	int x2 = runnee->getPosition().x;
	int y2 = runnee->getPosition().y;
	runner->move(BWAPI::Position(2 * x2 - x1, 2 * y2 - y1));
	return BWAPI::Position(2 * x2 - x1, 2 * y2 - y1);
	//Broodwar->drawLineMap(x1, y1, x2, y2, Colors::Orange);
	//Broodwar->drawLineMap(x1, y1, 2 * x2 - x1, 2 * y2 - y1, Colors::Purple);
	//Broodwar->drawCircle(CoordinateType::Map, 2 * x2 - x1, 2 * y2 - y1, 10, Colors::Purple, true);
}

BWAPI::Position RunTowards(BWAPI::Unit runner, BWAPI::Position runnee)
{
	if (runner == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}
	int x1 = runner->getPosition().x;
	int y1 = runner->getPosition().y;
	int x2 = runnee.x;
	int y2 = runnee.y;
	runner->move(BWAPI::Position(2 * x2 - x1, 2 * y2 - y1));
	return BWAPI::Position(2 * x2 - x1, 2 * y2 - y1);
}


BWAPI::Position RunSideways(BWAPI::Unit runner, BWAPI::Unit runnee, bool left = false)
{

	//Broodwar << "top" << runner->getTop() << "right" << runner->getRight() << "bottom" << runner->getBottom() << "left" << runner->getLeft() << std::endl;
	//Broodwar << "mapWidth * 32 - 32" << Broodwar->mapWidth() * 32 - 96 << "mapHeight * 32 - 32" << Broodwar->mapHeight() * 32 - 96 << std::endl;

	if (runner == nullptr || runnee == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}

	if (runner->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
	{
		if (ShuttleMap[runner].sideSwap < 1)
		{
			ShuttleMap[runner].sideSwap = 300;
			if (runner->getTop() < 96 || runner->getRight() > Broodwar->mapWidth() * 32 - 96 || runner->getBottom() > Broodwar->mapHeight() * 32 - 96 || runner->getLeft() < 96)
			{
				left = !left;
				ShuttleMap[runner].direction = !ShuttleMap[runner].direction;
				LatencyMap[runner] = Broodwar->getLatencyFrames() + 24;
			}
		}
	}
	else if (runner->getType() == BWAPI::UnitTypes::Protoss_Corsair)
	{
		if (CorsairMap[runner].sideSwap < 1)
		{
			CorsairMap[runner].sideSwap = 400;
			if (runner->getTop() < 96 || runner->getRight() > Broodwar->mapWidth() * 32 - 96 || runner->getBottom() > Broodwar->mapHeight() * 32 - 96 || runner->getLeft() < 96)
			{
				left = !left;
				CorsairMap[runner].direction = !CorsairMap[runner].direction;
				LatencyMap[runner] = Broodwar->getLatencyFrames() + 24;
			}
		}
	}

	int x1 = runner->getPosition().x;
	int y1 = runner->getPosition().y;
	int x2 = runnee->getPosition().x;
	int y2 = runnee->getPosition().y;
	if (left)
	{
		if (x2 - x1 == 0)
		{
			runner->move(BWAPI::Position(x1, 100));
			return BWAPI::Position(x1, 100);
		}
		else if (x2 - x1 > 0)
		{
			int y3 = y1 - 100 * 1;
			int x3 = x1 + 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
		else
		{
			int y3 = y1 + 100 * 1;
			int x3 = x1 - 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
		//Broodwar->drawLineMap(x1, y1, x2, y2, Colors::Orange);
		//Broodwar->drawLineMap(x1, y1, x1 + y1 - y2, y1 + x1 - x2, Colors::Purple);
		//Broodwar->drawCircle(CoordinateType::Map, x1 + y1 - y2, y1 + x1 - x2, 10, Colors::Purple, true);
	}
	else
	{
		if (x2 - x1 == 0)
		{
			runner->move(BWAPI::Position(x1, 100));
			return BWAPI::Position(x1, 100);
		}
		else if (x2 - x1 > 0)
		{
			int y3 = y1 + 100 * 1;
			int x3 = x1 - 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
		else //if (x2 - x1 < 0)
		{
			int y3 = y1 - 100 * 1;
			int x3 = x1 + 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
		//Broodwar->drawLineMap(x1, y1, x2, y2, Colors::Orange);
		//Broodwar->drawLineMap(x1, y1, x1 + y2 - y1, y1 + x2 - x1, Colors::Purple);
		//Broodwar->drawCircle(CoordinateType::Map, x1 + y2 - y1, y1 + x2 - x1, Colors::Purple, true);
	}

	//Broodwar->drawTextMap(runner->getPosition(), "Running Sideways");

}

BWAPI::Position RunSideways(BWAPI::Unit runner, BWAPI::Position runnee, bool left = false)
{
	if (runner == nullptr)
	{
		return BWAPI::Position(-1, -1);
	}

	if (runner->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
	{
		if (ShuttleMap[runner].sideSwap < 1)
		{
			ShuttleMap[runner].sideSwap = 300;
			if (runner->getTop() < 96 || runner->getRight() > Broodwar->mapWidth() * 32 - 96 || runner->getBottom() > Broodwar->mapHeight() * 32 - 96 || runner->getLeft() < 96)
			{
				left = !left;
				ShuttleMap[runner].direction = !ShuttleMap[runner].direction;
				LatencyMap[runner] = Broodwar->getLatencyFrames() + 24;
			}
		}
	}
	else if (runner->getType() == BWAPI::UnitTypes::Protoss_Corsair)
	{
		if (CorsairMap[runner].sideSwap < 1)
		{
			CorsairMap[runner].sideSwap = 400;
			if (runner->getTop() < 96 || runner->getRight() > Broodwar->mapWidth() * 32 - 96 || runner->getBottom() > Broodwar->mapHeight() * 32 - 96 || runner->getLeft() < 96)
			{
				left = !left;
				CorsairMap[runner].direction = !CorsairMap[runner].direction;
				LatencyMap[runner] = Broodwar->getLatencyFrames() + 24;
			}
		}
	}

	int x1 = runner->getPosition().x;
	int y1 = runner->getPosition().y;
	int x2 = runnee.x;
	int y2 = runnee.y;
	if (left)
	{
		if (x2 - x1 == 0)
		{
			runner->move(BWAPI::Position(x1, 100));
			return BWAPI::Position(x1, 100);
		}
		else if (x2 - x1 > 0)
		{
			int y3 = y1 - 100 * 1;
			int x3 = x1 + 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
		else
		{
			int y3 = y1 + 100 * 1;
			int x3 = x1 - 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
	}
	else
	{
		if (x2 - x1 == 0)
		{
			runner->move(BWAPI::Position(x1, 100));
			return BWAPI::Position(x1, 100);
		}
		else if (x2 - x1 > 0)
		{
			int y3 = y1 + 100 * 1;
			int x3 = x1 - 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
		else
		{
			int y3 = y1 - 100 * 1;
			int x3 = x1 + 100 * (y2 - y1) / (x2 - x1);
			runner->move(BWAPI::Position(x3, y3));
			return BWAPI::Position(x3, y3);
		}
	}

}

#endif