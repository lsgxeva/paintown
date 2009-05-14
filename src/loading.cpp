#include "util/bitmap.h"
#include <math.h>
#include <iostream>

#include "loading.h"
#include "util/font.h"
#include "util/funcs.h"
#include "globals.h"
#include <vector>
#include <pthread.h>
#include "init.h"

using namespace std;

typedef struct pair{
	int x, y;
} ppair;

void * loadingScreen( void * arg ){
	int load_x = 80;
	int load_y = 220;
	string name = Util::getDataPath() + "/fonts/arial.ttf";
	const Font & myFont = Font::getFont( name, 24, 24 );
        /* make this configurable */
	const char * the_string = (arg != NULL) ? (const char *) arg : "Loading...";
	int load_width = myFont.textLength( the_string );
	int load_height = myFont.getHeight( the_string );

	Global::debug( 2 ) << "loading screen" << endl;

	Bitmap work( load_width, load_height );
	Bitmap letters( load_width, load_height );

	letters.fill( Bitmap::MaskColor );
	myFont.printf( 0, 0, Bitmap::makeColor( 255, 255, 255 ), letters, the_string, 0 ); 

	vector< ppair > pairs;
	/* store every pixel we need to draw */
	for ( int x = 0; x < letters.getWidth(); x++ ){
		for ( int y = 0; y < letters.getHeight(); y++ ){
			int pixel = letters.getPixel( x, y );
			if ( pixel != Bitmap::MaskColor ){
				ppair p;
				p.x = x;
				p.y = y;
				pairs.push_back( p );
			}
		}
	}

	const int MAX_COLOR = 200;
	
	int colors[ MAX_COLOR ];
	int c1 = Bitmap::makeColor( 16, 16, 16 );
	int c2 = Bitmap::makeColor( 192, 8, 8 );
	/* blend from dark grey to light red */
	Util::blend_palette( colors, MAX_COLOR / 2, c1, c2 );
	Util::blend_palette( colors + MAX_COLOR / 2, MAX_COLOR / 2, c2, c1 );

	Global::speed_counter = 0;

        { /* force scoping */
            Bitmap background( Global::titleScreen() );
            background.Blit( load_x, load_y, load_width, load_height, 0, 0, work );
            Font::getDefaultFont().printf( 400, 480 - Font::getDefaultFont().getHeight() * 5 / 2 - Font::getDefaultFont().getHeight(), Bitmap::makeColor( 192, 192, 192 ), background, "Paintown version %s", 0, Global::getVersionString().c_str());
            Font::getDefaultFont().printf( 400, 480 - Font::getDefaultFont().getHeight() * 5 / 2, Bitmap::makeColor( 192, 192, 192 ), background, "Made by Jon Rafkind", 0 );
            background.BlitToScreen();
        }
	bool quit = false;

	/* keeps the colors moving */
	static unsigned mover = 0;

	while ( ! quit ){

		bool draw = false;
		if ( Global::speed_counter > 0 ){
			double think = Global::speed_counter;	
			Global::speed_counter = 0;
			draw = true;
			while ( think > 0 ){
				mover = (mover + 1) % MAX_COLOR;
				think -= 1;
			}
		} else {
			Util::rest( 1 );
		}

		if ( draw ){
			for ( vector< ppair >::iterator it = pairs.begin(); it != pairs.end(); it++ ){
				int color = colors[ (it->x - mover + MAX_COLOR) % MAX_COLOR ];
				work.putPixel( it->x, it->y, color );
			}
			/* work already contains the correct background */
			// work.Blit( load_x, load_y, *Bitmap::Screen );
                        work.BlitAreaToScreen( load_x, load_y );
		}

		pthread_mutex_lock( &Global::loading_screen_mutex );
		quit = Global::done_loading;
		pthread_mutex_unlock( &Global::loading_screen_mutex );
	}

	return NULL;
}
