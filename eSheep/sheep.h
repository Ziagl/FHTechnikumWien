#pragma once

#include "globals.h"
#include "position2d.h"
#include "gameobject.h"
#include "map.h"
#include "astar.h"
#include "log.h"

class Sheep:public GameObject
{
public:
	Sheep(clan::Canvas &canvas, clan::Image* tile, Position2D &pos, int number):GameObject(canvas,tile,pos){
		_sheepID=number;
		_reachedGoal=true;
		_sheepAlive=true;
		_hasGoal=false;
		_sheepTile=0;
		_sheepAge=0;
		_desireHunger=0;
		_desireSocial=0;
		_desireHygiene=0;
		_desireSexuality=0;
		_desireCombat=0;
		_path=0;
		_sheepIntention=IDLE;

		init(irand(SHEEP_MIN, SHEEP_MAX), irand(0,1)==0?true:false,irand(SHEEP_MIN, SHEEP_MAX),irand(SHEEP_MIN, SHEEP_MAX));
	};
	Sheep(clan::Canvas &canvas, clan::Image* tile, Position2D &pos, int number, short size, short mojo, short aggression):GameObject(canvas,tile,pos){
		_sheepID=number;
		_reachedGoal=true;
		_sheepAlive=true;
		_hasGoal=false;
		_sheepTile=0;
		_sheepAge=0;
		_desireHunger=0;
		_desireSocial=0;
		_desireHygiene=0;
		_desireSexuality=0;
		_desireCombat=0;
		_path=0;
		_sheepIntention=IDLE;

		init(size, irand(0,1)==0?true:false, aggression, mojo);
	};
	void move(Map* map);
	void compute(Map* map, std::vector<Sheep> &sheeps);
	void render(Map* map);
	
//private:
	void init(short size, bool gender, short agression, short mojo);
	void findNewIntention(SheepIntention &intention);
	void findNext(SheepIntention, Map* map, std::vector<Sheep> &sheeps);
	void generateNewSheep(int id, Map* map, std::vector<Sheep> &sheeps);
	inline void nextAge(Map* map);									// sheep gets older
	inline void updateDesires(Map* map, std::vector<Sheep> &sheeps);// update desires
	inline void findPath(Map* map, std::vector<Sheep> &sheeps);		// search a new path
	inline void lowerDesires(Map* map, std::vector<Sheep> &sheeps);
	inline void checkDeath(Map* map);								// check all possible deaths
	inline void combineParents(int id, short &size, short &mojo, short &aggression, std::vector<Sheep> &sheeps);
	inline int wrap_around(int a, int b);
	int irand(int a, int b);										// randomize

	std::vector<Sheep> mychildren;

	// variables
	int _sheepID;				// id of sheep
	// path planning
	SheepIntention _sheepIntention;	// what sheep currently wants to do
	bool _reachedGoal;				// if sheep on the way
	std::vector<Position2D> _tempPos;
	bool _hasGoal;
	Position2D _goal;
	Position2D* _path;
	int _pathPos;
	// intention
	// attributes
	bool _sheepAlive;		// is sheep still alive?
	short _sheepTile;		// 0-2 state (child, adult, old)
	short _sheepAge;		// 0-255 age of sheep (child, adult, old)
	short _sheepSize;		// 0-255 size
	bool _sheepGender;		// male or female
	short _sheepAgression;	// 0-255 aggression
	short _sheepMojo;		// 0-255 mating value
	// desires
	short _desireHunger;	//0-255	(255=starved)
	short _desireSocial;	//0-255 (255=lonely)
	short _desireHygiene;	//0-255
	short _desireSexuality;	//0-255
	short _desireCombat;	//0-255
};
