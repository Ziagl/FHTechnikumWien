#include "map.h"

Map::Map(clan::Canvas &canvas)
{
	_canvas = canvas;
	readTiles();
	int lvl = LEVEL;
	_activeBuffer=0;
	switch(lvl)
	{
	case 0: generateRandomMap(); break;
	case 1: readLevelFile(".\\Data.\\Maps\\map1.txt"); break;
	case 2: readLevelFile(".\\Data.\\Maps\\map2.txt"); break;
	case 3: readLevelFile(".\\Data.\\Maps\\map3.txt"); break;
	case 4: readLevelFile(".\\Data.\\Maps\\map4.txt"); break;
	default: readLevelFile(".\\Data.\\Maps\\map1.txt"); break;
	}
	resetSheep();
	resetDirt();
	srand(clock_t(0));

	_font = new clan::Font("Arial",14);
}

Map::~Map(void)
{
}

void Map::readLevelFile(std::string file){
	std::string line;
	std::ifstream iFile(file);
	int x;
	for(int y = 0; !iFile.eof();y++){
		getline(iFile, line);
		x=0;
		for(std::string::iterator it = line.begin(); it!=line.end();++it){
			parseTiles(*it,Position2D(x++,y));
		}
	}
	iFile.close();
}

// render every tile of this map
void Map::render()
{
	clan::Image* tile = NULL;
	float x_koord;
	float y_koord;
	float scaleFactor;
#if SMALL==1
	scaleFactor=TILES_SCALE_FACTOR_SMALL;
#else
	scaleFactor=TILES_SCALE_FACTOR;
#endif

	for(int x=0;x<LEVEL_WIDTH;x++)
	{
		for(int y=0;y<LEVEL_HEIGHT;y++)
		{
				switch(_buffer[x][y][_activeBuffer])
				{
				case 0: tile = _tilesMap["water"]; break;
				case 1: tile = _tilesMap["sand"]; if(_dirt[x][y]>0) tile = _tilesMap["dirt"]; break;
				case 2: tile = _tilesMap["gras0"]; break;
				case 3: tile = _tilesMap["gras1"]; break;
				case 4: tile = _tilesMap["gras2"]; break;
				case 5: case 6: case 7: case 8: case 9: case 10: tile = _tilesMap["gras3"]; break;
				}
				if(tile!=NULL){
					//Tile zeichnen
					x_koord = static_cast<float>((x*scaleFactor*TILES_SIZE_X)+((y%2==0)?TILES_OFFSET_X*scaleFactor:0));
					y_koord = static_cast<float>((y*scaleFactor*TILES_OFFSET_Y));
					tile->set_scale(scaleFactor,scaleFactor);
					tile->draw(_canvas, x_koord, y_koord);
				}
		}
	}
}

void Map::changeTile(Position2D pos)
{
	int v = _buffer[pos._x][pos._y][_activeBuffer];
	if(v>0)
		_buffer[pos._x][pos._y][_activeBuffer]=0;
	else
		_buffer[pos._x][pos._y][_activeBuffer]=1;
}

// create tile for given position and information from map file
void Map::parseTiles(char identifier, Position2D position){
	int value=0;
	int r=0;

	clan::Image* surface = NULL;
	switch(identifier){
		case '0': surface = _tilesMap["water"];value=0;break;
		case '1': surface = _tilesMap["sand"];value=1;break;
		case '2': surface = _tilesMap["gras0"];value=2;break;
		case '3': surface = _tilesMap["gras1"];value=3;break;
		case '4': surface = _tilesMap["gras2"];value=4;break;
		case '5': surface = _tilesMap["gras3"];value=5;break;
		default: break;
	}
	_fields[position._x][position._y] = Field(surface, value);
	_buffer[position._x][position._y][_activeBuffer]=value;
}

void Map::readTiles(void)
{
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("water",new clan::Image(_canvas, ".\\Data\\Tiles\\water.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sand",new clan::Image(_canvas, ".\\Data\\Tiles\\sand.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("dirt",new clan::Image(_canvas, ".\\Data\\Tiles\\dirt.png")));

	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("gras0",new clan::Image(_canvas, ".\\Data\\Tiles\\gras_0.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("gras1", new clan::Image(_canvas, ".\\Data\\Tiles\\gras_1.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("gras2",new clan::Image(_canvas, ".\\Data\\Tiles\\gras_2.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("gras3",new clan::Image(_canvas, ".\\Data\\Tiles\\gras_3.png")));

	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sheep0", new clan::Image(_canvas, ".\\Data\\Tiles\\sheep_0.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sheep1",new clan::Image(_canvas, ".\\Data\\Tiles\\sheep_1.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sheep2",new clan::Image(_canvas, ".\\Data\\Tiles\\sheep_2.png")));

	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sheep0w", new clan::Image(_canvas, ".\\Data\\Tiles\\sheep_0w.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sheep1w",new clan::Image(_canvas, ".\\Data\\Tiles\\sheep_1w.png")));
	_tilesMap.insert(std::map<std::string,clan::Image*>::value_type("sheep2w",new clan::Image(_canvas, ".\\Data\\Tiles\\sheep_2w.png")));
}

int Map::isValid(Position2D &p)
{
	// check if this field is valid
	// special fields also are not valid --> 0...for water!
	if ( p._x < 0 || p._x >= LEVEL_WIDTH || p._y < 0 || p._y >= LEVEL_HEIGHT || _buffer[p._x][p._y][_activeBuffer]<1 || _sheeps[p._x][p._y])
		return 0;
	return 1;
}

void Map::generateRandomMap()
{
	for(int x=0;x<LEVEL_WIDTH;x++)
	{
		for(int y=0;y<LEVEL_HEIGHT;y++)
		{
			_buffer[x][y][_activeBuffer]=irand(0,2);
			if(_buffer[x][y][_activeBuffer]==0)
				_buffer[x][y][_activeBuffer]=irand(0,2);
			if(_buffer[x][y][_activeBuffer]==2)
				_buffer[x][y][_activeBuffer]=irand(2,10);
		}
	}
}

void Map::growGras()
{
	int backBuffer=0;
	if(_activeBuffer==0)
		backBuffer=1;

	for(int x=0;x<LEVEL_WIDTH;x++)
	{
		for(int y=0;y<LEVEL_HEIGHT;y++)
		{
			int val = _buffer[x][y][_activeBuffer];
			_buffer[x][y][backBuffer]=val;

			if(val!=0)				// if it is no water
			{
				if(_dirt[x][y]>0)	// is there dirt?
				{
					// turns to plant
					if(irand(0,10)>=DIRTGRAS)
						_buffer[x][y][backBuffer]=2;
					else
						_buffer[x][y][backBuffer]=1;
					_dirt[x][y]=0;
				}
				else				// if it is plant -> grow
				{
					if(val>1)
							_buffer[x][y][backBuffer]=_buffer[x][y][_activeBuffer]+1;
					else
					{
						if(val==1)
						{
							// grow a plant on sand -> if there are neighbour plants
							if(hasNeighbour(x,y))
							{
								if(irand(0,10)>=SPREADGRAS)
									_buffer[x][y][backBuffer]=2;
							}
						}
					}
				}
			}
			if(_buffer[x][y][backBuffer]>10)
				_buffer[x][y][backBuffer]=1;
		}
	}

	// switch buffers
	_activeBuffer=backBuffer;
}

bool Map::hasNeighbour(int x, int y)
{
	Position2D p(x,y);
	for(int i=0; i<NEIGHBOURS; ++i)
	{
		int x=p.neighbour(i)._x;
		int y=p.neighbour(i)._y;
		if(_buffer[x][y][_activeBuffer]>1)
			return true;
	}
	return false;
}

void Map::resetSheep()
{
	for(int x=0;x<LEVEL_WIDTH;x++)
	{
		for(int y=0;y<LEVEL_HEIGHT;y++)
		{
			_sheeps[x][y] = false;
		}
	}
}

void Map::resetSheep(Position2D pos)
{
	_sheeps[pos._x][pos._y] = false;
}

void Map::setSheep(Position2D pos)
{
	_sheeps[pos._x][pos._y] = true;
}

void Map::resetDirt()
{
	for(int x=0;x<LEVEL_WIDTH;x++)
	{
		for(int y=0;y<LEVEL_HEIGHT;y++)
		{
			_dirt[x][y] = 0;
		}
	}
}
void Map::resetDirt(Position2D pos)
{
	_dirt[pos._x][pos._y];
}

void Map::addDirt(Position2D pos, int value)
{
	_dirt[pos._x][pos._y]+=value;
}

int Map::irand( int a, int b)
{
	double r = b - a + 1;
	return a + static_cast<int>(r * rand()/(RAND_MAX+1.0));
}