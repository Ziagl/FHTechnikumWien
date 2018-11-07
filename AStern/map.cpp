#include "map.h"

Map::Map(clan::Canvas &canvas)
{
	_canvas = canvas;
	readTiles();
	readLevelFile(".\\Data.\\Maps\\map1.txt");		// load first map by default
	srand(clock_t(0));
	clearPotentials();
}

Map::~Map(void)
{
}

void Map::readLevelFile(std::string file) {
	std::string line;
	std::ifstream iFile(file);
	int x;
	for (int y = 0; !iFile.eof(); y++) {
		getline(iFile, line);
		x = 0;
		for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
			parseTiles(*it, Position2D(x++, y));
		}
	}
	iFile.close();
}

// render every tile of this map
void Map::render()
{
	clan::Image* tile;
	float x_koord;
	float y_koord;
	float scaleFactor;
#if SMALL==1
	scaleFactor = TILES_SCALE_FACTOR_SMALL;
#else
	scaleFactor = TILES_SCALE_FACTOR;
#endif

	for (int x = 0; x < LEVEL_WIDTH; x++)
	{
		for (int y = 0; y < LEVEL_HEIGHT; y++)
		{
			tile = _fields[x][y].getTile();
			if (tile != NULL) {
				x_koord = static_cast<float>((x*scaleFactor*TILES_SIZE_X) + ((y % 2 == 0) ? TILES_OFFSET_X * scaleFactor : 0));
				y_koord = static_cast<float>((y*scaleFactor*TILES_OFFSET_Y));
				tile->set_scale(scaleFactor, scaleFactor);
				tile->draw(_canvas, x_koord, y_koord);
			}
		}
	}
}

// create tile for given position and information from map file
void Map::parseTiles(char identifier, Position2D position) {
	int value = 0;
	int r = 0;

	clan::Image* surface = NULL;
	switch (identifier) {
		case '0': surface = _tilesMap["water"]; value = 0; break;
		case '1': surface = _tilesMap["sand"]; value = 1; break;
		case '2': surface = _tilesMap["gras"]; value = 2; break;
		case '3':
			r = rand() % 3;
			switch (r)
			{
			case 0: surface = _tilesMap["trees2"]; break;
			case 1: surface = _tilesMap["trees1"]; break;
			case 2: surface = _tilesMap["trees"]; break;
			}
			value = 3;
			break;
		case '5': surface = _tilesMap["mountain_s"]; value = 5; break;
		case '6': surface = _tilesMap["mountain_l"]; value = 5; break;
		default: break;				
	}
	_fields[position._x][position._y] = Field(surface, value);
}

// get all defined images for tiles, enemy, start, goal/target and path
void Map::readTiles(void)
{
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("water", new clan::Image(_canvas, ".\\Data\\Tiles\\water.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("gras", new clan::Image(_canvas, ".\\Data\\Tiles\\gras.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("sand", new clan::Image(_canvas, ".\\Data\\Tiles\\sand.png")));

	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("trees", new clan::Image(_canvas, ".\\Data\\Tiles\\trees_1.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("trees1", new clan::Image(_canvas, ".\\Data\\Tiles\\trees_2.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("trees2", new clan::Image(_canvas, ".\\Data\\Tiles\\trees_full.png")));

	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("mountain_s", new clan::Image(_canvas, ".\\Data\\Tiles\\mountain_small.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("mountain_l", new clan::Image(_canvas, ".\\Data\\Tiles\\mountain_large.png")));

	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("enemy", new clan::Image(_canvas, ".\\Data\\Tiles\\goblin.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("start", new clan::Image(_canvas, ".\\Data\\Tiles\\pirate.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("target", new clan::Image(_canvas, ".\\Data\\Tiles\\treasure.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("path", new clan::Image(_canvas, ".\\Data\\Tiles\\path.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("path_enemy1", new clan::Image(_canvas, ".\\Data\\Tiles\\path_enemy1.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("path_enemy2", new clan::Image(_canvas, ".\\Data\\Tiles\\path_enemy2.png")));
}

void Map::clearPotentials()
{
	for (int x = 0; x < LEVEL_WIDTH; x++)
	{
		for (int y = 0; y < LEVEL_HEIGHT; y++)
		{
			_potentials[x][y] = 1;
		}
	}
}

void Map::setPotentials(Position2D pos, int maxValue, int deep)
{
	// increase potentials around position
	if (deep > 0)
	{
		_potentials[pos._x][pos._y] += maxValue;
		for (int i = 0; i < NEIGHBOURS; ++i)
		{
			Position2D d = pos.neighbour(i);
			if (isValid(d))
				setPotentials(d, maxValue - 1, deep - 1);
		}
	}
}

int Map::isValid(Position2D &p)
{
	// check if this field is valid
	// special fields also are not valid --> 0...for water!
	if (p._x < 0 || p._x >= LEVEL_WIDTH || p._y < 0 || p._y >= LEVEL_HEIGHT)
		return 0;
	return 1;
}
