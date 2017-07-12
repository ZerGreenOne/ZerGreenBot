#include "ZerGreenBot.h"
#include <time.h>
#include <iostream>
#include "VariableDeclarations.h"
#include "FFEPlacement.h"
#include "RunDirection.h"

//#define DRAGOON_DEFAULT_POSITION Position( -1, -1 )

using namespace BWAPI;
using namespace Filter;

void CreateEpicText(std::string Text, Position p, int duration)
{
	EpicText newText;
	newText.Position = p;
	newText.Text = Text;
	newText.time = duration;
	EpicTextSet.insert(newText);
}

void CreateEpicText(std::string Text, int x, int y, int duration)
{
	EpicText newText;
	newText.Position = Position(x,y);
	newText.Text = Text;
	newText.time = duration;
	EpicTextSet.insert(newText);
}

void SetNexusMinerals(Unit u)
{
	for (const auto &m : BWTA::getNearestBaseLocation(u->getTilePosition())->getMinerals())
	{
		NexusMap[u].Patches.insert(m);
		PatchVars tempVars;
		tempVars.Probes = 0;
		PatchMap[m] = tempVars;
	}
	for (const auto &g : BWTA::getNearestBaseLocation(u->getTilePosition())->getGeysers())
	{
		NexusMap[u].Geysers.insert(g);
		GeyserVars tempVars;
		tempVars.Probes = 0;
		GeyserMap[g] = tempVars;
	}

}

void NewNexus(Unit u)
{
	NexusVars tempVars;
	NexusMap[u] = tempVars;
	bases.insert(u);
	LatencyMap[u] = 0;
}

void NewProbe(Unit u)
{
	ProbeVars tempVars;
	tempVars.Patch = nullptr;
	ProbeMap[u] = tempVars;
	harvesters.insert(u);
	LatencyMap[u] = 0;
}

void NewShuttle(Unit u)
{
	ShuttleVars tempVars;
	tempVars.Reaver[0] = nullptr;
	tempVars.Reaver[1] = nullptr;
	tempVars.Zealot[0] = nullptr;
	tempVars.Zealot[1] = nullptr;
	tempVars.Zealot[2] = nullptr;
	tempVars.Zealot[3] = nullptr;
	tempVars.CurrentDestination = SHUTTLE_UNDEFINED_POSITION;
	tempVars.direction = false;
	tempVars.sideSwap = 0;

	ShuttleMap[u] = tempVars;
	Shuttles.insert(u);
	LatencyMap[u] = 0;
}

void NewReaver(Unit u)
{
	ReaverVars tempVars;
	tempVars.Shuttle = nullptr;
	ReaverMap[u] = tempVars;
	Reavers.insert(u);
	LatencyMap[u] = 0;
}

void NewObs(Unit u)
{
	ObsVars tempVars;
	tempVars.Location = Army;
	ObsMap[u] = tempVars;
	Obses.insert(u);
	LatencyMap[u] = 0;
}

void NewDragoon(Unit u)
{
	DragoonVars tempVars;
	tempVars.timeSinceAttackBegin = 0;
	for (int i = 0; i < DRAGOON_WAYPOINTS; i++)
	{
		tempVars.prevPosition[i] = Position(-1, -1);
	}
	tempVars.unstuck = 0;
	DragoonMap[u] = tempVars;
	Dragoons.insert(u);
	LatencyMap[u] = 0;
}

/*UnitFilter MineralFree(
	bool test(Unit u){ return true; }
	);*/

bool mineralFree(Unit u)
{
	if (PatchMap[u].Probes < 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool mineralOne(Unit u)
{
	if (PatchMap[u].Probes < 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isHarvester(Unit u)
{
	if (harvesters.contains(u) && ProbeMap[u].Patch != nullptr && ProbeMap[u].Patch->getType() != UnitTypes::Protoss_Assimilator)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool enoughMineralsAndGas(Unit u)
{
	if (u->getType().mineralPrice() < Broodwar->self()->minerals() || u->getType().gasPrice() < Broodwar->self()->gas())
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool enoughMineralsAndGas(UnitType u)
{
	if (u.mineralPrice() < Broodwar->self()->minerals() || u.gasPrice() < Broodwar->self()->gas())
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool isGeyser(Unit u)
{
	if (u->getType() == UnitTypes::Resource_Vespene_Geyser || u->getType() == UnitTypes::Protoss_Assimilator)
	{
		return true;
	}
	return false;
}

UnitFilter IsGeyser = isGeyser;

bool hasAssimilator(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Assimilator)
	{
		return true;
	}
	return false;
}

UnitFilter HasAssimilator = hasAssimilator;

bool assimilatorTransferFilter(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Probe && harvesters.contains(u) && ProbeMap[u].Patch != nullptr && ProbeMap[u].Patch->getType() != UnitTypes::Protoss_Assimilator)
	{
		return true;
	}
	return false;
}

UnitFilter AssimilatorTransferFilter = assimilatorTransferFilter;

bool isReaver(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Reaver)
	{
		return true;
	}
	return false;
}

UnitFilter IsReaver = isReaver;

bool isDragoon(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Dragoon)
	{
		return true;
	}
	return false;
}

UnitFilter IsDragoon = isDragoon;

bool isPylon(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Pylon)
	{
		return true;
	}
	return false;
}

UnitFilter IsPylon = isPylon;

bool isCannon(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Photon_Cannon)
	{
		return true;
	}
	return false;
}

UnitFilter IsCannon = isCannon;

bool isStargate(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Stargate)
	{
		return true;
	}
	return false;
}

UnitFilter IsStargate = isStargate;

bool isZealot(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Zealot)
	{
		return true;
	}
	return false;
}

UnitFilter IsZealot = isZealot;

bool isCarrier(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Carrier)
	{
		return true;
	}
	return false;
}

UnitFilter IsCarrier = isCarrier;

namespace compareNamespace{
	Unit currentUnit;
}

bool unitLevelSame(Unit u)
{
	if (Broodwar->getGroundHeight(u->getTilePosition()) != Broodwar->getGroundHeight(compareNamespace::currentUnit->getTilePosition()))
	{
		return false;
	}
	return true;
}

UnitFilter UnitLevelSame = unitLevelSame;

bool unitTypeCompare(Unit u)
{
	if (u->getType() == compareNamespace::currentUnit->getType())
	{
		return true;
	}
	return false;
}

UnitFilter UnitTypeCompare = unitTypeCompare;

bool unitOwnerCompare(Unit u)
{
	if (u->getPlayer() == compareNamespace::currentUnit->getPlayer())
	{
		return true;
	}
	return false;
}

UnitFilter UnitOwnerCompare = unitOwnerCompare;

bool isSameUnit(Unit u)
{
	if (u == compareNamespace::currentUnit)
	{
		return true;
	}
	return false;
}

UnitFilter IsSameUnit = isSameUnit;

bool isObserver(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Observer)
	{
		return true;
	}
	return false;
}

UnitFilter IsObserver = isObserver;

bool detectUnit(Unit u)
{
	if ((!IsCloaked && !IsBurrowed)(u) || Obses.size() > 0)
	{
		return true;
	}
	return false;
}

UnitFilter DetectUnit = detectUnit;

bool reaverNoShuttle(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Reaver && ReaverMap[u].Shuttle == nullptr)
	{
		return true;
	}
	return false;
}

UnitFilter ReaverNoShuttle = reaverNoShuttle;

bool zealotNoShuttle(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Zealot && ZealotMap[u].Shuttle == nullptr)
	{
		return true;
	}
	return false;
}

UnitFilter ZealotNoShuttle = zealotNoShuttle;

TilePosition getCustomBuildLocation(UnitType type, TilePosition desiredPosition)
{
	if (type == UnitTypes::Protoss_Pylon)
	{
		TilePosition cpbp = TilePosition(initialPylonBuildPosition.x, initialPylonBuildPosition.y - 3); // Note: cpbp == currentPylonBuildPosition
		for (; Broodwar->isBuildable(cpbp) && cpbp.isValid(); cpbp = TilePosition(cpbp.x, cpbp.y - 2))
		{
			if (Broodwar->canBuildHere(cpbp, UnitTypes::Protoss_Pylon))
			{
				return cpbp;
			}
		}
		cpbp = initialPylonBuildPosition;
		for (; Broodwar->isBuildable(cpbp) && cpbp.isValid(); cpbp = TilePosition(cpbp.x, cpbp.y + 2))
		{
			if (Broodwar->canBuildHere(cpbp, UnitTypes::Protoss_Pylon))
			{
				return cpbp;
			}
		}
		for (unsigned int e = 1; e < Expansions.size(); e++)
		{
			cpbp = ExpPylPos[e];
			for (; Broodwar->isBuildable(cpbp) && cpbp.isValid() && ExpXdir[e] != 0; cpbp = TilePosition(cpbp.x + ExpXdir[e], cpbp.y))
			{
				if (Broodwar->canBuildHere(cpbp, UnitTypes::Protoss_Pylon))
				{
					return cpbp;
				}
			}
			cpbp = ExpPylPos[e];
			for (; Broodwar->isBuildable(cpbp) && cpbp.isValid() && ExpYdir[e] != 0; cpbp = TilePosition(cpbp.x, cpbp.y + ExpYdir[e]))
			{
				if (Broodwar->canBuildHere(cpbp, UnitTypes::Protoss_Pylon))
				{
					return cpbp;
				}
			}
		}
		return FAILED_PLACEMENT_POSITION;
	}
	else if (type == UnitTypes::Protoss_Assimilator)
	{
		for (unsigned int e = 0; e < Expansions.size(); e++)
		{
			if (!hasAssimilator(Broodwar->getClosestUnit(Expansions[e]->getPosition(), IsGeyser)))
			{
				return Broodwar->getClosestUnit(Expansions[e]->getPosition(), IsGeyser)->getTilePosition();
			}
		}
		return FAILED_PLACEMENT_POSITION;
	}
	else if (type == UnitTypes::Protoss_Nexus)
	{
		for (unsigned int e = 0; e < Expansions.size(); e++)
		{
			if (Broodwar->canBuildHere(Expansions[e]->getTilePosition(), type, nullptr))
			{
				return Expansions[e]->getTilePosition();
			}
		}
		return FAILED_PLACEMENT_POSITION;
	}
	else // Standard
	{
		TilePosition cobp;
		if (type.width() + 32 - (type.width() % 32) == 128) // Gateway, Stargate
		{
			cobp = TilePosition(initialOtherBuildPosition[0].x, initialOtherBuildPosition[0].y - 4);
			for (; Broodwar->isBuildable(cobp) && cobp.isValid(); cobp = TilePosition(cobp.x, cobp.y - 3))
			{
				if (Broodwar->canBuildHere(cobp, type))
				{
					return cobp;
				}
			}
			cobp = initialOtherBuildPosition[0];
			for (; Broodwar->isBuildable(cobp) && cobp.isValid(); cobp = TilePosition(cobp.x, cobp.y + 3))
			{
				if (Broodwar->canBuildHere(cobp, type))
				{
					return cobp;
				}
			}

			// Around Expos

			for (unsigned int e = 1; e < Expansions.size(); e++)
			{
				cobp = TilePosition(ExpGatePos[e].x, ExpGatePos[e].y);
				for (; Broodwar->isBuildable(cobp) && cobp.isValid() && ExpXdir[e] != 0; cobp = TilePosition(cobp.x + 2 * ExpXdir[e], cobp.y))
				{
					if (Broodwar->canBuildHere(cobp, type))
					{
						return cobp;
					}
				}
				cobp = TilePosition(ExpGatePos[e].x, ExpGatePos[e].y);
				for (; Broodwar->isBuildable(cobp) && cobp.isValid() && ExpYdir[e] != 0; cobp = TilePosition(cobp.x, cobp.y + (3 * ExpYdir[e] / 2)))
				{
					if (Broodwar->canBuildHere(cobp, type))
					{
						return cobp;
					}
				}
			}


		}
		else // Tech buildings
		{
			cobp = TilePosition(initialOtherBuildPosition[1].x, initialOtherBuildPosition[1].y - 3);
			for (; Broodwar->isBuildable(cobp) && cobp.isValid(); cobp = TilePosition(cobp.x, cobp.y - 2))
			{
				if (Broodwar->canBuildHere(cobp, type))
				{
					return cobp;
				}
			}
			cobp = initialOtherBuildPosition[1];
			for (; Broodwar->isBuildable(cobp) && cobp.isValid(); cobp = TilePosition(cobp.x, cobp.y + 2))
			{
				if (Broodwar->canBuildHere(cobp, type))
				{
					return cobp;
				}
			}
		}
		/*TilePosition cobp; // Note: cobp == currentOtherBuildPosition
		for (int k = 0; k < 2; k++)
		{
		cobp = TilePosition(initialOtherBuildPosition[k].x, initialOtherBuildPosition[k].y - 4);
		for (; Broodwar->isBuildable(cobp) && cobp.isValid(); cobp = TilePosition(cobp.x, cobp.y - 3))
		{
		if (Broodwar->canBuildHere(cobp, type))
		{
		return cobp;
		}
		}
		cobp = initialOtherBuildPosition[k];
		for (; Broodwar->isBuildable(cobp) && cobp.isValid(); cobp = TilePosition(cobp.x, cobp.y + 3))
		{
		if (Broodwar->canBuildHere(cobp, type))
		{
		return cobp;
		}
		}
		}*/
		return FAILED_PLACEMENT_POSITION;
	}



	//int x = -100;
	//int y = -100;
	//TilePosition currentPosition;
	//if (type == UnitTypes::Protoss_Pylon)
	//{
	//	currentPosition = Broodwar->getBuildLocation(type, desiredPosition);
	//	while (currentPosition == TilePositions::Invalid)
	//	{
	//		desiredPosition = TilePosition(desiredPosition.x + (rand() % 3 - 1), desiredPosition.y + (rand() % 3 - 1));
	//		currentPosition = Broodwar->getBuildLocation(type, desiredPosition);
	//	}
	//	return currentPosition;
	//}
	//else if (type == UnitTypes::Protoss_Assimilator)
	//{
	//	for (unsigned int e = 0; e < Expansions.size(); e++)
	//	{
	//		if (!hasAssimilator(Broodwar->getClosestUnit(Expansions[e]->getPosition(), IsGeyser)))
	//		{
	//			return Broodwar->getClosestUnit(Expansions[e]->getPosition(), IsGeyser)->getTilePosition();
	//		}
	//	}
	//	return  Broodwar->getBuildLocation(type, desiredPosition);
	//}
	//else if (type == UnitTypes::Protoss_Nexus)
	//{
	//	for (unsigned int e = 0; e < Expansions.size(); e++)
	//	{
	//		if (Broodwar->canBuildHere(Expansions[e]->getTilePosition(), type, nullptr))
	//		{
	//			return Expansions[e]->getTilePosition();
	//		}
	//	}
	//}
	//else
	//{
	//	int px = mainBuildPosition.x;
	//	int py = mainBuildPosition.y;

	//	for (y = -100; x < 100; y++, y++)  //Used to be 4
	//	{
	//		if (y > 100)
	//		{
	//			y = -100;
	//			x++;
	//			x++;
	//		}
	//		currentPosition = TilePosition((32 * px + 32 * x) / 32, (32 * py + 32 * y) / 32);
	//		Broodwar->drawDot(CoordinateType::Map, (32 * px + 32 * x), (32 * py + 32 * y), Colors::Orange);
	//		//Broodwar << "x" << x << "y" << y << "fx" << (32  *px + 32 * x) / 32 << "fy" << (32 * px + 32 * y) / 32 << "px" << px << "py" << py << std::endl;
	//		if (Broodwar->canBuildHere(currentPosition, type, nullptr, true) && currentPosition.getDistance(Broodwar->getClosestUnit(TilePositionConvert(mainBuildPosition), IsGeyser)->getTilePosition()) > 7)
	//		{
	//			return currentPosition;
	//		}
	//	}
	//}
	//return TilePosition(-1, -1);
}

Position TilePositionConvert(TilePosition t)
{
	return Position(t.x * 32, t.y * 32);
}

TilePosition TilePositionConvert(Position t)
{
	return TilePosition((t.x - (t.x % 32)) / 32, (t.y - (t.y % 32)) / 32);
}



UnitFilter MineralFree = mineralFree;
UnitFilter MineralOne = mineralOne;
UnitFilter IsHarvester = isHarvester;

// ------------------------------------------------------------------
// Against All
// ------------------------------------------------------------------

void HarvesterDecide(Unit u)
{
	Unit Patch = ProbeMap[u].Patch;

	//Broodwar->drawCircleMap(TilePositionConvert(u->getPosition()).x * 32, TilePositionConvert(u->getPosition()).y * 32, 10, Colors::Green, true);
	//Broodwar->drawCircleMap(u->getTilePosition().x * 32, u->getTilePosition().y * 32, 5, Colors::Orange, true);

	//RunSideways(u, *bases.begin());
	//Broodwar->drawLineMap(u->getPosition(), u->getOrderTargetPosition(), Colors::Orange);

	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
	}
	else
	{
		Unit AttackUnit = u->getClosestUnit(IsEnemy && !IsFlying && !IsCloaked, 48);
		if (AttackUnit != nullptr && u->getGroundWeaponCooldown() < 1 && u->getHitPoints() + u->getShields() > 16)
		{
			u->attack(AttackUnit);
			LatencyMap[u] = Broodwar->getLatencyFrames();
			return;
		}
		else if (u->getShields() < 20)
		{
			RunAway(u, AttackUnit);
			return;
		}

		if (u->isCarryingMinerals() || u->isCarryingGas())
		{
			if (u->getOrder() != Orders::ReturnMinerals && u->getOrder() != Orders::ReturnGas)
			{
				u->returnCargo();
				LatencyMap[u] = Broodwar->getLatencyFrames();
			}
		}
		else if (Patch == nullptr || !Patch->exists())
		{
			for (auto const &b : bases)
			{
				bool foundPatch = false;
				for (auto const &m : BWTA::getNearestBaseLocation(b->getPosition())->getMinerals())
				{
					if (foundPatch)
					{
						break;
					}
					if (mineralFree(m))
					{
						ProbeMap[u].Patch = m;
						foundPatch = true;
					}
				}
				for (auto const &m : BWTA::getNearestBaseLocation(b->getPosition())->getMinerals())
				{
					if (foundPatch)
					{
						break;
					}
					if (mineralOne(m))
					{
						ProbeMap[u].Patch = m;
						foundPatch = true;
					}
				}
				if (!foundPatch && u->isIdle())
				{
					u->gather(u->getClosestUnit(IsMineralField));
				}
			}

			//if (!(ProbeMap[u].Patch = u->getClosestUnit(IsMineralField && MineralFree, 384)))
			//{
			//	if (!(ProbeMap[u].Patch = u->getClosestUnit(IsMineralField && MineralOne, 384)))
			//	{
			//		bool foundPatch = false;
			//		for (auto const &b : bases)
			//		{
			//			for (auto const &m : BWTA::getNearestBaseLocation(b->getPosition())->getMinerals())
			//			{
			//				if (PatchMap[m].Probes < 2)
			//				{
			//					u->gather(m);
			//					foundPatch = true;
			//				}
			//			}
			//		}
			//		if (!foundPatch)
			//		{
			//			u->gather(u->getClosestUnit(IsMineralField, 384));
			//		}
			//	}
			//}
			PatchMap[ProbeMap[u].Patch].Probes++;
			LatencyMap[u] = Broodwar->getLatencyFrames();
		}
		else
		{
			if (u->isIdle() || (u->getOrderTarget() != Patch))
			{
				u->gather(Patch);
				LatencyMap[u] = Broodwar->getLatencyFrames();
			}
		}
	}
}

Unit BuilderDecide(Unit u)
{
	BuilderVars * MapPointer = &BuilderMap[u];
	if (u == nullptr || !u->exists())
	{
		if (u == nullptr)
		{
			Broodwar << "Error, u==nullptr in BuilderDecide" << std::endl;
		}
		else
		{
			Broodwar << "Error, u->exists == false in BuilderDecide" << std::endl;
		}
		return nullptr;
	}

	if (BuilderMap[u].MaxTimer < 1)
	{
		MapPointer->BuildType = UnitTypes::None;
		harvesters.insert(u);
		return u;
	}
	else
	{
		BuilderMap[u].MaxTimer--;
	}

	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return nullptr;
	}

	if (MapPointer->BuildPosition == FAILED_PLACEMENT_POSITION)
	{
		Broodwar << "Error Initially Placing Building" << std::endl;
		u->stop();
		MapPointer->BuildPosition = getCustomBuildLocation(MapPointer->BuildType, mainBuildPosition);
	}

	if (MapPointer->BuildType != UnitTypes::None)
	{
		if ((!enoughMineralsAndGas(MapPointer->BuildType) || !Broodwar->isExplored(MapPointer->BuildPosition)) && u->getOrder() != Orders::Move && u->getOrder() != Orders::PlaceBuilding)
		{
			u->move(TilePositionConvert(MapPointer->BuildPosition));
			LatencyMap[u] = Broodwar->getLatencyFrames();
		}
		else
		{
			if (!u->build(MapPointer->BuildType, MapPointer->BuildPosition) && MapPointer->BuildType != UnitTypes::Protoss_Nexus && MapPointer->BuildType != UnitTypes::Protoss_Assimilator)
			{
				if (MapPointer->RecalculateTimer < 1 && !MapPointer->Immutable)
				{
					MapPointer->RecalculateTimer = 100;
					MapPointer->BuildPosition = getCustomBuildLocation(MapPointer->BuildType, MapPointer->BuildPosition);
					if (u->getOrder() != Orders::Move && u->getOrder() != Orders::PlaceBuilding)
					{

						u->move(TilePositionConvert(MapPointer->BuildPosition));
						LatencyMap[u] = Broodwar->getLatencyFrames();
					}
				}
				else
				{
					MapPointer->RecalculateTimer--;
				}
			}
			else
			{
				LatencyMap[u] = Broodwar->getLatencyFrames();
			}
		}
	}
	return nullptr;
}

Unit RequestBuilder(TilePosition buildPosition)
{
	BuilderVars tempVars;
	tempVars.Immutable = false;
	Unit newBuilder = Broodwar->getClosestUnit(Position(buildPosition.x * 32, buildPosition.y * 32), IsHarvester);
	if (newBuilder == nullptr)
	{
		newBuilder = Broodwar->getClosestUnit(Position(mainBuildPosition.x * 32, mainBuildPosition.y * 32), IsHarvester);
	}
	harvesters.erase(newBuilder);
	builders.insert(newBuilder);
	BuilderMap[newBuilder] = tempVars;
	return newBuilder;
}

Unit RequestBuilding(UnitType BuildingType, TilePosition buildPosition = mainBuildPosition)
{
	Unit u;
	u = RequestBuilder(buildPosition);
	BuilderMap[u].BuildType = BuildingType;
	BuilderMap[u].BuildPosition = getCustomBuildLocation(BuildingType, buildPosition);
	LatencyMap[u] = Broodwar->getLatencyFrames();
	return u;
}

void UseBuilders()
{
	Unitset removedBuilders;

	for (const auto &u : builders)//const auto &u : builders)
	{

		if (u == nullptr)
		{
			continue;
		}

		Broodwar->drawTextMap(u->getPosition(), "%s", BuilderMap[u].BuildType.getName().c_str());
		UnitType BuildType = BuilderMap[u].BuildType;
		int width = BuildType.width() - (BuildType.width() % 32) + 32;
		int height = BuildType.height() - (BuildType.height() % 32) + 32;
		int centreX = BuilderMap[u].BuildPosition.x * 32 + (int)(0.5 * width);
		int centreY = BuilderMap[u].BuildPosition.y * 32 + (int)(0.5 * height);
		Position TopLeft = Position(centreX - BuildType.dimensionLeft(), centreY - BuildType.dimensionUp());
		Position BottomRight = Position(centreX + BuildType.dimensionRight(), centreY + BuildType.dimensionDown());
		Broodwar->drawBoxMap(TopLeft, BottomRight, Colors::Red, false);
		if (LatencyMap[u] > 0)
		{
			LatencyMap[u]--;
			continue;
		}
		//Broodwar << "I use my Builders" << std::endl;
		Unit currentUnit;
		currentUnit = BuilderDecide(u);
		if (currentUnit != nullptr)
		{
			removedBuilders.insert(u);
		}
	}

	for (const auto &u : removedBuilders)
	{
		builders.erase(u);
	}
}

Unit RequestScout()
{
	/*std::unordered_map<Unit, ScoutVars> testMap;
	ScoutVars testVars;
	testVars.ExtraTime = 0;
	Unit newScout = nullptr;
	testMap[newScout] = testVars;*/


	Unit newScout = Broodwar->getClosestUnit(mainBase->getPosition(), IsHarvester);
	ScoutVars tempVars;
	tempVars.ExtraTime = 0;
	ScoutMap[newScout] = tempVars;
	harvesters.erase(newScout);
	scouts.insert(newScout);
	return newScout;
}

void RequestDefender()
{
	Unit newDefender = Broodwar->getClosestUnit(mainBase->getPosition(), IsHarvester);
	if (newDefender == nullptr || !newDefender->exists())
	{
		Broodwar << "No defender probes Available, I'm probably screwed" << std::endl;
		return;
	}
	harvesters.erase(newDefender);
	defenders.insert(newDefender);
}

bool CheckDefenders()
{
	int attack = mainBase->getUnitsInRadius(800, IsEnemy && !IsWorker && !IsFlying && !IsCloaked && !IsBurrowed).size();
	int defence = Dragoons.size() + Zealots.size() + (int)(0.5 * defenders.size());

	//Broodwar << mainBase << std::endl;

	Broodwar->drawCircleMap(mainBase->getPosition(), 800, Colors::Red);

	Broodwar->drawCircleMap(mainBase->getPosition(), 1600, Colors::Red);

	if (attack > defence)
	{
		for (int i = 0; i < attack - defence; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				RequestDefender();
			}
		}
	}
	else if (attack < 1)
	{
		return false;
	}
	return true;
}

void RemoveDefenders()
{
	Unitset removeDefenders;
	for (auto const &u : defenders)
	{
		removeDefenders.insert(u);
		harvesters.insert(u);
	}
	for (auto const &u : removeDefenders)
	{
		defenders.erase(u);
	}
}

void DefenderDecide(Unit u)
{
	if (u == nullptr)
	{
		return;
	}
	if (!(u->exists()))
	{
		return;
	}

	Unit AttackUnit = u->getClosestUnit(IsEnemy && !IsWorker && !IsFlying, 800);
	if (AttackUnit != nullptr)
	{
		if (u->getGroundWeaponCooldown() < 1)
		{
			u->attack(AttackUnit);
		}
		else
		{
			u->gather(u->getClosestUnit(IsMineralField));
			return;
		}
	}
}

void CheckSupply()
{
	int supplyUsed = Broodwar->self()->supplyUsed();
	if (allSupply - supplyUsed < -1 + 8 * log(supplyUsed / 8) && Broodwar->self()->minerals() > 80)
	{
		RequestBuilding(UnitTypes::Protoss_Pylon);
		allSupply += 16;
	}
}

void groupedUnits(Unit u, Unitset & uSet)
{
	if (u == nullptr)
	{
		return;
	}
	compareNamespace::currentUnit = u;
	for (auto const &n : u->getUnitsInRadius(100, UnitTypeCompare && UnitOwnerCompare))
	{
		if (!uSet.contains(n))
		{
			uSet.insert(n);
			groupedUnits(n, uSet);
			compareNamespace::currentUnit = u;
			Broodwar->drawLineMap(u->getPosition(), n->getPosition(), Colors::White);
		}
	}
}

void checkCancel()
{
	for (const auto &u : constructingBuildings)
	{
		if (u->getHitPoints() < 50 && u->isUnderAttack())
		{
			u->cancelConstruction();
		}
	}
}

/*void CreateUnitGroups()
{
Unitset currentAllUnits;
UnitType currentType = UnitTypes::None;
Unitset removeSet;
Unitset currentSet;

for (auto const &u : currentAllUnits)
{
if (u == nullptr || !u->exists());
{
removeSet.insert(u);
}
}
for (auto const &r : removeSet)
{
currentAllUnits.erase(r);
}

while (currentAllUnits.size() > 0)
{
currentType = UnitTypes::None;
for (auto const &u : currentAllUnits)
{
if (currentType == UnitTypes::None)
{
currentType = u->getType();
break;
}
}
for (auto const &u : currentAllUnits)
{
if (u->getType() == currentType)
{
currentSet.insert(u);
}
}

}
}*/


// ------------------------------------------------------------------
// Against Terran
// ------------------------------------------------------------------
//
//	Currently goes for a 2 gateway observer build
//
//	Should - - - Mixes High Templar / Arbiter into late game play
//
//	Does decent early game (with dragoons)
//
//	Does decent late game (with carriers)
//
//	Needs to switch up dragoon strategy in the midgame (defined by the reasearch of legspeed), get more zealots and all group up and go in at the same time.
//
//	Also zealot/shuttle AI needs improvement.
//

bool isTank(Unit u)
{
	if (u->getType() == UnitTypes::Terran_Siege_Tank_Siege_Mode || u->getType() == UnitTypes::Terran_Siege_Tank_Tank_Mode)
	{
		return true;
	}
	return false;
}

UnitFilter IsTank = isTank;

bool isVulture(Unit u)
{
	if (u->getType() == UnitTypes::Terran_Vulture)
	{
		return true;
	}
	return false;
}

UnitFilter IsVulture = isVulture;


bool isSpiderMine(Unit u)
{
	if (u->getType() == UnitTypes::Terran_Vulture_Spider_Mine)
	{
		return true;
	}
	return false;
}

UnitFilter IsSpiderMine = isSpiderMine;

bool isMissileTurret(Unit u)
{
	if (u->getType() == UnitTypes::Terran_Missile_Turret)
	{
		return true;
	}
	return false;
}

UnitFilter IsMissileTurret = isMissileTurret;

//bool isBio(Unit u)
//{
//	if (u->getType() == UnitTypes::Terran_Marine || u->getType() == UnitTypes::Terran_Firebat || u->getType() == UnitTypes::)
//}

bool isMarine(Unit u)
{
	if (u->getType() == UnitTypes::Terran_Marine)
	{
		return true;
	}
	return false;
}

void TerranCheckGateway()
{
	if (numGates < 1 && pylons.size() > 0 && Broodwar->self()->minerals() > 100)
	{
		RequestBuilding(UnitTypes::Protoss_Gateway);
		numGates++;
	}
	else if (numGates < 2 && numFacCreate > 0 && Broodwar->self()->minerals() > 100)
	{
		RequestBuilding(UnitTypes::Protoss_Gateway);
		numGates++;
	}
	//else if (bases.size() > 1 && (unsigned int)numGates < 2 * bases.size() && Broodwar->self()->minerals() > 250)
	else if (bases.size() > 1 && numGates < 4 && Broodwar->canMake(UnitTypes::Protoss_Gateway))
	{
		RequestBuilding(UnitTypes::Protoss_Gateway);
		numGates++;
	}
	//else
	//{
	//	Broodwar << "numGates < 2 - " << numGates << " - numFacCreate > 1 - " << numFacCreate << " - Broodwar->self()->minerals() > 100 - " << Broodwar->self()->minerals() << std::endl;
	//}
}

void TerranCheckGas()
{
	if (Broodwar->self()->supplyUsed() >= 24 && numGatesCreate > 0 && numGas < 1 && Broodwar->self()->minerals() > 75)
	{
		RequestBuilding(UnitTypes::Protoss_Assimilator);
		numGas++;
	}
	else if (numGas > 0 && numGas <= numGasCreate)
	{
		for (auto const &e : bases)
		{
			for (auto const &g : BWTA::getNearestBaseLocation(e->getPosition())->getGeysers())
			{
				if (!hasAssimilator(g))
				{
					RequestBuilding(UnitTypes::Protoss_Assimilator);
					numGas++;
				}
			}
		}
	}
}

void TerranCheckCyber()
{
	if (numGasCreate > 0 && numCyber < 1 && Broodwar->self()->minerals() > 175)
	{
		RequestBuilding(UnitTypes::Protoss_Cybernetics_Core);
		numCyber++;
	}

}

void TerranCheckRobo()
{
	if (numCyberCreate > 0 && numFac < 1 && Broodwar->self()->minerals() > 175 && Broodwar->self()->gas() > 175)
	{
		RequestBuilding(UnitTypes::Protoss_Robotics_Facility);
		numFac++;
	}
}

void TerranCheckObservatory()
{
	if (numFacComplete > 0 && numObs < 1 && Broodwar->self()->gas() > 75 && Broodwar->self()->minerals() > 25)
	{
		RequestBuilding(UnitTypes::Protoss_Observatory);
		numObs++;
	}
}

void TerranCheckNexus()
{
	if (numNexus < 1 && numObs > 0 && Broodwar->self()->minerals() > 325)
	{
		RequestBuilding(UnitTypes::Protoss_Nexus);
		numNexus++;
	}
	else if (bases.size() == 2 && numNexus == 1 && Broodwar->self()->minerals() > 325 && Expansions[1]->getGeysers().size() < 1)
	{
		RequestBuilding(UnitTypes::Protoss_Nexus);
		numNexus++;
	}
	else if (Broodwar->self()->minerals() > 700 && ((unsigned int)numNexus) <= bases.size())
	{
		RequestBuilding(UnitTypes::Protoss_Nexus);
		numNexus++;
	}
}

void TerranCheckBay()
{
	if (bases.size() > 1 && numBay < 1 && Broodwar->canMake(UnitTypes::Protoss_Robotics_Support_Bay))
	{
		numBay++;
		//RequestBuilding(UnitTypes::Protoss_Robotics_Support_Bay);
	}
}

void TerranCheckStargate()
{
	if ((bases.size() > 2 || Broodwar->self()->minerals() > 1200) && numStargate < 2 && Broodwar->canMake(UnitTypes::Protoss_Stargate))
	{
		numStargate++;
		RequestBuilding(UnitTypes::Protoss_Stargate);
	}
}

void TerranCheckFleet()
{
	if (numStargateComplete > 0 && numFleet < 1)
	{
		numFleet++;
		RequestBuilding(UnitTypes::Protoss_Fleet_Beacon);
	}
}

void TerranCheckCitadel()
{
	if (bases.size() > 1 && numCitadel < 1 && Broodwar->canMake(UnitTypes::Protoss_Citadel_of_Adun))
	{
		numCitadel++;
		RequestBuilding(UnitTypes::Protoss_Citadel_of_Adun);
	}
}

void TerranBuilderDecide()
{
	CheckSupply();
	TerranCheckGateway();
	TerranCheckGas();
	TerranCheckCyber();
	TerranCheckRobo();
	TerranCheckObservatory();
	TerranCheckNexus();
	TerranCheckBay();
	TerranCheckStargate();
	TerranCheckFleet();
	TerranCheckCitadel();
}

void TerranGatewayDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	int numUnemployedStargates = u->getUnitsInRadius(INT_MAX, IsStargate && !IsTraining).size();

	if (pylons.size() < 2 || numCyberCreate < 1)
	{
		return;
	}

	if (numFleet > 0 && numUnemployedStargates > 0)
	{
		return;
	}

	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{

		if (Zealots.size() >= 4 * Shuttles.size())
		{
			if (Broodwar->canMake(UnitTypes::Protoss_Dragoon))
			{
				u->train(UnitTypes::Protoss_Dragoon);
			}
		}
		else if (Broodwar->canMake(UnitTypes::Protoss_Zealot))
		{
			u->train(UnitTypes::Protoss_Zealot);
		}

		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void TerranRoboDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}
	if ((u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames()))
	{

		if (Obses.size() < 4 && Broodwar->canMake(UnitTypes::Protoss_Observer))
		{
			u->train(UnitTypes::Protoss_Observer);
		}
		else if (Shuttles.size() < 2 && Broodwar->canMake(UnitTypes::Protoss_Shuttle))
		{
			u->train(UnitTypes::Protoss_Shuttle);
		}
		else if (Broodwar->canMake(UnitTypes::Protoss_Reaver))
		{
			u->train(UnitTypes::Protoss_Reaver);
		}
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void TerranCyberDecide(Unit u)
{
	if (!dRange &&  bases.size() > 1 && u != nullptr && Broodwar->canUpgrade(UpgradeTypes::Singularity_Charge))
	{
		if (u->upgrade(UpgradeTypes::Singularity_Charge)) { dRange = true; }
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void TerranCitadelDecide(Unit u)
{
	if (!legEnhance && u != nullptr && Broodwar->canUpgrade(UpgradeTypes::Leg_Enhancements))
	{
		if (u->upgrade(UpgradeTypes::Leg_Enhancements)) { legEnhance = true; }
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void TerranFleetDecide(Unit u)
{
	if (!carrierCap && u != nullptr && Broodwar->canUpgrade(UpgradeTypes::Carrier_Capacity))
	{
		if (u->upgrade(UpgradeTypes::Carrier_Capacity)) { carrierCap = true; }
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void TerranStargateDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}
	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (Broodwar->canMake(UnitTypes::Protoss_Carrier))
		{
			u->train(UnitTypes::Protoss_Carrier);
		}
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void TerranBuildingDecide()
{
	for (auto const &u : gateways)
	{
		TerranGatewayDecide(u);
	}

	for (auto const &u : roboFacs)
	{
		TerranRoboDecide(u);
	}

	TerranCyberDecide(cyber);

	for (const auto &u : stargates)
	{
		TerranStargateDecide(u);
	}

	TerranCitadelDecide(citadel);

	TerranFleetDecide(fleet);

	checkCancel();

}

void DragoonDisplay(Unit u)
{
	for (int i = 0; i < DRAGOON_WAYPOINTS - 1; i++)
	{
		if (DragoonMap[u].prevPosition[i] == DRAGOON_DEFAULT_POSITION)
		{
			continue;
		}
		Broodwar->drawLineMap(DragoonMap[u].prevPosition[i], DragoonMap[u].prevPosition[i + 1], Colors::Green);
		Broodwar->drawCircleMap(DragoonMap[u].prevPosition[i], 6, Colors::Cyan);
	}
	Broodwar->drawCircleMap(DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1], 6, Colors::Cyan);
	Broodwar->drawLineMap(DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1], u->getPosition(), Colors::Green);

	if (u->getOrderTargetPosition() != Positions::None && u->getOrderTargetPosition() != Positions::Invalid && u->getOrderTargetPosition() != Positions::Unknown)
	{
		Broodwar->drawLineMap(u->getPosition(), u->getOrderTargetPosition(), Colors::Orange);
	}
}

void DragoonMark(Unit u)
{
	if (DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1] == DRAGOON_DEFAULT_POSITION || u->getDistance(DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1]) > 64)
	{
		for (int i = 0; i < DRAGOON_WAYPOINTS - 1; i++)
		{
			DragoonMap[u].prevPosition[i] = DragoonMap[u].prevPosition[i + 1];
		}
		DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1] = u->getPosition();
	}
}

bool DragoonFallBack(Unit u)
{
	if (DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1] == DRAGOON_DEFAULT_POSITION)
	{
		return false;
	}
	if (u->getDistance(DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1]) < 64)
	{
		for (int i = DRAGOON_WAYPOINTS - 1; i > 0; i--)
		{
			DragoonMap[u].prevPosition[i] = DragoonMap[u].prevPosition[i - 1];
		}
		DragoonMap[u].prevPosition[0] = DRAGOON_DEFAULT_POSITION;
	}
	//if (u->getOrder() != Orders::Move || u->getOrderTargetPosition() == Positions::Invalid || u->getOrderTargetPosition() == Positions::None || u->getOrderTargetPosition().getApproxDistance(DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1]) > 32)
	//{

	//if (!u->isMoving() && DragoonMap[u].unstuck > Broodwar->getLatencyFrames() + 1)
	//{
	//	u->stop();
	//	DragoonMap[u].unstuck = 0;
	//}
	//else
	//{
	u->move(DragoonMap[u].prevPosition[DRAGOON_WAYPOINTS - 1]);
	//	DragoonMap[u].unstuck++;
	//}
	//}

	Broodwar->drawTextMap(u->getPosition(), "Falling Back");
	return true;
}

bool DragoonAttemptFallBack(Unit u)
{
	if (!DragoonFallBack(u))
	{
		if (u->getDistance(Expansions[1]->getPosition()) > 256)
		{
			u->move(Expansions[1]->getPosition());
		}
		return false;
	}
	return true;
}

bool DragoonAttemptFallBack(Unit u, Unit runee)
{
	if (!DragoonFallBack(u))
	{
		RunAway(u, runee);
		return false;
	}
	return true;
}

void TerranDragoonDecide(Unit u) // Todo: make attack if tanks mostly unseige
{
	Unit AttackUnit = u->getClosestUnit(IsEnemy, 256);
	Unit AttackUnitUnrestricted = u->getClosestUnit(IsEnemy);
	Unit SpiderMine = u->getClosestUnit(IsSpiderMine, 256);
	Unit Tank = u->getClosestUnit(IsSieged, 400);
	Unit Vulture = u->getClosestUnit(IsVulture, 256);

	compareNamespace::currentUnit = u;
	Unit OtherDragoon = u->getClosestUnit(IsDragoon && !IsSameUnit);

	DragoonMap[u].timeSinceAttackBegin++;

	DragoonDisplay(u);

	DragoonMark(u);

	if (LatencyMap[u] > 0)
	{
		Broodwar->drawTextMap(u->getPosition(), "In Latency");
		LatencyMap[u]--;
		return;
	}

	//if (u->)

	if (DragoonVars::mainDragoon == nullptr || !DragoonVars::mainDragoon->exists())
	{
		DragoonVars::mainDragoon = u;
	}


	bool FallingBack = false;
	for (auto const &m : SpiderMines)  // Prority 1 - Run Away from Spider Mines
	{
		if (u->getDistance(m) < 192)
		{
			FallingBack = true;
			Broodwar->drawTextMap(u->getPosition(), "In Danger of Spider Mine");
			break;
		}
	}

	for (auto &h : TankHits) // Priority 2 - Run Away from Tanks (let zealots take care of them)
	{
		if (u->getDistance(h) < 128 || Tank != nullptr)
		{
			Broodwar->drawTextMap(u->getPosition(), "             Tank");
			FallingBack = true;
		}
	}

	if (FallingBack) // Fall Back
	{
		DragoonAttemptFallBack(u);
	}
	else if (SpiderMine != nullptr && !SpiderMine->isBurrowed()) // Unburrowed Spider Mines
	{
		DragoonAttemptFallBack(u);
		Broodwar->drawTextMap(u->getPosition(), "             Spider");
		bool isNearExisting = false;
		for (auto const &m : SpiderMines)
		{
			if (SpiderMine->getDistance(m) < 32)
			{
				isNearExisting = true;
				break;
			}
		}
		if (!isNearExisting)
		{
			SpiderMines.insert(SpiderMine->getPosition());
		}
	}
	else if (u->isAttackFrame() && AttackUnit != nullptr) // Priority 3 - Keep Attacking if you are doing so
	{
		if (DragoonMap[u].timeSinceAttackBegin >= 20)
		{
			DragoonMap[u].timeSinceAttackBegin = 0;
			Broodwar->drawTextMap(u->getPosition(), "Setting Time to 0");
		}
		else if (DragoonMap[u].timeSinceAttackBegin + Broodwar->getRemainingLatencyFrames() >= 7)
		{
			u->stop();
			//DragoonAttemptFallBack(u, AttackUnit);

			//if (!DragoonFallBack(u))
			//{
			//	RunAway(u, AttackUnit);
			//	Broodwar->drawTextMap(u->getPosition(), "Running Away");
			//}
			Broodwar->drawTextMap(u->getPosition(), "Stopping Attack");

		}
		else
		{
			Broodwar->drawTextMap(u->getPosition(), "Mid-attack");
		}
	}
	else if (u->getOrder() == Orders::AttackUnit) // If on the way to attack, check if there is a better target
	{
		Broodwar->drawTextMap(u->getPosition(), "Attacking");
		if (u->getDistance(u->getTargetPosition()) > 16 + u->getDistance(AttackUnit))
		{
			u->attack(AttackUnit);
			LatencyMap[u] = Broodwar->getRemainingLatencyFrames();
		}
	}
	else if (u->getShields() < 20) // If shields down, wait
	{
		DragoonAttemptFallBack(u);
	}
	else if (AttackUnit != nullptr) // If attackunit exists
	{
		Unitset nearbyVults;
		Unitset nearbyGoons;
		nearbyVults.clear();
		nearbyGoons.clear();
		groupedUnits(Vulture, nearbyVults);
		groupedUnits(u, nearbyGoons);
		if (nearbyVults.size() > nearbyGoons.size() * 2 && nearbyGoons.size() < 6) // Priority 4 - If outnumbered, run away
		{
			Broodwar->drawTextMap(u->getPosition(), "Outnumbered");
			DragoonAttemptFallBack(u, AttackUnit);
		}
		else if (u->getDistance(AttackUnit) < 192 && u->getGroundWeaponCooldown() < Broodwar->getRemainingLatencyFrames() + 1)
		{											//Priority 5 - If attack unit in range and weapon off cooldown, attack
			u->attack(AttackUnit);
			LatencyMap[u] = Broodwar->getRemainingLatencyFrames();
		}
		else if (u->getDistance(AttackUnit) < 160) // Priority 6 - If attack unit closer than 100, fall back
		{
			Broodwar->drawTextMap(u->getPosition(), "Too Close");
			DragoonAttemptFallBack(u, AttackUnit);
		}
		else if (u->getDistance(AttackUnit) < 192) // Priority 7 - If attack unit in range, hold position
		{
			Broodwar->drawTextMap(u->getPosition(), "Right Distance");
			u->holdPosition();
		}
		else if (OtherDragoon != nullptr) // Priority 8 - If attack unit out of range move sideways relative to it
		{
			double AttackAngle = atan2(AttackUnit->getPosition().y - u->getPosition().y, AttackUnit->getPosition().x - u->getPosition().x);
			double DragoonAngle = atan2(OtherDragoon->getPosition().y - u->getPosition().y, OtherDragoon->getPosition().x - u->getPosition().x);
			double TrueAngle = (((int)((DragoonAngle - AttackAngle + 6.28) * 100) % 628) / 100);
			if (TrueAngle < 3.14)
			{
				RunSideways(u, AttackUnit, true);
			}
			else
			{
				RunSideways(u, AttackUnit, false);
			}
			Broodwar->drawTextMap(u->getPosition(), "Crab");

		}
		else // Default case, hold position
		{
			Broodwar->drawTextMap(u->getPosition(), "Default");
			u->holdPosition();
		}
	}			// If it reaches here, there is no attackunit
	else if (OtherDragoon != nullptr && u->getDistance(OtherDragoon) < 16) // If dragoon too close, spread out 
	{
		Broodwar->drawTextMap(u->getPosition(), "Spread out");
		RunAway(u, OtherDragoon);
	}
	else if (OtherDragoon != nullptr && u->getDistance(OtherDragoon) > 96) // If dragoon too far away, bunch up
	{
		Broodwar->drawTextMap(u->getPosition(), "Bunch up");
		u->move(OtherDragoon->getPosition());
	}
	else if (AttackUnitUnrestricted != nullptr) // If someone is attacking us somewhere, go stop them
	{
		Broodwar->drawTextMap(u->getPosition(), "In Sights");
		u->attack(AttackUnitUnrestricted);
	}
	else // Everything is nice and peaceful. Lay some DragoonMarks, and mosey over to the enemy base
	{
		if (enemyBase != Position(-1, -1))
		{
			Broodwar->drawTextMap(u->getPosition(), "Let's Get Em!");
			u->move(enemyBase);
		}
	}
}

void TerranZealotDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	Unit AttackUnit = u->getClosestUnit(IsEnemy && !IsSpiderMine && !IsVulture, 36);
	int numZealotsGround = u->getUnitsInRadius(96, IsZealot).size();

	if (ZealotMap[u].Shuttle != nullptr && ZealotMap[u].Shuttle->exists())
	{
		/*if (u->getClosestUnit(IsTank, 24) != nullptr)
		{
		ZealotMap[u].Shuttle->unload(u);
		if (u->getOrder() != Orders::AttackUnit)
		{
		u->attack(u->getClosestUnit(IsTank, 24));
		}
		}
		else*/ if (AttackUnit != nullptr)
		{
			ZealotMap[u].Shuttle->unload(u);
			if (u->getOrder() != Orders::AttackUnit)
			{
				u->attack(AttackUnit);
			}

		}
		else
		{
			if (ZealotMap[u].Shuttle->getOrder() != Orders::PickupTransport)
			{
				ZealotMap[u].Shuttle->load(u);
			}
			u->rightClick(ZealotMap[u].Shuttle);
		}
	}
}

bool TerranPickUp(Unit u)
{
	int MaxReavers = 0;
	int MaxZealots = 4;

	for (int r = 0; r < MaxReavers; r++)
	{
		if (ShuttleMap[u].Reaver[r] == nullptr)
		{
			Unit reaver = u->getClosestUnit(ReaverNoShuttle);
			ShuttleMap[u].Reaver[r] = reaver;
			ReaverMap[reaver].Shuttle = u;
		}
	}
	for (int z = 0; z < MaxZealots; z++)
	{
		if (ShuttleMap[u].Zealot[z] == nullptr)
		{
			Unit zealot = u->getClosestUnit(ZealotNoShuttle);
			ShuttleMap[u].Zealot[z] = zealot;
			ZealotMap[zealot].Shuttle = u;
		}
	}
	//for (int r = 0; r < MaxReavers; r++)
	//{
	//	if (ShuttleMap[u].Reaver[r] != nullptr && !ShuttleMap[u].Reaver[r]->isLoaded())
	//	{
	//		Position p = RunTowards(u, ShuttleMap[u].Reaver[r]);
	//		Broodwar->drawCircle(CoordinateType::Map, p.x, p.y, 10, Colors::Purple, true);
	//		return true;
	//	}
	//}
	//for (int z = 0; z < MaxZealots; z++)
	//{
	//	if (ShuttleMap[u].Zealot[z] != nullptr && ShuttleMap[u].Zealot[z]->exists() && !ShuttleMap[u].Zealot[z]->isLoaded())
	//	{
	//		Position p = RunTowards(u, ShuttleMap[u].Zealot[z]);
	//		Broodwar->drawCircle(CoordinateType::Map, p.x, p.y, 10, Colors::Purple, true);
	//		return true;
	//	}
	//}
	return false;
}


void TerranShuttleDecide(Unit u)
{
	if (u == nullptr || !u->exists())
	{
		return;
	}

	//if (TerranPickUp(u)){ return; }
	TerranPickUp(u);
	if (u->getOrder() == Orders::PickupTransport){ return; }

	Unit NearestTank = u->getClosestUnit(IsTank);
	Unit NearestDragoon = u->getClosestUnit(IsDragoon);
	Unit NearestMissile = u->getClosestUnit(IsMissileTurret, 256);
	if (NearestMissile != nullptr)
	{
		RunAway(u, NearestMissile);
	}
	if (NearestTank != nullptr)
	{
		u->move(NearestTank->getPosition());
	}
	else if (u->getUnitsInRadius(180, IsEnemy).size() > 5)
	{
		u->move(u->getClosestUnit(IsEnemy)->getPosition());
	}
	else
	{
		if (NearestDragoon != nullptr)
		{
			u->move(NearestDragoon->getPosition());
		}
	}

}

void TerranScoutDecide(Unit u)
{
	for (auto const &b : BWTA::getStartLocations())
	{
		if (!Broodwar->isExplored(b->getPosition().x / 32, b->getPosition().y / 32))
		{
			Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Green, true);
			ScoutMap[u].ExtraTime = 0;
			u->move(b->getPosition());
			return;
		}
		else
		{
			if (ScoutMap[u].ExtraTime < 25)
			{
				ScoutMap[u].ExtraTime++;
				return;
			}
			Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Red, true);
		}
	}
}

void TerranObserverDecide(Unit u)
{
	Unit Dragoon = Broodwar->getClosestUnit(enemyBase, IsDragoon);

	if (Dragoon == nullptr)
	{
		Dragoon = u;
	}

	Unit Vulture = Dragoon->getClosestUnit(IsVulture, 256);
	Unit Tank = Dragoon->getClosestUnit(IsTank, 256);
	Unit MissileTurret = u->getClosestUnit(IsMissileTurret, 280);
	Unit SpiderMine = Dragoon->getClosestUnit(IsSpiderMine, 200);
	Unit Observer = u->getClosestUnit(IsObserver, 96);

	Position nearestMine = Position(-1, -1);

	std::unordered_set<Position> removeSet;

	for (auto const &m : SpiderMines)
	{
		if (u->getDistance(m) < 256)
		{
			removeSet.insert(m);
		}
		else
		{
			nearestMine = m;
		}
	}

	for (auto const &r : removeSet)
	{
		SpiderMines.erase(r);
	}

	if (ObsMap[u].Location == Army)
	{
		if (MissileTurret != nullptr)
		{
			RunAway(u, MissileTurret);
		}
		else if (Observer != nullptr)
		{
			RunAway(u, Observer);
		}
		else if (nearestMine != Position(-1, -1))
		{
			u->move(nearestMine);
		}
		else if (SpiderMine != nullptr)
		{
			u->move(SpiderMine->getPosition());
		}
		else if (Tank != nullptr)
		{
			u->move(Tank->getPosition());
		}
		else if (Vulture != nullptr)
		{
			u->move(Vulture->getPosition());
		}
		else if (Dragoon != nullptr)
		{
			u->move(Dragoon->getPosition());
		}
	}
}

void TerranCarrierDecide(Unit u)
{
	u->train(UnitTypes::Protoss_Interceptor);
	if (Broodwar->getUnitsInRadius(u->getPosition(), 10000, IsCarrier).size() > 3)
	{
		Unit AttackUnit = u->getClosestUnit(IsEnemy);
		if (AttackUnit != nullptr && u->isIdle())
		{
			u->attack(AttackUnit->getPosition());
			//u->move(AttackUnit->getPosition());
		}
	}
}

void TerranUnitDecide()
{
	int LocalEventTime;

	LocalEventTime = clock();

	for (const auto &u : harvesters)
	{
		HarvesterDecide(u);
	}

	Broodwar->drawTextScreen(0, 110, "Harvesters Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	UseBuilders();

	Broodwar->drawTextScreen(0, 100, "Builders Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	for (const auto &u : Zealots)
	{
		TerranZealotDecide(u);
	}

	Broodwar->drawTextScreen(0, 80, "Zealots Decided in: %d", (clock() - LocalEventTime));
	LocalEventTime = clock();

	for (const auto &u : Dragoons)
	{
		TerranDragoonDecide(u);
	}

	for (const auto &h : TankHits)
	{
		Broodwar->drawCircleMap(h, 128, Colors::Orange);
	}

	Broodwar->drawTextScreen(0, 90, "Dragoons Decided in: %d", (clock() - LocalEventTime));

	for (const auto &u : Shuttles)
	{
		TerranShuttleDecide(u);
	}

	for (const auto &u : scouts)
	{
		TerranScoutDecide(u);
	}

	for (const auto &u : Obses)
	{
		TerranObserverDecide(u);
	}

	if (!CheckDefenders()){ RemoveDefenders(); }

	for (const auto &u : defenders)
	{
		DefenderDecide(u);
	}

	for (const auto &u : Carriers)
	{
		TerranCarrierDecide(u);
	}

}

void TerranScoutCheck()
{
	if (Broodwar->elapsedTime() < 70 && Broodwar->elapsedTime() > 60 && scouts.size() < 1)
	{
		RequestScout();
	}
}

void TerranBulletReact()
{
	std::unordered_set < Position > removePositions;

	for (auto const &h : TankHits)
	{
		ExpiryMap[h]--;
		if (ExpiryMap[h] < 1)
		{
			removePositions.insert(h);
		}
	}

	for (auto const &h : removePositions)
	{
		TankHits.erase(h);
	}

	for (auto const &b : Broodwar->getBullets())
	{
		if (b->getType() == BulletTypes::Arclite_Shock_Cannon_Hit)
		{
			TankHits.insert(b->getPosition());
			ExpiryMap[b->getPosition()] = 720;
		}
	}
}

void TerranDisplay()
{
	for (auto const &m : SpiderMines)
	{
		Broodwar->drawCircleMap(m, 192, Colors::Grey);
	}
}

void TerranOnFrame()
{
	/*int LocalEventTime = clock();
	CreateUnitGroups();
	Broodwar->drawTextScreen(0, 80, "Unit Groups determined in: %d", (clock() - LocalEventTime));*/
	TerranDisplay();
	TerranBulletReact();
	TerranUnitDecide();
	TerranBuildingDecide();
	TerranBuilderDecide();
	TerranScoutCheck();

	/*int LocalEventTime;

	LocalEventTime = clock();

	for (const auto &u : harvesters)
	{
	HarvesterDecide(u);
	}

	Broodwar->drawTextScreen(0, 110, "Harvesters Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	UseBuilders();

	Broodwar->drawTextScreen(0, 100, "Builders Decided in: %d", (clock() - LocalEventTime));

	for (const auto &u : gateways)
	{
	GatewayDecide(u);
	}

	CyberDecide(cyber);

	LocalEventTime = clock();

	for (const auto &u : Zealots)
	{
	ZealotDecide(u);
	}

	for (const auto &u : Dragoons)
	{
	DragoonDecide(u);
	}

	Broodwar->drawTextScreen(0, 90, "Zealots/Dragoons Decided in: %d", (clock() - LocalEventTime));

	for (const auto &u : Obses)
	{
	ObserverDecide(u);
	}


	BayDecide(bay);

	for (const auto &u : roboFacs)
	{
	RoboDecide(u);
	}

	LocalEventTime = clock();

	for (const auto &u : Shuttles)
	{
	ShuttleDecide(u);
	}

	Broodwar->drawTextScreen(0, 80, "Shuttles Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	for (const auto &u : Reavers)
	{
	ReaverDecide(u);
	}

	Broodwar->drawTextScreen(0, 70, "Reavers Decided in: %d", (clock() - LocalEventTime));



	for (const auto &u : defenders)
	{
	DefenderDecide(u);
	}
	CheckDefenders();*/
}

// ------------------------------------------------------------------
// Against Protoss
// ------------------------------------------------------------------
//
//
//	Will probably go for a 2 gate rush into Dragoon Reaver
//
//

void ProtossCheckSupply()
{
	CheckSupply();
}

void ProtossCheckGates()
{
	if (numGates < 3 && numGates <= numGatesComplete && Broodwar->canMake(UnitTypes::Protoss_Gateway))
	{
		RequestBuilding(UnitTypes::Protoss_Gateway);
		numGates++;
	}
	else if (numGates < 4 && numGates <= numGatesComplete && Broodwar->self()->minerals() > 300)
	{
		RequestBuilding(UnitTypes::Protoss_Gateway);
		numGates++;
	}
}

void ProtossBuilderDecide()
{
	ProtossCheckSupply();
	ProtossCheckGates();
}

void ProtossGateDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (u->canTrain(UnitTypes::Protoss_Zealot))
		{
			u->train(UnitTypes::Protoss_Zealot);
		}
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}

}

void ProtossBuildingDecide()
{
	for (const auto &u : gateways)
	{
		ProtossGateDecide(u);
	}
}

void ProtossScoutDecide(Unit u)
{
	TerranScoutDecide(u);
}

Unit zealotRushBest(Unit a, Unit b)
{
	
	return nullptr;
}

BestUnitFilter ZealotRushBest = zealotRushBest;

void ProtossZealotDecide(Unit u)
{
	Unit Enemy = u->getClosestUnit(IsEnemy && (!IsBuilding || IsCannon));
	Unit Dragoon = u->getClosestUnit(IsDragoon && IsEnemy);
	Unit Cannon = u->getClosestUnit(IsCannon && IsEnemy);
	Unit Zealot = u->getClosestUnit(IsZealot && IsEnemy, 512);
	if (enemyBase != Position(-1, -1))
	{
		if (ZealotVars::mainZealot == nullptr || !ZealotVars::mainZealot->exists())
		{
			ZealotVars::mainZealot = u;
			ZealotVars::armyZealots.insert(u);
		}
		else if (u == ZealotVars::mainZealot)
		{
			Broodwar->drawTextMap(u->getPosition().x, u->getPosition().y + 20, "Lord Commander");
		}

		if (!ZealotVars::armyZealots.contains(u))
		{
			if (u->getDistance(ZealotVars::mainZealot) < 64)
			{
				ZealotVars::armyZealots.insert(u);
			}
			else
			{
				u->follow(ZealotVars::mainZealot);
				Broodwar->drawTextMap(u->getPosition(), "Catching up");
			}
		}
		else
		{
			int numLowShield = 0;
			int numZealots = 0;
			int numEnemyZealots = 0;

			if (u->getDistance(ZealotVars::mainZealot) > 256 && u != ZealotVars::mainZealot)
			{
				ZealotVars::armyZealots.erase(u);
			}

			for (auto const &u : ZealotVars::armyZealots)
			{
				numZealots++;
				if (u->getShields() < 10)
				{
					Broodwar->drawTextMap(u->getPosition().x, u->getPosition().y + 10, "Low Shields");
					numLowShield++;
				}
			}

			//if (numLowShield * 2 > numZealots) //retreat
			//{
			//	u->move(Expansions[1]);
			//}
			//else	//Attack

			Unitset Allyset;
			Unitset Enemyset;
			Allyset.clear();
			Enemyset.clear();
			groupedUnits(u, Allyset);
			groupedUnits(Zealot, Enemyset);

			if (Allyset.size() < Enemyset.size() + 2)
			{
				u->move(Expansions[1]->getPosition());
			}
			else if (u->getDistance(Enemy) < 17)
			{
				//Broodwar << u->getOrder() << std::endl;
				if (u->getOrder() != Orders::AttackUnit || u->getDistance(u->getOrderTarget()) > 24 || (u->getOrderTarget()->getType().isBuilding() && !Enemy->getType().isBuilding()) )
				{
					u->attack(Enemy);
					LatencyMap[u] = Broodwar->getLatencyFrames();
				}
				else
				{
					if (u->isAttackFrame())
					{
						Broodwar->drawTextMap(u->getPosition(), "Mid-Attack");
					}
					else
					{
						//Unit AttackingUnit;
						//for (auto const &e : u->getUnitsInRadius(20, IsEnemy && IsZealot))
						//{
						//	if (e->getOrder() == Orders::AttackUnit && e->getOrderTarget == u)
						//	{
						//		AttackingUnit = e;
						//		break;
						//	}
						//}
						//if (AttackingUnit != nullptr && u->getShields() < 1)
						//{
						//	RunAway(u, )
						//}
						Broodwar->drawTextMap(u->getPosition(), "Attacking Nearby");
					}
				}

			}
			else
			{
				if (Dragoon != nullptr)
				{
					u->move(Dragoon->getPosition());
				}
				else if (Cannon != nullptr)
				{
					u->move(Cannon->getPosition());
				}
				else if (Zealot != nullptr)
				{
					u->move(Zealot->getPosition());
				}
				else
				{
					Unit Building = u->getClosestUnit(IsEnemy && IsBuilding);
					if (Building != nullptr)
					{
						if (u->getOrderTarget() != Building)
						{
							Broodwar->drawTextMap(u->getPosition(), "Boring Attack");
							u->attack(Building);
						}
					}
					else
					{
						Broodwar->drawTextMap(u->getPosition(), "Smash Em!");
						u->move(closestEnemyBase->getPosition());
					}
				}
			}
		}

	}
}

void ProtossUnitDecide()
{
	int LocalEventTime;

	LocalEventTime = clock();

	for (const auto &u : harvesters)
	{
		HarvesterDecide(u);
	}

	Broodwar->drawTextScreen(0, 110, "Harvesters Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	UseBuilders();

	Broodwar->drawTextScreen(0, 100, "Builders Decided in: %d", (clock() - LocalEventTime));

	for (const auto &u : scouts)
	{
		ProtossScoutDecide(u);
	}

	for (const auto &u : Zealots)
	{
		ProtossZealotDecide(u);
	}
}

void ProtossScoutCheck()
{
	TerranScoutCheck();
}

void ProtossOnFrame()
{
	ProtossBuilderDecide();
	ProtossBuildingDecide();
	ProtossUnitDecide();
	ProtossScoutCheck();
}


// ------------------------------------------------------------------
// Against Zerg
// ------------------------------------------------------------------
//
//	Will probably go for an FFE into Corsair/Reaver
//
//	
//

bool isCorsair(Unit u)
{
	if (u->getType() == UnitTypes::Protoss_Corsair)
	{
		return true;
	}
	return false;
}

UnitFilter IsCorsair = isCorsair;

bool isCorsairCombTactic(Unit u)
{
	if (isCorsair(u) && CorsairMap[u].Tactic == Comb)
	{
		return true;
	}
	return false;
}

UnitFilter IsCorsairCombTactic = isCorsairCombTactic;

bool isCorsairGroupTactic(Unit u)
{
	if (isCorsair(u) && CorsairMap[u].Tactic == Group)
	{
		return true;
	}
	return false;
}

UnitFilter IsCorsairGroupTactic = isCorsairGroupTactic;

bool isReaverDefendTactic(Unit u)
{
	if (isReaver(u) && ReaverMap[u].Tactic == Defend)
	{
		return true;
	}
	return false;
}

UnitFilter IsReaverDefendTactic = isReaverDefendTactic;

bool isReaverArmyTactic(Unit u)
{
	if (isReaver(u) && ReaverMap[u].Tactic == ArmyReaver)
	{
		return true;
	}
	return false;
}

UnitFilter IsReaverArmyTactic = isReaverArmyTactic;

bool isReaverHarassTactic(Unit u)
{
	if (isReaver(u) && ReaverMap[u].Tactic == Harass)
	{
		return true;
	}
	return false;
}

UnitFilter IsReaverHarassTactic = isReaverHarassTactic;

bool isReaverEngageTactic(Unit u)
{
	if (isReaver(u) && ReaverMap[u].Tactic == Engage)
	{
		return true;
	}
	return false;
}

UnitFilter IsReaverEngageTactic = isReaverEngageTactic;

bool isHydralisk(Unit u)
{
	if (u->getType() == UnitTypes::Zerg_Hydralisk)
	{
		return true;
	}
	return false;
}

UnitFilter IsHydralisk = isHydralisk;

bool isSunken(Unit u)
{
	if (u->getType() == UnitTypes::Zerg_Sunken_Colony)
	{
		return true;
	}
	return false;
}

UnitFilter IsSunken = isSunken;

bool isMutalisk(Unit u)
{
	if (u->getType() == UnitTypes::Zerg_Mutalisk)
	{
		return true;
	}
	return false;
}

UnitFilter IsMutalisk = isMutalisk;

bool isSporeColony(Unit u)
{
	if (u->getType() == UnitTypes::Zerg_Spore_Colony)
	{
		return true;
	}
	return false;
}

UnitFilter IsSporeColony = isSporeColony;

bool isZergSTA(Unit u) // Surface to Air
{
	if (isSporeColony(u) || isHydralisk(u))
	{
		//Broodwar->drawCircleMap(u->getPosition(), 10, Colors::Red);
		return true;
	}
	else
	{
		//Broodwar->drawCircleMap(u->getPosition(), 10, Colors::Green);
		return false;
	}
}

UnitFilter IsZergSTA = isZergSTA;

void ZergCheckSupply()
{
	//Broodwar << "pylons.size() - " << pylons.size() << " - supplyUsed() - " << Broodwar->self()->supplyUsed() << " - minerals - " << Broodwar->self()->minerals() << std::endl;
	if (pylons.size() > 0)
	{
		CheckSupply();
	}
	else if (Broodwar->self()->supplyUsed() >= 14 && allSupply <= 20)
	{
		Unit u = RequestBuilding(UnitTypes::Protoss_Pylon);
		BuilderMap[u].Immutable = true;
		BuilderMap[u].BuildPosition = FFEPositions[closestFFE].Pylon;
		allSupply += 16;
		Broodwar << BuilderMap[u].BuildPosition << std::endl;
	}
}

void ZergCheckForge()
{
	if (numForge < 1 && pylons.size() > 0)
	{
		numForge++;
		Unit u = RequestBuilding(UnitTypes::Protoss_Forge);
		BuilderMap[u].Immutable = true;
		BuilderMap[u].BuildPosition = FFEPositions[closestFFE].Forge;
		Broodwar << BuilderMap[u].BuildPosition << std::endl;
	}
}

void ZergCheckGates()
{
	if (numGates < 1 && bases.size() > 1)
	{
		numGates++;
		Unit u = RequestBuilding(UnitTypes::Protoss_Gateway);
		BuilderMap[u].Immutable = true;
		BuilderMap[u].BuildPosition = FFEPositions[closestFFE].Gateway;
		Broodwar << BuilderMap[u].BuildPosition << std::endl;
	}
	else if (numGates < 2 && numFacCreate > 1 && Broodwar->self()->minerals() > 300)
	{
		numGates++;
		RequestBuilding(UnitTypes::Protoss_Gateway);
	}
}

void ZergCheckCannons()
{
	if ((numCannon < 1 && numForgeComplete > 0) || (numCannon < 2 && numGatesCreate > 0))
	{
		Unit u = RequestBuilding(UnitTypes::Protoss_Photon_Cannon);
		BuilderMap[u].Immutable = true;
		BuilderMap[u].BuildPosition = FFEPositions[closestFFE].Cannon[numCannon];
		numCannon++;
		Broodwar << BuilderMap[u].BuildPosition << std::endl;
	}
}

void ZergCheckNexus()
{
	if (Broodwar->self()->minerals() > 300)
	{
		if (numNexus < 2)
		{
			Unit u = RequestBuilding(UnitTypes::Protoss_Nexus);
			BuilderMap[u].Immutable = true;
			BuilderMap[u].BuildPosition = Expansions[1]->getTilePosition();
			numNexus++;
		}
		else if (Broodwar->self()->minerals() > 700 && ((unsigned int)numNexus) <= bases.size())
		{
			RequestBuilding(UnitTypes::Protoss_Nexus);
			numNexus++;
		}
	}
}

void ZergCheckGas()
{
	if (numCannonCreate > 1 && numGas < 1)
	{
		RequestBuilding(UnitTypes::Protoss_Assimilator);
		numGas++;
	}
	else if (numGas > 0 && numGas <= numGasCreate)
	{
		for (auto const &e : bases)
		{
			for (auto const &g : BWTA::getNearestBaseLocation(e->getPosition())->getGeysers())
			{
				if (!hasAssimilator(g))
				{
					RequestBuilding(UnitTypes::Protoss_Assimilator);
					numGas++;
				}
			}
		}
	}
}

void ZergCheckStargate()
{
	if (numStargate < 1 && Broodwar->canMake(UnitTypes::Protoss_Stargate))
	{
		RequestBuilding(UnitTypes::Protoss_Stargate);
		numStargate++;
	}
	//else if (bases.size() > 2 && numStargate < 2)
	//{
	//	RequestBuilding(UnitTypes::Protoss_Stargate);
	//	numStargate++;
	//}
}

void ZergCheckRobo()
{
	if (numFac < 1 && numStargateCreate > 0 && Broodwar->canMake(UnitTypes::Protoss_Robotics_Facility))
	{
		RequestBuilding(UnitTypes::Protoss_Robotics_Facility);
		numFac++;
	}
	else if (numStargateComplete > 0 && numFac < 2)
	{
		RequestBuilding(UnitTypes::Protoss_Robotics_Facility);
		numFac++;
	}
	/*else if (numStargateCreate > 0 && numFac < 2)
	{
	RequestBuilding(UnitTypes::Protoss_Robotics_Facility);
	numFac++;
	}*/
}

void ZergCheckBay()
{
	if (numBay < 1 && Broodwar->canMake(UnitTypes::Protoss_Robotics_Support_Bay))
	{
		RequestBuilding(UnitTypes::Protoss_Robotics_Support_Bay);
		numBay++;
	}
}

void ZergCheckCyber()
{
	if (bases.size() > 1 && Broodwar->canMake(UnitTypes::Protoss_Cybernetics_Core) && numCyber < 1)
	{
		RequestBuilding(UnitTypes::Protoss_Cybernetics_Core);
		numCyber++;
	}
}

void ZergCheckObs()
{
	if (numBayComplete > 0 && numObs < 1)
	{
		RequestBuilding(UnitTypes::Protoss_Observatory);
		numObs++;
	}
}

void ZergCheckFleet()
{
	if (Reavers.size() > 0 && numFleet < 1)
	{
		RequestBuilding(UnitTypes::Protoss_Fleet_Beacon);
		numFleet++;
	}
}

void ZergBuilderDecide()
{
	ZergCheckSupply();
	ZergCheckForge();
	ZergCheckGates();
	ZergCheckCannons();
	ZergCheckNexus();
	ZergCheckGas();
	ZergCheckStargate();
	ZergCheckRobo();
	ZergCheckBay();
	ZergCheckCyber();
	ZergCheckObs();
	ZergCheckFleet();
}

void ZergGatewayDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (u->canTrain(UnitTypes::Protoss_Zealot))
		{
			u->train(UnitTypes::Protoss_Zealot);
		}

		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void ZergCyberDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	if (u == nullptr || u->isResearching())
	{
		return;
	}

	if (numStargateCreate < 1 || numFacCreate < 1)
	{
		return;
	}

	if (u->canUpgrade(UpgradeTypes::Protoss_Air_Weapons))
	{
		u->upgrade(UpgradeTypes::Protoss_Air_Weapons);
	}
	else if (u->canUpgrade(UpgradeTypes::Protoss_Air_Armor))
	{
		u->upgrade(UpgradeTypes::Protoss_Air_Armor);
	}

	LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
}

void ZergStargateDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (u->canTrain(UnitTypes::Protoss_Corsair))
		{
			u->train(UnitTypes::Protoss_Corsair);
		}

		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void ZergRoboDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (seenLurker && Obses.size() < 2 && u->canTrain(UnitTypes::Protoss_Observer))
		{
			u->train(UnitTypes::Protoss_Observer);
		}
		else if (Reavers.size() <= 2 * Shuttles.size() + 1 && u->canTrain(UnitTypes::Protoss_Reaver))
		{
			u->train(UnitTypes::Protoss_Reaver);
		}
		else if ((Reavers.size() >= 2 * Shuttles.size() + 1 || Shuttles.size() < 1) && u->canTrain(UnitTypes::Protoss_Shuttle))
		{
			u->train(UnitTypes::Protoss_Shuttle);
		}

		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void ZergBayDecide(Unit u)
{
	if (u == nullptr)
	{
		return;
	}
	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (!gSpeed && u->canUpgrade(UpgradeTypes::Gravitic_Drive))
		{
			u->upgrade(UpgradeTypes::Gravitic_Drive);
			gSpeed = true;
		}
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void ZergFleetDecide(Unit u)
{
	if (u == nullptr)
	{
		return;
	}
	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (!disruptionWeb	&& u->canResearch(TechTypes::Disruption_Web))
		{
			u->research(TechTypes::Disruption_Web);
			disruptionWeb = true;
		}
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void ZergForgeDecide(Unit u)
{
	if (u == nullptr)
	{
		return;
	}
	if (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames())
	{
		if (Reavers.size() > 1 && u->canUpgrade(UpgradeTypes::Protoss_Ground_Weapons))
		{
			u->upgrade(UpgradeTypes::Protoss_Ground_Weapons);
		}
		LatencyMap[u] = Broodwar->getLatencyFrames() + 5;
	}
}

void ZergBuildingDecide()
{
	for (const auto &u : gateways)
	{
		ZergGatewayDecide(u);
	}

	for (const auto &u : stargates)
	{
		ZergStargateDecide(u);
	}

	for (const auto &u : roboFacs)
	{
		ZergRoboDecide(u);
	}

	ZergCyberDecide(cyber);

	ZergBayDecide(bay);

	ZergFleetDecide(fleet);

	ZergForgeDecide(forge[0]);

	checkCancel();
}

void ZergZealotDecide(Unit u)
{
	Unit AttackUnit = u->getClosestUnit(IsEnemy);
	Unit Cannon = u->getClosestUnit(IsCannon);

	if (ZealotVars::mainZealot == nullptr || !ZealotVars::mainZealot->exists())
	{
		ZealotVars::mainZealot = u;
		ZealotMap[u].rank = 0;
	}

	if (numReaverArmy < 2) // Hang around cannons
	{
		Broodwar->drawTextMap(u->getPosition(), "Hangin Around");
		if (AttackUnit != nullptr && u->getDistance(AttackUnit) < 20)
		{
			u->attack(AttackUnit);
		}
		else if (Cannon != nullptr && u->getDistance(Cannon) > 100)
		{
			u->move(Cannon->getPosition());
		}
		else if (u->isAttackFrame())
		{
			return;
		}
		else if (AttackUnit != nullptr)
		{
			u->attack(AttackUnit);
		}
		else if (Cannon != nullptr)
		{
			u->move(Cannon->getPosition());
		}
	}
	else	//Get out there
	{
		Unit Reaver = u->getClosestUnit(IsReaverArmyTactic);
		compareNamespace::currentUnit = u;
		Unit Zealot = u->getClosestUnit(IsZealot && !IsSameUnit);

		if (u == ZealotVars::mainZealot)
		{
			Broodwar->drawTextMap(u->getPosition().x, u->getPosition().y + 10, "Lord Commander");
			if (ZealotVars::target == ZEALOT_UNDEFINED_POSITION || u->getDistance(ZealotVars::target) > 96)
			{
				ZealotVars::target = enemyBase;
			}
		}

		//if (u->getDistance(ZealotVars::mainZealot) > 256 && u!=ZealotVars::mainZealot)
		/*if (ZealotMap[u].fallback == true)
		{
			u->move(Reaver->getPosition());
			Broodwar->drawTextMap(u->getPosition(), "Falling Back");
			if (u->getDistance(Reaver->getPosition()) < 128)
			{
				ZealotMap[u].fallback = false;
				//Broodwar << "fallback false" << std::endl;
			}
		}*/
		if (AttackUnit != nullptr && Reaver->isUnderAttack())
		{
			u->attack(AttackUnit);
		}
		else if (AttackUnit != nullptr && u->getDistance(AttackUnit->getPosition()) < 20 && u->getGroundWeaponCooldown() < 1)
		{
			u->attack(AttackUnit);
		}
		/*else if (Reaver != nullptr && u->getDistance(Reaver->getPosition()) < 32)
		{
		Broodwar->drawTextMap(u->getPosition(), "Making way");
		RunAway(u, Reaver);
		}*/
		else if (AttackUnit != nullptr && u->getDistance(AttackUnit) < 198 && u->getDistance(Reaver) > 32)
		{
			RunTowards(u, Reaver);
		}
		else if (Zealot != nullptr && u->getDistance(Zealot->getPosition()) < 16)
		{
			Broodwar->drawTextMap(u->getPosition(), "Making way");
			RunAway(u, Zealot);
		}
		else if (AttackUnit != nullptr)
		{
			Broodwar->drawTextMap(u->getPosition(), "Attacking");
			u->attack(AttackUnit);
		}
		else if (u->getDistance(Reaver) > 300)
		{
			RunTowards(u, Reaver);
		}
		else
		{
			Broodwar->drawTextMap(u->getPosition(), "Go on, lads!");
			u->attack(ZealotVars::target);
		}

	}
}

void CorsairChooseWander(Unit u)
{
	if (enemyBase != Position(-1, -1))
	{
		if (u->getDistance(enemyBase) < 128)
		{
			for (auto const &b : BWTA::getStartLocations())
			{
				if (!Broodwar->isExplored(b->getPosition().x / 32, b->getPosition().y / 32))
				{
					Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Green, true);
					CorsairMap[u].targetPoint = b->getPosition();
					return;
				}
				else
				{
					Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Red, true);
				}
			}
		}
		else
		{
			CorsairMap[u].targetPoint = enemyBase;
		}
	}
	else
	{
		for (auto const &b : BWTA::getStartLocations())
		{
			if (!Broodwar->isExplored(b->getPosition().x / 32, b->getPosition().y / 32))
			{
				Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Green, true);
				CorsairMap[u].targetPoint = b->getPosition();
				return;
			}
			else
			{
				Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Red, true);
			}
		}
	}




	/*if (rand() % 2 == 0)
	{
	int i = 0;
	Position p;
	do
	{
	i++;
	p = Expansions[rand() % Expansions.size()]->getPosition();
	} while (Broodwar->isVisible(TilePositionConvert(p)) && i < 10);
	CorsairMap[u].targetPoint = p;
	}
	else
	{
	for (auto const &b : BWTA::getStartLocations())
	{
	if (rand() % 4 == 0)
	{
	CorsairMap[u].targetPoint = b->getPosition();
	break;
	}
	}
	}*/
	/*Position p;
	int i = 0;
	do
	{
	i++;
	p = Position(rand() % (Broodwar->mapWidth() * 32), rand() % (Broodwar->mapHeight() * 32));
	p.makeValid();
	} while (Broodwar->isVisible(TilePositionConvert(p)) && i < 10);
	CorsairMap[u].targetPoint = p;*/
}

void ZergCorsairDecide(Unit u)
{
	if (u->getOrderTargetPosition() != CORSAIR_UNDEFINED_POSITION && u->getOrderTargetPosition() != Positions::Invalid && u->getOrderTargetPosition() != Positions::Unknown)
	{
		Broodwar->drawLineMap(u->getPosition(), u->getOrderTargetPosition(), Colors::Orange);
	}

	if (CorsairMap[u].sideSwap-- > 0)
	{
		CorsairMap[u].sideSwap--;
	}

	Position beenClose = Position(-1, -1);

	for (auto &p : CorsairBeenHere)
	{
		if (u->getDistance(p) < 512)
		{
			beenClose = p;
			break;
		}
	}

	if (beenClose == Position(-1,-1) && u->getClosestUnit(IsEnemy && IsFlying, 512) == nullptr)
	{
		CorsairBeenHere.insert(beenClose);
		CorsairBeenHereExpiryMap[beenClose] = 5000;
	}

	if (CorsairMap[u].Tactic == Comb)
	{
		Broodwar->drawTextMap(u->getPosition(), "Comb");
		Unit AttackUnit = u->getClosestUnit(IsEnemy && IsFlying);
		Unit RunUnit = u->getClosestUnit(IsEnemy && IsZergSTA, 288);
		//Unitset RunUnits = u->getUnitsInRadius(256, IsEnemy && CanAttackAir);
		if (RunUnit != nullptr)
		{
			Unitset RunUnits = u->getUnitsInRadius(512, IsEnemy && IsZergSTA);
			Broodwar->drawLineMap(u->getPosition(), RunUnits.getPosition(), Colors::Red);
			if (u->getDistance(RunUnit) < 256 || u->getShields() < 20)
			{
				if (!RunAway(u, RunUnits.getPosition()).isValid())
				{
					RunSideways(u, RunUnits.getPosition(), CorsairMap[u].direction);
				}
			}
			else
			{
				if (AttackUnit != nullptr && u->getDistance(AttackUnit) < 180)
				{
					u->attack(AttackUnit);
				}
				else if (AttackUnit != nullptr)
				{
					RunSideways(u, AttackUnit->getPosition(), CorsairMap[u].direction);
				}
				else if (beenClose == Position(-1,-1))
				{
					RunAway(u, beenClose);	
				}
				else
				{
					RunSideways(u, RunUnits.getPosition(), CorsairMap[u].direction);
				}
			}
		}
		else if (AttackUnit != nullptr)
		{
			u->attack(AttackUnit);
		}
		else if (CorsairMap[u].targetPoint != CORSAIR_UNDEFINED_POSITION)
		{
			u->move(CorsairMap[u].targetPoint);
			if (u->getDistance(CorsairMap[u].targetPoint) < 64)
			{
				CorsairMap[u].targetPoint = CORSAIR_UNDEFINED_POSITION;
			}
		}
		else
		{
			CorsairChooseWander(u);

		}
	}
	else if (CorsairMap[u].Tactic == Group)
	{
		Broodwar->drawTextMap(u->getPosition(), "Group");
		if (CorsairVars::mainCorsair == nullptr || !CorsairVars::mainCorsair->exists())
		{
			CorsairVars::mainCorsair = u;
		}
		else if (u == CorsairVars::mainCorsair)
		{
			Broodwar->drawTextMap(Position(u->getPosition().x, u->getPosition().y + 10), "Rogue Leader");
			Unit RunUnit = u->getClosestUnit(IsEnemy && IsZergSTA && !IsFlying, 288);
			Unit AttackUnit = u->getClosestUnit(IsEnemy && IsFlying);
			if (RunUnit != nullptr)
			{
				Unitset RunUnits = u->getUnitsInRadius(512, IsEnemy && IsZergSTA && !IsFlying);
				if (u->getDistance(RunUnit) < 256)
				{
					if (!RunAway(u, RunUnits.getPosition()).isValid())
					{
						RunSideways(u, RunUnits.getPosition(), CorsairMap[u].direction);
					}
				}
				else
				{
					if (AttackUnit != nullptr && u->getDistance(AttackUnit) < 180)
					{
						u->attack(AttackUnit);
					}
					else if (AttackUnit != nullptr)
					{
						RunSideways(u, AttackUnit->getPosition(), CorsairMap[u].direction);
					}
					else
					{
						RunSideways(u, RunUnits.getPosition(), CorsairMap[u].direction);
					}
				}
			}
			else if (AttackUnit != nullptr)
			{
				u->attack(AttackUnit);
			}
			else if (CorsairMap[u].targetPoint != CORSAIR_UNDEFINED_POSITION)
			{
				u->move(CorsairMap[u].targetPoint);
				if (u->getDistance(CorsairMap[u].targetPoint) < 64)
				{
					CorsairMap[u].targetPoint = CORSAIR_UNDEFINED_POSITION;
				}
			}
			else
			{
				CorsairChooseWander(u);
			}
			/*Broodwar->drawTextMap(Position(u->getPosition().x, u->getPosition().y + 10), "Rogue Leader");
			Unitset group = u->getUnitsInRadius(99999, IsCorsairGroupTactic);
			group.insert(u);
			Unit RunUnit = u->getClosestUnit(IsEnemy && IsZergSTA && !IsFlying, 288);
			Unit AttackUnit = u->getClosestUnit(IsEnemy && IsFlying);

			if (RunUnit != nullptr)
			{
			Unitset RunUnits = u->getUnitsInRadius(512, IsEnemy && IsZergSTA && !IsFlying);
			if (u->getDistance(RunUnit) < 256)
			{
			RunAway(group, RunUnits.getPosition());
			}
			else
			{
			if (AttackUnit != nullptr && u->getDistance(AttackUnit) < 180)
			{
			group.attack(AttackUnit);
			}
			group.move(RunSideways(u, RunUnits.getPosition(), CorsairMap[u].direction));
			}
			}
			else if (AttackUnit != nullptr)
			{
			group.attack(AttackUnit);
			}
			else if (CorsairMap[u].targetPoint != CORSAIR_UNDEFINED_POSITION)
			{
			group.move(CorsairMap[u].targetPoint);
			if (u->getDistance(CorsairMap[u].targetPoint) < 64)
			{
			CorsairMap[u].targetPoint = CORSAIR_UNDEFINED_POSITION;
			}
			}
			else
			{
			CorsairChooseWander(u);
			}*/
		}
		else // u != CorsairVars::mainCorsair && CorsairVars::mainCorsair != nullptr && CorsairVars::mainCorsair->exists()
		{
			if (u->getDistance(CorsairVars::mainCorsair) > 64)
			{
				u->move(CorsairVars::mainCorsair->getPosition());
			}
			else
			{
				u->issueCommand(CorsairVars::mainCorsair->getLastCommand());
			}
			//u->issueCommand(CorsairVars::mainCorsair->getLastCommand());
		}
	}
}

bool ZergPickUp(Unit u)
{
	int MaxReavers = 1;
	int MaxZealots = 0;

	for (int r = 0; r < MaxReavers; r++)
	{
		if (ShuttleMap[u].Reaver[r] == nullptr)
		{
			Unit reaver = u->getClosestUnit(ReaverNoShuttle && !IsReaverDefendTactic && !IsReaverArmyTactic);
			ShuttleMap[u].Reaver[r] = reaver;
			ReaverMap[reaver].Shuttle = u;
		}
	}
	/*for (int z = 0; z < MaxZealots; z++)
	{
	if (ShuttleMap[u].Zealot[z] == nullptr)
	{
	Unit zealot = u->getClosestUnit(ZealotNoShuttle);
	ShuttleMap[u].Zealot[z] = zealot;
	ZealotMap[zealot].Shuttle = u;
	return true;
	}
	}*/
	for (int r = 0; r < MaxReavers; r++)
	{
		if (ShuttleMap[u].Reaver[r] != nullptr && !ShuttleMap[u].Reaver[r]->isLoaded())
		{
			RunTowards(u, ShuttleMap[u].Reaver[r]);
			return true;
		}
	}
	return false;
}

bool isInconsequential(Unit u)
{
	if (u->getType() == UnitTypes::Zerg_Larva || u->getType() == UnitTypes::Zerg_Egg)
	{
		return true;
	}
	return false;
}

UnitFilter IsInconsequential = isInconsequential;

bool isShuttleThreat(Unit u)
{
	if ((IsEnemy && IsVisible && !IsFlying && !IsCloaked && !IsBurrowed && !IsWorker && !IsInconsequential)(u) && (!(IsBuilding)(u) || u->getType() == UnitTypes::Zerg_Spore_Colony || u->getType() == UnitTypes::Zerg_Sunken_Colony))
	{
		return true;
	}
	return false;
}

UnitFilter IsShuttleThreat = isShuttleThreat;

int AntiReaverDropWeight(Unit u)
{
	return Broodwar->getUnitsInRadius(ReaverMap[u].Shuttle->getPosition(), 288, !IsBuilding && IsEnemy && !IsFlying && !IsWorker && !IsInconsequential).size();
}

void ReaverUnload(Unit u)
{
	if (Broodwar->enemy()->getRace() == Races::Terran)
	{
		Unit s = ReaverMap[u].Shuttle;
		Unit * z = ShuttleMap[s].Zealot;
		for (int i = 0; i < 2; i++)
		{
			if (z[i] != nullptr && !s->unload(z[i]))
			{
				s->unload(u);
			}
			else if (z[i] != nullptr)
			{
				LatencyMap[z[i]] = Broodwar->getLatencyFrames() + 8;
			}
		}
	}
	else
	{
		ReaverMap[u].Shuttle->unload(u);
	}
}

Unit mostNearby(Unit a, Unit b)
{
	if (a->getUnitsInRadius(32, IsEnemy).size() > b->getUnitsInRadius(32, IsEnemy).size())
	{
		if (!IsBuilding(a))
		{
			return a;
		}
	}
	return b;
}

BestUnitFilter MostNearby = mostNearby;

void ZergReaverDecide(Unit u)
{
	u->train(UnitTypes::Protoss_Scarab);

	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	//if (ReaverMap[u].Shuttle == nullptr && ReaverMap[u].Tactic != Defend)
	//{
	//	return;
	//}

	if (ReaverMap[u].Tactic == ReaverUndefinedTactic)
	{
		Broodwar->drawTextMap(u->getPosition(), "Undefined");
	}
	else if ((ReaverMap[u].Shuttle == nullptr && ReaverMap[u].Tactic != ArmyReaver) || ReaverMap[u].Tactic == Defend)
	{
		Unit AttackUnit = u->getClosestUnit(IsEnemy && !IsFlying);
		Broodwar->drawTextMap(u->getPosition(), "Defend");
		if (AttackUnit != nullptr && Expansions[1]->getPosition().getApproxDistance(AttackUnit->getPosition()) < 550)
		{
			u->attack(AttackUnit);
		}
		else
		{
			u->move(Expansions[1]->getPosition());
		}

	}
	else if (ReaverMap[u].Tactic == Harass)
	{
		Unit RunUnit = u->getClosestUnit(IsShuttleThreat, 256);
		Unit AttackUnit = u->getClosestUnit(IsWorker && IsEnemy, 256);

		Broodwar->drawTextMap(u->getPosition(), "Harass");

		if (u->getOrder() == Orders::AttackUnit && u->isAttackFrame() == false)
		{
			ReaverMap[u].Attacktime--;
			if (ReaverMap[u].Attacktime < 1)
			{
				u->load(ReaverMap[u].Shuttle);
			}
			return;
		}

		if (u->isLoaded())
		{
			ReaverMap[u].Scarabs = u->getScarabCount();
		}
		if (RunUnit != nullptr)
		{
			ReaverMap[u].Shuttle->load(u);
			u->load(ReaverMap[u].Shuttle);
		}
		else if (AttackUnit != nullptr)
		{
			if (u->getGroundWeaponCooldown() < 1)
			{
				ReaverMap[u].Shuttle->unload(u);
				u->attack(AttackUnit);
				ReaverMap[u].Attacktime = 128;
			}

			Broodwar->drawLineMap(u->getPosition(), AttackUnit->getPosition(), Colors::Yellow);
		}
		else
		{
			u->load(ReaverMap[u].Shuttle);
		}
	}
	else if (ReaverMap[u].Tactic == Engage)
	{
		Broodwar->drawTextMap(u->getPosition(), "Engage");
		if (u->getOrder() == Orders::AttackUnit && u->isAttackFrame() == false)
		{
			ReaverMap[u].Attacktime--;
			if (ReaverMap[u].Attacktime < 1)
			{
				u->load(ReaverMap[u].Shuttle);
			}
			return;

		}

		compareNamespace::currentUnit = u;
		Unit AttackUnit = Broodwar->getClosestUnit(ReaverMap[u].Shuttle->getPosition(), (IsHydralisk || IsSporeColony) && UnitLevelSame, 350);
		Unit RunUnit = Broodwar->getClosestUnit(ReaverMap[u].Shuttle->getPosition(), IsShuttleThreat);
		if (u->isLoaded())
		{
			if (ReaverMap[u].Shuttle->getShields() < 1)
			{
				if (ReaverMap[u].Shuttle->getShields() + ReaverMap[u].Shuttle->getHitPoints() < 20)
				{
					ReaverMap[u].Shuttle->unload(u);
				}
				return;
			}
			ReaverMap[u].Scarabs = u->getScarabCount();
		}

		if (AttackUnit != nullptr)
		{
			if (u->getGroundWeaponCooldown() < 1 && ReaverMap[u].Shuttle->getDistance(AttackUnit) > 280)
			{
				ReaverMap[u].Shuttle->unload(u);
				u->attack(AttackUnit);
				ReaverMap[u].Attacktime = 128;
			}

			Broodwar->drawLineMap(u->getPosition(), AttackUnit->getPosition(), Colors::Yellow);
		}
		else
		{
			u->load(ReaverMap[u].Shuttle);
		}
	}
	else if (ReaverMap[u].Tactic == ArmyReaver)
	{
		Broodwar->drawTextMap(u->getPosition(), "ArmyReaver");
		//Broodwar->drawTextMap(u->getPosition().x, u->getPosition().y + 10, "Seriously, I just don't think Reaver drops are that effective when handled by bots, slow reavers are the way to go for now.");
		Unit AttackUnit = Broodwar->getBestUnit(MostNearby, IsEnemy, u->getPosition(), 240);
		if (AttackUnit != nullptr)
		{
			u->attack(AttackUnit);
		}
		else if (ZealotVars::mainZealot != nullptr && numReaverArmy > 1)
		{
			u->move(closestEnemyBase->getPosition());
			//u->move(ZealotVars::mainZealot->getPosition());
		}
		else
		{
			u->move(Expansions[1]->getPosition());
		}
	}
	else
	{
		Broodwar->drawTextMap(u->getPosition(), "Tactic Not Found");
	}
}

void ZergShuttleRunAway(Unit u, Unit EnemyUnit, Position AttackPosition)
{
	Unitset EnemyUnits = u->getUnitsInRadius(300, IsShuttleThreat);
	Broodwar->drawLineMap(u->getPosition(), EnemyUnits.getPosition(), Colors::Red);
	if (u->getDistance(EnemyUnit) > 270)
	{
		Broodwar->drawTextMap(u->getPosition(), "Run sideways outside 270");
		LatencyMap[u] = 2;
		if (AttackPosition.isValid())
		{
			RunSideways(u, AttackPosition, ShuttleMap[u].direction);
		}
		else
		{
			RunSideways(u, EnemyUnits.getPosition(), ShuttleMap[u].direction);
		}
		//RunSideways(u, EnemyUnits->getPosition, ShuttleMap[u].direction);
	}
	else
	{
		Broodwar->drawTextMap(u->getPosition(), "Run Away within 270");
		RunAway(u, EnemyUnit);
		return;
	}
}

void ZergShuttleRunAway(Unit u, Unit EnemyUnit, Unit AttackUnit)
{
	if (AttackUnit != nullptr)
	{
		ZergShuttleRunAway(u, EnemyUnit, AttackUnit->getPosition());
	}
	else
	{
		ZergShuttleRunAway(u, EnemyUnit, Position(-1, -1));
	}
}

void ZergShuttleDecide(Unit u)
{
	Broodwar->drawLineMap(u->getPosition(), u->getOrderTargetPosition(), Colors::Orange);
	//Broodwar->drawCircleMap(u->getPosition(), 240, Colors::Grey);
	//Broodwar->drawCircleMap(u->getPosition(), 220, Colors::Grey);

	if (ShuttleMap[u].sideSwap-- > 0)
	{
		ShuttleMap[u].sideSwap--;
	}

	if (u == ArmyShuttle)
	{
		Unit EnemyUnit = u->getClosestUnit(IsEnemy && !IsFlying, 255);
		Broodwar->drawTextMap(ArmyShuttle->getPosition(), "Army Shuttle");
		if (u->getLoadedUnits().size() > 0)
		{
			Unit Reaver = Broodwar->getClosestUnit(closestEnemyBase->getPosition(), IsReaverArmyTactic);
			if (Reaver != nullptr)
			{
				Broodwar->drawLineMap(u->getPosition(), Reaver->getPosition(), Colors::Green);
				if (EnemyUnit != nullptr)
				{ 
					RunAway(u, EnemyUnit);
					u->unloadAll();
				}
				else if (u->getDistance(closestEnemyBase->getPosition()) + 50 < Reaver->getDistance(closestEnemyBase->getPosition()))
				{
					u->move(closestEnemyBase->getPosition());
					u->unloadAll();
				}
				else if (u->getDistance(Reaver) > 60)
				{
					RunTowards(u, Reaver);
				}
				else
				{
					RunTowards(u, closestEnemyBase->getPosition());
				}
			}
			else
			{
				u->unloadAll();
			}
		}
		else
		{
			Unit Reaver = mainBase->getClosestUnit(IsReaverArmyTactic);
			if (Reaver != nullptr)
			{
				Broodwar->drawLineMap(u->getPosition(), Reaver->getPosition(), Colors::Green);
				if (u->getDistance(Reaver) < 92 && !Reaver->isAttacking() && EnemyUnit == nullptr)
				{
					u->load(Reaver);
				}
				else
				{
					RunTowards(u, Reaver);
				}
			}
		}

	}
	else if (ZergPickUp(u)){  }
	else if (ReaverMap[ShuttleMap[u].Reaver[0]].Tactic == Harass)
	{
		Unit EnemyUnit = u->getClosestUnit(IsShuttleThreat, 300);
		Unit AttackUnit = u->getClosestUnit(IsEnemy && IsWorker, 500);
		Broodwar->drawCircleMap(u->getPosition(), 250, Colors::Grey);
		Broodwar->drawCircleMap(u->getPosition(), 300, Colors::Grey);
		if (EnemyUnit != nullptr && EnemyUnit->exists())
		{
			Broodwar->drawLineMap(u->getPosition(), EnemyUnit->getPosition(), Colors::Red);
			if (u->getDistance(EnemyUnit) < 250)
			{
				ZergShuttleRunAway(u, EnemyUnit, AttackUnit);
				//RunAway(u, EnemyUnit);
			}
			else
			{
				RunSideways(u, EnemyUnit);
			}
		}
		else if (AttackUnit != nullptr)
		{
			RunTowards(u, AttackUnit);
		}
		else
		{
			if (ShuttleMap[u].CurrentDestination != SHUTTLE_UNDEFINED_POSITION)
			{
				u->move(ShuttleMap[u].CurrentDestination);
			}
			else
			{
				if (EnemyBaseLocations.size() > 0)
				{
					auto p = EnemyBaseLocations.begin();
					for (unsigned int i = 0; i < rand() % EnemyBaseLocations.size(); i++)
					{
						p++;
						if (p == EnemyBaseLocations.end())
						{
							p--;
						}
					}
					if (p->isValid())
					{
						ShuttleMap[u].CurrentDestination = *p;
					}
				}
			}
		}
	}
	else if (ReaverMap[ShuttleMap[u].Reaver[0]].Tactic == Engage)
	{

		Unit EnemyUnit = u->getClosestUnit(IsShuttleThreat, 300);
		Unit AttackUnit = u->getClosestUnit(IsHydralisk);
		if (EnemyUnit != nullptr)
		{
			ZergShuttleRunAway(u, EnemyUnit, AttackUnit);
		}
		else if (AttackUnit != nullptr && ZealotVars::mainZealot != nullptr && ZealotVars::mainZealot->getDistance(AttackUnit) < 512)
		{
			RunTowards(u, AttackUnit);
		}
		else
		{
			Unit SecondaryAttackUnit = u->getClosestUnit(IsEnemy && !IsFlying);
			if (SecondaryAttackUnit != nullptr && ZealotVars::mainZealot != nullptr && ZealotVars::mainZealot->getDistance(SecondaryAttackUnit) < 512)
			{
				RunTowards(u, SecondaryAttackUnit);
			}
			u->move(Expansions[1]->getPosition());
		}
	}
}

void ZergObsDecide(Unit u)
{
	Unit BurrowedUnit = u->getClosestUnit(IsBurrowed);
	if (BurrowedUnit != nullptr)
	{
		u->move(BurrowedUnit->getPosition());
	}
}


bool ZergCheckDefenders()
{
	Unit expansionPylon = Broodwar->getClosestUnit(Expansions[1]->getPosition(), IsPylon);
	if (expansionPylon == nullptr || !expansionPylon->exists())
	{
		return false;
	}

	int attack = expansionPylon->getUnitsInRadius(150, IsEnemy && !IsWorker && !IsFlying && !IsCloaked && !IsBurrowed).size();
	int defence = Dragoons.size() + Zealots.size() + (int)(0.5 * defenders.size());

	//Broodwar << mainBase << std::endl;

	Broodwar->drawCircleMap(expansionPylon->getPosition(), 150, Colors::Red);

	if (attack > defence)
	{
		for (int i = 0; i < attack - defence; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				RequestDefender();
			}
		}
	}
	else if (attack < 1)
	{
		return false;
	}
	return true;
}

void ZergUnitDecide()
{
	int LocalEventTime;

	LocalEventTime = clock();

	for (const auto &u : harvesters)
	{
		HarvesterDecide(u);
	}

	Broodwar->drawTextScreen(0, 110, "Harvesters Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	UseBuilders();

	Broodwar->drawTextScreen(0, 100, "Builders Decided in: %d", (clock() - LocalEventTime));

	if (Zealots.size() < 4 && Reavers.size() < 1)
	{
		if (!CheckDefenders() && !ZergCheckDefenders()){ RemoveDefenders(); }
	}

	for (const auto &d : defenders)
	{
		DefenderDecide(d);
	}

	for (const auto &u : Zealots)
	{
		ZergZealotDecide(u);
	}

	LocalEventTime = clock();

	for (const auto &u : Corsairs)
	{
		ZergCorsairDecide(u);
	}

	Broodwar->drawTextScreen(0, 130, "Corsairs Decided in: %d", (clock() - LocalEventTime));

	LocalEventTime = clock();

	for (const auto &u : Reavers)
	{
		ZergReaverDecide(u);
	}

	Broodwar->drawTextScreen(0, 120, "Reavers Decided in: %d", (clock() - LocalEventTime));

	for (const auto &u : Shuttles)
	{
		ZergShuttleDecide(u);
	}

	for (const auto &u : Obses)
	{
		ZergObsDecide(u);
	}
}

void ZergMemoryDecide()
{
	std::unordered_set<Position> tempPositions;
	for (auto const &p : CorsairBeenHere)
	{
		Unit closeUnit = Broodwar->getClosestUnit(p, IsEnemy && IsFlying);
		Broodwar->drawCircleMap(p, 512, Colors::Cyan);
		if (CorsairBeenHereExpiryMap[p] < 1 || (closeUnit != nullptr && p.getApproxDistance(closeUnit->getPosition()) < 512))
		{
			tempPositions.insert(p);
		}
		else
		{
			CorsairBeenHereExpiryMap[p]--;
		}
	}

	for (auto const &p : tempPositions)
	{
		CorsairBeenHere.erase(p);
	}
}

void ZergOnFrame()
{
	int LocalEventTime;
	LocalEventTime = clock();
	ZergUnitDecide();
	Broodwar->drawTextScreen(50, 100, "Units as a whole Decided in: %d", (clock() - LocalEventTime));
	ZergBuildingDecide();
	ZergBuilderDecide();
	ZergMemoryDecide();
}



void NexusDecide(Unit u)
{
	if (LatencyMap[u] > 0)
	{
		LatencyMap[u]--;
		return;
	}

	if (!u->exists() || !u->isCompleted())
	{
		return;
	}

	//Building Probe Logic

	if (harvesters.size() + builders.size() + defenders.size() <= bases.size() * 19 + 10 && harvesters.size() + builders.size() + defenders.size() <= 57 && (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames()))// && u->getRemainingTrainTime() == Broodwar->getLatencyFrames() + 1)
	{
		u->train(UnitTypes::Protoss_Probe);
		LatencyMap[u] = Broodwar->getLatencyFrames();
	}
}

/*bool isFac(Unit u)
{
if (u->getType() == UnitTypes::Protoss_Robotics_Facility)
{
return true;
}
return false;
}

UnitFilter IsFac = isFac;

void GatewayDecide(Unit u)
{
if ((numCyberCreate > 0) && (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames()))// && u->getRemainingTrainTime() == Broodwar->getLatencyFrames() + 1)
{
if (LatencyMap[u] > 0)
{
LatencyMap[u]--;
return;
}

Unit FacNotTrain = u->getClosestUnit(IsFac && !IsTraining && IsCompleted);

if (FacNotTrain == nullptr && (!u->train(UnitTypes::Protoss_Dragoon) && (Dragoons.size() > 0 || (numCyberComplete < 1 && Zealots.size() < 1))))
{
u->train(UnitTypes::Protoss_Zealot);
}
LatencyMap[u] = Broodwar->getLatencyFrames();
}
}

void CyberDecide(Unit u)
{
if (!dRange &&  numFacCreate > 0 && u != nullptr && (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames()))
{
//Broodwar << "I build a Probe" << std::endl;
//if (u->upgrade(UpgradeTypes::Singularity_Charge)) { dRange = true; }
LatencyMap[u] = Broodwar->getLatencyFrames();
}
}

void RoboDecide(Unit u)
{
if ((u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames()))// && u->getRemainingTrainTime() == Broodwar->getLatencyFrames() + 1)
{
if (LatencyMap[u] > 0)
{
LatencyMap[u]--;
return;
}

if (obs != nullptr && Reavers.size() > 1 && Obses.size() < 1)
{
u->train(UnitTypes::Protoss_Observer);
}
else	if (Shuttles.size() < 0.5 * Reavers.size() || Shuttles.size() == 0)
//if (Shuttles.size() < 1 * Reavers.size() || Shuttles.size() == 0)
{
u->train(UnitTypes::Protoss_Shuttle);
}
else if (!u->train(UnitTypes::Protoss_Reaver))
{
}
LatencyMap[u] = Broodwar->getLatencyFrames();
}
}

void BayDecide(Unit u)
{
if (!gSpeed && u != nullptr && Broodwar->self()->gas() > 250 && (u->getTrainingQueue().size() < 1 || u->getRemainingTrainTime() < Broodwar->getLatencyFrames()))
{
//Broodwar << "I build a Probe" << std::endl;
if (u->upgrade(UpgradeTypes::Gravitic_Drive)) { gSpeed = true; }
LatencyMap[u] = Broodwar->getLatencyFrames();
}
}

Position GetDefendLocation()
{
if (Dragoons.size() > 10 || bases.size() > 1)
{
return enemyBase;
//BWTA::Chokepoint * closestChoke;
//double closestDistance = DBL_MAX;
//for (const auto &c : BWTA::getChokepoints())
//{
//if (c != BWTA::getNearestChokepoint(mainBase->getPosition()) && c->getCenter().getDistance(Expansions[1]->getPosition()) < closestDistance)
//{
//closestChoke = c;
//closestDistance = c->getCenter().getDistance(Expansions[1]->getPosition());
//}
//}
//return closestChoke->getCenter();
}
else
{
return BWTA::getNearestChokepoint(mainBase->getPosition())->getCenter();
}
}

void ZealotDecide(Unit u)
{
if (u == nullptr || !u->exists())
{
return;
}
if (LatencyMap[u] > 0)
{
LatencyMap[u]--;
return;
}

if (ZealotMap[u].Shuttle != nullptr)
{
u->load(ZealotMap[u].Shuttle);
return;
}

LatencyMap[u] = Broodwar->getLatencyFrames();
Position choke = GetDefendLocation();
Unit AttackUnit = u->getClosestUnit(IsEnemy, 20);
if (AttackUnit != nullptr)
{
if (u->getShields() < 20 && !(u->getGroundWeaponCooldown() < 1))
{
RunAway(u, AttackUnit);
}
u->attack(AttackUnit);
return;
}
if (u->getDistance(choke) < 70)
{
if (u->getDistance(choke) > 50)
{
u->stop();
}
else
{
auto b = *(bases.begin());
u->move(b->getPosition());
}
}
else
{
u->move(choke);
}
}

Unit CurrentUnit;

bool isOtherDragoon(Unit u)
{
if (isDragoon(u) && u != CurrentUnit)
{
return true;
}
return false;
}

UnitFilter IsOtherDragoon = isOtherDragoon;

void DragoonDecide(Unit u)
{
if (u == nullptr || !u->exists())
{
return;
}
if (LatencyMap[u] > 0)
{
LatencyMap[u]--;
return;
}
LatencyMap[u] = Broodwar->getLatencyFrames();

Position choke = GetDefendLocation();

//Position choke = BWTA::getNearestChokepoint(u->getPosition())->getCenter();
Unit AttackUnit = u->getClosestUnit(IsEnemy, 256);
if (AttackUnit != nullptr && detectUnit(AttackUnit))
{
if (u->isAttackFrame())
{
LatencyMap[u] = 5;
return;
}
if (u->getGroundWeaponCooldown() < 1)//&& (AttackUnit->getType() != UnitTypes::Protoss_Zealot || u->getDistance(AttackUnit) > 64))
{
u->attack(AttackUnit);
//LatencyMap[u] = 6;
return;
}
else if (u->getClosestUnit(IsEnemy, 90) != nullptr)
{
RunAway(u, AttackUnit);
//int x1 = u->getPosition().x;
//int y1 = u->getPosition().y;
//int x2 = AttackUnit->getPosition().x;
//int y2 = AttackUnit->getPosition().y;
//u->move(Position(2 * x1 - x2, 2 * y1 - y2));
//LatencyMap[u] = Broodwar->getLatencyFrames();
}
}
else if (u->getDistance(choke) < 120)
{
if (u->getDistance(choke) > 100)
{
u->stop();
}
int x1 = u->getPosition().x;
int y1 = u->getPosition().y;
int x2 = choke.x;
int y2 = choke.y;
u->move(Position(2 * x1 - x2, 2 * y1 - y2));
LatencyMap[u] = Broodwar->getLatencyFrames();
//CurrentUnit = u;
//Unit ClosestDragoon = u->getClosestUnit(isOtherDragoon, 32);
//if (ClosestDragoon != nullptr)
//{
//	int x = u->getPosition().x * 2 -ClosestDragoon->getPosition().x;
//	int y = u->getPosition().y * 2 - ClosestDragoon->getPosition().y;
//	u->move(Position(x,y));
//}
//else
//{
//	auto b = *(bases.begin());
//	u->move(b->getPosition());
//}
}
else
{
u->move(choke);
}
}

void ObserverDecide(Unit u)
{
Unit Cloaked = u->getClosestUnit(IsCloaked || IsBurrowed && !IsAlly);
if (Cloaked != nullptr)
{
u->move(Cloaked->getPosition());
}
else
{

Unit Dragoon = u->getClosestUnit(IsOtherDragoon);
if (Dragoon != nullptr)
{
u->move(Dragoon->getPosition());
}
}
}

Unit currentReaver;

bool reaverDrop(Unit u)
{
if ((IsEnemy && !IsFlying && !IsCloaked && !IsBurrowed)(u) && Broodwar->getRegionAt(u->getPosition())->isHigherGround() == Broodwar->getRegionAt(currentReaver->getPosition())->isHigherGround()) //&& BWTA::getGroundDistance(currentReaver->getTilePosition(), u->getTilePosition()) < 256)
{
return true;
}
return false;
}

UnitFilter ReaverDrop = reaverDrop;

Unit reaverBest(Unit a, Unit b)
{
if (a->getPlayer()->getRace() == Races::Protoss)
{
if (a->getType() == UnitTypes::Protoss_Reaver)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_Reaver)
{
return b;
}

if (a->getType() == UnitTypes::Protoss_High_Templar)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_High_Templar)
{
return b;
}

if (a->getType() == UnitTypes::Protoss_Dragoon)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_Dragoon)
{
return b;
}

if (a->getType() == UnitTypes::Protoss_Photon_Cannon)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_Photon_Cannon)
{
return b;
}

if (a->getType() == UnitTypes::Protoss_Probe)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_Probe)
{
return b;
}

if (a->getType() == UnitTypes::Protoss_Dark_Archon)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_Dark_Archon)
{
return b;
}

if (a->getType() == UnitTypes::Protoss_Archon)
{
return a;
}
else if (b->getType() == UnitTypes::Protoss_Archon)
{
return b;
}

if (!IsBuilding(a))
{
return a;
}

return b;

}
else if (a->getPlayer()->getRace() == Races::Terran)
{
if (a->getType() == UnitTypes::Terran_Goliath)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_Goliath)
{
return b;
}

if (a->getType() == UnitTypes::Terran_Siege_Tank_Siege_Mode)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_Siege_Tank_Siege_Mode)
{
return b;
}

if (a->getType() == UnitTypes::Terran_Siege_Tank_Tank_Mode)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_Siege_Tank_Tank_Mode)
{
return b;
}

if (a->getType() == UnitTypes::Terran_Missile_Turret)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_Missile_Turret)
{
return b;
}

if (a->getType() == UnitTypes::Terran_Marine)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_Marine)
{
return b;
}

if (a->getType() == UnitTypes::Terran_Bunker)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_Bunker)
{
return b;
}

if (a->getType() == UnitTypes::Terran_SCV)
{
return a;
}
else if (b->getType() == UnitTypes::Terran_SCV)
{
return b;
}

if (!IsBuilding(a))
{
return a;
}
return b;

}
else // Zerg
{
if (a->getType() == UnitTypes::Zerg_Lurker)
{
return a;
}
else if (b->getType() == UnitTypes::Zerg_Lurker)
{
return b;
}

if (a->getType() == UnitTypes::Zerg_Hydralisk)
{
return a;
}
else if (b->getType() == UnitTypes::Zerg_Hydralisk)
{
return b;
}

if (a->getType() == UnitTypes::Zerg_Drone)
{
return a;
}
else if (b->getType() == UnitTypes::Zerg_Drone)
{
return b;
}

if (a->getType() == UnitTypes::Zerg_Defiler)
{
return a;
}
else if (b->getType() == UnitTypes::Zerg_Defiler)
{
return b;
}

if (a->getType() == UnitTypes::Zerg_Spore_Colony)
{
return a;
}
else if (b->getType() == UnitTypes::Zerg_Spore_Colony)
{
return b;
}

if (a->getType() == UnitTypes::Zerg_Ultralisk)
{
return a;
}
else if (b->getType() == UnitTypes::Zerg_Ultralisk)
{
return b;
}

if (!IsBuilding(a))
{
return a;
}
return b;

}

if (IsBuilding(a))
{
return b;
}
return a;
}

BestUnitFilter ReaverBest = reaverBest;

bool isInconsequential(Unit u)
{
//if (u->getType() == UnitTypes::Zerg_Larva || u->getType() == UnitTypes::Zerg_Egg || u->getType() == UnitTypes::Protoss_Interceptor || u->getType() == UnitTypes::Terran_Vulture_Spider_Mine)
if (!u->getType().airWeapon().isValid())
{
return true;
}
return false;
}

UnitFilter IsInconsequential = isInconsequential;

int AntiReaverDropWeight(Unit u)
{
return Broodwar->getUnitsInRadius(ReaverMap[u].Shuttle->getPosition(), 288, !IsBuilding && IsEnemy && !IsFlying && !IsWorker && !IsInconsequential).size();
}

void ReaverUnload(Unit u)
{
if (Broodwar->enemy()->getRace() == Races::Terran)
{
Unit s = ReaverMap[u].Shuttle;
Unit * z = ShuttleMap[s].Zealot;
for (int i = 0; i < 2; i++)
{
if (z[i] != nullptr && !s->unload(z[i]))
{
s->unload(u);
}
else if (z[i] != nullptr)
{
LatencyMap[z[i]] = Broodwar->getLatencyFrames() + 8;
}
}
}
else
{
ReaverMap[u].Shuttle->unload(u);
}
}

void ReaverDecide(Unit u)
{
u->train(UnitTypes::Protoss_Scarab);

currentReaver = u;

//Broodwar << "Order: " << u->getOrder() << " - Scarabs: " << u->getScarabCount() << " - Loaded: " << u->isLoaded() << " - Scarabs2: " << ReaverMap[u].Scarabs << std::endl;//" - IsOverLand?: " << Broodwar->isWalkable(ReaverMap[u].Shuttle->getPosition().x / 8, ReaverMap[u].Shuttle->getPosition().y / 8) << std::endl;

if (LatencyMap[u] > 0)
{
LatencyMap[u]--;
return;
}

if (ReaverMap[u].Shuttle == nullptr)
{
return;
}

for (auto const &b : Broodwar->getBullets())
{
if (b->getTarget() == u && b->getType() != BulletTypes::Arclite_Shock_Cannon_Hit)
{
u->load(ReaverMap[u].Shuttle);
return;
}
}

if (u->getOrder() == Orders::AttackUnit && u->isAttackFrame() == false)
{

//if (BWTA::getGroundDistance(u->getTilePosition(), TilePositionConvert(u->getOrderTargetPosition())) > 300)
//{
//	u->load(ReaverMap[u].Shuttle);
//}
ReaverMap[u].Attacktime--;
if (ReaverMap[u].Attacktime < 1)
{
u->load(ReaverMap[u].Shuttle);
}
return;

}

Unit AttackUnit;
AttackUnit = Broodwar->getClosestUnit(ReaverMap[u].Shuttle->getPosition(), ReaverDrop, 400);
//AttackUnit = Broodwar->getBestUnit(ReaverBest, ReaverDrop, ReaverMap[u].Shuttle->getPosition(), 256);
if (u->isLoaded())
{
if (ReaverMap[u].Shuttle->getShields() < 30)
{
if (ReaverMap[u].Shuttle->getShields() + ReaverMap[u].Shuttle->getHitPoints() < 20)
{
ReaverMap[u].Shuttle->unload(u);
}
return;
}
ReaverMap[u].Scarabs = u->getScarabCount();
}

if (AttackUnit != nullptr)
{
Broodwar->drawLineMap(u->getPosition(), AttackUnit->getPosition(), Colors::Yellow);
}

if (AttackUnit != nullptr && u->getShields() > 40 && u->getGroundWeaponCooldown() < 1 && AntiReaverDropWeight(u) < 4)
{
ReaverUnload(u);
u->attack(AttackUnit);
ReaverMap[u].Attacktime = 48;
return;
}
if (ReaverMap[u].Shuttle->getHitPoints() + ReaverMap[u].Shuttle->getShields() > 30 && (ReaverMap[u].Scarabs != u->getScarabCount() || u->getScarabCount() == 0))
{
u->load(ReaverMap[u].Shuttle);
}
if (AttackUnit == nullptr)
{
u->load(ReaverMap[u].Shuttle);
}
}

bool isShuttleThreat(Unit u)
{
if ((IsEnemy && IsVisible && !IsFlying && !IsCloaked && !IsBurrowed && !IsWorker)(u) && (u->getType() != UnitTypes::Zerg_Larva && u->getType() != UnitTypes::Zerg_Egg) && (!(IsBuilding)(u) || u->getType() == UnitTypes::Terran_Bunker || u->getType() == UnitTypes::Terran_Missile_Turret || u->getType() == UnitTypes::Zerg_Spore_Colony || u->getType() == UnitTypes::Protoss_Photon_Cannon))
{
return true;
}
return false;
}

UnitFilter IsShuttleThreat = isShuttleThreat;



void ShuttleDecide(Unit u)
{
Broodwar->drawLineMap(u->getPosition(), u->getOrderTargetPosition(), Colors::Orange);
Broodwar->drawCircleMap(u->getPosition(), 240, Colors::Grey);
Broodwar->drawCircleMap(u->getPosition(), 220, Colors::Grey);

if (ShuttleMap[u].sideSwap-- > 0)
{
ShuttleMap[u].sideSwap--;
}

if (TerranPickUp(u)){ return; }

//Broodwar->setLocalSpeed(24);
//Broodwar->setGUI(true);
Unit EnemyUnit = u->getClosestUnit(IsShuttleThreat, 300);
if (EnemyUnit != nullptr)
{
Broodwar->drawLineMap(u->getPosition(), EnemyUnit->getPosition(), Colors::Red);
if (EnemyUnit->getType() == UnitTypes::Terran_Missile_Turret || EnemyUnit->getType() == UnitTypes::Terran_Bunker || EnemyUnit->getType() == UnitTypes::Zerg_Spore_Colony || EnemyUnit->getType() == UnitTypes::Protoss_Photon_Cannon)
{
if (u->getDistance(EnemyUnit) < 240)
{
Broodwar->drawTextMap(u->getPosition(), "Run Away AA");
LatencyMap[u] = 2;
RunAway(u, EnemyUnit);
return;
}
else
{
Broodwar->drawTextMap(u->getPosition(), "Run Sideways AA");
LatencyMap[u] = 2;
RunSideways(u, EnemyUnit, ShuttleMap[u].direction);
return;
}
}
else
{

//if (u->getDistance(EnemyUnit) > EnemyUnit->getType().airWeapon().maxRange() + 64 && (u->getDistance(EnemyUnit) > EnemyUnit->getType().groundWeapon().maxRange() + 32 || EnemyUnit->getType().groundWeapon().maxRange() > 96))
if (u->getDistance(EnemyUnit) > 220)
{
Broodwar->drawTextMap(u->getPosition(), "Run sideways outside 220");
LatencyMap[u] = 2;
RunSideways(u, EnemyUnit, ShuttleMap[u].direction);
return;

//if (!Broodwar->isWalkable(u->getPosition().x / 8, u->getPosition().y / 8))
//{
//int ux = u->getPosition().x;
//int uy = u->getPosition().y;
//int y = -4;
//int x = -4;
//Position currentPosition = Position(-1, -1);
//for (y = -4; x < 5; y++)  //Used to be 4
//{
//if (y > 4)
//{
//y = -4;
//x++;
//}
//Broodwar->drawDot(CoordinateType::Map, (ux + 32 * x), (uy + 32 * y), Colors::Orange);
////Broodwar << "x" << x << "y" << y << "fx" << (32  *px + 32 * x) / 32 << "fy" << (32 * px + 32 * y) / 32 << "px" << px << "py" << py << std::endl;
//if (Broodwar->isWalkable((ux + 32 * x) / 8, (uy + 32 * y) / 8))
//{
//currentPosition = Position((ux + 32 * x) / 8, (uy + 32 * y) / 8);
//break;
//}
//}
//if (currentPosition != Position(-1, -1))
//{
//u->move(currentPosition);
//}
//else
//{
//RunSideways(u, EnemyUnit);
//}
//}
//else
//{
//RunTowards(u, EnemyUnit);
//}
}
else
{
Broodwar->drawTextMap(u->getPosition(), "Run Away within 220");
LatencyMap[u] = 2;
RunAway(u, EnemyUnit);
return;
}
}

//if (((u->getDistance(EnemyUnit) > 240 || (!EnemyUnit->canAttack(u) && u->getDistance(EnemyUnit) > 64)) || !Broodwar->isWalkable(u->getPosition().x / 8, u->getPosition().y / 8)) && EnemyUnit->getType() != UnitTypes::Terran_Missile_Turret && EnemyUnit->getType() != UnitTypes::Zerg_Spore_Colony && EnemyUnit->getType() != UnitTypes::Protoss_Photon_Cannon)
//{
//	u->move(EnemyUnit->getPosition());
//	Broodwar->drawCircle(CoordinateType::Map, EnemyUnit->getPosition().x, EnemyUnit->getPosition().y, 10, Colors::Purple, true);
//	return;
//}
//else
//{
//	int x1 = u->getPosition().x;
//	int y1 = u->getPosition().y;
//	int x2 = EnemyUnit->getPosition().x;
//	int y2 = EnemyUnit->getPosition().y;
//	u->move(Position(2 * x1 - x2, 2 * y1 - y2));
//	Broodwar->drawCircle(CoordinateType::Map, 2 * x1 - x2, 2 * y1 - y2, 10, Colors::Purple, true);
//	return;
//}
}

Unit baseAttacker = mainBase->getClosestUnit(IsEnemy && !IsWorker && !IsFlying && !IsCloaked && !IsBurrowed, 1600);

if (baseAttacker != nullptr)
{
RunTowards(u, baseAttacker);
return;
}

if (enemyBase != Position(-1, -1))
{
u->move(enemyBase);
return;

}
else
{
for (const auto &b : BWTA::getStartLocations())
{
//Broodwar << "Check Visbile" << std::endl;
if (!Broodwar->isExplored(b->getPosition().x / 32, b->getPosition().y / 32))
//if (!Broodwar->isVisible(TilePositionConvert(b->getPosition())))
{
Broodwar->drawDot(CoordinateType::Map, b->getPosition().x, b->getPosition().y, Colors::Green);
Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Green, true);
//Broodwar << "Is not Visible" << std::endl;
u->move(b->getPosition());
return;
}
else
{
Broodwar->drawDot(CoordinateType::Map, b->getPosition().x, b->getPosition().y, Colors::Red);
Broodwar->drawCircleMap(b->getPosition().x, b->getPosition().y, 10, Colors::Red, true);
}
}
}

}


void CheckGates()
{
//if (allSupply > 20 && numGates < 1 && Broodwar->self()->minerals() > 120)o
if ((pylons.size() > 0 && numGates < 1) || (Broodwar->self()->minerals() > 500 && numGates - numGatesComplete < 1 && numGatesComplete < 2))
{
//TilePosition Temptest = Broodwar->getBuildLocation(UnitTypes::Protoss_Gateway, mainBuildPosition);
//Broodwar->drawBox(CoordinateType::Map, Temptest.x*32 - 64, Temptest.y*32 - 48, Temptest.x*32 + 64, Temptest.y*32 + 48, Colors::Red);
RequestBuilding(UnitTypes::Protoss_Gateway);
//RequestBuilding(UnitTypes::Protoss_Nexus, Broodwar->getBuildLocation(UnitTypes::Protoss_Nexus, mainBuildPosition));
numGates++;
}
}

void CheckCyber()
{
if (numGatesComplete > 0 && numCyber < 1 && Broodwar->self()->minerals() > 150)
{
RequestBuilding(UnitTypes::Protoss_Cybernetics_Core);
numCyber++;
}
}

void CheckAssimilator()
{
if (numGates > 0)
{
if (AssimilatorTimer > 0)
{
AssimilatorTimer--;
return;
}
for (const auto &b : bases)
{
for (const auto &g : NexusMap[b].Geysers)
{
if (!g->getType().isRefinery() && Broodwar->self()->minerals() > 75)
{
RequestBuilding(UnitTypes::Protoss_Assimilator);
AssimilatorTimer += 500;
}
}
}
}
}

void CheckFac()
{
if (numCyberComplete > 0 && numFac < 1 && numFac - numBayComplete < 1 && Broodwar->self()->minerals() > 150 && Broodwar->self()->gas() > 200)
{
RequestBuilding(UnitTypes::Protoss_Robotics_Facility);
numFac++;
}
}

void CheckBay()
{
if (numFacComplete > 0 && numBay < 1 && Broodwar->self()->gas() > 90)
{
RequestBuilding(UnitTypes::Protoss_Robotics_Support_Bay);
numBay++;
}
}

void CheckObs()
{
if (numBayComplete > 0 && numObs < 1 && gSpeed && Broodwar->self()->gas() > 200)
{
RequestBuilding(UnitTypes::Protoss_Observatory);
numObs++;
}
}

void CheckNexus()
{
if (gSpeed && Broodwar->self()->minerals() > 350)
{
//	RequestBuilding(UnitTypes::Protoss_Nexus);
}
}

void CheckShield()
{
if (numShield < 1 && numBayComplete > 0 && Broodwar->self()->minerals() > 100)
{
numShield++;
RequestBuilding(UnitTypes::Protoss_Shield_Battery);
}
}


void CheckBuilderWork()
{
CheckSupply();
CheckGates();
CheckCyber();
CheckAssimilator();
CheckFac();
CheckBay();
CheckObs();
CheckNexus();
}*/

std::map <int, BWTA::BaseLocation *> ExpansionQuicksort(std::map <int, BWTA::BaseLocation *> input, BWTA::BaseLocation * comparePos)
{
	//for (auto const &e : input)
	//{
	//	if (e.second == nullptr)
	//	{
	//		Broodwar << "e is equal to nullptr everybody!";
	//	}
	//	else
	//	{
	//		Broodwar << "BLARRRRGHHH";
	//	}
	//}

	//if (input[0] == nullptr)
	//{
	//	Broodwar << "a";
	//}
	//else
	//{

	//}

	BWTA::BaseLocation * i;

	if (!input[0]->isIsland())
	{
		i = input[0];
	}
	else
	{
		i = input[1];
		islandBases.insert(input[1]);
	}

	std::map <int, BWTA::BaseLocation *> first;
	std::map <int, BWTA::BaseLocation *> second;
	std::map <int, BWTA::BaseLocation *> returnMap;

	int firsti = 0;
	int secondi = 0;

	//if (i == nullptr)
	//{
	//	Broodwar << "b";
	//}
	//else
	//{
	//	Broodwar << "a";
	//}


	//	if (i == nullptr)
	//	{
	//		Broodwar << "WHOA didn't see that coming" << std::endl;
	//	}
	//	else
	//	{
	//		Broodwar << "Although it no longer surprises me";
	//	}
	//	Broodwar << "i - " << i << std::endl;
	//	Broodwar << "e - " << e << std::endl;
	//	Broodwar << "comparePos - " << comparePos << std::endl;
	//	if (i == nullptr)
	//	{
	//		Broodwar << "gfds" << std::endl;
	//	}
	//	else
	//	{
	//		Broodwar << "blah";
	//	}
	//	Broodwar << "i->getGroundDistance(comparePos) - " << i->getGroundDistance(comparePos) << std::endl;
	//	Broodwar << "input[e]->getGroundDistance(comparePos)s - " << input[e]->getGroundDistance(comparePos) << std::endl;

	for (unsigned int e = 1; e < input.size(); e++)
	{
		if (input[e]->isIsland())
		{
			islandBases.insert(input[e]);
			continue;
		}
		if (i->getGroundDistance(comparePos) > input[e]->getGroundDistance(comparePos))
		{
			first[firsti] = input[e];
			firsti++;
		}
		else
		{
			second[secondi] = input[e];
			secondi++;
		}
	}

	if (first.size() > 1)
	{
		first = ExpansionQuicksort(first, comparePos);
	}
	if (second.size() > 1)
	{
		second = ExpansionQuicksort(second, comparePos);
	}

	firsti = 0;

	for (unsigned int e = 0; e < first.size(); e++)
	{
		returnMap[firsti] = first[e];
		firsti++;
	}

	returnMap[firsti] = i;
	firsti++;

	for (unsigned int e = 0; e < second.size(); e++)
	{
		returnMap[firsti] = second[e];
		firsti++;
	}

	return returnMap;

}

void BuildPosInit()
{
	TilePosition nexusPosition = mainBase->getTilePosition();

	if (Broodwar->canBuildHere(TilePosition(nexusPosition.x + 9, nexusPosition.y), UnitTypes::Protoss_Nexus))
	{
		initialPylonBuildPosition = TilePosition(nexusPosition.x + 7, nexusPosition.y);
		initialOtherBuildPosition[1] = TilePosition(nexusPosition.x + 4, nexusPosition.y);
		initialOtherBuildPosition[0] = TilePosition(nexusPosition.x + 9, nexusPosition.y);
	}
	else if (Broodwar->canBuildHere(TilePosition(nexusPosition.x - 10, nexusPosition.y), UnitTypes::Protoss_Nexus))
	{
		initialPylonBuildPosition = TilePosition(nexusPosition.x - 7, nexusPosition.y);
		initialOtherBuildPosition[0] = TilePosition(nexusPosition.x - 5, nexusPosition.y);
		initialOtherBuildPosition[1] = TilePosition(nexusPosition.x - 10, nexusPosition.y);
	}
	else if (Broodwar->canBuildHere(TilePosition(nexusPosition.x, nexusPosition.y - 3), UnitTypes::Protoss_Nexus))
	{
		initialPylonBuildPosition = TilePosition(nexusPosition.x, nexusPosition.y - 2);
		initialOtherBuildPosition[0] = TilePosition(nexusPosition.x + 2, nexusPosition.y - 3);
		initialOtherBuildPosition[1] = TilePosition(nexusPosition.x - 3, nexusPosition.y - 3);
	}
	else if (Broodwar->canBuildHere(TilePosition(nexusPosition.x, nexusPosition.y + 3), UnitTypes::Protoss_Nexus))
	{
		initialPylonBuildPosition = TilePosition(nexusPosition.x, nexusPosition.y + 4);
		initialOtherBuildPosition[0] = TilePosition(nexusPosition.x + 2, nexusPosition.y + 4);
		initialOtherBuildPosition[1] = TilePosition(nexusPosition.x - 3, nexusPosition.y + 4);
	}
	else
	{
		Broodwar << "Error testing Nexus placability" << std::endl;
	}

	for (unsigned int e = 1; e < Expansions.size(); e++)
	{
		TilePosition nexusPosition = Expansions[e]->getTilePosition();
		int pylX = nexusPosition.x;
		int pylY = nexusPosition.y;
		int gateX = nexusPosition.x;
		int gateY = nexusPosition.y;
		ExpXdir[e] = 2;
		ExpYdir[e] = 2;
		if (Broodwar->canBuildHere(TilePosition(nexusPosition.x + 4, nexusPosition.y), UnitTypes::Protoss_Nexus))
		{
			ExpXdir[e] = -2;
			pylX = nexusPosition.x + 4;
			pylY = nexusPosition.y - 2;
			gateX = nexusPosition.x + 6;
			gateY = nexusPosition.y - 5;
		}
		else if (Broodwar->canBuildHere(TilePosition(nexusPosition.x - 4, nexusPosition.y), UnitTypes::Protoss_Nexus))
		{
			ExpXdir[e] = 2;
			pylX = nexusPosition.x - 2;
			pylY = nexusPosition.y + 3;
			gateX = nexusPosition.x - 6;
			gateY = nexusPosition.y + 5;
		}

		if (Broodwar->canBuildHere(TilePosition(nexusPosition.x, nexusPosition.y - 3), UnitTypes::Protoss_Nexus))
		{
			ExpYdir[e] = 2;
			pylY = nexusPosition.y - 2;
			gateY = nexusPosition.y - 5;
		}
		else if (Broodwar->canBuildHere(TilePosition(nexusPosition.x, nexusPosition.y + 3), UnitTypes::Protoss_Nexus))
		{
			ExpYdir[e] = -2;
			pylY = nexusPosition.y + 3;
			gateY = nexusPosition.y + 5;
		}

		//Broodwar << "nexusPosition.x - " << nexusPosition.x << " - nexusPosition.y - " << nexusPosition.y << " - ExpXdir - " << ExpXdir[e] << " - ExpYdir[e] - " << ExpYdir[e] << " - pylX - " << pylX << " - pylY - " << pylY << std::endl;

		ExpPylPos[e] = TilePosition(pylX, pylY);
		ExpGatePos[e] = TilePosition(gateX, gateY);
	}
}

void ZerGreenBot::onStart()
{
	Broodwar->setLocalSpeed(10);
	//Broodwar->setGUI(true);
	// Hello World!
	Broodwar->sendText(":-) (-: :-) (-: :-) (-: :-) (-:");

	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar->sendText("Hello, my name is %s!", Broodwar->mapName().c_str());
	Broodwar->sendText("What's your name?");

	//Broodwar->sendText("black sheep wall");

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);
	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Check if this is a replay
	if (Broodwar->isReplay())
	{
	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		if (Broodwar->enemy()) // First make sure there is an enemy
		{
			Broodwar << "Haha, it's my favorite matchup, " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
			Broodwar << "I've got this ;-)" << std::endl;
		}

		if (Broodwar->self()->getRace() != Races::Protoss)
		{
			Broodwar << "..." << std::endl << "Hey..." << std::endl << "Wait a minute" << std::endl << "You know I'm a protoss bot, right?" << std::endl;
			return;
		}
	}

	if (Broodwar->mapFileName() == "(0)Neural.scx")
	{
		Broodwar << "Testing" << std::endl;
		for (const auto &u : Broodwar->self()->getUnits())
		{
			if (isCarrier(u))
			{
				Carriers.insert(u);
			}
		}
	}

	mapNumber = getMapInt();

	double distance = 99999;
	for (int f = 0; f < 4; f++)
	{
		if (FFEPositions[f].Pylon == TilePositions::Unknown)
		{
			Broodwar << "FFE TilePositions is None - This is probably not a map from the SSCAIT map pool (as of 28/9/2016)" << std::endl;
			continue;
		}
		else if (FFEPositions[f].Pylon == TilePositions::None)
		{
			continue;
		}
		else if (Broodwar->self()->getStartLocation().getDistance(FFEPositions[f].Pylon) < distance)
		{
			closestFFE = f;
			distance = Broodwar->self()->getStartLocation().getDistance(FFEPositions[f].Pylon);
		}
	}

	BWTA::readMap();
	BWTA::analyze();
	BWTA::buildChokeNodes();

	for (const auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType() == UnitTypes::Protoss_Nexus)
		{
			mainBase = u;
			actualMainBase = BWTA::getNearestBaseLocation(mainBase->getPosition());

			NewNexus(u);
			SetNexusMinerals(u);
			numNexus++;

			auto m = NexusMap[u].Patches.begin();
			m++;
			m++;
			m++;
			m++;
			int x1 = u->getTilePosition().x;
			int y1 = u->getTilePosition().y;
			int x2 = (*m)->getTilePosition().x;
			int y2 = (*m)->getTilePosition().y;

			//mainBuildPosition = u->getTilePosition();

			//Broodwar << x1 << " - " << x2 << " - " << y1 << " - " << y2 << std::endl;

			mainBuildPosition = TilePosition((2 * x1 - x2), (2 * y1 - y2));

			/*Broodwar << mainBuildPosition.x;
			Broodwar << mainBuildPosition.y << std::endl;*/

		}

	}

	int i = 0;

	for (auto const &e : BWTA::getBaseLocations())
	{
		Expansions[i] = e;
		i++;
	}

	//BWTA::BaseLocation * temp = BWTA::getNearestBaseLocation(Broodwar->self()->getStartLocation());

	//if (temp == nullptr)
	//{
	//	Broodwar << "temp == nullptr";
	//}
	//else
	//{
	//	Broodwar << "temp != nullptr";
	//}

	//for (auto const &e : Expansions)
	//{
	//	if (e.second == nullptr)o
	//	{
	//		Broodwar << "e is equal to nullptr everybody!";
	//	}
	//	else
	//	{
	//		Broodwar << "BLARRRRGHHH";
	//	}
	//}

	Expansions = ExpansionQuicksort(Expansions, BWTA::getNearestBaseLocation(Broodwar->self()->getStartLocation()));

	auto farExpansion = Expansions.end();
	farExpansion--;
	closestEnemyBase = farExpansion->second;

	/*for (unsigned int e = 0; e < Expansions.size(); e++)
	{
	Broodwar << Expansions[e]->getPosition() << std::endl;
	}*/

	BuildPosInit();


	for (const auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType() == UnitTypes::Protoss_Probe)
		{
			NewProbe(u);
		}
	}

}

void ZerGreenBot::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}

EpicText UpdateEpicText(EpicText text)
{
	text.time--;
	if (text.time < 1)
	{
		return text;

	}
	else
	{
		Broodwar->drawTextMap(text.Position,text.Text.c_str());
		EpicText temp;
		return temp;
	}
}

void ZerGreenBot::onFrame()
{
	if (Broodwar->self()->getRace() != Races::Protoss)
	{
		return;
	}

	if (Broodwar->getMousePosition() != Positions::Unknown && Broodwar->getKeyState(Key::K_S))
	{
		Broodwar->drawTextMouse(-20, 0, "%d", Broodwar->getMousePosition().x);
		Broodwar->setLocalSpeed(Broodwar->getMousePosition().x);
	}

	// Called once every game frame

	Broodwar->drawTextMap(100, 0, "Shoutout to Rising Kingdoms, the best RTS game ever! (Ignore the bad design choices)");

	Broodwar->drawTextScreen(0, 0, "Start Time: %d", clock());
	Broodwar->drawTextScreen(0, 40, ">55: %d", moreThan55);


	Broodwar->drawTextScreen(0, 190, "Map: %s", Broodwar->mapFileName());
	Broodwar->drawTextScreen(0, 200, "My lost minerals: %d", minsLost);
	Broodwar->drawTextScreen(0, 210, "My lost gas: %d", gasLost);
	Broodwar->drawTextScreen(0, 220, "Their lost minerals: %d", enemyMinsLost);
	Broodwar->drawTextScreen(0, 230, "Their lost gas: %d", enemyGasLost);

	std::unordered_set<Position> removeLocations;

	for (auto const &e : EnemyBaseLocations)
	{
		Broodwar->drawTextMap(e.x, e.y + 10, "Enemy Base");
		if (Broodwar->isVisible(e.x / 8, e.y / 8) && Broodwar->getClosestUnit(e, IsEnemy && IsBuilding, 400) == nullptr)
		{
			removeLocations.insert(e);
			if (closestEnemyBase == BWTA::getNearestBaseLocation(e))
			{
				double distance = 999999;
				Position nearPos = Position(-1,-1);

				for (auto const &b : EnemyBaseLocations)
				{
					if (e.getDistance(b) < distance)
					{
						distance = e.getDistance(b);
						nearPos = b;
					}
				}

				if (nearPos != Position(-1, -1))
				{
					closestEnemyBase = BWTA::getNearestBaseLocation(nearPos);
				}
			}
		}
	}

	for (auto const &r : removeLocations)
	{
		EnemyBaseLocations.erase(r);
	}

	Broodwar->drawTextMap(closestEnemyBase->getPosition().x, closestEnemyBase->getPosition().y + 20, "Closest Enemy Base (By ground distance)");

	// Draw Expansions, draw chokepoints

	for (unsigned int e = 0; e < Expansions.size() - 1; e++)
	{
		Broodwar->drawLineMap(Expansions[e]->getPosition(), Expansions[e + 1]->getPosition(), Colors::Purple);
		Broodwar->drawTextMap(Expansions[e]->getPosition(), "Expansion %d", e);
		Broodwar->drawCircleMap(TilePositionConvert(ExpPylPos[e]), 5, Colors::Black, true);
		Broodwar->drawCircleMap(TilePositionConvert(ExpGatePos[e]), 5, Colors::Black, true);
		//Broodwar->drawCircleMap(TilePositionConvert(ExpPylPos[e]), 100, Colors::Purple, true);
	}

	for (auto const &c : BWTA::getChokepoints())
	{
		Broodwar->drawCircleMap(c->getCenter(), 5, Colors::Cyan, true);
	}

	Broodwar->drawCircleMap(ZerGreen[0], ZerGreen[1], 7, Colors::Green, true);
	Broodwar->drawCircleMap(ZerGreen[0] + 100, ZerGreen[1] - 20, 5, Colors::Green, true);
	//(0, -80), (40, -110), (100, -75);
	Broodwar->drawLineMap(ZerGreen[0], ZerGreen[1] + 80, ZerGreen[0] + 40, ZerGreen[1] + 110, Colors::Green);
	Broodwar->drawLineMap(ZerGreen[0] + 40, ZerGreen[1] + 110, ZerGreen[0] + 100, ZerGreen[1] + 75, Colors::Green);

	EventTime = clock();

	Broodwar->drawTextScreen(200, 0, "Latency: %d", Broodwar->getLatencyFrames());
	Broodwar->drawTextScreen(200, 10, "Remaining Latency: %d", Broodwar->getRemainingLatencyFrames());
	//Broodwar->drawTextScreen(200, 10, "AllSupply: %d", allSupply);
	//Broodwar->drawTextScreen(200, 20, "Total Supply: %d", Broodwar->self()->supplyTotal());
	//Broodwar->drawTextScreen(200, 30, "Used Supply: %d", Broodwar->self()->supplyUsed());
	//Broodwar->drawTextScreen(200, 40, "Allsupply - Used Supply: %d", allSupply - Broodwar->self()->supplyUsed());

	//Broodwar->drawTextScreen(0, 0, "NumGates: %d", numGates);
	//Broodwar->drawTextScreen(0, 10, "NumCyber: %d", numCyber);

	//Broodwar->drawDotMap(TilePositionConvert(mainBuildPosition), Colors::Purple);

	Broodwar->drawCircleMap(TilePositionConvert(mainBuildPosition), 8, Colors::Green, true);

	std::unordered_set <EpicText> removeEpicSet;

	for (const auto &t : EpicTextSet)
	{
		removeEpicSet.insert(UpdateEpicText(t));
	}

	for (const auto &t : removeEpicSet)
	{
		EpicTextSet.erase(t);
	}

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;

	for (auto &u : bases)
	{
		Broodwar->drawCircle(CoordinateType::Map, u->getPosition().x, u->getPosition().y, 384, Colors::Red, false);
		NexusDecide(u);
	}

	if (Broodwar->enemy()->getRace() == Races::Terran || Broodwar->enemy()->getRace() == Races::Unknown)
	{
		TerranOnFrame();
	}
	else if (Broodwar->enemy()->getRace() == Races::Protoss)
	{
		//TerranOnFrame();
		ProtossOnFrame();
	}
	else // Zerg
	{
		ZergOnFrame();
	}

	Broodwar->drawTextScreen(0, 10, "End Time: %d", (clock()));
	Broodwar->drawTextScreen(0, 20, "Time Difference: %d", (clock() - EventTime));
	if (clock() - EventTime > 55)
	{
		moreThan55++;
	}

}

void ZerGreenBot::onSendText(std::string text)
{

	// Send the text to the game if it is not being processed.
	Broodwar->sendText("%s", text.c_str());

	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

}

void ZerGreenBot::onReceiveText(BWAPI::Player player, std::string text)
{

}

void ZerGreenBot::onPlayerLeft(BWAPI::Player player)
{
	Broodwar->sendText("OMG you just got rekt so bad %s!", player->getName().c_str());
}

void ZerGreenBot::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ZerGreenBot::onUnitDiscover(BWAPI::Unit unit)
{
	if ((IsBuilding && IsEnemy && !IsFlyingBuilding)(unit))
	{
		BWTA::BaseLocation * tempBase = BWTA::getNearestBaseLocation(unit->getPosition());
		if (EnemyBaseLocations.find(tempBase->getPosition()) == EnemyBaseLocations.end())
		{
			EnemyBaseLocations.insert(tempBase->getPosition());
		}

		if ( tempBase->getGroundDistance(actualMainBase) < closestEnemyBase->getGroundDistance(actualMainBase) && !tempBase->isIsland())
		{
			closestEnemyBase = tempBase;
		}
	}

	if (Broodwar->enemy()->getRace() == Races::Zerg)
	{
		if (isHydralisk(unit))
		{
			numHydra++;
		}
		else if (isMutalisk(unit))
		{
			numMuta++;
		}
	}
}

void ZerGreenBot::onUnitEvade(BWAPI::Unit unit)
{
	if (Broodwar->enemy()->getRace() == Races::Zerg)
	{
		if (isHydralisk(unit))
		{
			numHydra--;
		}
		else if (isMutalisk(unit))
		{
			numMuta--;
		}
	}
}

void ZerGreenBot::onUnitShow(BWAPI::Unit unit)
{
	if ((IsBuilding && IsEnemy && !IsFlyingBuilding)(unit))
	{
		enemyBase = unit->getPosition();
		enemyBaseReal = BWTA::getNearestBaseLocation(enemyBase)->getPosition();
		//enemyExpansion1 = Position(-1, -1);
		//enemyExpansion2 = Position(-1, -1);


		if (Broodwar->enemy()->getRace() == Races::Terran || Broodwar->enemy()->getRace() == Races::Protoss)
		{
			Unitset scoutsRemove;
			for (auto const &s : scouts)
			{
				scoutsRemove.insert(s);
			}
			for (auto const &s : scoutsRemove)
			{
				scouts.erase(s);
				harvesters.insert(s);
			}
		}
	}

	if (unit->getType() == UnitTypes::Zerg_Lurker)
	{
		seenLurker = true;
	}
}

void ZerGreenBot::onUnitHide(BWAPI::Unit unit)
{

}

UnitType compareType;

bool nearestBuilderProbe(Unit u)
{
	if (builders.contains(u) == true && BuilderMap[u].BuildType == compareType)
	{
		return true;
	}
	return false;
}

UnitFilter NearestBuilderProbe = nearestBuilderProbe;


void ZerGreenBot::onUnitCreate(BWAPI::Unit unit)
{
	if ((IsBuilding && IsAlly)(unit))
	{
		compareType = unit->getType();
		Unit builder = unit->getClosestUnit(NearestBuilderProbe);
		if (builder != nullptr)
		{
			harvesters.insert(builder);
			builders.erase(builder);
		}

		constructingBuildings.insert(unit);

		//Broodwar << unit->getType() << " created at " << unit->getTilePosition() << " in map " << Broodwar->mapFileName() << std::endl;
		Broodwar << unit->getType() << " created at " << unit->getTilePosition() << std::endl;
	}

	if (unit->getType() == UnitTypes::Protoss_Scarab)
	{
		Unit reaver = unit->getClosestUnit(IsReaver);
		if (reaver == nullptr)
		{
			return;
		}
		reaver->load(ReaverMap[reaver].Shuttle);
		LatencyMap[reaver] = 60;
	}

	if (unit->getType() == UnitTypes::Protoss_Probe)
	{
		NewProbe(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Nexus)
	{
		NewNexus(unit);
		SetNexusMinerals(unit);

		//auto m = NexusMap[unit].Patches.begin();
		//m++;
		//m++;
		//m++;
		//m++;
		//int x1 = unit->getTilePosition().x;
		//int y1 = unit->getTilePosition().y;
		//int x2 = (*m)->getTilePosition().x;
		//int y2 = (*m)->getTilePosition().y;

		//mainBuildPosition = TilePosition((2 * x1 - x2), (2 * y1 - y2));o

	}
	else if (unit->getType() == UnitTypes::Protoss_Gateway)
	{
		numGatesCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Forge)
	{
		numForgeCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Cybernetics_Core)
	{
		numCyberCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Stargate)
	{
		numStargateCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Facility)
	{
		numFacCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Observatory)
	{
		obs = unit;
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Support_Bay)
	{
		numBayCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Photon_Cannon)
	{
		numCannonCreate++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Forge)
	{
		if (forge[0] == nullptr || !forge[0]->exists())
		{
			forge[0] = unit;
		}
		else
		{
			forge[1] = unit;
		}
	}
	else if (unit->getType() == UnitTypes::Protoss_Zealot)
	{
		Zealots.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Dragoon)
	{
		NewDragoon(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Observer)
	{
		NewObs(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Carrier)
	{
		Carriers.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Corsair)
	{
		if (rand() % 1000 == 0)
		{
			CreateEpicText("Glory for Sky's Cigar!!!", unit->getPosition().x, unit->getPosition().y + 20, 48 + rand() % 1000);
		}
		Corsairs.insert(unit);
	}
	//else if (unit->getType() == )
	//{
	//}

	if (Broodwar->enemy()->getRace() == Races::Terran)	// Decide strategy of created units
	{

	}
	else if (Broodwar->enemy()->getRace() == Races::Protoss)
	{

	}
	else if (Broodwar->enemy()->getRace() == Races::Zerg)
	{
		if (unit->getType() == UnitTypes::Protoss_Corsair)
		{
			CorsairMap[unit].targetPoint = CORSAIR_UNDEFINED_POSITION;

			int numCorsair = Corsairs.size();
			int numComb = unit->getUnitsInRadius(99999, IsCorsairCombTactic).size();
			int numGroup = unit->getUnitsInRadius(99999, IsCorsairGroupTactic).size();

			if (numComb * 2 <= numGroup)
			{
				CorsairMap[unit].Tactic = Comb;
			}
			else
			{
				CorsairMap[unit].Tactic = Group;
			}
		}
	}

}

void ZerGreenBot::onUnitDestroy(BWAPI::Unit unit)
{

	if (unit->getPlayer() != Broodwar->self())
	{
		enemyMinsLost += unit->getType().mineralPrice();
		enemyGasLost += unit->getType().gasPrice();
		return;
	}
	else
	{
		if (unit->getKillCount() > 10)
		{
			Broodwar->sendText("Nooooooo.... You killed %s. He was my hero, and he had %d kills.", name[rand() % NUM_NAMES].c_str(), unit->getKillCount());
			Broodwar->sendText("He was my favorite %s. How could you do such a thing???", unit->getType().getName().c_str());
		}
		minsLost += unit->getType().mineralPrice();
		gasLost += unit->getType().gasPrice();
	}

	if (unit->getType() == UnitTypes::Protoss_Probe)
	{
		PatchMap[ProbeMap[unit].Patch].Probes--;
		harvesters.erase(unit);
		defenders.erase(unit);
		if (builders.contains(unit))
		{
			RequestBuilding(BuilderMap[unit].BuildType, BuilderMap[unit].BuildPosition);
			builders.erase(unit);
		}
		if (ProbeMap[unit].Patch != nullptr && ProbeMap[unit].Patch->getType() == UnitTypes::Protoss_Assimilator)
		{
			Unit tempunit = ProbeMap[unit].Patch->getClosestUnit(AssimilatorTransferFilter);
			PatchMap[ProbeMap[tempunit].Patch].Probes--;
			ProbeMap[tempunit].Patch = ProbeMap[unit].Patch;
		}
	}
	else if (unit->getType() == UnitTypes::Protoss_Pylon)
	{
		allSupply -= 16;
		pylons.erase(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Nexus)
	{
		allSupply -= 18;
		bases.erase(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Gateway)
	{
		numGates--;
		numGatesCreate--;
		numGatesComplete--;
		gateways.erase(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Facility)
	{
		numFac--;
		numFacCreate--;
		numFacComplete--;
		roboFacs.erase(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Support_Bay)
	{
		numBay--;
		numBayComplete--;
		bay = nullptr;
	}
	else if (unit->getType() == UnitTypes::Protoss_Reaver)
	{
		Reavers.erase(unit);

		if (ReaverMap[unit].Tactic == Defend)
		{
			numReaverDefend--;
		}
		else if (ReaverMap[unit].Tactic == Harass)
		{
			numReaverHarass--;
		}
		else if (ReaverMap[unit].Tactic == Engage)
		{
			numReaverEngage--;
		}
		else if (ReaverMap[unit].Tactic == ArmyReaver)
		{
			numReaverArmy--;
		}

		if (ReaverMap[unit].Shuttle == nullptr)
		{
			return;
		}
		if (ShuttleMap[ReaverMap[unit].Shuttle].Reaver[0] == unit)
		{
			ShuttleMap[ReaverMap[unit].Shuttle].Reaver[0] = nullptr;
		}
		else
		{
			ShuttleMap[ReaverMap[unit].Shuttle].Reaver[1] = nullptr;
		}
		ReaverMap[unit].Shuttle = nullptr;
	}
	else if (unit->getType() == UnitTypes::Protoss_Shuttle)
	{
		ReaverMap[ShuttleMap[unit].Reaver[0]].Shuttle = nullptr;
		ReaverMap[ShuttleMap[unit].Reaver[1]].Shuttle = nullptr;
		ReaverMap[ShuttleMap[unit].Zealot[0]].Shuttle = nullptr;
		ReaverMap[ShuttleMap[unit].Zealot[1]].Shuttle = nullptr;
		ReaverMap[ShuttleMap[unit].Zealot[2]].Shuttle = nullptr;
		ReaverMap[ShuttleMap[unit].Zealot[3]].Shuttle = nullptr;
		Shuttles.erase(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Dragoon)
	{
		Dragoons.erase(unit);
		for (int i = 0; i < DRAGOON_WAYPOINTS; i++)
		{
			DragoonMap[unit].prevPosition[i] = DRAGOON_DEFAULT_POSITION;
		}
	}
	else if (unit->getType() == UnitTypes::Protoss_Zealot)
	{
		Zealots.erase(unit);
		for (int i = 0; i < 4; i++)
		{
			if (ShuttleMap[ZealotMap[unit].Shuttle].Zealot[i] == unit)
			{
				ShuttleMap[ZealotMap[unit].Shuttle].Zealot[i] = nullptr;
			}
		}
		//ShuttleMap[ZealotMap[unit].Shuttle].Zealot
	}
	else if (unit->getType() == UnitTypes::Protoss_Observer)
	{
		Obses.erase(unit);
	}

	if (Broodwar->enemy()->getRace() == Races::Zerg)
	{
		if (isHydralisk(unit))
		{
			numHydra--;
		}
		else if (isMutalisk(unit))
		{
			numMuta--;
		}
	}
}

void ZerGreenBot::onUnitMorph(BWAPI::Unit unit)
{
	if (unit->getType() == UnitTypes::Protoss_Assimilator)
	{
		numGasCreate++;
		compareType = unit->getType();
		Unit builder = unit->getClosestUnit(NearestBuilderProbe);
		if (builder != nullptr)
		{
			harvesters.insert(builder);
			builders.erase(builder);
		}
	}
}

void ZerGreenBot::onUnitRenegade(BWAPI::Unit unit)
{
}

void ZerGreenBot::onSaveGame(std::string gameName)
{

}

void ZerGreenBot::onUnitComplete(BWAPI::Unit unit)
{
	if ((IsBuilding && IsAlly)(unit))
	{
		constructingBuildings.erase(unit);
	}

	LatencyMap[unit] = 0;
	if (unit->getType() == UnitTypes::Protoss_Pylon)
	{
		pylons.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Gateway)
	{
		numGatesComplete++;
		gateways.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Cybernetics_Core)
	{
		numCyberComplete++;
		cyber = unit;
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Facility)
	{
		roboFacs.insert(unit);
		numFacComplete++;
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Support_Bay)
	{
		numBayComplete++;
		bay = unit;
	}
	else if (unit->getType() == UnitTypes::Protoss_Assimilator)
	{
		for (int i = 0; i < 3; i++)
		{
			Unit tempunit = unit->getClosestUnit(AssimilatorTransferFilter);
			PatchMap[ProbeMap[tempunit].Patch].Probes--;
			ProbeMap[tempunit].Patch = unit;
		}
	}
	else if (unit->getType() == UnitTypes::Protoss_Robotics_Facility)
	{
		numFacComplete++;
		roboFacs.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Stargate)
	{
		numStargateComplete++;
		stargates.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Forge)
	{
		numForgeComplete++;
		forges.insert(unit);
	}
	else if (unit->getType() == UnitTypes::Protoss_Reaver)
	{
		Reavers.insert(unit);
		NewReaver(unit);
		if (Broodwar->enemy()->getRace() == Races::Zerg)
		{
			if (numReaverDefend < 1)
			{
				ReaverMap[unit].Tactic = Defend;
				numReaverDefend++;
			}
			else if (numReaverHarass < numReaverArmy)
			{
				ReaverMap[unit].Tactic = Harass;
				numReaverHarass++;
			}
			else
			{
				ReaverMap[unit].Tactic = ArmyReaver;
				numReaverArmy++;
			}
		}
	}
	else if (unit->getType() == UnitTypes::Protoss_Shuttle)
	{
		Shuttles.insert(unit);
		NewShuttle(unit);
		if (Broodwar->enemy()->getRace() == Races::Zerg)
		{
			if (ArmyShuttle == nullptr || !ArmyShuttle->exists())
			{
				ArmyShuttle = unit;
			}
		}
	}
	else if (unit->getType() == UnitTypes::Protoss_Citadel_of_Adun)
	{
		citadel = unit;
	}
	else if (unit->getType() == UnitTypes::Protoss_Fleet_Beacon)
	{
		fleet = unit;
	}
}

/*

/*for (auto &u : harvesters)
{
if (!u->exists)
{
continue;
}
if (u->isStasised() || u->isLoaded() || !u->isCompleted)
{
continue;
}



}*/

// Iterate through all the units that we own
/*for (auto &u : Broodwar->self()->getUnits())
{
// Ignore the unit if it no longer exists
// Make sure to include this block when handling any Unit pointer!
if ( !u->exists() )
continue;

// Ignore the unit if it has one of the following status ailments
if ( u->isLockedDown() || u->isMaelstrommed() || u->isStasised() )
continue;

// Ignore the unit if it is in one of the following states
if ( u->isLoaded() || !u->isPowered() || u->isStuck() )
continue;

// Ignore the unit if it is incomplete or busy constructing
if ( !u->isCompleted() || u->isConstructing() )
continue;


// Finally make the unit do some stuff!


// If the unit is a worker unit
if ( u->getType().isWorker() )
{
// if our worker is idle
if ( u->isIdle() )
{
// Order workers carrying a resource to return them to the center,
// otherwise find a mineral patch to harvest.
if ( u->isCarryingGas() || u->isCarryingMinerals() )
{
u->returnCargo();
}
else if ( !u->getPowerUp() )  // The worker cannot harvest anything if it
{                             // is carrying a powerup such as a flag
// Harvest from the nearest mineral patch or gas refinery
if ( !u->gather( u->getClosestUnit( IsMineralField || IsRefinery )) )
{
// If the call fails, then print the last error message
Broodwar << Broodwar->getLastError() << std::endl;
}

} // closure: has no powerup
} // closure: if idle

}
else if ( u->getType().isResourceDepot() ) // A resource depot is a Command Center, Nexus, or Hatchery
{

// Order the depot to construct more workers! But only when it is idle.
if ( u->isIdle() && !u->train(u->getType().getRace().getWorker()) )
{
// If that fails, draw the error at the location so that you can visibly see what went wrong!
// However, drawing the error once will only appear for a single frame
// so create an event that keeps it on the screen for some frames
Position pos = u->getPosition();
Error lastErr = Broodwar->getLastError();
Broodwar->registerEvent([pos,lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
nullptr,    // condition
Broodwar->getLatencyFrames());  // frames to run

// Retrieve the supply provider type in the case that we have run out of supplies
UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
static int lastChecked = 0;

// If we are supply blocked and haven't tried constructing more recently
if (  lastErr == Errors::Insufficient_Supply &&
lastChecked + 400 < Broodwar->getFrameCount() &&
Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0 )
{
lastChecked = Broodwar->getFrameCount();

// Retrieve a unit that is capable of constructing the supply needed
Unit supplyBuilder = u->getClosestUnit(  GetType == supplyProviderType.whatBuilds().first &&
(IsIdle || IsGatheringMinerals) &&
IsOwned);
// If a unit was found
if ( supplyBuilder )
{
if ( supplyProviderType.isBuilding() )
{
TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
if ( targetBuildLocation )
{
// Register an event that draws the target build location
Broodwar->registerEvent([targetBuildLocation,supplyProviderType](Game*)
{
Broodwar->drawBoxMap( Position(targetBuildLocation),
Position(targetBuildLocation + supplyProviderType.tileSize()),
Colors::Blue);
},
nullptr,  // condition
supplyProviderType.buildTime() + 100 );  // frames to run

// Order the builder to construct the supply structure
supplyBuilder->build( supplyProviderType, targetBuildLocation );
}
}
else
{
// Train the supply provider (Overlord) if the provider is not a structure
supplyBuilder->train( supplyProviderType );
}
} // closure: supplyBuilder is valid
} // closure: insufficient supply
} // closure: failed to train idle unit

}

} // closure: unit iterator*/