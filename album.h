/* album.h */

/* forward declarations */
bool initAlbum();
bool loopAlbum();
void termAlbum();

/* class declarations */
class Album {
private:
    void reset();
protected:
public:
    /* helper variables */
    bool success;

    /* variables */
    sf::Sprite spr;
    sf::Texture tex;
    float selAnimY;
    std::string dir;
    std::string artist;
    std::string date;
    std::string imgDir;
    std::string name;

    /* main functions */
    Album(const char *_name);
    void build();
    ~Album();
    void clean();
    void loop();
    void buildTags();

    /* grid/anim functions */
    void selAnim();
    unsigned int getBottomY();
    unsigned int getX();
    unsigned int getY(bool fromView);
    unsigned int getY();

    /* dir functions */
    void loadArt(std::string _dir, std::string path);
    const char *dirTrim(std::string _dir);
    void printDir(bool recurs);
    void printDir();
};

/* helper struct for sorting */
bool helper_sort(Album i, Album j) {
    if (i.artist == j.artist)
        return (i.name < j.name);
    return (i.artist < j.artist);
};
