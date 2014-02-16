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
    while (_dir[_dir.length() - 1] != '/')
        _dir.erase(_dir.length() - 1);
    return _dir.c_str();
}

void Album::buildTags()
    { artist = mpd->getTag(name.c_str(), MPD_TAG_ARTIST); }

void Album::build() {
    std::vector<std::string> artPath;
    std::string _dir = DIR_BASE;
    _dir += mpd->getDir(name);
    _dir = dirTrim(_dir);

    success = false;
    
    artPath = sys->artPath; while (!artPath.empty()) { loadArt(_dir, artPath[0]); artPath.erase(artPath.begin()); }
    artPath = sys->artPath; while (!artPath.empty()) { loadArt(dirTrim(_dir), artPath[0]); artPath.erase(artPath.begin()); }
    if (!success) { dir = ""; }

    spr.setTexture(tex);

}

void Album::loadArt(std::string _dir, std::string path)
    { std::cout << "Loading album art: " << _dir << path << " (" << name << ") ... " << std::endl; if (!success) { dir = _dir; dir += path; success = tex.loadFromFile(dir); } }

Album::~Album() {
}

void Album::clean() {
}

void Album::loop() {
}
