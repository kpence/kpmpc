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
/* using */
enum InputType {INPUT_KEYBOARD, INPUT_MOUSE, INPUT_JOYSTICK};
enum modeType {MODE_NORMAL, MODE_TYPING_FILTER};
struct KeyCmd {
    InputType inputType;
    sf::Event::EventType eventType;
    sf::Key::Code keyCode;
    sf::Mouse::Button mouseButton;
};
//
void cleanUp();
//
/////////////////////////////////////////////////////////////////////
// CONFIG
/////////////////////////////////////////////////////////////////////
/*class Config {
private:  CONFIG 
    std::string line;
    const char *config;
protected:  CONFIG 
public:  CONFIG 
    Config() {
        ifstream myfile ("config");
        config = ctrl::config;
    };
    ~Config() {
        delete [] config;
        config = NULL;
    };
    void store_line(std::string, std::string) {
    };

    void readConfig() {
        if (myfile.is_open()) {
            while (std::getline(is_file, line)) {
                std::istringstream is_line(line);
                std::string key;
                if (std::getline(is_line, key, '=')) {
                    std::string value;
                    if (std::getline(is_line, value))
                        //store_line(key, value);
                }
            }
            myfile.close();
        
    };
};*/
/////////////////////////////////////////////////////////////////////
// ALBUM
/////////////////////////////////////////////////////////////////////
class Album {
private: /* ALBUM */
    unsigned int num;
    sf::Sprite *spr;
    sf::Image *img;
    Album *next;
    char *name;
    char *dir;
    char *artist;
    char *date;
    char *imgDir;
    float selAnimY;
protected: /* ALBUM */
public: /* ALBUM */
    char *getName();
    char *getDir();
    const char *getArtist();
    const char *getDate();
    char *dirTrim();
    bool loadImg();
    bool loadImg(const char *filename, bool _success);
    void selAnim();
    Album(const char *_name);
    ~Album();
    Album *mkAlbum(const char *_name);
    unsigned int getBottomY();
    unsigned int getNum();
    void setNum();
    void setNum(unsigned int _num);
    void printDir(bool recurs);
    void printDir();
    unsigned int getX();
    unsigned int getY(bool fromView);
    unsigned int getY();
    bool isImg();
    Album *getPrev(Album *_from);
    void setNext(Album *_next);
    Album *getLast();
    Album *getNext();
    sf::Sprite *getSpr();
    void setImg();
    void drawSpr();
}; /* END OF ALBUM */
/////////////////////////////////////////////////////////////////////
// DRAW
/////////////////////////////////////////////////////////////////////
class Draw { // http://sfml-dev.org/tutorials/1.6/graphics-sprite.php
private: /* DRAW */
    sf::VideoMode *videoMode;
    sf::RenderWindow *app;
    sf::Event *event;
    sf::Shape *bar;
    sf::Font *font;
    sf::String *text;
    unsigned int viewY;
    bool running;
    std::map<std::string, KeyCmd> keyCmd;
    KeyCmd key;
    void setKeys();
    bool testEvent(KeyCmd k, sf::Event *e);
    void initBar();
protected: /* DRAW */
public: /* DRAW */
    void initLoop();
    Draw();
    ~Draw();
    void setText(const char *_text);
    void drawBar();
    void updateBar();
    unsigned int getViewY();
    float getWidth();
    float getHeight();
    void setTitle(const char *_title);
    void drawSpr(sf::Sprite *spr);
    void selHMiddle();
    void selVMiddle();
    void selTop();
    void selBottom();
    void selEnd();
    void selBegin();
    void selBeginTop();
    void selBeginBottom();
}; /* END OF DRAW */
/////////////////////////////////////////////////////////////////////
// CONTROL
/////////////////////////////////////////////////////////////////////
class Control {
private: /* CONTROL */
    mpd_connection *conn;
    //sf::Image placeholder;
    bool errStatus;
    Draw *draw;
    //Config *config;
    Album *albums;
    Album *sel;
    Album *mkAlbum(const char *name);
    int mode;
    char *filter;
    void err();
    bool update(mpd_tag_type type, const char *value);
protected: /* CONTROL */
public: /* CONTROL */
    bool addAlbum(char *_album);
    bool addAlbum(Album *_sel);
    void addSongList();
    void addAlbum();
    bool isFirst(Album *_album);
    bool isSel(Album *_album);
    unsigned int getColNum();
    unsigned int getRowNum();
    void initDraw(int &argc, char **argv);
    void searchPrev();
    void searchNext();
    void selPrev(bool wrap = false);
    void selNext(bool wrap = false);
    bool isSelNext();
    Control();
    ~Control();
    float getViewHeight();
    float getViewWidth();
    Album *getSel();
    int getMode();
    void setMode(int _mode);
    const char *getFilter();
    void rmFilter();
    void charCatFilter(const char _filter);
    unsigned int getSelY();
    unsigned int getSelX();
    unsigned int getBottomY();
    unsigned int getBottomViewY();
    void play();
    void drawSprs();
    void drawSpr(sf::Sprite *spr);
    void loadImg();
    bool isErr();
    Draw *getDraw();
    char *getTag(const char *_album, mpd_tag_type type);
    char *getDir(const char *_album);
    void printDirs();
    void setImgs();
    void sortAlbums(mpd_tag_type type = MPD_TAG_ARTIST);
    struct SongList {
        char *node;
        SongList *next;
        SongList(const char *_node);
        ~SongList();
        void mkSongList(const char *_node);
        void add();
    };
    SongList *sl;
}; /*END OF CONTROL*/
/////////////////////////////////////////////////////////////////////
// FUNCTIONS
/////////////////////////////////////////////////////////////////////
bool getInput();
void cleanUp();
namespace ctrl {
    Control *control;
    //const char config[] = "# list of configurations in README\nmusic_dir=\nmain.exe\n";
};
