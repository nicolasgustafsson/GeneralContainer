#include <iostream>
#include "Container.h"
#include "ContainerRequirements.h"

int main()
{
    Container<int, Iterable, StableElements, ConstantDeletion> container;

	container.Add(45);
	container.Add(333);
	container.Add(12);
	container.Add(5);
	container.Add(12);

    for (auto&& bla : container)
    {
		std::cout << bla << std::endl;
    }
    std::cin.get();
}
