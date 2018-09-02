#include "precomp.h"
#include "globals.h"
#include "app.h"

using namespace clan;

clan::ApplicationInstance<App> clanapp;

App::App()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("Sanduhr");
	desc.set_size(clan::Sizef(WINDOW_HEIGHT, WINDOW_WIDTH), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);
	font = clan::Font("Courir", 35);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&]() {quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard));

	// Connect a mouse handler
	sc.connect(window.get_mouse().sig_key_down(), this, &App::on_mouse);
	sc.connect(window.get_mouse().sig_key_dblclk(), this, &App::on_mouse);

	game_time.reset();
	init();
}

bool App::update()
{
	game_time.update();

	if (!_pause)
		compute();
	
	// Clear the display in a dark blue nuance
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.2f));
	
	render();

	window.flip(1);

	return !quit;
}

// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_escape:
			quit = true;
			break;
		case clan::keycode_space:
			_pause = !_pause;
			_lasttime = game_time.get_current_time();
			break;
	}
}

// mouse key was pressed
void App::on_mouse(const clan::InputEvent &key) {
	// Mausposition
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;

	// Mausaktion
	switch (key.id)
	{
		case mouse_left:
			break;
		case mouse_right:
			break;
		case mouse_middle:
			break;
		case mouse_wheel_up:
			break;
		case mouse_wheel_down:
			break;
		//ix
	}
}

void App::init(void)
{
	quit = false;
	_pause = false;
	_currentStep = 0;
	_board = 0;
	_nextboard = 1;
	srand(static_cast<int>(time(0)));			//Zufallsgenerator initialisieren
	init_pattern();								//Muster initialisieren
	_time = 0;
	_lasttime = 0;
	_mode = 0;

	int k = 0;
	int d_max = SAND_DIFF;
	int d = 0;

	for (int j = 0; j<SAND_WIDTH; ++j)
	{
		_sand[_board][0][j] = GLASS;
		_sand[_board][SAND_HEIGHT - 1][j] = GLASS;
	}

	for (int i = 1; i<SAND_HEIGHT - 1; ++i)
	{
		for (int j = 0; j<SAND_WIDTH; ++j)
		{
			//Glas
			if (j == k || j == (SAND_WIDTH - 1) - k)
				_sand[_board][i][j] = GLASS;
			else
			{
				//obere Hälfte
				if (i<SAND_HEIGHT / 2)
				{
					//kein Glas
					//außerhalb --> leer
					if (j<k || j>(SAND_WIDTH - 1) - k)
						_sand[_board][i][j] = EMPTY;
					else
						_sand[_board][i][j] = SAND;
				}
				else
					_sand[_board][i][j] = EMPTY;
			}
		}
		d++;
		if (d == d_max)
		{
			d = 0;
			if (i<SAND_HEIGHT / 2)
				++k;
			else
				if (k>0)
					--k;
		}
	}

	//2. Board Initialisieren (damit die Ränder vom Glas auch passen)
	for (int i = 0; i<SAND_HEIGHT; ++i)
		for (int j = 0; j<SAND_WIDTH; ++j)
			_sand[_nextboard][i][j] = _sand[_board][i][j];
}

void App::render(void) {

	for (int i = 0; i<SAND_HEIGHT; ++i)
		for (int j = 0; j<SAND_WIDTH; ++j)
		{
			clan::Colorf color;
			//Optimierung: Schwarze Pixel werden nicht gezeichnet == Hintergrund
			switch (_sand[_board][i][j])
			{
			case EMPTY: break;
			case SAND:
				color = clan::Colorf::yellow;
				//CL_Draw::fill(m_gc, (float)j, (float)i, (float)j + 1, (float)i + 1, color);
				canvas.fill_rect(clan::Rectf((float)j, (float)i, (float)j + 1, (float)i + 1), color);
				//CL_Draw::point(m_gc, (float)j, (float)i, color); 
				break;
			case GLASS:
				color = clan::Colorf::blue;
				//CL_Draw::fill(m_gc, (float)j, (float)i, (float)j + 1, (float)i + 1, color);
				canvas.fill_rect(clan::Rectf((float)j, (float)i, (float)j + 1, (float)i + 1), color);
				//CL_Draw::point(m_gc, (float)j, (float)i, color); 
				break;
			}
		}

	//Gibt die verlaufene Zeit aus
	std::stringstream str;

	char timebuf[32];
	time_t _ctime = static_cast<time_t>(_time);
	tm *_tm = localtime(&_ctime);
	_tm->tm_hour--;

	strftime(timebuf, 32, "%H:%M:%S", _tm);
	str << timebuf;
	std::string text = str.str();
	clan::Sizef text_size = font.measure_text(canvas, text).bbox_size;
	font.draw_text(canvas, 150.0f, 150.0f, text, clan::Colorf::yellow);

	if (show_fps) {
		std::string fps = clan::string_format("%1 fps", (int)game_time.get_updates_per_second());
		font.draw_text(canvas, 150.0f, 180.0f, fps, clan::Colorf::red);
	}
} // render()

void App::compute()
{
	//Unterschiedliche Quadrate werden als Pattern genommen (damit Sandkörner nicht irgendwo im Eck liegen bleiben)
	switch (_mode)
	{
	case 0:
		for (int i = 1; i<SAND_HEIGHT - 1; i += 2)
			for (int j = 1; j<SAND_WIDTH - 1; j += 2)
				compute_pattern(i, j, _board, _nextboard);
		break;
	case 1:
		for (int i = 0; i<SAND_HEIGHT; i += 2)
			for (int j = 0; j<SAND_WIDTH; j += 2)
				compute_pattern(i, j, _board, _nextboard);
		break;
	case 2:
		for (int i = 1; i<SAND_HEIGHT - 1; i += 2)
			for (int j = 0; j<SAND_WIDTH; j += 2)
				compute_pattern(i, j, _board, _nextboard);
		break;
	case 3:
		for (int i = 0; i<SAND_HEIGHT; i += 2)
			for (int j = 1; j<SAND_WIDTH - 1; j += 2)
				compute_pattern(i, j, _board, _nextboard);
		break;
	}

	_mode++;
	if (_mode == 4)
		_mode = 0;

	//die Boards vertauschen
	_board = _nextboard;
	if (_nextboard == 1)
		_nextboard = 0;
	else
		_nextboard = 1;

	//Generationenzähler
	_currentStep++;

	//Zeit aktualisieren
	float t = game_time.get_current_time();
	_time += t - _lasttime;
	_lasttime = t;
}

inline void App::compute_pattern(int x, int y, int board_in, int board_out)
{
	int _in;
	int _out;

	_in = _sand[board_in][x][y];
	_in = (_in * 10) + _sand[board_in][x][y + 1];
	_in = (_in * 10) + _sand[board_in][x + 1][y];
	_in = (_in * 10) + _sand[board_in][x + 1][y + 1];

	//jetzt die Pattern mit der Musterliste vergleichen und entsprechendes neues Muster setzen
	_out = _in;		//falls es keinen Pattern gibt bleibts gleich
					//1. Spezialfall
	if (_in == 1100)
	{
		int r = irand(MIN, MAX);

		if (r<PROP1)
			_out = 11;		//beide Körner fallen
		if (r >= PROP1 && r<PROP2)
			_out = 1001;		//rechtes Korn fällt
		if (r >= PROP2 && r<PROP3)
			_out = 1010;		//linkes Korn fällt
		if (r >= PROP3)
			_out = 1100;		//Körner bleiben stecken
	}
	else
	{
		//kein Spezialfall? dann einfach mit den verfügbaren Pattern vergleichen
		for (std::vector<Pattern>::iterator it = p_vec.begin();
			it != p_vec.end();
			++it)
		{
			if (it->_in == _in)
			{
				_out = it->_out;
				break;
			}
		}
	}

	//jetzt das neue Pattern wieder umrechnen und Elemente im Board setzen
	int o1, o2, o3, o4 = 0;

	o1 = _out / 1000;
	o2 = (_out / 100) % 10;
	o3 = (_out / 10) % 10;
	o4 = _out % 10;

	//und noch ins board übertragen
	_sand[board_out][x][y] = o1;
	_sand[board_out][x][y + 1] = o2;
	_sand[board_out][x + 1][y] = o3;
	_sand[board_out][x + 1][y + 1] = o4;
}

void App::init_pattern()
{
	p_vec.push_back(Pattern(1000, 10));
	p_vec.push_back(Pattern(100, 1));
	p_vec.push_back(Pattern(1010, 11));
	p_vec.push_back(Pattern(101, 11));
	p_vec.push_back(Pattern(1001, 11));
	p_vec.push_back(Pattern(110, 11));
	p_vec.push_back(Pattern(1101, 111));
	p_vec.push_back(Pattern(1110, 1011));
	//p_vec.push_back(Pattern(1100,11));			//-->Spezialfall
	p_vec.push_back(Pattern(1020, 21));
	p_vec.push_back(Pattern(102, 12));
	p_vec.push_back(Pattern(1120, 1021));
	p_vec.push_back(Pattern(1102, 112));
	p_vec.push_back(Pattern(120, 21));
	p_vec.push_back(Pattern(1002, 12));
	p_vec.push_back(Pattern(2101, 2011));
	p_vec.push_back(Pattern(1210, 211));
	p_vec.push_back(Pattern(2100, 2001));
	p_vec.push_back(Pattern(1200, 210));
	p_vec.push_back(Pattern(2120, 2021));
	p_vec.push_back(Pattern(1202, 212));
}

int App::irand(int a, int b) {
	double r = b - a + 1;
	return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
}