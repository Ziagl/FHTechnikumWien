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
	desc.set_title("Convex Hull");
	desc.set_size(clan::Sizef(WINDOW_WIDTH, WINDOW_HEIGHT), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// connect the window close event
	sc.connect(window.sig_window_close(), [&]() {m_bQuit = true; });

	// connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard));

	// connect a mouse handler (only for example it is not used)
	sc.connect(window.get_mouse().sig_key_down(), this, &App::on_mouse);
	sc.connect(window.get_mouse().sig_key_dblclk(), this, &App::on_mouse);

	m_bQuit = false;
	m_currentStep = 0;
	srand(static_cast<int>(time(0)));	// initialize random number generator
}

bool App::update()
{
	// clear display with white background
	canvas.clear(clan::Colorf(1.0f, 1.0f, 1.0f));

	render();

	window.flip();

	return !m_bQuit;
}

void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_q:
			m_bQuit = true;
			break;
		case clan::keycode_r:
			createRandomPoints(50);
			break;
		case clan::keycode_c:
			m_vHull.clear();
			m_vPoints.clear();
			m_currentStep = 0;
			break;
		case clan::keycode_enter:
			float fTime;
			{
				MyTimer t(&fTime);
				calculateHull();
			}
			clan::Console::write_line("Hull calculation took: %1 seconds", fTime);
			break;
		case clan::keycode_space:
			calculateHullStep();
			break;
	}
} // on_keyDown

void App::on_mouse(const clan::InputEvent &key) 
{
	// mouse position
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;

	// check if the mouse click occured inside the application window
	if (x >= 0 && x <= WINDOW_WIDTH &&
		y >= 0 && y <= WINDOW_HEIGHT)
	{
		// add the point to the points list
		m_vPoints.push_back(Position2D(x, y));
	} // if - mouse click occured inside app window
} // on_mousePress()

void App::render(void) {

	// draw points
	T_points::iterator it = m_vPoints.begin();
	for (; it != m_vPoints.end(); ++it) {
		canvas.fill_circle(
			(float)it->_x, 
			(float)it->_y, 
			2, 
			clan::Colorf(1.0f, 0.0f, 0.0f)
		);
	} // for - iterate points

	// draw hull
	Position2D posFrom, posTo;

	it = m_vHull.begin();
	if (it != m_vHull.end()) {
		posFrom = *it;
		while (it != m_vHull.end()) {

			posTo = *it++;

			canvas.draw_line(
				(float)posFrom._x, (float)posFrom._y,
				(float)posTo._x, (float)posTo._y,
				clan::Colorf(0.0f, 0.0f, 1.0f)
			);

			posFrom = posTo;

		} // while - iterate hull points

		//posTo = m_vHull[0];
		//CL_Draw::line(m_gc, 
		 //       (float)posFrom._x, (float)posFrom._y,
		 //       (float)posTo._x, (float)posTo._y,
		 //       CL_Colorf(0.0f, 0.0f, 1.0f));
	}

} // render()

void App::createRandomPoints(int in_count) {
	boost::mt19937 rng((boost::int32_t)clock());
	boost::uniform_int<> uniformX(10, WINDOW_WIDTH - 10);
	boost::uniform_int<> uniformY(10, WINDOW_HEIGHT - 10);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > generateX(rng, uniformX);
	boost::variate_generator<boost::mt19937&, boost::uniform_int<> > generateY(rng, uniformY);

	for (int i = 0; i < in_count; ++i) {
		m_vPoints.push_back(Position2D(generateX(), generateY()));
	}
}

void App::calculateHull(int in_step)
{
	// return if list is empty
	if (m_vPoints.size() == 0) return;

	// get maximal number of points
	int iMaxStep = in_step == 0 ? m_vPoints.size() : in_step;

	int index_minimum = 0;
	int index_maximum = 0;

	m_vHull.clear();

	int i = 0;

	// find min and max
	for (std::vector<Position2D>::iterator it = m_vPoints.begin();
		it != m_vPoints.end();
		++it)
	{
		// it there are more points with same y values
		if ((*it)._y > m_vPoints[index_minimum]._y)
			index_minimum = i;
		if ((*it)._y == m_vPoints[index_minimum]._y)
			if ((*it)._x < m_vPoints[index_minimum]._x)
				index_minimum = i;

		if ((*it)._y < m_vPoints[index_maximum]._y)
			index_maximum = i;
		if ((*it)._y == m_vPoints[index_maximum]._y)
			if ((*it)._x > m_vPoints[index_maximum]._x)
				index_maximum = i;
		++i;
	}

	// add min to hull
	m_vHull.push_back(m_vPoints[index_minimum]);

	// get min angle for all points and add to hull as long as we are at the min value
	Position2D achse(1, 0);
	int index = index_minimum;	// next point to check
	int next_index;
	double length;
	double angle;

	int j = 0;
	for (; j < iMaxStep; ++j)
	{
		// initialize angle with max (2 * pi)
		angle = 2 * PI;
		length = 99999.0f;
		// compute angle for every point that is not this point
		for (unsigned int i = 0; i < m_vPoints.size(); ++i)
		{
			if (m_vPoints[i] != m_vPoints[index])
			{
				Position2D pos = m_vPoints[i] - m_vPoints[index];
				double a = pos.getAngle(achse);
				if (a < angle)
				{
					angle = a;
					length = pos.getLength();
					next_index = i;
				}
				// if points are on line -> choose nearest one
				if (a == angle)
				{
					if (pos.getLength() < length)
					{
						length = pos.getLength();
						next_index = i;
					}
				}
			}
		}
		index = next_index;
		// if we are at max value -> choose negative axis
		if (index == index_maximum)
			achse._x = -1;
		if (index == index_minimum)
			break;
		m_vHull.push_back(m_vPoints[index]);
	}

	// loop was interrupted before reaching iMaxStep
	// we found a hull
	if (j < iMaxStep) {
		m_vHull.push_back(m_vHull[0]);
	}
}

void App::calculateHullStep(void)
{
	m_vHull.clear();
	calculateHull(++m_currentStep);
}