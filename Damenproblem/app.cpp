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

	// set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("Damenproblem");
	desc.set_size(clan::Sizef(WINDOW_WIDTH, WINDOW_HEIGHT + TEXT_HEIGHT), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);
	font = clan::Font("Courir", TEXT_HEIGHT);
	queen = clan::Image(canvas, "queen.png");

	// connect the window close event
	sc.connect(window.sig_window_close(), [&]() {_quit = true; });

	// connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard));

	// connect a mouse handler (only for example it is not used)
	sc.connect(window.get_mouse().sig_key_down(), this, &App::on_mouse);
	sc.connect(window.get_mouse().sig_key_dblclk(), this, &App::on_mouse);

	_quit = false;
	srand(static_cast<int>(time(0)));	// initialize random number generator

	init();		//initialize
}

bool App::update()
{
	// clear display with white background
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));

	render();

	window.flip(1);

	return !_quit;
}

// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_escape:
		case clan::keycode_q: 
				 _quit = true; break;
		case clan::keycode_space: 
				compute(); break;
	}
}

// mouse key was pressed
void App::on_mouse(const clan::InputEvent &key) {
	// mouse position
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;

	// mouse action
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
		//nothing
	}
}

void App::init()
{
	generation = 0;		// counts generations in loop --> mutation every X generation
} // init()

void App::render()
{
	clan::Colorf color = clan::Colorf::white;
	float length = (float)WINDOW_WIDTH / (float)MAX;
	int i = 0;
	for (int j = MIN; j < MAX; ++j)
	{
		for (int k = MIN; k < MAX; ++k)
		{
			++i;
			// draw board
			if (i % 2)
				canvas.fill_rect(clan::Rectf(j * length, k * length, j * length + length, k * length + length), color);

			// draw queen
			if (generation > 0 && (*it_g)->g[k] == j)
			{
				queen.set_scale(length / queen.get_width(), length / queen.get_height());
				queen.draw(canvas, j * length, k * length);
			}
		}
		if (MAX % 2 == 0)
			++i;
	}

	std::string text;
	if(generation == 0)
		text = "Press SPACE to compute";
	else
		text = clan::string_format("Solution found after  %1  generations", generation);
	clan::Sizef text_size = font.measure_text(canvas, text).bbox_size;
	font.draw_text(canvas, 0, WINDOW_HEIGHT + TEXT_HEIGHT - 3, text, clan::Colorf::red);
} // render()

void App::compute()
{
	srand(irand(0, 1000));
	generation = 0;
	bool solution = false;
	// draw computing information
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));
	std::string text = "computing, please wait...";
	clan::Sizef text_size = font.measure_text(canvas, text).bbox_size;
	font.draw_text(canvas, (WINDOW_WIDTH / 2) - (text_size.width / 2), WINDOW_HEIGHT / 2, text, clan::Colorf::red);
	window.flip(1);
	do
	{
		int fitnesssum = 0;
		createPopulation(POPULATION_SIZE);							// create population of given size
		evaluatePopulation(fitnesssum);								// evaluate fitness function
		evolutePopulation(fitnesssum, generation%MUTATION_GENERATION == 0);		// generate children, mutate if needed
		selectionOfPopulation();
		generation++;
		it_g = population.begin();
		if ((*it_g)->fitness == 28)
			solution = true;
	} while (solution != true);
} // compute()

void App::selectionOfPopulation()
{
	// sort population by fitness
	population.sort([](const Genom* p1, const Genom* p2){
		return p1->fitness > p2->fitness;
	});
	// remove last COPULATIONS_PER_EVOLUTION * CHILDREN
	for (int i = 0; i < COPULATIONS_PER_EVOLUTION * CHILDREN; ++i)
		population.pop_back();
}

void App::evolutePopulation(int f, bool mutation)
{
	// how many pairs have chilrdren?
	for (int i = 0; i < COPULATIONS_PER_EVOLUTION; ++i)
	{
		//mit Roulette 2 Eltern aus Population herausnehmen
		Genom* p1 = GetParent(irand(0, f));
		f -= p1->fitness;
		Genom* p2 = GetParent(irand(0, f));
		f -= p2->fitness;
		//aus diesen beiden Eltern Kinder zeugen
		copulateParents(p1, p2, mutation);
	}
	//nun noch die temporäre Liste mit Eltern und Kinder mit der Liste von kinderlosen Individuumen mergen
	population.splice(population.end(), temp_population);
}

void App::copulateParents(Genom* p1, Genom* p2, bool mutation)
{
	for (int i = 0; i < CHILDREN; ++i)
	{
		//Klon von Parent 1
		Genom* child = p1->copy();
		//über Bitmaske die Elemente von Parent 1 mit denen von Parent 2 austauschen
		for (int j = 0; j < GENOM_SIZE; ++j)
			if (irand(0, 2) == 0)
				child->g[j] = p2->g[j];
		if (mutation)
		{
			//ein zufällige Mutation einfügen
			child->g[irand(MIN, MAX)] = irand(MIN, MAX);
		}
		//Kind bewerten --> Fitnessfunktion
		evaluateIndividual(child);
		//jetzt das Kind der neuen Liste hinzufügen
		temp_population.push_back(child);
	}
	//nun auch die beiden Eltern der neuen Liste hinzufügen
	temp_population.push_back(p1);
	temp_population.push_back(p2);
}

Genom* App::GetParent(int k)
{
	for (std::list<Genom*>::iterator it = population.begin();
		it != population.end();
		++it)
	{
		if (k > (*it)->fitness)
			k -= (*it)->fitness;
		else
		{
			Genom* ret = (*it);
			population.remove(ret);
			return ret;
		}
	}
}

void App::evaluatePopulation(int &sum)
{
	for (std::list<Genom*>::iterator it = population.begin();
		it != population.end();
		++it)
	{
		evaluateIndividual(*it);

		sum += (*it)->fitness;
	}
}

void App::evaluateIndividual(Genom* o)
{
	o->fitness = 0;

	for (int i = 0; i < GENOM_SIZE - 1; ++i)
	{
		for (int j = i + 1; j < GENOM_SIZE; ++j)
		{
			//wenn das Damenpaar sich nicht schlägt --> +1 bei Fitness
			if (checkDamen(i, o->g[i], j, o->g[j]))
				o->fitness++;
		}
	}
}

bool App::checkDamen(int x1, int y1, int x2, int y2)
{
	/*if(x1 == x2)
		return false;*/
	if (y1 == y2)
		return false;
	if (abs(x1 - x2) == abs(y1 - y2))
		return false;
	return true;
}

//erzeuge Population
void App::createPopulation(int size)
{
	population.clear();
	for (int i = 0; i < size; ++i)
	{
		population.push_back(new Genom(irand(MIN, MAX), irand(MIN, MAX), irand(MIN, MAX), irand(MIN, MAX), irand(MIN, MAX), irand(MIN, MAX), irand(MIN, MAX), irand(MIN, MAX)));
	}
}

//erzeugt eine Zufallszahl zwischen a und b
int App::irand(int a, int b) 
{
	double r = b - a + 1;
	return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
}