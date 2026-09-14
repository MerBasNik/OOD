//
// Created by Вадим Патрушев on 14.09.2026.
//

#include "Ducks.h"

int main()
{
	const auto decoyDuck = DecoyDuck();
	decoyDuck.Display();
	decoyDuck.Quack();
	decoyDuck.Dance();
	decoyDuck.Fly();
	decoyDuck.Fly();
	decoyDuck.Fly();
	std::cout << std::endl;

	const auto mallardDuck = MallardDuck();
	mallardDuck.Display();
	mallardDuck.Quack();
	mallardDuck.Dance();
	mallardDuck.Fly();
	mallardDuck.Fly();
	mallardDuck.Fly();
	std::cout << std::endl;

	const auto modelDuck = ModelDuck();
	modelDuck.Display();
	modelDuck.Quack();
	modelDuck.Dance();
	modelDuck.Fly();
	modelDuck.Fly();
	modelDuck.Fly();
	std::cout << std::endl;

	const auto redheadDuck = RedheadDuck();
	redheadDuck.Display();
	redheadDuck.Quack();
	redheadDuck.Dance();
	redheadDuck.Fly();
	redheadDuck.Fly();
	redheadDuck.Fly();
	std::cout << std::endl;

	const auto rubberDuck = RubberDuck();
	rubberDuck.Display();
	rubberDuck.Quack();
	rubberDuck.Dance();
	rubberDuck.Fly();
	rubberDuck.Fly();
	rubberDuck.Fly();
	std::cout << std::endl;
}