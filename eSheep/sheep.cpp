#include "sheep.h"

LOG_USE();

void Sheep::move(Map* map)
{
	_pathPos++;
	Position2D newPos = _path[_pathPos];
	if(map->isValid(newPos)!=0)
	{
		map->resetSheep(_pos);		// update sheep position
		_pos = newPos;
		map->setSheep(_pos);
	}
	else
	{
		// we need to find another path
		// or a nother intention
		if(_path!=0)
		{
			delete[] _path;
			_path=0;
			_reachedGoal=false;
			_hasGoal=false;
		}
		_sheepIntention=IDLE;
	}
}

void Sheep::compute(Map* map, std::vector<Sheep> &sheeps)
{
	if(_sheepAlive)		// only if sheep is still alive
	{
		char s[200];

		// all variables that are affected by time are changed
		nextAge(map);			// sheep gets older (changes icon or dies)
		// desires
		updateDesires(map,sheeps);
		
		// update desire and set new intention
		if(_sheepIntention==IDLE)
		{
			// only it sheep is in idle mode -> find new intention
			SheepIntention intention=IDLE;
			findNewIntention(intention);
			_sheepIntention=intention;
			// logging
			switch(intention)
			{
			case HUNGER: sprintf(s,"Schaf%d; hat Hunger",_sheepID); break;
			case SOZIAL: sprintf(s,"Schaf%d; braucht Kontakt",_sheepID); break;
			case HYGIENE: sprintf(s,"Schaf%d; muss auf die Toilette",_sheepID); break;
			case SEXUALITY: sprintf(s,"Schaf%d; sucht einen Partner",_sheepID); break;
			case COMBAT: sprintf(s,"Schaf%d; sucht Streit",_sheepID); break;
			}
			LOG_SHEEP(s);
			
			findPath(map, sheeps);
		}

		// if there is a path and sheep does not reached goal
		if(!_reachedGoal && _hasGoal)
		{
			// move sheep
			move(map);
		}
		else
		{
			// sheep reached goal -> clear desire
			if(_reachedGoal)
			{
				lowerDesires(map, sheeps);
			}
		}

		// check if sheep needs to die
		checkDeath(map);
	}	
}

void Sheep::init(short size, bool gender, short agression, short mojo)
{
	_sheepSize = size;
	_sheepGender = gender;
	_sheepAgression = agression;
	_sheepMojo = mojo;

	char s[200];
	sprintf(s,"Schaf%d; erstellt;%d;%d;%d",_sheepID,_sheepSize,_sheepAgression,_sheepMojo);
	LOG_SHEEP(s);
}

void Sheep::render(Map* map)
{
	if(_sheepAlive)		// only if sheep is still alive
	{
		// set tile (based on age)
		switch(_sheepTile)
		{
		case 0: if(_sheepGender) setTile(map->_tilesMap["sheep0"]); else setTile(map->_tilesMap["sheep0w"]); break;	// child
		case 1: if(_sheepGender) setTile(map->_tilesMap["sheep1"]); else setTile(map->_tilesMap["sheep1w"]); break;	// adult
		case 2: if(_sheepGender) setTile(map->_tilesMap["sheep2"]); else setTile(map->_tilesMap["sheep2w"]); break;	// old
		}
		// render
		GameObject::render();
		// debugoutput
		float x_koord;
		float y_koord;
		float scaleFactor;
#if SMALL==1
		scaleFactor=TILES_SCALE_FACTOR_SMALL;
#else
		scaleFactor=TILES_SCALE_FACTOR;
#endif
//#if DEBUG == 1
		// draw text
		x_koord = static_cast<float>((_pos._x*scaleFactor*TILES_SIZE_X)+((_pos._y%2==0)?TILES_OFFSET_X*scaleFactor:0));
		y_koord = static_cast<float>((_pos._y*scaleFactor*TILES_OFFSET_Y));
		// print all desires
		std::stringstream str;
		str<<_desireHunger;
		map->_font->draw_text(_canvas,x_koord+30.0f,y_koord+15.0f,str.str(),clan::Colorf::green);
		std::stringstream str1;
		str1<<_desireHygiene;
		map->_font->draw_text(_canvas,x_koord+60.0f,y_koord+25.0f,str1.str(),clan::Colorf::brown);
		std::stringstream str2;
		str2<<_desireSocial;
		map->_font->draw_text(_canvas,x_koord+60.0f,y_koord+55.0f,str2.str(),clan::Colorf::blue);
		std::stringstream str3;
		str3<<_desireSexuality;
		map->_font->draw_text(_canvas,x_koord+30.0f,y_koord+65.0f,str3.str(),clan::Colorf::black);
		std::stringstream str4;
		str4<<_desireCombat;
		map->_font->draw_text(_canvas,x_koord+10.0f,y_koord+55.0f,str4.str(),clan::Colorf::red);
		std::stringstream str5;
		str5<<_sheepID;
		map->_font->draw_text(_canvas,x_koord+10.0f,y_koord+45.0f,str5.str(),clan::Colorf::darkviolet);
//#endif
	}
}

void Sheep::generateNewSheep(int id, Map* map, std::vector<Sheep> &sheeps)
{
	char s[200];

	// create new sheep based on parents genome
	// 1. find empty field
	Position2D p;
	bool field=false;
	// neighbours of mother sheep
	for(int i=0; i<NEIGHBOURS; ++i)
	{
		Position2D neighbour = _pos.neighbour(i);
		if(map->isValid(neighbour))
		{
			p=neighbour;
			field=true;
			i=NEIGHBOURS;
		}
	}
	// no empty field -> search father sheep neighbours
	if(field==false)
		for(int i=0; i<NEIGHBOURS; ++i)
		{
			Position2D neighbour = sheeps[id].getPosition().neighbour(i);
			if(map->isValid(neighbour))
			{
				p=neighbour;
				field=true;
				i=NEIGHBOURS;
			}
		}
	// if empty field found -> create child sheep
	if(field)
	{
		int number = (int) sheeps.size();
		short size = 0, mojo = 0, aggression = 0;
		combineParents(id, size, mojo, aggression,sheeps);		// combine parents genomes
 		mychildren.push_back(Sheep(_canvas, map->_tilesMap["sheep0"], p, number, size, mojo, aggression));
		sprintf(s, "Schaf%d; und Schaf%d bekommen Nachwuchs",_sheepID,id);
		LOG_SHEEP(s);
	}
	else
	{
		sprintf(s, "Schaf%d; kein Platz für neues Schaf",_sheepID);
		LOG_SHEEP(s);
	}
}

void Sheep::findNext(SheepIntention s, Map* map, std::vector<Sheep> &sheeps)
{
	_tempPos.clear();
	int min_cost=100;
	
	switch(s)
	{
	case HUNGER:
		// find next plant
		for(int x=0;x<LEVEL_WIDTH;x++)
		{
			for(int y=0;y<LEVEL_HEIGHT;y++)
			{
				if(map->/*_fields[x][y].getValue()*/getValue(x,y)>1)
				{
					int cost = abs(x-_pos._x) + abs(y-_pos._y);
					// new min value -> create new list
					if(cost<min_cost)
					{
						min_cost=cost;
						_tempPos.clear();
						_tempPos.push_back(Position2D(x,y));

					}
					// same min vale -> add to list
					if(cost==min_cost)
					{
						_tempPos.push_back(Position2D(x,y));
					}
				}
			}
		}
		break;
	case HYGIENE:
		// find next empty field
		for(int x=0;x<LEVEL_WIDTH;x++)
		{
			for(int y=0;y<LEVEL_HEIGHT;y++)
			{
				if(map->/*_fields[x][y].getValue()*/getValue(x,y)==1)
				{
					int cost = abs(x-_pos._x) + abs(y-_pos._y);
					// new min value -> create new list
					if(cost<min_cost)
					{
						min_cost=cost;
						_tempPos.clear();
						_tempPos.push_back(Position2D(x,y));

					}
					// same min vale -> add to list
					if(cost==min_cost)
					{
						_tempPos.push_back(Position2D(x,y));
					}
				}
			}
		}
		break;
	case SOZIAL:
		// search all other sheeps
		for(int i=0; i<sheeps.size(); ++i)
		{
			// do not find this sheep
			if(i!=_sheepID)
			{
				// only search sheeps that are doing nothing
				if(sheeps[i]._sheepIntention==IDLE && sheeps[i]._sheepAlive)
				{
					Position2D spos = sheeps[i].getPosition();
					int cost = abs(spos._x-_pos._x) + abs(spos._y-_pos._y);
					// new min value -> create new list
					if(cost<min_cost)
					{
						min_cost=cost;
						_tempPos.clear();
						_tempPos.push_back(Position2D(spos._x,spos._y));

					}
					// same min vale -> add to list
					if(cost==min_cost)
					{
						_tempPos.push_back(Position2D(spos._x,spos._y));
					}
				}
			}
		}
		break;
	case SEXUALITY:
		// search all other sheeps
		for(int i=0; i<sheeps.size(); ++i)
		{
			// do not find this sheep
			// and only sheep of other sex
			if(i!=_sheepID && sheeps[i]._sheepGender != _sheepGender && sheeps[i]._sheepAlive)
			{
				Position2D spos = sheeps[i].getPosition();
				int cost = abs(spos._x-_pos._x) + abs(spos._y-_pos._y);
				// new min value -> create new list
				if(cost<min_cost)
				{
					min_cost=cost;
					_tempPos.clear();
					_tempPos.push_back(Position2D(spos._x,spos._y));

				}
				// same min vale -> add to list
				if(cost==min_cost)
				{
					_tempPos.push_back(Position2D(spos._x,spos._y));
				}
			}
		}
		break;
	case COMBAT:
		// search all other sheeps
		for(int i=0; i<sheeps.size(); ++i)
		{
			// do not find this sheep
			// and only sheep of same sex
			if(i!=_sheepID && sheeps[i]._sheepGender == _sheepGender && sheeps[i]._sheepAlive)
			{
				Position2D spos = sheeps[i].getPosition();
				int cost = abs(spos._x-_pos._x) + abs(spos._y-_pos._y);
				// new min value -> create new list
				if(cost<min_cost)
				{
					min_cost=cost;
					_tempPos.clear();
					_tempPos.push_back(Position2D(spos._x,spos._y));

				}
				// same min vale -> add to list
				if(cost==min_cost)
				{
					_tempPos.push_back(Position2D(spos._x,spos._y));
				}
			}
		}
		break;
	}
}

void Sheep::findNewIntention(SheepIntention &intention)
{
	short maximalWert=0;

	if(_desireHunger>maximalWert)
	{
		maximalWert=_desireHunger;
		intention=HUNGER;
	}
	if(_desireSocial>maximalWert)
	{
		maximalWert=_desireSocial;
		intention=SOZIAL;
	}
	if(_desireHygiene>maximalWert)
	{
		maximalWert=_desireHygiene;
		intention=HYGIENE;
	}
	if(_desireSexuality>maximalWert && maximalWert<SURVIVAL_EMERGENCY && _desireSexuality>NEEDED_SEXUALITY_FOR_CHILD)
	{
		maximalWert=_desireSexuality;
		intention=SEXUALITY;
	}
	if(_desireCombat>maximalWert && maximalWert<SURVIVAL_EMERGENCY)
	{
		maximalWert=_desireCombat;
		intention=COMBAT;
	}
}

void Sheep::nextAge(Map* map)
{
	char s[200];

	_sheepAge++;				// sheep gets older
	if(_sheepAge==SHEEP_CHILDHOOD)
	{
		_sheepTile=1;			// sheep turns adult
		sprintf(s,"Schaf%d; ist erwachsen",_sheepID);
		LOG_SHEEP(s);
	}
	if(_sheepAge==SHEEP_ADULTHOOD)
	{
		_sheepTile=2;			// sheep turns old
		sprintf(s,"Schaf%d; ist alt",_sheepID);
		LOG_SHEEP(s);
	}
	// sheep can die if it is too old
	if(_sheepAge>SHEEP_ADULTHOOD && _sheepAlive)
	{
		int chance = irand(_sheepAge,SHEEP_SENIOR);
		if(chance == SHEEP_SENIOR)
		{
			_sheepAlive=false;
			map->resetSheep(_pos);
			sprintf(s,"Schaf%d; stirbt eines natürlichen Todes",_sheepID);
			LOG_SHEEP(s);
		}
	}
}

void Sheep::updateDesires(Map* map, std::vector<Sheep> &sheeps)
{
	char s[200];

	// change based on current values
	_desireHunger+=static_cast<short>(((_sheepSize*1.0f/SHEEP_MAX) * DESIRE_HUNGER) + 1);	// faster growth for big sheeps
	_desireSocial+=static_cast<short>((1.0-_sheepAge/SHEEP_MAX) * DESIRE_SOZIAL);			// more important for younger sheeps
	_desireHygiene+=static_cast<short>((1.0-_sheepSize/SHEEP_MAX) * DESIRE_HYGIENE);		// more important for younger sheeps
	_desireCombat+=static_cast<short>((_sheepAgression*1.0f/SHEEP_MAX) * DESIRE_COMBAT);
	if(_sheepAge>SHEEP_CHILDHOOD)	// only for adult sheeps and faster for younger
		_desireSexuality+=static_cast<short>((_sheepMojo/SHEEP_MAX * DESIRE_SEXUALITY) + (1.0-(_sheepAge/SHEEP_MAX)) * DESIRE_SEXUALITY);
	// clip maximal values
	if(_desireCombat>SHEEP_MAX)
		_desireCombat=SHEEP_MAX;
	if(_desireSexuality>SHEEP_MAX)
		_desireSexuality=SHEEP_MAX;

	// how does neighbour sheeps affect social, sexuality and combat?

	// social
	int helpvalue=0;
	// for every neighbour sheep
	for(std::vector<Sheep>::iterator it = sheeps.begin();
		it != sheeps.end();
		++it)
	{
		Position2D spos = it->getPosition();
		int i = abs(_pos._x - spos._x) + abs(_pos._y - spos._y);
		// and all with distance of 2
		if(i==2 && it->_sheepAlive)
			helpvalue++;
	}
	if(helpvalue>0)
	{
		int vorher = _desireSocial;
		_desireSocial-=static_cast<short>(helpvalue * LOWER_SOZIAL);
		if(_desireSocial<0)
			_desireSocial=0;
		sprintf(s,"Schaf%d; trifft %d Schafe. Soziales von %d auf %d",_sheepID,helpvalue,vorher,_desireSocial);
		LOG_SHEEP(s);
	}

	// sexuality
	if(_desireSexuality>NEEDED_SEXUALITY_FOR_CHILD)
	{
		// only for sheeps of other sex
		helpvalue=-1;
		// is there a sheep of another sex?
		for(std::vector<Sheep>::iterator it = sheeps.begin();
			it != sheeps.end();
			++it)
		{
			Position2D spos = it->getPosition();
			int i = abs(_pos._x - spos._x) + abs(_pos._y - spos._y);
			// also distance of 2
			if(i==2 && it->_sheepGender != _sheepGender && it->_sheepAge>=SHEEP_CHILDHOOD && it->_sheepAlive)
				helpvalue=it->_sheepID;
		}
		if(helpvalue!=-1)		// no other sex sheep?
		{
			int vorher = _desireSexuality;
			_desireSexuality=0;
			sheeps[helpvalue]._desireSexuality=0;		// change also for other sheep
			sprintf(s,"Schaf%d; paart sich mit Schaf%d. Sexualität von %d auf %d",_sheepID,helpvalue,vorher,_desireSexuality);
			LOG_SHEEP(s);
			// create new sheep
			generateNewSheep(helpvalue,map,sheeps);
			_sheepIntention=IDLE;
		}
	}
		
	// fight against other sheep
	helpvalue=-1;
	// is there a sheep of same sex on neighbour field?
	for(std::vector<Sheep>::iterator it = sheeps.begin();
		it != sheeps.end();
		++it)
	{
		Position2D spos = it->getPosition();
		int i = abs(_pos._x - spos._x) + abs(_pos._y - spos._y);
		// also distance of 2
		if(i==2 && it->_sheepGender == _sheepGender && it->_sheepAge<SHEEP_ADULTHOOD && it->_sheepAlive)
			helpvalue=it->_sheepID;
	}
	if(helpvalue!=-1)		// no enemy sheep?
	{
		int vorher = _desireCombat;
		_desireCombat=0;
		sheeps[helpvalue]._desireCombat=0;			// change also for other sheep
		sprintf(s,"Schaf%d; kämpft gegen Schaf%d. Kampflust von %d auf %d",_sheepID,helpvalue,vorher,_desireCombat);
		LOG_SHEEP(s);
		if(sheeps[helpvalue]._sheepSize>_sheepSize)
			if(irand(sheeps[helpvalue]._sheepSize-_sheepSize,SHEEP_MAX)==SHEEP_MAX)
			//sheep dies
			{
				_sheepAlive=false;
				map->resetSheep(_pos);
				sprintf(s,"Schaf%d; stirbt an den Verletzungen.",_sheepID);
				LOG_SHEEP(s);
			}
		_sheepIntention=IDLE;
	}
}

void Sheep::checkDeath(Map* map)
{
	char s[200];

	if(_desireHunger>255)		// starved
	{
		_sheepAlive=false;
		map->resetSheep(_pos);
		sprintf(s,"Schaf%d; ist verhungert",_sheepID);
		LOG_SHEEP(s);
	}
	if(_desireSocial>255)		// lonely
	{
		_sheepAlive=false;
		map->resetSheep(_pos);
		sprintf(s,"Schaf%d; ist vereinsamt",_sheepID);
		LOG_SHEEP(s);
	}
	if(_desireHygiene>255)		// illness
	{
		_sheepAlive=false;
		map->resetSheep(_pos);
		sprintf(s,"Schaf%d; ist an einer Krankheit gestorben",_sheepID);
		LOG_SHEEP(s);
	}
}

void Sheep::findPath(Map* map, std::vector<Sheep> &sheeps)
{
	char s[200];

	// remove old path
	/*if(_path!=0)
	{
		delete[] _path;
		_path=0;
		_reachedGoal=false;
		_hasGoal=false;
	}*/

	// create new path based on next intention
	findNext(_sheepIntention, map, sheeps);
	// all possibilities
	//1. there are no objects
	if(_tempPos.size()==0)	// there is no path
	{
		//Logging
		switch(_sheepIntention)
		{
			case IDLE: sprintf(s, "Schaf%d; blöckt!",_sheepID); break;
			case HUNGER: sprintf(s,"Schaf%d; findet nichts zu essen!",_sheepID); break;
			case SOZIAL: sprintf(s,"Schaf%d; findet kein anderes Schaf!",_sheepID); break;
			case HYGIENE: sprintf(s,"Schaf%d; findet kein freies Feld!",_sheepID); break;
			case SEXUALITY: sprintf(s,"Schaf%d; findet keinen Partner!",_sheepID); break;
			case COMBAT: sprintf(s,"Schaf%d; findet keinen Gegner!",_sheepID); break;
		}
		LOG_SHEEP(s);
		_sheepIntention=IDLE;
	}
	else	// create path
	{
		//2. there is one object
		if(_tempPos.size()==1)
			_goal = _tempPos[0];
		//3. there are more than one objects
		if(_tempPos.size()>1)
		{
			// random
			int i=irand(0, (int)_tempPos.size()-1);
			_goal = _tempPos[i];
		}

		// special case:
		// for social, combat and sexuality add neighbour field
		if(_sheepIntention==SOZIAL || _sheepIntention==SEXUALITY || _sheepIntention==COMBAT)
		{
			// find next neighbour
			Position2D neighbour;
			Position2D newgoal=_goal;
			int cost = 1000;

			for(int i=0; i<NEIGHBOURS; ++i)
			{
				neighbour = _goal.neighbour(i);
				int mycost = abs(_pos._x - neighbour._x) + abs(_pos._y - neighbour._y);
				if(map->isValid(neighbour)==false)
					mycost+=1000;
				if(mycost < cost)
				{
					cost=mycost;
					newgoal = neighbour;
				}
			}
			_goal = newgoal;	
		}
				
		// create new path
		_pathPos=0;
		_hasGoal=true;
		_reachedGoal=false;
		AStar a(_pos, _goal, map);
		if(_path!=0)
		{
			delete[] _path;
			_path=0;
		}
		int length = a.searchPath();
		if(length!=-1)
		{
			_path = new Position2D[length];
			a.getPath(_path);
		}
		else
			clan::Console::write_line("Keinen Weg gefunden...");
		if(_pos == _goal)
			_reachedGoal=true;
	}
}

void Sheep::lowerDesires(Map* map, std::vector<Sheep> &sheeps)
{
	char s[200];

	int helpvalue;
	int vorher;

	switch(_sheepIntention)
	{
	//	if sheep is hungry, eat grass on field
	case HUNGER:
		helpvalue=map->/*_fields[_pos._x][_pos._y].getValue()*/getValue(_pos._x,_pos._y);
		if(helpvalue>1)	// only if sheep is on gras
		{
			int wert = helpvalue-1;
			vorher = _desireHunger;
			int pflanzenwert = static_cast<short>(wert * LOWER_HUNGER);
			_desireHunger-=pflanzenwert;	// reduce plant value
			//_desireHygiene+=pflanzenwert;	//...add plant value
			if(_desireHunger<0)
				_desireHunger=0;
			sprintf(s,"Schaf%d; isst. Hunger von %d auf %d",_sheepID,vorher,_desireHunger);
			LOG_SHEEP(s);
			map->/*_fields[_pos._x][_pos._y].setValue(1)*/setValue(_pos._x,_pos._y,1);
		}
		break;
	// if sheep needs to go to toilet
	case HYGIENE:
		helpvalue=map->/*_fields[_pos._x][_pos._y].getValue()*/getValue(_pos._x,_pos._y);
		if(helpvalue==1)	// only if field is no gras
		{
			vorher = _desireHygiene;
			map->addDirt(_pos,_desireHygiene);		// add dirt to map (based on sheeps value)
			_desireHygiene=0;
			sprintf(s,"Schaf%d; entleert sich. Hygienebedarf von %d auf 0",_sheepID,vorher);
			LOG_SHEEP(s);
		}
		break;
	case SOZIAL:
		sprintf(s,"Schaf%d; blöckt",_sheepID);
		LOG_SHEEP(s);
		break;
	}
	_sheepIntention=IDLE;
	_hasGoal=false;
	_reachedGoal=false;
}

void Sheep::combineParents(int id, short &size, short &mojo, short &aggression, std::vector<Sheep> &sheeps)
{
	// random value from one of its parents
	size=irand(0,1)==0?_sheepSize:sheeps[id]._sheepSize;
	mojo=irand(0,1)==0?_sheepMojo:sheeps[id]._sheepMojo;
	aggression=irand(0,1)==0?_sheepAgression:sheeps[id]._sheepAgression;

	// mutation?
	if(irand(0,100)>MUTATION_RATE)
	{
		int value = irand(1,MUTATION_MAX);
		size=irand(0,1)==0?size+=value:size-=value;
	}
	if(irand(0,100)>MUTATION_RATE)
	{
		int value = irand(1,MUTATION_MAX);
		mojo=irand(0,1)==0?mojo+=value:mojo-=value;
	}
	if(irand(0,100)>MUTATION_RATE)
	{
		int value = irand(1,MUTATION_MAX);
		aggression=irand(0,1)==0?aggression+=value:aggression-=value;
	}

	// clip values to range
	if(size<0) size=0;
	if(size>SHEEP_MAX) size=SHEEP_MAX;
	if(mojo<0) mojo=0;
	if(mojo>SHEEP_MAX) mojo=SHEEP_MAX;
	if(aggression<0) aggression=0;
	if(aggression>SHEEP_MAX) aggression=SHEEP_MAX;
}

int Sheep::wrap_around(int a, int b)
{
	if(a<0)
		return a+b;
	if(a>=b)
		return a-b;
	return a;
}

int Sheep::irand( int a, int b) {
	double r = b - a + 1;
	return a + static_cast<int>(r * rand()/(RAND_MAX+1.0));
}