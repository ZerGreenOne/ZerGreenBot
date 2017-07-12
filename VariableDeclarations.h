#include <BWTA.h>

#ifndef VARIABLE_DECLARATIONS
#define VARIABLE_DECLARATIONS

#define NUM_BASES 6
#define DRAGOON_WAYPOINTS 10
#define DRAGOON_DEFAULT_POSITION Expansions[1]->getPosition()
#define FAILED_PLACEMENT_POSITION Expansions[0]->getTilePosition()
#define CORSAIR_UNDEFINED_POSITION Position( -1, -1 )
#define SHUTTLE_UNDEFINED_POSITION Position( -1, -1 )
#define ZEALOT_UNDEFINED_POSITION Position( -1, -1 )
#define NUM_NAMES 50

using namespace BWAPI;
using namespace Filter;

std::string name[NUM_NAMES] = { "Sammy", "Scarface", "Dark Tiger the Hungry", "Whoshisface", "Bob", "Daniel", "John Smith", "Harry Potter", "Jim Raynor", "Sarah Kerrigan", "Little Timmy", "The Lich King", "John F Kennedy", "Julia Gillard", "J.J. Thompson", "The Hand", "Mara Jade", "Thomas Edison", "Sean Plott", "Bill Graner", "Sean Bouchard", "Tasteless", "Artosis", "Douglas Adams", "Slartibartfast", "The Lord of the Rings", "Bilbo Baggins", "Gandalf the Grey", "Grant Imahara", "Dark Snout the Proud", "My Dear Uncle Scrooge", "Black Helm the Infested", "Shakespeare the Strong", "Dr. Billy Blue", "The Smartest Man in the World", "My Best Friend", "The pot of the Greed-ness", "Exodia the Forbidden One", "IronBot", "Captain Jack Private", "Lieutenant Kernal [sic] Major", "Snowball", "Napolean", "The Red Baron", "The Biggest Troll of All Time", "That Guy", "Usain Bolt", "Bill Gates", "Gandhi the Bloodthirsty", "Andrew the Pacifist"};

Unitset harvesters;
Unitset builders;
Unitset defenders;
Unitset scouts;
Unitset bases;
Unitset pylons;
Unitset gateways;
Unitset roboFacs;
Unitset stargates;
Unitset forges;
Unitset Zealots;
Unitset Dragoons;
Unitset Shuttles;
Unitset Reavers;
Unitset Corsairs;
Unitset Obses;
Unitset Carriers;
Unitset constructingBuildings;

TilePosition mainBuildPosition;

TilePosition initialPylonBuildPosition = TilePosition(-1, -1);
TilePosition initialOtherBuildPosition[2] = { TilePosition(-1, -1), TilePosition(-1, -1) };

Unit cyber = nullptr;
Unit bay = nullptr;
Unit obs = nullptr;
Unit citadel = nullptr;
Unit fleet = nullptr;
Unit forge[2] = { nullptr, nullptr };

Unit ArmyShuttle = nullptr;
//const TilePosition mainBasePos = Broodwar->self()->getStartLocation();

Unit mainBase;
BWTA::BaseLocation * actualMainBase;

int allSupply = 18;
int numGates = 0;
int numCyber = 0;
int numFac = 0;
int numBay = 0;
int numObs = 0;
int numNexus = 0;
int numForge = 0;
int numShield = 0;
int numCannon = 0;
int numGas = 0;
int numStargate = 0;
int numFleet = 0;
int numCitadel = 0;
int numBayCreate = 0;
int numFacCreate = 0;
int numGasCreate = 0;
int numGatesCreate = 0;
int numCyberCreate = 0;
int numForgeCreate = 0;
int numCannonCreate = 0;
int numStargateCreate = 0;
int numGatesComplete = 0;
int numCyberComplete = 0;
int numFacComplete = 0;
int numBayComplete = 0;
int numForgeComplete = 0;
int numStargateComplete = 0;

int numHydra;
int numMuta;

int numReaverDefend = 0;
int numReaverEngage = 0;
int numReaverHarass = 0;
int numReaverArmy = 0;


int AssimilatorTimer = 200;

int EventTime;

int moreThan55 = 0;

int minsLost;
int enemyMinsLost;
int gasLost;
int enemyGasLost;

int mapNumber = 0;
int closestFFE = 0;

int ZerGreen[2] = { rand() % 3072, rand() % 3072 };

Position Expansion = Position(-1, -1);

Position enemyBase = Position(-1, -1);
Position enemyBaseReal = Position(-1, -1);
Position enemyExpansion1 = Position(-1, -1);
Position enemyExpansion2 = Position(-1, -1);

BWTA::BaseLocation * closestEnemyBase;
std::unordered_set <BWTA::BaseLocation *>  islandBases;

Position regroup = Position(-1, -1);

boolean dRange = false;
boolean gSpeed = false;
boolean legEnhance = false;
boolean carrierCap = false;
boolean disruptionWeb = false;

boolean seenLurker = false;
boolean seenSpire = false;


Position TilePositionConvert(TilePosition t);
TilePosition TilePositionConvert(Position t);

struct PatchVars
{
	int Probes;
};

struct GeyserVars
{
	int Probes;
};

struct NexusVars
{
	//BWTA::BaseLocation loc;
	Unitset Patches;
	Unitset Geysers;
};

struct ProbeVars
{
	Unit Patch;
};

struct BuilderVars
{
	TilePosition BuildPosition;
	UnitType BuildType;
	int MaxTimer = 10000;
	int RecalculateTimer = 0;
	bool Immutable;
};

struct ScoutVars
{
	int ExtraTime;
};

struct ZealotVars
{
	static Unit mainZealot;
	static Unitset armyZealots;
	static Position target;
	Unit Shuttle;
	int attackTime;
	int rank = 0;
	bool fallback;
};

Unit ZealotVars::mainZealot = nullptr;
Unitset ZealotVars::armyZealots;
Position ZealotVars::target = ZEALOT_UNDEFINED_POSITION;

struct DragoonVars
{
	static Unit mainDragoon;
	int timeSinceAttackBegin;
	Position prevPosition[DRAGOON_WAYPOINTS];
	int unstuck;
};

Unit DragoonVars::mainDragoon = nullptr;

struct ShuttleEncounterHistory
{
	Position ShuttlePosition = Position(-1, -1);
	Position EnemyPosition = Position(-1, -1);
};

struct ShuttleVars
{
	Unit Reaver[2];
	Unit Zealot[4];
	Position CurrentDestination;
	bool direction = 0;
	//ShuttleEncounterHistory encounterPosition[2];
	int sideSwap = 0;
};

enum ReaverTactic { ReaverUndefinedTactic = 0, Harass = 1, Defend = 2, Engage = 3, ArmyReaver = 4 };

struct ReaverVars
{
	Unit Shuttle;
	int Scarabs;
	int Attacktime;
	ReaverTactic Tactic;
};

enum ObsType { Army = 0, EnemyArmy = 1, EnemyBase = 2, EnemyExpand = 3, EnemyFont = 4, Base = 5 };

struct ObsVars
{
	ObsType Location;
};

enum CorsairTactic { CorsairUndefinedTactic = 0, Comb = 1, Group = 2, Scout = 3 };

struct CorsairVars
{
	static Unit mainCorsair;
	CorsairTactic Tactic;
	Position targetPoint;
	bool direction = false;
	int sideSwap = 0;
};

Unit CorsairVars::mainCorsair = nullptr;

struct EpicText
{
	std::string Text;
	Position Position;
	int time;

	friend bool operator== (const EpicText &e1, const EpicText &e2)
	{
		return e1.Text == e2.Text;
	}
};

namespace std
{
	template <>
	struct hash < Position >
	{
		size_t operator()(Position const & x) //const noexcept
		{
			return (
				std::hash<int>()((x.x + 51) * 51 + x.y)
				);
		}
	};

	template <>
	struct hash < EpicText >
	{
		size_t operator()(EpicText const & x) //const noexcept
		{
			return (
				std::hash<Position>()(x.Position)
				);
		}
	};

	//template <>
	//struct hash < Unitset >
	//{
	//	size_t operator()(Unitset const & x) //const noexcept
	//	{
	//		return (
	//			std::hash<Unit>()(*x.begin())
	//			);
	//	}
	//};
}

std::unordered_map<Unit, ProbeVars> ProbeMap;
std::unordered_map<Unit, PatchVars> PatchMap;
std::unordered_map<Unit, GeyserVars> GeyserMap;
std::unordered_map<Unit, NexusVars> NexusMap;
std::unordered_map<Unit, BuilderVars> BuilderMap;
std::unordered_map<Unit, ScoutVars> ScoutMap;
std::unordered_map<Unit, ReaverVars> ReaverMap;
std::unordered_map<Unit, ShuttleVars> ShuttleMap;
std::unordered_map<Unit, ZealotVars> ZealotMap;
std::unordered_map<Unit, DragoonVars> DragoonMap;
std::unordered_map<Unit, CorsairVars> CorsairMap;
std::unordered_map<Unit, ObsVars> ObsMap;
std::unordered_map<Unit, int> LatencyMap;

std::map <int, BWTA::BaseLocation *> Expansions;

std::unordered_set < Position > CorsairBeenHere;
std::unordered_map < Position, int > CorsairBeenHereExpiryMap;

std::unordered_set < BWTA::BaseLocation *> visitedBases;

std::unordered_map <int, Unit> ReaverDefenceMap;

std::unordered_map< Position, int > ExpiryMap;
std::unordered_set < Position > TankHits;
std::unordered_set < Position > SpiderMines;

std::unordered_set < Position > EnemyBaseLocations;

std::unordered_map < int, TilePosition > ExpPylPos;
std::unordered_map < int, int > ExpXdir;
std::unordered_map < int, int > ExpYdir;
std::unordered_map < int, TilePosition > ExpGatePos;

//std::unordered_set < Unitset > nearbyGroups;

std::unordered_set < EpicText > EpicTextSet;

#endif