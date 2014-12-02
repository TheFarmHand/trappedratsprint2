
#include "Items.h"


Items::Items()
{
}


Items::~Items()
{
}
bool Items::operator==(Items &rhs)
{
	return name == rhs.name;
}