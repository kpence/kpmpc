// KPMPC see LICENSE for copyright information (MIT license)
//
/* constants */
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_WIDTH_FLOAT 800.f
#define WIN_HEIGHT_FLOAT 600.f
#define ALBUM_WIN_WIDTH_FLOAT (WIN_WIDTH_FLOAT - 0.f)
#define ALBUM_WIN_HEIGHT_FLOAT (WIN_HEIGHT_FLOAT - 48.f)
#define IS_GUI true
#define DIR_BASE "/var/lib/mpd/music/"
#define CONFIG_DIR "/var/lib/mpd/music/"

/* flags */
#define HAS_FLAG(list, flag) ((list & flag) == flag)
#define BIT_MASK(flags) ((1 << sizeof(flags)) - 1)
enum sFlag {        S_ALBUM =       1 << 0,
                    S_ARTIST =      1 << 1,
                    S_TRACK =       1 << 2,
                    S_GENRE =       1 << 3,
                    S_YEAR =        1 << 4,
                    S_REVERSE =     1 << 5,
           };

/* typedefs and enums */
enum InputType {INPUT_KEYBOARD, INPUT_MOUSE, INPUT_JOYSTICK};
enum modeType {MODE_NORMAL, MODE_TYPING_SEARCH};

/* using */
using std::cout;
using std::cin;
using std::endl;

/* union declarations */
union Arg {
    int i;
    const char *s;
    unsigned int ui;
    float f;
    const void *v;
};

/* forward declarations */
void cleanUp();

/* variables declarations */
bool isRunning = true;
