/* TODO Make albums sort by year instead of alphabetically
 *
 * KPMPC see LICENSE for copyright information (MIT license)
 * In order for this to work I think you'll need mpdclient and SFML 1.6
*/

/* includes */
#include <iostream>
#include <cmath>
#include <cstring>
#include <mpd/client.h> /* http://libmpdclient.sourcearchive.com/documentation/2.1-1/example_8c-source.html */
#include <fstream>
#include <SFML/Graphics.hpp> /* http://www.sfml-dev.org/documentation/1.6/index.php */
#include <boost/filesystem.hpp>

/* main header */
#include "main.h"

/* helpers */
//#include "config.h"
//#include "list.h"

/* headers */
#include "sys.h"
#include "album.h"
#include "mpd.h"
#include "draw.h"

/* sources */
//#include "list.cpp"
#include "sys.cpp"
#include "album.cpp"
#include "mpd.cpp"
#include "draw.cpp"

int main(int argc, char **argv) {
    /* initialization */
    if (!initSys())
        return -1;
    if (!initMpd())
        return -1;
    if (!initDraw(argc, argv))
        return -1;
    if (!initAlbum())
        return -1;

    /* loop */
    while (isRunning) {
        if (!loopSys())
            return -1;
        if (!loopMpd())
            return -1;
        if (!loopDraw())
            return -1;
        if (!loopAlbum())
            return -1;
    }

    /* termination */
    termSys();
    termMpd();
    termDraw();
    termAlbum();
    return 0;
}
