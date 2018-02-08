#include <SFML/Graphics.hpp>
#include <list>
#include <sstream>

#define K 9000000000 // = [N*m^2/C^2]

#define fieldVisStep 5
#define offsetVisDegree 5
#define hardness 5

float getVecL(sf::Vector2f vec)
	{
	return sqrt(vec.x*vec.x + vec.y*vec.y);
	}

class Charge
	{
	private:
		sf::Vector2f r;
		float charge = 0;
	public:
		Charge(sf::Vector2f pos, float CHRG)
			{
			r = pos;
			charge = CHRG;
			}

		void draw(sf::RenderWindow &window, sf::Text &text, bool newCharge = false)
			{
			// Charge circle
			sf::CircleShape cs;
			cs.setPosition(r);
			cs.setFillColor(sf::Color(255*(charge > 0 && !newCharge), 255*(charge < 0 || newCharge), 255*(charge < 0 && !newCharge)));
			cs.setRadius(pow (fabs (charge)*100, 0.33333f));
			cs.setOrigin(cs.getRadius() / 2.f, cs.getRadius() / 2.f);
			window.draw(cs);

			// Charge info text
			std::ostringstream str;
			str << charge << " C";
			text.setString(str.str());
			text.setPosition(r + sf::Vector2f(10, 10));
			text.setFillColor(cs.getFillColor ());
			window.draw(text);
			}

		float getCharge()
			{
			return charge;
			}
		sf::Vector2f getR()
			{
			return r;
			}
	};

int main()
	{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Electric field strength");
	std::list <Charge> charges;
	window.setMouseCursorVisible(false);
	
	int displayMode = 0;
	float newCharge_charge = 0;
	float maxStrength = 100;

	sf::Font font;
	font.loadFromFile ("arial.ttf");
	sf::Text text ("", font, 12);

	sf::Vector2u windowSize = window.getSize ();
	sf::Texture texture;
	texture.create (windowSize.x, windowSize.y);

	while (window.isOpen ())
		{
		// Mouse, keyboard & window events
		sf::Event windowEvent;

		sf::Vector2i MousePos = sf::Mouse::getPosition (window);
		sf::Vector2f PointerPos = window.mapPixelToCoords (MousePos);

		bool updateFlag = false;

		while (window.pollEvent (windowEvent))
			{
			if (windowEvent.type == sf::Event::Closed)
				window.close ();
			if (windowEvent.type == sf::Event::MouseButtonPressed)
				{
				if (windowEvent.key.code == sf::Mouse::Left)
					{
					charges.push_back (Charge (PointerPos+sf::Vector2f(0.5f, 0.5f), newCharge_charge));
					updateFlag = true;
					}
				if (windowEvent.key.code == sf::Mouse::Right)
					{
					displayMode = (displayMode + 1) % 2;
					updateFlag = true;
					}
				}
			}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal) && newCharge_charge <  10)
			newCharge_charge += 0.1f;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)  && newCharge_charge > -10)
			newCharge_charge -= 0.1f;

		// Graphics
		window.clear ();

		// Field strenght
		if (updateFlag)
			{
			for (unsigned int x = 0; x < window.getSize().x; x += fieldVisStep)
				for (unsigned int y = 0; y < window.getSize().y; y += fieldVisStep)
					{
					float strength = 0;
					for (auto a : charges)
						strength += K*fabs (a.getCharge()) / pow(getVecL(sf::Vector2f(x, y) - a.getR()), 2.f);
					
					if (strength > maxStrength)
						maxStrength = strength;
					}

			if (displayMode == 0)
				for (unsigned int x = 0; x < window.getSize().x; x += fieldVisStep)
					for (unsigned int y = 0; y < window.getSize().y; y += fieldVisStep)
						{
						sf::Vector2f dir;
						for (auto a : charges)
							dir += K*a.getCharge() / pow(getVecL(sf::Vector2f(x, y) - a.getR()), 3.f) * (sf::Vector2f(x, y) - a.getR());

						float strength = getVecL(dir);
						dir /= strength / fieldVisStep;

						sf::Vertex line[] =
							{
							sf::Vertex(sf::Vector2f(x, y) - dir / 2.f, sf::Color(255, 0, 0, 255 * pow(strength / maxStrength, 1.f / offsetVisDegree))),
							sf::Vertex(sf::Vector2f(x, y) + dir / 2.f, sf::Color(0, 255, 255, 255 * pow(strength / maxStrength, 1.f / offsetVisDegree)))
							};
						window.draw(line, 2, sf::PrimitiveType::LineStrip);
						}
			else if (displayMode == 1)
				for (unsigned int x = 0; x < window.getSize().x; x += fieldVisStep)
					for (unsigned int y = 0; y < window.getSize().y; y += fieldVisStep)
						{
						float strength = 0;
						for (auto a : charges)
							strength += K*fabs (a.getCharge()) / pow(getVecL(sf::Vector2f(x, y) - a.getR()), 2.f);

						sf::RectangleShape rs;
						rs.setSize (sf::Vector2f(fieldVisStep, fieldVisStep));
						rs.setFillColor(sf::Color(255, 255, 255, int (255 / hardness * (pow(strength / maxStrength, 1.f / offsetVisDegree)))*hardness));
						rs.setPosition(x, y);

						window.draw(rs);
						}

			texture.update(window);
			}
		else
			{
			sf::Sprite spr;
			spr.setTexture (texture);
			window.draw(spr);
			}

		// New charge point
		Charge newCharge(PointerPos, newCharge_charge);
		newCharge.draw(window, text, true);

		for (auto a : charges)
			a.draw(window, text);	

		window.display();
		}
	}