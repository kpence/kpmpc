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
    std::vector<Map> map;
    std::vector<std::string> sl;

    /* main functions */
    Sys();
    void build();
    void buildMap();
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
} _sys;
Sys *sys;
