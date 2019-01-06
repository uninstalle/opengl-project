#include "fighter.h"
#include "ammunition.h"
#include <set>
void synchronizeTime();
float getDeltaTime();
void check(std::set<Fighter*> &fighters, std::set<Ammunition*> &ammunition);