/* global function defintions */
bool initDraw(int &_argc, char **_argv) {
    draw = &_draw;
    //sortAlbums();
    draw->view.reset(sf::FloatRect(0, 0, WIN_WIDTH, WIN_HEIGHT));
    draw->view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    sys->app.setView(draw->view);
    return true;
}

bool loopDraw() {
    draw->loop();
    return true;
}

void termDraw() {
    draw = NULL;
}

/* main functions */
Draw::Draw()//: w(128), h(128)
    { /* */ };

Draw::~Draw() {};

void Draw::loop() {
    float x,w,y,h;
    sys->app.clear();
    for (unsigned int ii = 0; ii < mpd->album.size(); ++ii) {
        if (mpd->album[ii].dir.compare("") == 0) continue;

            w = mpd->album[ii].tex.getSize().x;
            h = mpd->album[ii].tex.getSize().y;
            x = WIN_WIDTH_FLOAT / getWidth();
            y = WIN_HEIGHT_FLOAT / getHeight();

        if (ii == (unsigned int)sel)
            mpd->album[ii].selAnimY = 32.f;
        else
            mpd->album[ii].selAnimY /= 1.5;

        mpd->album[ii].spr.setPosition((float)((int)ii % getWidth()) * x, (float)floor((int)ii / getWidth() - viewY) * y + mpd->album[ii].selAnimY / 3);
        mpd->album[ii].spr.setColor(sf::Color(255, 255, 255, 127 + 128 * (1 - (mpd->album[ii].selAnimY / 32.f))));
        mpd->album[ii].spr.setScale(x / w, y / h);
        sys->app.draw(mpd->album[ii].spr);
    }

    for (std::vector<Album>::iterator ii = mpd->album.begin(); ii != mpd->album.end(); ++ii) {
        if (ii->dir.compare("") == 0) continue;
        sys->app.draw(ii->spr);
    }
    sys->app.display();
    std::string st = mpd->album[sel].artist;
    st += " - ";
    st += mpd->album[sel].name;
    st += " | cmd: ";
    st += sys->cmd;
    sys->app.setTitle(st);
}

/* grid functions */
int Draw::getWidth() {
    if (sys == NULL || round((float)sys->app.getSize().x / TILE_SIZE_FLOAT) < 1)
        return 1;
    return round((float)sys->app.getSize().x / TILE_SIZE_FLOAT);
}

int Draw::getHeight() {
    if (sys == NULL || round((float)sys->app.getSize().y / TILE_SIZE_FLOAT) < 1)
        return 1;
    return round((float)sys->app.getSize().y / TILE_SIZE_FLOAT);
}
