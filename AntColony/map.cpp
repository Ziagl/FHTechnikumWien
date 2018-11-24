#include "map.h"

Map::Map(clan::Canvas &canvas)
{
	_canvas = canvas;
	readTiles();
	int lvl = LEVEL;
	switch (lvl)
	{
		case 1: readLevelFile(".\\Data.\\Maps\\map1.txt"); break;
		case 2: readLevelFile(".\\Data.\\Maps\\map2.txt"); break;
		case 3: readLevelFile(".\\Data.\\Maps\\map3.txt"); break;
		case 4: readLevelFile(".\\Data.\\Maps\\map4.txt"); break;
		default: readLevelFile(".\\Data.\\Maps\\map1.txt"); break;
	}

	srand(clock_t(0));
	clearPheromones();

	_font = new clan::Font("Arial", 14);

	_maxPheromones = PHEROMONES_MIN;
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
	clan::Image* tile = NULL;
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
			//tile = _fields[x][y].getTile();
			switch (_fields[x][y].getValue())
			{
				case 0: tile = _tilesMap["water"]; break;
				case 1: tile = _tilesMap["sand"]; break;
				case 2: tile = _tilesMap["gras"]; break;
			}
			if (tile != NULL) {
				// draw tiles
				x_koord = static_cast<float>((x*scaleFactor*TILES_SIZE_X) + ((y % 2 == 0) ? TILES_OFFSET_X * scaleFactor : 0));
				y_koord = static_cast<float>((y*scaleFactor*TILES_OFFSET_Y));
				tile->set_scale(scaleFactor, scaleFactor);
				tile->draw(_canvas, x_koord, y_koord);

				if (_pheromones[x][y] > 1.0f)
				{
					// draw path
					tile = _tilesMap["path"];
					tile->set_scale(scaleFactor, scaleFactor);
					tile->set_alpha(_pheromones[x][y] / _maxPheromones);
					tile->draw(_canvas, x_koord, y_koord);

					// draw text
					std::stringstream str;
					str << static_cast<int>(_pheromones[x][y]);
					std::string s = str.str();
					_font->draw_text(_canvas, x_koord + 32.0f, y_koord + 58.0f, s, clan::Colorf::black);
				}
			}
		}
	}
}

void Map::changeTile(Position2D pos)
{
	int v = _fields[pos._x][pos._y].getValue();
	if (v > 0)
		_fields[pos._x][pos._y].setValue(0);
	else
		_fields[pos._x][pos._y].setValue(1);
}

// parses the FileCodec into an usable form
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
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("start", new clan::Image(_canvas, ".\\Data\\Tiles\\anthill.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("target", new clan::Image(_canvas, ".\\Data\\Tiles\\treasure.png")));
	_tilesMap.insert(std::map<std::string, clan::Image*>::value_type("path", new clan::Image(_canvas, ".\\Data\\Tiles\\ant.png")));
}

void Map::clearPheromones()
{
	for (int x = 0; x < LEVEL_WIDTH; x++)
	{
		for (int y = 0; y < LEVEL_HEIGHT; y++)
		{
			_pheromones[x][y] = PHEROMONES_MIN;
		}
	}
}

void Map::computePheromones()
{
	_maxPheromones = PHEROMONES_MIN;

	for (int i = 0; i < LEVEL_WIDTH; ++i)
		for (int j = 0; j < LEVEL_HEIGHT; ++j)
		{
			_pheromones[i][j] *= (1 - PHEROMONES_LOWER);
			if (_pheromones[i][j] < PHEROMONES_MIN)
				_pheromones[i][j] = PHEROMONES_MIN;

			if (_maxPheromones < _pheromones[i][j])
				_maxPheromones = _pheromones[i][j];
		}
}

void Map::addPheromones(Position2D pos, float value)
{
	_pheromones[pos._x][pos._y] += value;
}

int Map::isValid(Position2D &p)
{
	// check if this field is valid
	// special fields also are not valid --> 0...for water!
	if (p._x < 0 || p._x >= LEVEL_WIDTH || p._y < 0 || p._y >= LEVEL_HEIGHT || _fields[p._x][p._y].getValue() < 1)
		return 0;
	return 1;
}