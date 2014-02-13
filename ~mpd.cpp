/* CONTROL */
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
    if (search != NULL) {
        if (search[0] != '\0' && strcmp(search, "") != 0) {
            if (_search->getNext() != NULL)
                _search = _search->getNext();
            else
                _search = albums;
            while (strstr(_search->getArtist(), getSearch()) == NULL && strstr(_search->getName(), getSearch()) == NULL) {
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
    if (search != NULL) {
        if (search[0] != '\0' && strcmp(search, "") != 0) {
            if (albums->getPrev(_search))
                _search = albums->getPrev(_search);
            else
                _search = albums->getLast();
            while (strstr(_search->getArtist(), getSearch()) == NULL && strstr(_search->getName(), getSearch()) == NULL) {
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
    if (search != NULL) {
        if (search[0] != '\0' && strcmp(search, "") != 0) {
            selNext(true);
            while (strstr(sel->getArtist(), getSearch()) == NULL && strstr(sel->getName(), getSearch()) == NULL) {
                selNext(true);
                if (sel == _start)
                    break;
            }
        }
    }
}
void Control::searchPrev() {
    Album *_start = sel;
    if (search != NULL) {
        if (search[0] != '\0' && strcmp(search, "") != 0) {
            selPrev(true);
            while (strstr(sel->getArtist(), getSearch()) == NULL && strstr(sel->getName(), getSearch()) == NULL) {
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
    search = new char[256];
    rmSearch();
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
    delete [] search;
    search = NULL;
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
void Control::rmSearch() {
    cout << "Control::rmSearch()" << endl;
    search[0] = '\0';
    cout << "Search: " << search << endl;
}
const char *Control::getSearch() {
    return (search);
}
void Control::charCatSearch(const char _search) {
    if (_search > 0) {
        if (search[0] == '\0') {
            printf("strcpy: %i, %c", _search, _search);
            if (_search != 8 && _search != 23 && _search != 21 && _search > 0) {
                search[0] = _search;
                search[1] = '\0';
            }
            else
                setMode(MODE_NORMAL);
        }
        else {
            if (_search == 8) {
                printf("deleting: %i, %c\n", search[strlen(search) - 1], search[strlen(search) - 1]);
                search[strlen(search) - 1] = '\0';
            }
            else if (_search == 21)
                rmSearch();
            else if (_search == 23) {
                while (search[0] != '\0' && search[std::max((long unsigned int)0, strlen(search) - 1)] == ' ')
                    search[strlen(search) - 1] = '\0';
                while (search[0] != '\0' && search[std::max((long unsigned int)0, strlen(search) - 1)] != ' ')
                    search[strlen(search) - 1] = '\0';
            }
            else if (_search > 31) {
                strncat(search, &_search, 1);
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
    if (search == NULL)
        return false;
    else {
        if (search[0] == '\0' && strcmp(search, "") == 0)
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
// TODO fix bug and make it sort albums of the same artist by date correctly
void Control::sortAlbums(mpd_tag_type type) {
    static bool isBW = true;
    bool match;
    isBW = (!isBW);
    Album *_pMax = NULL;
    Album *_max = albums;
    Album *max = albums;
    Album *_new = NULL;
    while (albums != NULL) {
        max = _max = albums;
        for (Album *_albums = albums; _albums != NULL; _albums = _albums->getNext()) {
            if (_albums != _max) {
                match = false;
                if (((strcmp(_albums->getArtist(), _max->getArtist()) >= 0 && isBW) || (strcmp(_albums->getArtist(), _max->getArtist()) <= 0 && !isBW)) && (type == MPD_TAG_ARTIST))
                    max = _albums;
                if (strcmp(_albums->getArtist(), _max->getArtist()) == 0 && (type == MPD_TAG_ARTIST || match)) {
                    cout << "Found match: " << max->getArtist() << endl;
                    max = _albums;
                    match = true;
                }
                if (((strcmp(_albums->getDate(), _max->getDate()) > 0 && isBW) || (strcmp(_albums->getDate(), _max->getDate()) < 0 && !isBW)) && (type == MPD_TAG_DATE || match)) {
                    max = _albums;
                    match = false;
                }
                if (strcmp(_albums->getDate(), _max->getDate()) == 0 && (type == MPD_TAG_DATE || match)) {
                    cout << "MATCH DATE: " << _albums->getDate() << _max->getDate() << endl;
                    max = _albums;
                    match = true;
                }
                if (((strcmp(_albums->getName(), _max->getName()) > 0 && isBW) || (strcmp(_albums->getName(), _max->getName()) < 0 && !isBW)) && (type == MPD_TAG_ALBUM || match))
                    max = _albums;
                _max = max;
            }
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
