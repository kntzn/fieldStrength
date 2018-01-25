#include <SFML/Graphics.hpp>
#include <list>

#define K 1000000

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
			cs.setFillColor(sf::Color(255, 0, 255));
			cs.setRadius(pow (fabs (charge), 0.33333f));
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
	charges.push_back (Charge (sf::Vector2f (405, 405),  10));
	charges.push_back (Charge (sf::Vector2f (1205, 405), -10));

	while (window.isOpen())
		{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
			{
			if (windowEvent.key.code == sf::Event::Closed)
				window.close();
			}

		window.clear();
		for (unsigned int x = 0; x < window.getSize().x; x += 20)
			for (unsigned int y = 0; y < window.getSize().y; y += 20)
				{
				sf::Vector2f dir;
				for (auto a : charges)
					{
					dir -= K*a.getCharge()/pow (getVecL (sf::Vector2f (x, y) - a.getR()), 3.f) * (a.getR() - sf::Vector2f(x, y));	
					}
				dir /= getVecL(dir) / 10;

				sf::Vertex line [] =
					{
					sf::Vertex(sf::Vector2f(x, y), sf::Color (255, 255, 255, 127)),
					sf::Vertex(sf::Vector2f(x, y)+dir, sf::Color(255, 255, 255, 127))
					};
				window.draw(line, 2, sf::PrimitiveType::LineStrip);
				}
		for (auto a : charges)
			a.draw(window);

		window.display();
		}
	}