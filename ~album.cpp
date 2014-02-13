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
        img = new Img_t;
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
    if (next) { next->loadImg(); }
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

