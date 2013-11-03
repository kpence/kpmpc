// KPMPC see LICENSE for copyright information (MIT license)
/* includes */
#include <iostream>
#include <cmath>
#include <cstring>
#include <mpd/client.h> // http://libmpdclient.sourcearchive.com/documentation/2.1-1/example_8c-source.html
#include <fstream>
#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/documentation/1.6/index.php
#include <boost/filesystem.hpp>
/* constants */
#define MPD_MUSIC_DIR "/var/lib/mpd/music/"
#define SPR_PLACEHOLDER "/home/kpence/music/mpd/placeholder.jpg"
#define WIN_WIDTH_FLOAT 800.f
#define WIN_HEIGHT_FLOAT 600.f
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
/* using */
using std::cout;
using std::cin;
using std::endl;
enum InputType {INPUT_KEYBOARD, INPUT_MOUSE, INPUT_JOYSTICK};
class Control;
class Draw;
struct KeyCmd {
    InputType inputType;
    sf::Event::EventType eventType;
    sf::Key::Code keyCode;
    sf::Mouse::Button mouseButton;
};
namespace ctrl {
    Control *control;
};
using ctrl::control;
//
void cleanUp();
//
class Album {
private:
    unsigned int num;
    sf::Sprite *spr;
    sf::Image *img;
    Album *next;
    char *name;
    char *dir;
    char *imgDir;
protected:
public:
    char *getName();
    char *getDir();
    bool loadImg();
    Album(const char *_name) {
        next = NULL;
        dir = NULL;
        imgDir = NULL;
        img = NULL;
        spr = NULL;
        num = 0;
        name = new char[128];
        strcpy(name, _name);
        img = new sf::Image;
        spr = new sf::Sprite;
        cout << "Album instantiated: " << getName() << endl;
    };
    ~Album() {
        cout << "Album deleted: " << getName() << endl;
        delete [] name;
        delete [] dir;
        delete img;
        delete spr;
        delete next;
        delete [] imgDir;
        dir = NULL;
        next = NULL;
        img = NULL;
        spr = NULL;
        imgDir = NULL;
    };
    Album *mkAlbum(const char *_name) {
        Album *ret;
        if (next) {
            next->num = num + 1;
            ret = next->mkAlbum(_name);
        } else
            ret = next = new Album(_name);
        return (ret);
    };
    unsigned int getBottomY() {
        if (next)
            return (next->getBottomY());
        return (getY());
    };
    unsigned int getNum() {
        return (num);
    };
    void setNum();
    void setNum(unsigned int _num);
    void printDir(bool recurs);
    void printDir() {
        printDir(false);
    };
    unsigned int getX();
    unsigned int getY(bool fromView);
    unsigned int getY() {
        return (getY(false));
    };
    bool isImg() {
        if (strcmp(imgDir, "<No image>") == 0)
            return false;
        return true;
    };
    Album *getPrev(Album *_from) {
        if (!next)
            return (this);
        if (next == _from)
            return (this);
        return (next->getPrev(_from));
    };
    Album *getNext() {
        return (next);
    };
    sf::Sprite *getSpr() {
        return (spr);
    };
    void setImg();
    void drawSpr();
};
class Draw { // http://sfml-dev.org/tutorials/1.6/graphics-sprite.php
private:
    sf::RenderWindow *app;
    unsigned int viewY;
    bool running;
    sf::Event *event;
    std::map<std::string, KeyCmd> keyCmd;
    KeyCmd key;
    void setKeys() {
        key.inputType = INPUT_KEYBOARD;
        key.eventType = sf::Event::KeyPressed;
        key.keyCode = sf::Key::J;
        keyCmd["Down"] = key;
        key.inputType = INPUT_KEYBOARD;
        key.eventType = sf::Event::KeyPressed;
        key.keyCode = sf::Key::K;
        keyCmd["Up"] = key;
        key.inputType = INPUT_KEYBOARD;
        key.eventType = sf::Event::KeyPressed;
        key.keyCode = sf::Key::H;
        keyCmd["Left"] = key;
        key.inputType = INPUT_KEYBOARD;
        key.eventType = sf::Event::KeyPressed;
        key.keyCode = sf::Key::L;
        keyCmd["Right"] = key;
    };
    bool testEvent(KeyCmd k, sf::Event *e) {
        if (!running)
            return (false);
        if (k.inputType == INPUT_MOUSE && k.eventType == e->Type && k.mouseButton == e->MouseButton.Button)
            return (true);
        if (k.inputType == INPUT_KEYBOARD && k.eventType == e->Type && k.keyCode == e->Key.Code)
            return (true);
        return (false);
    };
protected:
public:
    void initLoop();
    Draw() {
        viewY = 0;
        running = true;
        app = new sf::RenderWindow(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "KPMPC");
        event = new sf::Event();
        setKeys();
        //initLoop();
    };
    ~Draw() {
        delete app;
        delete event;
    };
    unsigned int getViewY() {
        return (viewY);
    };
    float getWidth() {
        return(float(app->GetWidth()));
    };
    float getHeight() {
        return(float(app->GetHeight()));
    };
    void setTitle(const char *_title) {
        // not sure if you can do this in SFML 1.6 but I'm too lazy to get 2.0
    }
    void drawSpr(sf::Sprite *spr) {
        //cout << "draw drawing spr" << endl;
        app->Draw(*spr);
    };
};
class Control {
private:
    mpd_connection *conn;
    //sf::Image placeholder;
    bool errStatus;
    Draw *draw;
    Album *albums;
    Album *sel;
    Album *mkAlbum(const char *name) {
        if (albums)
            return (albums->mkAlbum(name));
        else {
            sel = albums = new Album(name);
            return (albums);
        }
    };
    void err() {
        cout << "Err" << endl;
        fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
        mpd_connection_free(conn);
        errStatus = true;
        //cleanup();
    };
    bool update(mpd_tag_type type, const char *value) {
        mpd_pair *pair;
        delete albums;
        albums = NULL;
        if (!mpd_search_db_tags(conn, MPD_TAG_ALBUM)) {
            err();
            return false;
        }
        if (strcmp(value, "") != 0) {
            if (!mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT, type, value)) {
                err();
                return false;
            }
        }
        if (!mpd_search_commit(conn)) {
            err();
            return false;
        }
        while ((pair = mpd_recv_pair_tag(conn, MPD_TAG_ALBUM)) != NULL) {
                mkAlbum(pair->value);
                mpd_return_pair(conn, pair);
        }
        //delete pair;
        if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
            err();
            return false;
        }
        if (!mpd_response_finish(conn)) {
            err();
            return false;
        }

        cout << "Successful Update" << endl;
        albums->setNum();
        return true;
    };
    struct SongList {
        char *node;
        SongList *next;
        SongList(const char *_node) {
            node = new char[255];
            strcpy(node, _node);
            next = NULL;
        };
        ~SongList();
        void mkSongList(const char *_node) {
            if (next)
                next->mkSongList(_node);
            else
                next = new SongList(_node);
        }
        void add() {
            if (next)
                next->add();
            control->addAlbum(node);
        };
    };
    SongList *sl;
protected:
public:
    bool addAlbum(char *_album) {
        cout << "running add" << endl;
        return (mpd_run_add(conn, _album));
    };
    bool addAlbum(Album *_sel) {
        mpd_song *song;
        if (!mpd_search_db_songs(conn, false)) {
            err();
            return false;
        }
        cout << "Searched db songs" << endl;
        if (!mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT, MPD_TAG_ALBUM, _sel->getName())) {
            err();
            return false;
        }
        cout << "Adding tag constraint" << endl;
        if (!mpd_search_commit(conn)) {
            err();
            return false;
        }
        cout << "Searching commit" << endl;
        while ((song = mpd_recv_song(conn)) != NULL) {
            if (!sl)
                sl = new SongList(mpd_song_get_uri(song));
            else
                sl->mkSongList(mpd_song_get_uri(song));
            mpd_song_free(song);
        }
        if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
            err();
            return false;
        }
        if (!mpd_response_finish(conn)) {
            err();
            return false;
        }
        return true;
    };
    void addSongList() {
        if (sl) {
            cout << "Adding Songlist" << endl;
            sl->add();
            delete sl;
            sl = NULL;
        }
    };
    void addAlbum() {
        mpd_run_clear(conn);
        cout << "Cleared playlist" << endl;
        addAlbum(sel);
    };
    bool isFirst(Album *_album) {
        if (_album == albums)
            return true;
        return false;
    };
    bool isSel(Album *_album) {
        if (_album == sel)
            return true;
        return false;
    };
    unsigned int getColNum() {
    // returns the number of collumns of albums that will be displayed
        return (ceil(getViewWidth() / 160));
    };
    unsigned int getRowNum() {
    // returns the number of rows of albums that will be displayed
        return (ceil(getViewHeight() / 160));
    };
    void initDraw(int &argc, char **argv);
    void selPrev() {
        if (sel != albums)
            sel = albums->getPrev(sel);
        cout << "\n< Selecting previous album which is >:" << endl;
        sel->printDir();
        draw->setTitle(sel->getName());
        cout << "< / Selecting previous DONE >" << endl;
    };
    void selNext() {
        if (sel->getNext())
            sel = sel->getNext();
        cout << "\n< Selecting next album which is >:" << endl;
        sel->printDir();
        cout << "< / Selecting next DONE >" << endl;
    };
    bool isSelNext() {
        if (sel->getNext())
            return true;
        return false;
    };
    Control() {
        cout << "Control instantiating" << endl;
        conn = mpd_connection_new(NULL, 0, 30000);
        errStatus = false;
        albums = NULL;
        sl = NULL;
        if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
            err();
        else {
            cout << "Control instantiated and updated" << endl;
            update(MPD_TAG_ALBUM, "");
        }
    };
    ~Control() {
        delete draw;
        delete albums;
        albums = NULL;
        mpd_connection_free(conn);
        conn = NULL;
        cout << "\n~Good bye~\n" << endl;
    };
    float getViewHeight() { // returns height of view in pixels
        return (draw->getHeight());
    };
    float getViewWidth() { // returns width of view in pixels
        return (draw->getWidth());
    };
    unsigned int getSelY() { // returns the Y position of the selected album
        return (sel->getY());
    };
    unsigned int getSelX() { // returns the X position of the selected album
        return (sel->getX());
    };
    unsigned int getBottomY() { // returns the Y position of the last album in the album list
        return (albums->getBottomY());
    };
    unsigned int getBottomViewY() { // returns the minimum Y position of view
        return (getBottomY() - getRowNum() + 1);
    };
    void play() { // Makes mpd start playing the playlist
        mpd_run_play(conn);
    };
    void drawSprs() { // recursively draws all the album's art
        albums->drawSpr();
    };
    void drawSpr(sf::Sprite *spr) { //
        draw->drawSpr(spr);
    };
    void loadImg() { //
        albums->loadImg();
    };
    bool isErr() { // Checks for error
        return (errStatus);
    };
    Draw *getDraw() { // returns pointer to Draw class
        return (draw);
    };
    char *getDir(char *_album) { // returns directory (sans the library directory prefix which is defined in MPD_MUSIC_DIR) of first song in an album from given album name
        char *path;
        path = new char[255];
        mpd_song *song;
        if (!mpd_search_db_songs(conn, false)) {
            err();
            return NULL;
        }
        if (!mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT, MPD_TAG_ALBUM, _album)) {
            err();
            return NULL;
        }
        if (!mpd_search_commit(conn)) {
            err();
            return NULL;
        }
        if ((song = mpd_recv_song(conn)) != NULL)
            strcpy(path, mpd_song_get_uri(song));
        mpd_song_free(song);
        if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
            err();
            return NULL;
        }
        if (!mpd_response_finish(conn)) {
            err();
            return NULL;
        }
        return path;
    };
    void printDirs() { // recursively prints all album info
        albums->printDir(true);
    };
    void setImgs() { // recursively set's the album's sprites to cover art image
        albums->setImg();
    };
}; //End of Control
Control::SongList::~SongList() {
    delete next;
    delete [] node;
    if (control->sl == this)
        control->sl = NULL;
};
char *Album::getName() { // returns name of album
    return (name);
}
char *Album::getDir() { // returns directory of album
    if (!dir)
        dir = control->getDir(getName());
    int index = 0;
    while (dir[index] != '\0' && index < 255)
        index++;
    while (dir[index - 1] != '/' && index > 2)
        index--;
    dir[index] = '\0';
    return (dir);
}
unsigned int Album::getX() { // Returns the X position of album
    return (num % control->getColNum());
}
unsigned int Album::getY(bool fromView) { // Returns the Y position of album
    if (fromView)
        return (floor(num / control->getColNum()) - control->getDraw()->getViewY());
    return (floor(num / control->getColNum()));
}
void Album::setNum(unsigned int _num) {
    num = _num;
}
void Album::setNum() {
    if (next) {
        next->setNum(num + 1);
        next->setNum();
    }
}
void Album::printDir(bool recurs) {
    if (next && recurs)
        next->printDir(recurs);
    cout << "Album (#" << getNum() << ") title: " << getName() << endl;
    cout << "   Path: " << getDir() << endl;
    cout << "x, y: " << getX() << ", " << getY() << " / row, column: " << control->getColNum() << ", " << control->getRowNum() << endl;
    cout << "view: " << control->getDraw()->getViewY() << " / " << control->getBottomViewY() <<endl;
    cout << "bottom Y: " << control->getBottomY() << endl;
}
void Album::setImg() {
    if (next)
        next->setImg();
    if (strcmp(imgDir, "<No image>") != 0) {
        cout << "drawing: " << endl;
        printDir(false);
        spr->SetImage(*img);
    }
}
void Album::drawSpr() {
    spr->SetScale(WIN_WIDTH_FLOAT / control->getColNum() / img->GetWidth(), WIN_HEIGHT_FLOAT / control->getRowNum() / img->GetHeight());
    spr->SetPosition(0.f + (spr->GetSize().x * getX()), 0.f + (spr->GetSize().y * getY(true)));
    spr->SetColor(sf::Color(255, 255, 255, 255));
    if (control->isSel(this)) {
        spr->SetColor(sf::Color(255, 255, 255, 128));
    }
    if (next)
        next->drawSpr();
    if (strcmp(imgDir, "<No image>") != 0)
        control->drawSpr(spr);
}
bool Album::loadImg() {
    if (next) {
        next->loadImg();
    }
    char *dirTest = new char[255];
    if (!imgDir) {
        imgDir = new char[255];
        strcpy(imgDir, MPD_MUSIC_DIR);
        strcat(imgDir, getDir());
    }
    strcpy(dirTest, imgDir);
    strcat(dirTest, "front.jpg");
    if (!img->LoadFromFile(dirTest)) {
        strcpy(imgDir, "<No image>");
        delete [] dirTest;
        return false;
    }
    cout << "[!!!!!!!!!!!!!!!!!!!!!!!] Successful: " << imgDir << endl;
    delete [] dirTest;
    return true;
}
void Control::initDraw(int &argc, char **argv) {
    //draw = new Draw(argc, argv);
    draw = new Draw();
    setImgs();
    cout << "Gonna do loop" << endl;
    draw->initLoop();
}
void Draw::initLoop() {
    while (running) {
        // Manage Events
        while (app->GetEvent(*event)) {
            // Using Event normally
 
            // Window closed
            if (event->Type == sf::Event::Closed)
                running = false;
 
            // Key pressed
            if (event->Type == sf::Event::KeyPressed) {
                switch (event->Key.Code) {
                    case sf::Key::Escape:
                        running = false;
                        break;
                    case sf::Key::A:
                        std::cout << "Key A !" << std::endl;
                        control->addAlbum();
                        control->addSongList();
                        control->play();
                        break;
                    default:
                        break;
                }
            }
            // Using Event for binding
            if (testEvent(keyCmd["Up"], event) && control->getSelY() > 0) { // You can use a function
                for (unsigned int i = 0; i < control->getColNum(); i++) {
                    control->selPrev();
                }
            }
            if (testEvent(keyCmd["Down"], event) && control->getSelY() < control->getBottomY()) {
                for (unsigned int i = 0; i < control->getColNum(); i++) {
                    control->selNext();
                }
            }
            if (testEvent(keyCmd["Left"], event)) {
                control->selPrev();
            }
            if (testEvent(keyCmd["Right"], event)) {
                control->selNext();
            }
        }
        app->Clear();
        while ((control->getSelY() - getViewY()) >= (control->getRowNum() - 1) && getViewY() < control->getBottomViewY())
            viewY++;
        while (control->getSelY() - getViewY() <= 0 && getViewY() > 0)
            viewY--;
        control->drawSprs();
        app->Display(); // Display the result
    }
    app->Close();
};
bool getInput() {
    int input;
    cin >> input;
    if (input == 1)
        control->printDirs();
    if (control->isErr() || input == 0)
        return false;
    return true;
};
void cleanUp() {
    cout << "Cleaning up:" << endl;
    delete control;
   //exit(0);
};
// MAIN
int main(int argc, char **argv) {
    control = new Control();
    control->loadImg();
    control->initDraw(argc, argv);
    //while (getInput());
    cleanUp();
    return 0;
}
