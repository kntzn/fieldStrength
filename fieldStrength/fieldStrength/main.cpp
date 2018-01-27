#include <SFML/Graphics.hpp>
#include <list>

#define K 9000000000 // = [N*m^2/C^2]

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

		void draw(sf::RenderWindow &window)
			{
			sf::CircleShape cs;
			cs.setPosition(r);
			cs.setFillColor(sf::Color(255*(charge > 0), 255*(charge < 0), 255*(charge < 0)));
			cs.setRadius(pow (fabs (charge)*100, 0.33333f));
			cs.setOrigin(cs.getRadius() / 2.f, cs.getRadius() / 2.f);
			window.draw(cs);
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
	
	float newCharge_charge = 0;
	float maxStrength = 10000000;

	while (window.isOpen ())
		{
		sf::Event windowEvent;

		sf::Vector2i MousePos = sf::Mouse::getPosition (window);
		sf::Vector2f PointerPos = window.mapPixelToCoords (MousePos);


		while (window.pollEvent (windowEvent))
			{
			if (windowEvent.type == sf::Event::Closed)
				window.close ();
			if (windowEvent.type == sf::Event::MouseButtonPressed)
				{
				if (windowEvent.key.code == sf::Mouse::Left)
					{
					charges.push_back (Charge (PointerPos, newCharge_charge));
					}
				}
			}
		// Mouse

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Equal) && newCharge_charge <  1)
			newCharge_charge += 0.01f;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Dash)  && newCharge_charge > -1)
			newCharge_charge -= 0.01f;

		// Graphics
		window.clear ();
		for (unsigned int x = 0; x < window.getSize ().x; x += 20)
			for (unsigned int y = 0; y < window.getSize ().y; y += 20)
				{
				sf::Vector2f dir;
				for (auto a : charges)
					dir += K*a.getCharge ()/pow (getVecL (sf::Vector2f (x, y) - a.getR ()), 3.f) * (sf::Vector2f (x, y) - a.getR ());
					
				float strength = getVecL (dir);
				dir /= strength / 20;
				sf::Vertex line [] =
					{
					sf::Vertex (sf::Vector2f (x, y),     sf::Color (255, 127, 127, 255*pow (strength/maxStrength, 0.333f))),
					sf::Vertex (sf::Vector2f (x, y)+dir, sf::Color (127, 255, 255, 255*pow (strength/maxStrength, 0.333f)))
					};
				window.draw (line, 2, sf::PrimitiveType::LineStrip);
				}

		
		sf::CircleShape cs;
		cs.setPosition (PointerPos);
		cs.setFillColor (sf::Color (0, 255, 0));
		cs.setRadius (pow (fabs (newCharge_charge)*100, 0.33333f));
		cs.setOrigin (cs.getRadius () / 2.f, cs.getRadius () / 2.f);
		window.draw (cs);

		for (auto a : charges)
			a.draw(window);

		window.display();
		}
	}