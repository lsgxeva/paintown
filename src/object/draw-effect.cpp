#include "draw-effect.h"

DrawEffect::DrawEffect(const Character * owner, const int level):
owner(owner),
level(level){
}

DrawEffect::~DrawEffect(){
}

bool DrawEffect::compare(const DrawEffect * a, const DrawEffect * b){
    return a->getLevel() < b->getLevel();
}
