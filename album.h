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

    /* grid/anim functions */
    void selAnim();
    unsigned int getBottomY();
    unsigned int getX();
    unsigned int getY(bool fromView);
    unsigned int getY();

    /* dir functions */
    const char *dirTrim(std::string _dir);
    void printDir(bool recurs);
    void printDir();
};
