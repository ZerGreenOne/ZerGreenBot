#ifndef FFE_PLACEMENT
#define FFE_PLACEMENT

#include <BWAPI.h>

#define UNSUPPORTED_MAP 0
#define BENZENE 1
#define DESTINATION 2
#define HEARTBREAK_RIDGE 3
#define NEO_MOON_GLAIVE 4
#define TAU_CROSS 5
#define ANDROMEDA 6
#define CIRCUIT_BREAKER 7
#define ELECTRIC_CIRCUIT 8
#define EMPIRE_OF_THE_SUN 9
#define FIGHTING_SPIRIT 10
#define ICARUS 11
#define JADE 12
#define LA_MANCHA 13
#define PYTHON 14
#define ROADRUNNER 15

struct FFEPlacement
{
	BWAPI::TilePosition Pylon;
	BWAPI::TilePosition Forge;
	BWAPI::TilePosition Gateway;
	BWAPI::TilePosition Cannon[2];
	BWAPI::Position Zealot[3];
};

FFEPlacement FFEPositions[4];

int getMapInt()
{
	std::string map = BWAPI::Broodwar->mapFileName();
	if (map == "(2)Benzene.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(118, 50);					// Top Right
		FFEPositions[0].Forge = BWAPI::TilePosition(112, 53);
		FFEPositions[0].Gateway = BWAPI::TilePosition(113, 50);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(115, 53);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(120, 52);
		FFEPositions[0].Zealot[0] = BWAPI::Position(117 * 32, 52 * 32);
		FFEPositions[0].Zealot[1] = BWAPI::Position(118 * 32, 52 * 32);
		FFEPositions[0].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[1].Pylon = BWAPI::TilePosition(14,63);						// Bottom Left
		FFEPositions[1].Forge = BWAPI::TilePosition(8,61);
		FFEPositions[1].Gateway = BWAPI::TilePosition(11,59);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(11,62);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(8,63);
		FFEPositions[1].Zealot[0] = BWAPI::Position(15*32,45*32);
		FFEPositions[1].Zealot[1] = BWAPI::Position(16*32,45*32);
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePositions::None;
		FFEPositions[2].Forge = BWAPI::TilePositions::None;
		FFEPositions[2].Gateway = BWAPI::TilePositions::None;
		FFEPositions[2].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[2].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[2].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePositions::None;
		FFEPositions[3].Forge = BWAPI::TilePositions::None;
		FFEPositions[3].Gateway = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[3].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;

		return BENZENE;
	}
	else if (map == "(2)Destination.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(34,110);			//Bottom Left
		FFEPositions[0].Forge = BWAPI::TilePosition(38,110);
		FFEPositions[0].Gateway = BWAPI::TilePosition(35,107);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(36,110);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(31,110);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePosition(60,18);			//Top right
		FFEPositions[1].Forge = BWAPI::TilePosition(59,22);
		FFEPositions[1].Gateway = BWAPI::TilePosition(56,19);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(60,20);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(64,17);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePositions::None;
		FFEPositions[2].Forge = BWAPI::TilePositions::None;
		FFEPositions[2].Gateway = BWAPI::TilePositions::None;
		FFEPositions[2].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[2].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[2].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePositions::None;
		FFEPositions[3].Forge = BWAPI::TilePositions::None;
		FFEPositions[3].Gateway = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[3].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return DESTINATION;
	}
	else if (map == "(2)Heartbreak Ridge.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(109,28);		//Right
		FFEPositions[0].Forge = BWAPI::TilePosition(104,27);
		FFEPositions[0].Gateway = BWAPI::TilePosition(106,24);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(104,30);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(107,27);
		FFEPositions[0].Zealot[0] = BWAPI::Position((int)(103.5 * 32), 36 * 32);
		FFEPositions[0].Zealot[1] = BWAPI::Position((int)(103.5 * 32), 29 * 32);
		FFEPositions[0].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[1].Pylon = BWAPI::TilePosition(18,66);			//Left
		FFEPositions[1].Forge = BWAPI::TilePosition(18,70);
		FFEPositions[1].Gateway = BWAPI::TilePosition(20,67);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(18,68);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(18,64);
		FFEPositions[1].Zealot[0] = BWAPI::Position(24 * 32, 66 * 32);
		FFEPositions[1].Zealot[1] = BWAPI::Position(24 * 32, 67 * 32);
		FFEPositions[1].Zealot[2] = BWAPI::Position(18 * 32, 72 * 32);

		FFEPositions[2].Pylon = BWAPI::TilePositions::None;
		FFEPositions[2].Forge = BWAPI::TilePositions::None;
		FFEPositions[2].Gateway = BWAPI::TilePositions::None;
		FFEPositions[2].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[2].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[2].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePositions::None;
		FFEPositions[3].Forge = BWAPI::TilePositions::None;
		FFEPositions[3].Gateway = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[3].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return HEARTBREAK_RIDGE;
	}
	else if (map == "(3)Neo Moon Glaive.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(38,107);		//Bottom Left
		FFEPositions[0].Forge = BWAPI::TilePosition(35,107);
		FFEPositions[0].Gateway = BWAPI::TilePosition(32,104);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(33,107);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(37,109);
		FFEPositions[0].Zealot[0] = BWAPI::Position(32 * 32, (int)(102.5 * 32));
		FFEPositions[0].Zealot[1] = BWAPI::Position(32 * 32, (int)(103.5 * 32));
		FFEPositions[0].Zealot[2] = BWAPI::Position((int)(37.5 * 32), 107 * 32);

		FFEPositions[1].Pylon = BWAPI::TilePosition(110,66);		//Bottom Right
		FFEPositions[1].Forge = BWAPI::TilePosition(112,65);
		FFEPositions[1].Gateway = BWAPI::TilePosition(114,62);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(112,67);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(114,67);
		FFEPositions[1].Zealot[0] = BWAPI::Position((int)(109.5 * 32), 68 * 32);
		FFEPositions[1].Zealot[1] = BWAPI::Position(112*32,67*32);
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePosition(38,24);			//Top
		FFEPositions[2].Forge = BWAPI::TilePosition(38,28);
		FFEPositions[2].Gateway = BWAPI::TilePosition(40,25);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(38,26);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(42,23);
		FFEPositions[2].Zealot[0] = BWAPI::Position(37 * 32, (int)(29.5 * 32));
		FFEPositions[2].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePositions::None;
		FFEPositions[3].Forge = BWAPI::TilePositions::None;
		FFEPositions[3].Gateway = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[3].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return NEO_MOON_GLAIVE;
	}
	else if (map == "(3)Tau Cross.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(102,43); // Top Right
		FFEPositions[0].Forge = BWAPI::TilePosition(104,44);
		FFEPositions[0].Gateway = BWAPI::TilePosition(108,43);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(105,42);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(100,42);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePosition(24,15);		//Top Left
		FFEPositions[1].Forge = BWAPI::TilePosition(26,19);
		FFEPositions[1].Gateway = BWAPI::TilePosition(26,16);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(24,17);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(22,17);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePosition(55,109);		// Bottom Right
		FFEPositions[2].Forge = BWAPI::TilePosition(50,108);
		FFEPositions[2].Gateway = BWAPI::TilePosition(52,105);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(53,108);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(56,106);
		FFEPositions[2].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[3].Pylon = BWAPI::TilePositions::None;
		FFEPositions[3].Forge = BWAPI::TilePositions::None;
		FFEPositions[3].Gateway = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[0] = BWAPI::TilePositions::None;
		FFEPositions[3].Cannon[1] = BWAPI::TilePositions::None;
		FFEPositions[3].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return TAU_CROSS;
	}
	else if (map == "(4)Andromeda.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(98,34);			//Top Right
		FFEPositions[0].Forge = BWAPI::TilePosition(94,33);
		FFEPositions[0].Gateway = BWAPI::TilePosition(91,30);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(95,31);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(100,33);
		FFEPositions[0].Zealot[0] = BWAPI::Position(98*32,34*32);
		FFEPositions[0].Zealot[1] = BWAPI::Position(97*32,34*32);
		FFEPositions[0].Zealot[2] = BWAPI::Position((int)(90.5*32),(int)(29.5*32));

		FFEPositions[1].Pylon = BWAPI::TilePosition(26,32);			//Top Left
		FFEPositions[1].Forge = BWAPI::TilePosition(28,31);
		FFEPositions[1].Gateway = BWAPI::TilePosition(30,28);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(26,30);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(28,29);
		FFEPositions[1].Zealot[0] = BWAPI::Position(26*32,34*32);
		FFEPositions[1].Zealot[1] = BWAPI::Position(38*32,33*32);
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePosition(26,104);		//Bottom Left
		FFEPositions[2].Forge = BWAPI::TilePosition(25,102);
		FFEPositions[2].Gateway = BWAPI::TilePosition(22,99);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(23,102);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(28,103);
		FFEPositions[2].Zealot[0] = BWAPI::Position(22*32,98*32);
		FFEPositions[2].Zealot[1] = BWAPI::Position((int)(26.5 * 32), (int)(103.5 * 32));
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePosition(104,98);		//Bottom Right
		FFEPositions[3].Forge = BWAPI::TilePosition(98,101);
		FFEPositions[3].Gateway = BWAPI::TilePosition(99,98);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(101,101);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(103,102);
		FFEPositions[3].Zealot[0] = BWAPI::Position((int)(103.5 * 32), (int)(98.5 * 32));
		FFEPositions[3].Zealot[1] = BWAPI::Position(106*32,99*32);
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return ANDROMEDA;
	}
	else if (map == "(4)Circuit Breaker.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(108,96);		//Bottom Right
		FFEPositions[0].Forge = BWAPI::TilePosition(107,94);
		FFEPositions[0].Gateway = BWAPI::TilePosition(106,91);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(110,92);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(115,94);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePosition(109,37);				//Top Right
		FFEPositions[1].Forge = BWAPI::TilePosition(108,35);
		FFEPositions[1].Gateway = BWAPI::TilePosition(107,32);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(111,33);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(116,35);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePosition(17,37);
		FFEPositions[2].Forge = BWAPI::TilePosition(17,35);
		FFEPositions[2].Gateway = BWAPI::TilePosition(17,32);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(15,33);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(11,33);
		FFEPositions[2].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[3].Pylon = BWAPI::TilePosition(18,96);
		FFEPositions[3].Forge = BWAPI::TilePosition(18,94);
		FFEPositions[3].Gateway = BWAPI::TilePosition(18,91);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(16,92);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(12,92);
		FFEPositions[3].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::Unknown;
		return CIRCUIT_BREAKER;
	}
	else if (map == "(4)Electric Circuit.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(11,91);			//Bottom Left
		FFEPositions[0].Forge = BWAPI::TilePosition(15,89);
		FFEPositions[0].Gateway = BWAPI::TilePosition(12,86);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(10,87);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(13,89);
		FFEPositions[0].Zealot[0] = BWAPI::Position(12*32,86*32);
		FFEPositions[0].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[1].Pylon = BWAPI::TilePosition(36,12);			//Top Left
		FFEPositions[1].Forge = BWAPI::TilePosition(38,13);
		FFEPositions[1].Gateway = BWAPI::TilePosition(40,10);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(38,11);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(38,9);
		FFEPositions[1].Zealot[0] = BWAPI::Position(44*32,10*32);
		FFEPositions[1].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePosition(116,37);		//Top Right
		FFEPositions[2].Forge = BWAPI::TilePosition(115,41);
		FFEPositions[2].Gateway = BWAPI::TilePosition(112,38);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(116,39);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(114,36);
		FFEPositions[2].Zealot[0] = BWAPI::Position(110*32,38*32);
		FFEPositions[2].Zealot[1] = BWAPI::Position(111*32,38*32);
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePosition(89,118);		//Bottom Right
		FFEPositions[3].Forge = BWAPI::TilePosition(84,118);
		FFEPositions[3].Gateway = BWAPI::TilePosition(86,115);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(87,118);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(90,116);
		FFEPositions[3].Zealot[0] = BWAPI::Position(89*32, 114*32);
		FFEPositions[3].Zealot[1] = BWAPI::Position(89*32, 115*32);
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return ELECTRIC_CIRCUIT;
	}
	else if (map == "(4)Empire of the Sun.scm")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(93,12);			//Top Right
		FFEPositions[0].Forge = BWAPI::TilePosition(88,15);		// Originally (89,15)
		FFEPositions[0].Gateway = BWAPI::TilePosition(91,15);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(91,13);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(89,13);
		FFEPositions[0].Zealot[0] = BWAPI::Position(88*32,16*32);
		FFEPositions[0].Zealot[1] = BWAPI::Position((int)(90.5 * 32), 15 * 32);
		FFEPositions[0].Zealot[2] = BWAPI::Position(97*32,14*32);

		FFEPositions[1].Pylon = BWAPI::TilePosition(93,110);						//Bottom Right
		FFEPositions[1].Forge = BWAPI::TilePosition(90,110);
		FFEPositions[1].Gateway = BWAPI::TilePosition(88,112);  //
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(92,113);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(94,113);
		FFEPositions[1].Zealot[0] = BWAPI::Position(95*32,113*32);
		FFEPositions[1].Zealot[1] = BWAPI::Position((int)(92.5*32),112*32);
		FFEPositions[1].Zealot[2] = BWAPI::Position(96*32,113*32);

		FFEPositions[2].Pylon = BWAPI::TilePosition(32,112);						//Bottom Left
		FFEPositions[2].Forge = BWAPI::TilePosition(34,111);
		FFEPositions[2].Gateway = BWAPI::TilePosition(36,113); // Used to be (37,114)
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(34,114); // Used to be (35,114)
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(32,114); // Used to be (33,114)
		FFEPositions[2].Zealot[0] = BWAPI::Position((int)(31.5*32),111*32);
		FFEPositions[2].Zealot[1] = BWAPI::Position((int)(33.5*32),111*32);
		FFEPositions[2].Zealot[2] = BWAPI::Position(40*32,114*32);

		FFEPositions[3].Pylon = BWAPI::TilePosition(33,16);							//Top Left
		FFEPositions[3].Forge = BWAPI::TilePosition(37,14); // Used to be (36,13)
		FFEPositions[3].Gateway = BWAPI::TilePosition(35,16);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(33,14);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(35,14);
		FFEPositions[3].Zealot[0] = BWAPI::Position((int)(39.5*32),14*32);
		FFEPositions[3].Zealot[1] = BWAPI::Position((int)(31.5*32),16*32);
		FFEPositions[3].Zealot[2] = BWAPI::Position((int)(33.5*32),16*32);
		return EMPIRE_OF_THE_SUN;
	}
	else if (map == "(4)Fighting Spirit.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(30,110);	//Bottom Left
		FFEPositions[0].Forge = BWAPI::TilePosition(32,109);
		FFEPositions[0].Gateway = BWAPI::TilePosition(32,106);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(32,109);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(34,112);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePosition(16,34);		//Top Left
		FFEPositions[1].Forge = BWAPI::TilePosition(19,37);
		FFEPositions[1].Gateway = BWAPI::TilePosition(21,34);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(19,35);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(16,36);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePosition(94,16);		//Top Right
		FFEPositions[2].Forge = BWAPI::TilePosition(93,20);
		FFEPositions[2].Gateway = BWAPI::TilePosition(90,17);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(94,18);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(92,14);
		FFEPositions[2].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[3].Pylon = BWAPI::TilePosition(110,93);		//Bottom Right
		FFEPositions[3].Forge = BWAPI::TilePosition(104,93);
		FFEPositions[3].Gateway = BWAPI::TilePosition(105,90);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(107,93);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(110,91);
		FFEPositions[3].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::Unknown;
		return FIGHTING_SPIRIT;
	}
	else if (map == "(4)Icarus.scm")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(106,76);			//Right
		FFEPositions[0].Forge = BWAPI::TilePosition(108,75);
		FFEPositions[0].Gateway = BWAPI::TilePosition(108,72);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(112,73);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(111,75);
		FFEPositions[0].Zealot[0] = BWAPI::Position((int)(111.5 * 32), (int)(71.5 * 32));
		FFEPositions[0].Zealot[1] = BWAPI::Position(108*32,77*32);
		FFEPositions[0].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[1].Pylon = BWAPI::TilePosition(80,16);				//Top
		FFEPositions[1].Forge = BWAPI::TilePosition(74,14);
		FFEPositions[1].Gateway = BWAPI::TilePosition(76,11);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(81,13);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(74,12);
		FFEPositions[1].Zealot[0] = BWAPI::Position(80 * 32, (int)(14.5*32));
		FFEPositions[1].Zealot[1] = BWAPI::Position((int)(79.5 * 32), (int)(12.5*32));
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePosition(10,54);				//Left
		FFEPositions[2].Forge = BWAPI::TilePosition(12,53);
		FFEPositions[2].Gateway = BWAPI::TilePosition(14,50);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(10,52);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(12,51);
		FFEPositions[2].Zealot[0] = BWAPI::Position(10 * 32, (int)(55.5 * 32));
		FFEPositions[2].Zealot[1] = BWAPI::Position((int)(11.5 * 32), (int)(54.5*32));
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePosition(46,113);			//Bottom
		FFEPositions[3].Forge = BWAPI::TilePosition(48,113);
		FFEPositions[3].Gateway = BWAPI::TilePosition(50,110);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(51,113);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(47,115);
		FFEPositions[3].Zealot[0] = BWAPI::Position((int)(45.5 * 32), (int)(112.5*32));
		FFEPositions[3].Zealot[1] = BWAPI::Position((int)(47.5 * 32), (int)(114.5*32));
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return ICARUS;
	}
	else if (map == "(4)Jade.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(111,39);			//Top Right
		FFEPositions[0].Forge = BWAPI::TilePosition(106,41);
		FFEPositions[0].Gateway = BWAPI::TilePosition(105,38);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(109,39);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(109,41);
		FFEPositions[0].Zealot[0] = BWAPI::Position(107 * 32, (int)(37.5 * 32));
		FFEPositions[0].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[1].Pylon = BWAPI::TilePosition(89,116);			//Bottom Right
		FFEPositions[1].Forge = BWAPI::TilePosition(86,114);
		FFEPositions[1].Gateway = BWAPI::TilePosition(88,111);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(87,116);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(91,116);
		FFEPositions[1].Zealot[0] = BWAPI::Position(20*32,84*32);
		FFEPositions[1].Zealot[1] = BWAPI::Position(20*32,85*32);
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePosition(14,88);				//Bottom Left
		FFEPositions[2].Forge = BWAPI::TilePosition(18,89);
		FFEPositions[2].Gateway = BWAPI::TilePosition(18,86);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(16, 88);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(16, 90);
		FFEPositions[2].Zealot[0] = BWAPI::Position(20*32,84*32);
		FFEPositions[2].Zealot[1] = BWAPI::Position(20*32,85*32);
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePosition(41,12);				//Top Left
		FFEPositions[3].Forge = BWAPI::TilePosition(38,16);
		FFEPositions[3].Gateway = BWAPI::TilePosition(35,13);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(39,14);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(35,11);
		FFEPositions[3].Zealot[0] = BWAPI::Position(34*32,15*32);
		FFEPositions[3].Zealot[1] = BWAPI::Position(35*32,15*32);
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return JADE;
	}
	else if (map == "(4)La Mancha1.1.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(91,14);		//Top Right
		FFEPositions[0].Forge = BWAPI::TilePosition(88,18);
		FFEPositions[0].Gateway = BWAPI::TilePosition(92,18);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(88,16);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(93,14);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePosition(113,91);		//Bottom Right
		FFEPositions[1].Forge = BWAPI::TilePosition(108,92);
		FFEPositions[1].Gateway = BWAPI::TilePosition(107,89);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(111,92);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(113,89);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePosition(35,112);		//Bottom Left
		FFEPositions[2].Forge = BWAPI::TilePosition(32,110);
		FFEPositions[2].Gateway = BWAPI::TilePosition(36,109);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(32,112);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(40,112);
		FFEPositions[2].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[3].Pylon = BWAPI::TilePosition(17,39);		//Top Left
		FFEPositions[3].Forge = BWAPI::TilePosition(19,38);
		FFEPositions[3].Gateway = BWAPI::TilePosition(19,35);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(17,37);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(12,38);
		FFEPositions[3].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::Unknown;
		return LA_MANCHA;
	}
	else if (map == "(4)Python.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(73,117);				//Bottom
		FFEPositions[0].Forge = BWAPI::TilePosition(66,116);
		FFEPositions[0].Gateway = BWAPI::TilePosition(68,113);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(69,116);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(73,119);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[1].Pylon = BWAPI::TilePosition(114,62);				//Right
		FFEPositions[1].Forge = BWAPI::TilePosition(111,62);
		FFEPositions[1].Gateway = BWAPI::TilePosition(108,59);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(112,60);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(117,64);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[2].Pylon = BWAPI::TilePosition(54,11);					//Top
		FFEPositions[2].Forge = BWAPI::TilePosition(59,13);
		FFEPositions[2].Gateway = BWAPI::TilePosition(56,10);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(60,11);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(50,9);
		FFEPositions[2].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::None;

		FFEPositions[3].Pylon = BWAPI::TilePosition(8,63);				//Left
		FFEPositions[3].Forge = BWAPI::TilePosition(12,65);
		FFEPositions[3].Gateway = BWAPI::TilePosition(11,62);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(9,60);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(11,59);
		FFEPositions[3].Zealot[0] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::None;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::None;
		return PYTHON;
	}
	else if (map == "(4)Roadrunner.scx")
	{
		FFEPositions[0].Pylon = BWAPI::TilePosition(52,16);		// Top
		FFEPositions[0].Forge = BWAPI::TilePosition(55,16);
		FFEPositions[0].Gateway = BWAPI::TilePosition(56,13);
		FFEPositions[0].Cannon[0] = BWAPI::TilePosition(52,14);
		FFEPositions[0].Cannon[1] = BWAPI::TilePosition(52,12);
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePosition(115,59);		//Right
		FFEPositions[1].Forge = BWAPI::TilePosition(112,60);
		FFEPositions[1].Gateway = BWAPI::TilePosition(109,57);
		FFEPositions[1].Cannon[0] = BWAPI::TilePosition(113,58);
		FFEPositions[1].Cannon[1] = BWAPI::TilePosition(117,58);
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePosition(68,113);		//Bottom
		FFEPositions[2].Forge = BWAPI::TilePosition(71,114);
		FFEPositions[2].Gateway = BWAPI::TilePosition(72,111);
		FFEPositions[2].Cannon[0] = BWAPI::TilePosition(66,115);
		FFEPositions[2].Cannon[1] = BWAPI::TilePosition(74,114);
		FFEPositions[2].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[3].Pylon = BWAPI::TilePosition(18,70);		//Left
		FFEPositions[3].Forge = BWAPI::TilePosition(17,68);
		FFEPositions[3].Gateway = BWAPI::TilePosition(15,65);
		FFEPositions[3].Cannon[0] = BWAPI::TilePosition(15,68);
		FFEPositions[3].Cannon[1] = BWAPI::TilePosition(13,67);
		FFEPositions[3].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::Unknown;
		return ROADRUNNER;
	}
	else
	{
		BWAPI::Broodwar << "Map not recognised - This is probably not a map from the SSCAIT map pool (as of 28/9/2016)" << std::endl; // I know, I am probably ancient by now
		BWAPI::Broodwar << "The map is " << BWAPI::Broodwar->mapFileName() << std::endl;

		FFEPositions[0].Pylon = BWAPI::TilePositions::Unknown;
		FFEPositions[0].Forge = BWAPI::TilePositions::Unknown;
		FFEPositions[0].Gateway = BWAPI::TilePositions::Unknown;
		FFEPositions[0].Cannon[0] = BWAPI::TilePositions::Unknown;
		FFEPositions[0].Cannon[1] = BWAPI::TilePositions::Unknown;
		FFEPositions[0].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[0].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[1].Pylon = BWAPI::TilePositions::Unknown;
		FFEPositions[1].Forge = BWAPI::TilePositions::Unknown;
		FFEPositions[1].Gateway = BWAPI::TilePositions::Unknown;
		FFEPositions[1].Cannon[0] = BWAPI::TilePositions::Unknown;
		FFEPositions[1].Cannon[1] = BWAPI::TilePositions::Unknown;
		FFEPositions[1].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[1].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[2].Pylon = BWAPI::TilePositions::Unknown;
		FFEPositions[2].Forge = BWAPI::TilePositions::Unknown;
		FFEPositions[2].Gateway = BWAPI::TilePositions::Unknown;
		FFEPositions[2].Cannon[0] = BWAPI::TilePositions::Unknown;
		FFEPositions[2].Cannon[1] = BWAPI::TilePositions::Unknown;
		FFEPositions[2].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[2].Zealot[2] = BWAPI::Positions::Unknown;

		FFEPositions[3].Pylon = BWAPI::TilePositions::Unknown;
		FFEPositions[3].Forge = BWAPI::TilePositions::Unknown;
		FFEPositions[3].Gateway = BWAPI::TilePositions::Unknown;
		FFEPositions[3].Cannon[0] = BWAPI::TilePositions::Unknown;
		FFEPositions[3].Cannon[1] = BWAPI::TilePositions::Unknown;
		FFEPositions[3].Zealot[0] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[1] = BWAPI::Positions::Unknown;
		FFEPositions[3].Zealot[2] = BWAPI::Positions::Unknown;
		return UNSUPPORTED_MAP;
	}
}

#endif