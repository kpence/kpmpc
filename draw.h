/* draw.h */
/* http://sfml-dev.org/tutorials/1.6/graphics-sprite.php */

/* forward declations */
bool initDraw(int &_argc, char **_argv);
bool loopDraw();
void termDraw();

/* class */
class Draw {
private:
    //void initBar();
protected:
public:
    /* variables */
    unsigned int viewY;
    int sel;
    sf::Font font;
    sf::View view;

    /* main functions */
    Draw();
    void build();
    ~Draw();
    void clean();
    void loop();

    /* other */
    void setText(const char *_text);
    void drawBar();
    void updateBar();
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
    void scrollDown();
    void scrollUp();

    /* grid */
    int getWidth();
    int getHeight();

    /* draw */
    void drawAlbums();
    void drawStatus();
} _draw;
Draw *draw;
