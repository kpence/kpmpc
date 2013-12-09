// TODO Make albums sort by year instead of alphabetically
//
// KPMPC see LICENSE for copyright information (MIT license)
// In order for this to work I think you'll need mpdclient and SFML 1.6
//
/* includes */
#include <iostream>
#include <cmath>
#include <cstring>
#include <mpd/client.h> // http://libmpdclient.sourcearchive.com/documentation/2.1-1/example_8c-source.html
#include <fstream>
#include <SFML/Graphics.hpp> //http://www.sfml-dev.org/documentation/1.6/index.php
#include <boost/filesystem.hpp>
#include "config.h"
#include "main.h"
/* using */
using std::cout;
using std::cin;
using std::endl;
using ctrl::control;
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
Album::Album(const char *_name) {
    next = NULL;
    dir = NULL;
    artist = NULL;
    date = NULL;
    imgDir = NULL;
    img = NULL;
    spr = NULL;
    num = 0;
    selAnimY = 0;
    name = new char[128];
    strcpy(name, _name);
    if (IS_GUI) {
        img = new sf::Image;
        spr = new sf::Sprite;
    }
    cout << "Album instantiated: " << getName() << endl;
}
Album::~Album() {
    cout << "Album deleted: " << getName() << endl;
    delete [] name;
    delete [] dir;
    delete [] artist;
    delete [] date;
    if (IS_GUI) {
        delete img;
        delete spr;
    }
    delete next;
    delete [] imgDir;
    artist = NULL;
    date = NULL;
    dir = NULL;
    next = NULL;
    img = NULL;
    spr = NULL;
    imgDir = NULL;
}
Album *Album::mkAlbum(const char *_name) {
    Album *ret;
    if (next) {
        next->num = num + 1;
        ret = next->mkAlbum(_name);
    } else
        ret = next = new Album(_name);
    return (ret);
}
unsigned int Album::getBottomY() {
    if (!next)
        return (getY());
    return (next->getBottomY());
}
unsigned int Album::getNum() {
    return (num);
}
void Album::printDir() {
    printDir(false);
}
unsigned int Album::getY() {
    return (getY(false));
}
bool Album::isImg() {
    if (strcmp(imgDir, "<No image>") == 0)
        return false;
    return true;
}
Album *Album::getPrev(Album *_from) {
    if (!next)
        return (this);
    if (next == _from)
        return (this);
    return (next->getPrev(_from));
}
void Album::setNext(Album *_next) {
    next = _next;
}
Album *Album::getLast() {
    if (next != NULL)
        return (next->getLast());
    return (this);
}
Album *Album::getNext() {
    return (next);
}
sf::Sprite *Album::getSpr() {
    return (spr);
}
/////////////////////////////////////////////////////////////////////
// DRAW
/////////////////////////////////////////////////////////////////////
void Draw::setKeys() {
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::UP_KEY;
    keyCmd["Up"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::DOWN_KEY;
    keyCmd["Down"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::LEFT_KEY;
    keyCmd["Left"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::RIGHT_KEY;
    keyCmd["Right"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::PGUP_KEY;
    keyCmd["PgUp"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::PGDOWN_KEY;
    keyCmd["PgDown"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::SCROLLUP_KEY;
    keyCmd["ScrollUp"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::SCROLLDOWN_KEY;
    keyCmd["ScrollDown"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::LINEBEGIN_KEY;
    keyCmd["LineBegin"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::LINEEND_KEY;
    keyCmd["LineEnd"] = key;
    //
    key.inputType = INPUT_KEYBOARD;
    key.eventType = sf::Event::KeyPressed;
    key.keyCode = sf::Key::PLAY_KEY;
    keyCmd["Play"] = key;
}
bool Draw::testEvent(KeyCmd k, sf::Event *e) {
    if (!running || control->getMode() != MODE_NORMAL)
        return (false);
    if (k.inputType == INPUT_MOUSE && k.eventType == e->Type && k.mouseButton == e->MouseButton.Button)
        return (true);
    if (k.inputType == INPUT_KEYBOARD && k.eventType == e->Type && k.keyCode == e->Key.Code)
        return (true);
    return (false);
}
Draw::Draw() {
    viewY = 0;
    running = true;
    videoMode = new sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, 32);
    //sf::VideoMode videoMode_;
    //videoMode_ = sf::VideoMode::GetMode(0);
    app = new sf::RenderWindow(*videoMode, "KPMPC");
    event = new sf::Event();
    initBar();
    setKeys();
    //initLoop();
}
Draw::~Draw() {
    app->Close();
    delete font;
    delete text;
    delete bar;
    delete videoMode;
    delete app;
    delete event;
}
void Draw::initBar() {
    bar = new sf::Shape();
    bar->AddPoint(0.f, ALBUM_WIN_HEIGHT_FLOAT, sf::Color::Black, sf::Color::White);
    bar->AddPoint(WIN_WIDTH_FLOAT, ALBUM_WIN_HEIGHT_FLOAT, sf::Color::Black, sf::Color::White);
    bar->AddPoint(WIN_WIDTH_FLOAT, WIN_HEIGHT_FLOAT, sf::Color::Black, sf::Color::White);
    bar->AddPoint(0.f, WIN_HEIGHT_FLOAT, sf::Color::Black, sf::Color::White);
    bar->EnableFill(true);
    bar->SetColor(sf::Color::Black);
    font = new sf::Font();
    if (!font->LoadFromFile(FONT_DIR)) {
        while (true)
            cout << "[!!!!] Font could not load" << endl;
        delete font;
        font = NULL;
        text = NULL;
    }
    else {
        text = new sf::String();
        text->SetFont(*font);
        text->SetSize(100);
        text->SetText("");
        text->SetColor(sf::Color::White);
    }
}
void Draw::drawBar() {
    app->Draw(*bar);
    if (font != NULL) {
        text->SetPosition(5.f, ALBUM_WIN_HEIGHT_FLOAT - (150.f / std::max(control->getViewHeight() - 150.f, 0.f)));
        text->SetScaleX(90 / control->getViewWidth());
        text->SetScaleY(90 / control->getViewHeight());
        app->Draw(*text);
    }
    else
        cout << "WORK" << endl;
}
void Draw::updateBar() {
    char *msg = new char[256];
    strcpy(msg, control->getSel()->getArtist());
    strcat(msg, " - ");
    strcat(msg, control->getSel()->getDate());
    strcat(msg, " - ");
    strcat(msg, control->getSel()->getName());
    if (control->getMode() == MODE_TYPING_FILTER)
        strcat(msg, " - /");
    if (control->getFilter() != NULL) {
        if (strcmp(control->getFilter(), "") != 0) {
            if (control->getMode() != MODE_TYPING_FILTER)
                strcat(msg, " (/");
            strcat(msg, control->getFilter());
            if (control->getMode() != MODE_TYPING_FILTER)
                strcat(msg, ")");
        }
    }
    setText(msg);
    delete [] msg;
    msg = NULL;
}
void Draw::setText(const char *_text) {
    if (font != NULL)
        text->SetText(_text);
}
unsigned int Draw::getViewY() {
    return (viewY);
}
float Draw::getWidth() {
    return(float(app->GetWidth()));
}
float Draw::getHeight() {
    return(float(app->GetHeight()));
}
void Draw::setTitle(const char *_title) {
    // not sure if you can do this in SFML 1.6 but I'm too lazy to get 2.0
}
void Draw::drawSpr(sf::Sprite *spr) {
    //cout << "draw drawing spr" << endl;
    app->Draw(*spr);
}
/////////////////////////////////////////////////////////////////////
// CONTROL
/////////////////////////////////////////////////////////////////////
Control::SongList::SongList(const char *_node) {
    node = new char[255];
    strcpy(node, _node);
    next = NULL;
}
void Control::SongList::mkSongList(const char *_node) {
    if (next)
        next->mkSongList(_node);
    else
        next = new SongList(_node);
}
void Control::SongList::add() {
    cout << "Adding song: " << node << endl;
    control->addAlbum(node);
    if (next)
        next->add();
}
Album *Control::mkAlbum(const char *name) {
    if (albums)
        return (albums->mkAlbum(name));
    else {
        sel = albums = new Album(name);
        return (albums);
    }
}
void Control::err() {
    cout << "Err" << endl;
    fprintf(stderr,"%s\n", mpd_connection_get_error_message(conn));
    mpd_connection_free(conn);
    errStatus = true;
    //cleanup();
}
bool Control::update(mpd_tag_type type, const char *value) {
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
}
bool Control::addAlbum(char *_album) {
    cout << "running add" << endl;
    return (mpd_run_add(conn, _album));
}
bool Control::addAlbum(Album *_sel) {
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
}
void Control::addSongList() {
    if (sl) {
        cout << "Adding Songlist" << endl;
        sl->add();
        delete sl;
        sl = NULL;
    }
}
void Control::addAlbum() {
    mpd_run_clear(conn);
    cout << "Cleared playlist" << endl;
    addAlbum(sel);
}
bool Control::isFirst(Album *_album) {
    if (_album == albums)
        return true;
    return false;
}
bool Control::isSel(Album *_album) {
    if (_album == sel)
        return true;
    return false;
}
unsigned int Control::getColNum() {
// returns the number of collumns of albums that will be displayed
    if (ceil(getViewWidth() / 160) > 0)
        return (ceil(getViewWidth() / 160));
    return 1;
}
unsigned int Control::getRowNum() {
// returns the number of rows of albums that will be displayed
    if (ceil(getViewHeight() / 160) > 0)
        return (ceil(getViewHeight() / 160));
    return 1;
}
int Control::getMode() {
    return mode;
}
void Control::setMode(int _mode) {
    mode = _mode;
    draw->updateBar();
}
Album *Control::getSearchNext(Album *_search) {
    Album *_start = _search;
    if (filter != NULL) {
        if (filter[0] != '\0' && strcmp(filter, "") != 0) {
            if (_search->getNext() != NULL)
                _search = _search->getNext();
            else
                _search = albums;
            while (strstr(_search->getArtist(), getFilter()) == NULL && strstr(_search->getName(), getFilter()) == NULL) {
                if (_search->getNext())
                    _search = _search->getNext();
                else
                    _search = albums;
                if (_search == _start)
                    return NULL;
            }
        }
    }
    return _search;
}
Album *Control::getSearchPrev(Album *_search) {
    Album *_start = _search;
    if (filter != NULL) {
        if (filter[0] != '\0' && strcmp(filter, "") != 0) {
            if (albums->getPrev(_search))
                _search = albums->getPrev(_search);
            else
                _search = albums->getLast();
            while (strstr(_search->getArtist(), getFilter()) == NULL && strstr(_search->getName(), getFilter()) == NULL) {
                if (albums->getPrev(_search))
                    _search = albums->getPrev(_search);
                else
                    _search = albums->getLast();
                if (_search == _start)
                    return NULL;
            }
        }
    }
    return _search;
}
void Control::searchNext() {
    Album *_start = sel;
    if (filter != NULL) {
        if (filter[0] != '\0' && strcmp(filter, "") != 0) {
            selNext(true);
            while (strstr(sel->getArtist(), getFilter()) == NULL && strstr(sel->getName(), getFilter()) == NULL) {
                selNext(true);
                if (sel == _start)
                    break;
            }
        }
    }
}
void Control::searchPrev() {
    Album *_start = sel;
    if (filter != NULL) {
        if (filter[0] != '\0' && strcmp(filter, "") != 0) {
            selPrev(true);
            while (strstr(sel->getArtist(), getFilter()) == NULL && strstr(sel->getName(), getFilter()) == NULL) {
                selPrev(true);
                if (sel == _start)
                    break;
            }
        }
    }
}
void Control::selPrev(bool wrap) {
    if (getSearchPrev(sel) != NULL && isLimit())
        searchPrev();
    else if (!isLimit()) {
        if (sel != albums)
            sel = albums->getPrev(sel);
        else if (wrap)
            sel = albums->getLast();
        draw->updateBar();
        cout << "\n< Selecting previous album which is >:" << endl;
        sel->printDir();
        cout << "< / Selecting previous DONE >" << endl;
    }
}
void Control::selNext(bool wrap) {
    if (getSearchNext(sel) != NULL && isLimit())
        searchNext();
    else if (!isLimit()) {
        if (sel->getNext())
            sel = sel->getNext();
        else if (wrap)
            sel = albums;
        draw->updateBar();
        cout << "\n< Selecting next album which is >:" << endl;
        sel->printDir();
        cout << "< / Selecting next DONE >" << endl;
    }
}
bool Control::isSelNext() {
    if (sel->getNext())
        return true;
    return false;
}
Control::Control() {
    cout << "Control instantiating" << endl;
    mode = MODE_NORMAL;
    filter = new char[256];
    rmFilter();
    limit = false;
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
}
Control::~Control() {
    delete [] filter;
    filter = NULL;
    if (IS_GUI)
        delete draw;
    delete albums;
    //delete config;
    albums = NULL;
    mpd_connection_free(conn);
    conn = NULL;
    cout << "\n~Good bye~\n" << endl;
}
float Control::getViewHeight() { // returns height of view in pixels
    return (draw->getHeight());
}
float Control::getViewWidth() { // returns width of view in pixels
    return (draw->getWidth());
}
void Control::setSel(Album *_sel) { // sets the pointer to the selected album
    sel = _sel;
}
Album *Control::getSel() { // returns the pointer to the selected album
    return (sel);
}
unsigned int Control::getSelY() { // returns the Y position of the selected album
    return (sel->getY());
}
unsigned int Control::getSelX() { // returns the X position of the selected album
    return (sel->getX());
}
unsigned int Control::getBottomY() { // returns the Y position of the last album in the album list
    return (albums->getBottomY());
}
unsigned int Control::getBottomViewY() { // returns the minimum Y position of view
    if ((getBottomY() - getRowNum() + 1) > 3000000000)
        return 0;
    return (getBottomY() - getRowNum() + 1);
}
void Control::rmFilter() {
    cout << "Control::rmFilter()" << endl;
    filter[0] = '\0';
    cout << "Filter: " << filter << endl;
}
const char *Control::getFilter() {
    return (filter);
}
void Control::charCatFilter(const char _filter) {
    if (_filter > 0) {
        if (filter[0] == '\0') {
            printf("strcpy: %i, %c", _filter, _filter);
            if (_filter != 8 && _filter != 23 && _filter != 21 && _filter > 0) {
                filter[0] = _filter;
                filter[1] = '\0';
            }
            else
                setMode(MODE_NORMAL);
        }
        else {
            if (_filter == 8) {
                printf("deleting: %i, %c\n", filter[strlen(filter) - 1], filter[strlen(filter) - 1]);
                filter[strlen(filter) - 1] = '\0';
            }
            else if (_filter == 21)
                rmFilter();
            else if (_filter == 23) {
                while (filter[0] != '\0' && filter[std::max((long unsigned int)0, strlen(filter) - 1)] == ' ')
                    filter[strlen(filter) - 1] = '\0';
                while (filter[0] != '\0' && filter[std::max((long unsigned int)0, strlen(filter) - 1)] != ' ')
                    filter[strlen(filter) - 1] = '\0';
            }
            else if (_filter > 31) {
                strncat(filter, &_filter, 1);
                if (getSearchNext(getSel()) != getSel()->getNext() && getSearchNext(getSel()) != albums->getPrev(getSel()))
                    searchNext();
                else if (getSearchPrev(getSearchNext(getSel())) != getSel())
                    searchNext();
            }
        }
    }
    getDraw()->updateBar();
}
void Control::play() { // Makes mpd start playing the playlist
    mpd_run_play(conn);
}
void Control::drawSprs() { // recursively draws all the album's art
    albums->drawSpr();
    albums->selAnim();
}
void Control::drawSpr(sf::Sprite *spr) { //
    if (IS_GUI) {
        draw->drawSpr(spr);
    }
}
void Control::loadImg() { //
    albums->loadImg();
}
bool Control::isErr() { // Checks for error
    return (errStatus);
}
void Control::toggleLimit() { // returns pointer to Draw class
    if (limit)
        limit = false;
    else
        limit = true;
}
bool Control::isLimit() { // returns pointer to Draw class
    if (filter == NULL)
        return false;
    else {
        if (filter[0] == '\0' && strcmp(filter, "") == 0)
            return false;
    }
    return (limit);
}
Draw *Control::getDraw() { // returns pointer to Draw class
    return (draw);
}
char *Control::getTag(const char *_album, mpd_tag_type type) { // returns tag
    char *tagRet = new char[255];
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
    if ((song = mpd_recv_song(conn)) != NULL) {
        if (mpd_song_get_tag(song, type, 0) != NULL)
            strcpy(tagRet, mpd_song_get_tag(song, type, 0));
        else
            tagRet = NULL;
    }
    mpd_song_free(song);
    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        err();
        return NULL;
    }
    if (!mpd_response_finish(conn)) {
        err();
        return NULL;
    }
    return tagRet;
}
char *Control::getDir(const char *_album) { // returns directory (sans the library directory prefix which is defined in MPD_MUSIC_DIR) of first song in an album from given album name
    char *pathRet = new char[255];
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
        strcpy(pathRet, mpd_song_get_uri(song));
    mpd_song_free(song);
    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        err();
        return NULL;
    }
    if (!mpd_response_finish(conn)) {
        err();
        return NULL;
    }
    return pathRet;
}
void Control::printDirs() { // recursively prints all album info
    albums->printDir(true);
}
void Control::setImgs() { // recursively set's the album's sprites to cover art image
    albums->setImg();
}
/////////////////////////////////////////////////////////////////////
// FUNCTIONS
/////////////////////////////////////////////////////////////////////
void Control::sortAlbums(mpd_tag_type type) {
    static bool isBW = true;
    bool match;
    bool match2;
    isBW = (!isBW);
    Album *_pMax = NULL;
    Album *_max = albums;
    Album *_new = NULL;
    while (albums != NULL) {
        _max = albums;
        for (Album *_albums = albums; _albums != NULL; _albums = _albums->getNext()) {
            match = false;
            match2 = false;
            if (((strcmp(_albums->getArtist(), _max->getArtist()) >= 0 && isBW) || (strcmp(_albums->getArtist(), _max->getArtist()) <= 0 && !isBW)) && (type == MPD_TAG_ARTIST))
                _max = _albums;
            if (strcmp(_albums->getArtist(), _max->getArtist()) == 0 && (type == MPD_TAG_ARTIST || match)) {
                cout << "Found match: " << _max->getArtist() << endl;
                _max = _albums;
                match = true;
            }
            if (((strcmp(_albums->getDate(), _max->getDate()) >= 0 && isBW) || (strcmp(_albums->getDate(), _max->getDate()) <= 0 && !isBW)) && (type == MPD_TAG_DATE || match)) {
                _max = _albums;
                match = false;
                match2 = true;
            }
            if (((strcmp(_albums->getDate(), _max->getDate()) == 0 && isBW) || (strcmp(_albums->getDate(), _max->getDate()) <= 0 && !isBW)) && (type == MPD_TAG_DATE || match2)) {
                _max = _albums;
                match = true;
                match2 = false;
            }
            if (((strcmp(_albums->getName(), _max->getName()) >= 0 && isBW) || (strcmp(_albums->getName(), _max->getName()) <= 0 && !isBW)) && (type == MPD_TAG_ALBUM || match))
                _max = _albums;
        }
        if (_new == NULL) {
            _max->setNum(0);
            _new = _max;
        }
        if (albums == _max)
            albums = _max->getNext();
        else {
            albums->getPrev(_max)->setNext(_max->getNext());
            _max->setNext(NULL);
        }
        if (_new != _max) {
            if (_pMax == NULL)
                cout << "_pMax IS NULL" << endl;
            _max->setNum(_pMax->getNum() + 1);
            _pMax->setNext(_max);
        }
        _pMax = _max;
    }
    albums = _new;
    cout << "Done sorting!" << endl;
}
Control::SongList::~SongList() {
    delete next;
    delete [] node;
    if (control->sl == this)
        control->sl = NULL;
}
char *Album::getName() { // returns name of album
    return (name);
}
char *Album::dirTrim() { // returns directory of album
    int index = 0;
    while (dir[index] != '\0' && index < 254)
        index++;
    index--;
    while (dir[index - 1] != '/' && index > 2)
        index--;
    dir[index] = '\0';
    strcpy(imgDir, MPD_MUSIC_DIR);
    strcat(imgDir, dir);
    cout << "Trimmed dir: " << imgDir << endl;
    return (dir);
}
const char *Album::getDate() { // returns date of album
    if (date == NULL)
        date = control->getTag(getName(), MPD_TAG_DATE);
    if (date == NULL) {
        cout << "getDate: no date, name: " << getName() << endl;
        return ("<No date>");
    }
    return (date);
}
const char *Album::getArtist() { // returns artist of album
    if (artist == NULL)
        artist = control->getTag(getName(), MPD_TAG_ARTIST);
    if (artist == NULL) {
        cout << "getArtist: no artist, name: " << getName() << endl;
        return ("<No Artist>");
    }
    return (artist);
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
void Album::setNum(unsigned int _num) { // Sets album's num (Used to determine its X and Y positions)
    num = _num;
}
void Album::setNum() { // Recusively sets album's num (Used to determine its X and Y positions)
    if (next) {
        next->setNum(num + 1);
        next->setNum();
    }
}
void Album::printDir(bool recurs) { // Prints album's directory and other information
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
void Album::drawSpr() { // Draw album's sprite
    spr->SetScale(ALBUM_WIN_WIDTH_FLOAT / control->getColNum() / img->GetWidth(), ALBUM_WIN_HEIGHT_FLOAT / control->getRowNum() / img->GetHeight());
    spr->SetPosition(0.f + (spr->GetSize().x * getX()), 0.f + (spr->GetSize().y * getY(true)));
    spr->SetColor(sf::Color(255, 255, 255, 255));
    if (control->isSel(this)) {
        spr->SetColor(sf::Color(255, 255, 255, 128));
        spr->Move((selAnimY / 10.f) * 4.f, (selAnimY / 10.f) * 4.f);
    }
    if (next != NULL && !control->isLimit())
        next->drawSpr();
    if (control->getSearchNext(this) != NULL && control->isLimit())
        control->getSearchNext(this)->drawSpr();
    if (strcmp(imgDir, "<No image>") != 0)
        control->drawSpr(spr);
}
bool Album::loadImg(const char *filename, bool _success) { // Load album's image
    if (!IS_GUI) {
        cout << "No gui, no loading image" << endl;
        return (false);
    }
    if (_success) {
        return (_success);
        cout << "!!!!!!!!!!!!!!!!!" << endl << endl;
    }
    else {
        char *_dirTest = new char[255];
        strcpy(_dirTest, imgDir);
        strcat(_dirTest, filename);
        cout << _dirTest << endl;
        if (img->LoadFromFile(_dirTest))
            _success = true;
        delete [] _dirTest;
        _dirTest = NULL;
        return (_success);
    }
}
bool Album::loadImg() { // Recursively load each album's image
    if (next) {
        next->loadImg();
    }
    bool success = false;
    if (!imgDir) {
        imgDir = new char[255];
        strcpy(imgDir, MPD_MUSIC_DIR);
        strcat(imgDir, getDir());
    }
    success = loadImg("front.jpg", success);
    success = loadImg("front.png", success);
    success = loadImg("cover.jpg", success);
    success = loadImg("cover.png", success);
    success = loadImg("folder.jpg", success);
    success = loadImg("folder.png", success);
    dirTrim();
    success = loadImg("front.jpg", success);
    success = loadImg("front.png", success);
    success = loadImg("cover.jpg", success);
    success = loadImg("cover.png", success);
    success = loadImg("folder.jpg", success);
    success = loadImg("folder.png", success);
    if (!success)
        strcpy(imgDir, "<No image>");
    if (strcmp(imgDir, "<No image>") == 0) {
        return false;
    }
    cout << "[!!!!!!!!!!!!!!!!!!!!!!!] Successful: " << imgDir << endl;
    return true;
}
void Album::selAnim() { // Animation for when the album is selected or deselected
    if (next)
        next->selAnim();
    if (selAnimY < 10.f && control->isSel(this))
        selAnimY++;
    if (selAnimY > 0.f && !control->isSel(this))
        selAnimY--;
}
void Control::initDraw(int &argc, char **argv) {
    if (IS_GUI) {
        //draw = new Draw(argc, argv);
        draw = new Draw();
        setImgs();
        cout << "Gonna do loop" << endl;
        sortAlbums();
        draw->initLoop();
    }
}
void Draw::selVMiddle() {
    selBeginBottom();
    //if (control->getRowNum() - 1)
    //    unsigned int _f = (control->getRowNum() - 1) / 2;
    //else
    unsigned int _f = ceil(control->getRowNum() / 2);
    if (_f > 0 && floor(control->getRowNum() / 2) != ceil(control->getRowNum() / 2))
        _f--;
    for (unsigned int f = 0; f < _f; f++) {
        for (unsigned int i = 0; i < control->getColNum(); i++)
            control->selPrev();
    }
}
void Draw::selBottom() {
    while ((control->getSelY() + 1) < (getViewY() + control->getRowNum()) && (control->getSelY() < control->getBottomY()))
        control->selNext();
}
void Draw::selBeginTop() {
    selTop();
    selBegin();
}
void Draw::selBeginBottom() {
    selBottom();
    selBegin();
}
void Draw::selTop() {
    while (control->getSelY() > getViewY())
        control->selPrev();
}
void Draw::selBegin() {
    if (control->getSelY() >= 0) {
        for (unsigned int i = 0; i < control->getColNum(); i++) {
            if (control->getSelX() == 0)
                i = control->getColNum();
            else
                control->selPrev();
        }
    }
}
void Draw::selEnd() {
    if (control->getSelY() <= control->getBottomY()) {
        if (!event->Key.Control) {
            for (unsigned int i = 0; i < control->getColNum(); i++) {
                if (!control->getSel()->getNext())
                    i = control->getColNum();
                else {
                    if (control->getSel()->getNext()->getX() == 0)
                        i = control->getColNum();
                    else
                    control->selNext();
                }
            }
        }
    }
}
void Draw::initLoop() {
    int ignoreFirstChar = 2;
    Album *markSel = NULL;
    while (running) {
        // Manage Events
        while (app->GetEvent(*event)) {
            // Using Event normally
 
            // Window closed
            if (event->Type == sf::Event::Closed)
                running = false;
 
            // Key pressed
            if (event->Type == sf::Event::TextEntered && control->getMode() == MODE_TYPING_FILTER) {
                updateBar();
                char txt = (char)event->Text.Unicode;
                printf("Text: %i, %c\n", txt, txt);
                if (txt == 3) {
                    control->rmFilter();
                    control->setMode(MODE_NORMAL);
                    updateBar();
                    control->setSel(markSel);
                    markSel = NULL;
                } else if (txt == 13 || txt == 10) {
                    control->setMode(MODE_NORMAL);
                    updateBar();
                } else if ((txt != '/' || ignoreFirstChar <= 0) && txt != 31)
                    control->charCatFilter(txt);
                //8 is backspace
            }
            if (ignoreFirstChar > 0)
                ignoreFirstChar -= 1;
            // Key pressed
            if (event->Type == sf::Event::KeyPressed && control->getMode() == MODE_NORMAL) {
                switch (event->Key.Code) {
                    case sf::Key::S:
                        control->sortAlbums();
                        break;
                    case sf::Key::Q:
                        running = false;
                        break;
                    case sf::Key::Slash:
                        if (!event->Key.Control) {
                            markSel = control->getSel();
                            control->setMode(MODE_TYPING_FILTER);
                            control->rmFilter();
                            updateBar();
                            ignoreFirstChar = 2;
                        }
                        break;
                    case sf::Key::T:
                        control->toggleLimit();
                        break;
                    case sf::Key::N:
                        if (!event->Key.Shift)
                            control->searchNext();
                        else
                            control->searchPrev();
                        break;
                    case sf::Key::M:
                        selVMiddle();
                        break;
                    case sf::Key::Escape:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
            // Using Event for binding
            if (testEvent(keyCmd["LineBegin"], event)) { // You can use a function
                selBegin();
            }
            if (testEvent(keyCmd["LineEnd"], event)) { // You can use a function
                selEnd();
            }
            if (testEvent(keyCmd["ScrollUp"], event) && control->getSelY() >= 0 && viewY > 0) {
                if (event->Key.Control) {
                    viewY--;
                    for (unsigned int i = 0; i < control->getColNum(); i++)
                        control->selPrev();
                }
            }
            if (testEvent(keyCmd["ScrollDown"], event) && control->getSelY() >= 0 && viewY < control->getBottomViewY()) {
                if (event->Key.Control) {
                    viewY++;
                    for (unsigned int i = 0; i < control->getColNum(); i++)
                        control->selNext();
                }
            }
            if (testEvent(keyCmd["Up"], event) && control->getSelY() > 0) { // You can use a function
                for (unsigned int i = 0; i < control->getColNum(); i++)
                    control->selPrev();
            }
            if (testEvent(keyCmd["Down"], event) && control->getSelY() < control->getBottomY()) {
                for (unsigned int i = 0; i < control->getColNum(); i++)
                    control->selNext();
            }
            if (testEvent(keyCmd["PgUp"], event) && control->getSelY() > 0) {
                for (unsigned int i = 0; i < (control->getColNum() * control->getRowNum()); i++) {
                    if (control->getSelX() == 0 && viewY > 0)
                        viewY--;
                    control->selPrev();
                }
            }
            if (testEvent(keyCmd["PgDown"], event) && control->getSelY() < control->getBottomY()) {
                for (unsigned int i = 0; i < (control->getColNum() * control->getRowNum()); i++) {
                    control->selNext();
                }
            }
            if (testEvent(keyCmd["Left"], event)) {
                if (event->Key.Shift) {
                    selBeginTop();
                } else
                    control->selPrev();
            }
            if (testEvent(keyCmd["Right"], event)) {
                if (event->Key.Shift) {
                    selBeginBottom();
                } else
                    control->selNext();
            }
            if (testEvent(keyCmd["Play"], event)) {
                control->addAlbum();
                control->addSongList();
                control->play();
            }
        }
        app->Clear();
        unsigned int botY = control->getBottomViewY();
        while ((110 + control->getSelY() - getViewY()) > (109 + control->getRowNum()) && getViewY() < botY) {
            viewY++;
            while (getViewY() > botY) {
                viewY = botY;
            }
        }
        while (getViewY() > botY) {
            viewY = botY;
        }
        while ((110 + control->getSelY() - getViewY()) < 110 && getViewY() > 0) {
            viewY--;
        }
        control->drawSprs();
        drawBar();
        app->Display(); // Display the result
    }
}
bool getInput() {
    int input;
    cin >> input;
    if (input == 1)
        control->printDirs();
    if (control->isErr() || input == 0)
        return false;
    return true;
}
void cleanUp() {
    cout << "Cleaning up:" << endl;
    delete control;
   //exit(0);
}
/////////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    control = new Control();
    control->loadImg();
    control->initDraw(argc, argv);
    cleanUp();
    return 0;
}
