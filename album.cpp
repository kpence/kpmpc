/* album.cpp */

/* global functions defintions */
bool initAlbum() {
    return true;
}

bool loopAlbum() {
    return true;
}

void termAlbum() {
}

/* main functions */
Album::Album(const char *_name): selAnimY(0.f), dir(""), artist(""), date(""), imgDir(""), name(_name) {
    //cout << "Album instantiated: " << name << endl;
}

const char *Album::dirTrim(std::string _dir) {
    _dir.erase(_dir.length() - 1);
    _dir.erase(_dir.length() - 1);
    while (_dir[_dir.length() - 1] != '/')
        _dir.erase(_dir.length() - 1);
    return _dir.c_str();
}

void Album::buildTags()
    { artist = mpd->getTag(name.c_str(), MPD_TAG_ARTIST); }

void Album::build() {
    std::string _dir = DIR_BASE;
    _dir += mpd->getDir(name);

    bool success = false;
    if (!success) { dir = _dir; dir += "front.jpg"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "front.png"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "cover.jpg"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "cover.png"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "folder.jpg"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "folder.png"; success = tex.loadFromFile(dir); }
    _dir = dirTrim(_dir);
    if (!success) { dir = _dir; dir += "front.jpg"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "front.png"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "cover.jpg"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "cover.png"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "folder.jpg"; success = tex.loadFromFile(dir); }
    if (!success) { dir = _dir; dir += "folder.png"; success = tex.loadFromFile(dir); }
    if (!success) { dir = ""; }
    spr.setTexture(tex);

}

Album::~Album() {
}

void Album::clean() {
}

void Album::loop() {
}
