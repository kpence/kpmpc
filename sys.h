/* forward declarations */
#define CMD_SIZE 256
bool initSys();
bool loopSys();
void termSys();

/* struct declaration */
struct Map {
    /* variables */
    std::string str;
    std::string remap;
    bool whole;

    /* functions */
    Map(std::string _str, std::string _remap, bool _whole = true): str(_str), remap(_remap), whole(_whole) {};
};

/* class declaration */
class Sys {
private:
    void reset();
protected:
public:
    /* variables */
    sf::RenderWindow app;
    sf::Event event;
    std::string cmd;
    std::string search;
    std::vector<Map> map;
    std::vector<std::string> sl;
    std::vector<std::string> artPath;
    int tileSize;
    int searchFlags;

    /* main functions */
    Sys();
    void build();
    void buildConfig();
    ~Sys();
    void clean();
    void loop();
    
    /* input functions */
    bool onKeyPress(sf::Event::KeyEvent *ke);
    bool isMap();
    bool remapCmd(std::string &_cmd);
    bool testCmd(std::string &_cmd);

    /* music */
    bool addAlbum(const char *a);
    void sortAlbums(mpd_tag_type type);

    /* grid functions */

    /* search/sel functions */
    void searchNext(std::string _search, int _flags, bool realTime, bool wrap = true);
    void selNext(bool wrap = false);
    void selPrev(bool wrap = false);
} _sys;
Sys *sys;
