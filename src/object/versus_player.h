#ifndef _paintown_versus_player_h
#define _paintown_versus_player_h

#include "player.h"
#include <string>
#include "util/load_exception.h"

class World;
class Object;
class Bitmap;

class VersusPlayer: public Player {
public:
	VersusPlayer( const std::string & str ) throw( LoadException );
	VersusPlayer( int config, const Player & player ) throw( LoadException );
	
	virtual void act( vector< Object * > * others, World * world, vector< Object * > * add );
	
	virtual void draw( Bitmap * work, int rel_x );

	virtual ~VersusPlayer();
protected:

	using Player::getKey;
	virtual int getKey( int x, int facing );

	int config;
};

#endif
