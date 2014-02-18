/* global function defintions */
bool initDraw(int &_argc, char **_argv) {
    draw = &_draw;
    std::string s = getenv("HOME");
    s += "/.local/share/kpmpc/arial.ttf";
    draw->font.loadFromFile(s);
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
Draw::Draw()
    { /* */ };

Draw::~Draw() {};

void Draw::loop() {
    sys->app.clear();

    drawAlbums();
    drawStatus();

    sys->app.display();
}

/* grid functions */
int Draw::getWidth() {
    if (sys == NULL || round((float)sys->app.getSize().x / sys->tileSize) < 1)
        return 1;
    return round((float)sys->app.getSize().x / sys->tileSize);
}

int Draw::getHeight() {
    if (sys == NULL || round((float)sys->app.getSize().y / sys->tileSize) < 1)
        return 1;
    return round((float)sys->app.getSize().y / sys->tileSize);
}

/* draw functions */
void Draw::drawAlbums() {
    float x,w,y,h;
    for (unsigned int ii = 0; ii < mpd->album.size(); ++ii) {
        if (mpd->album[ii].dir.compare("") == 0) continue;

            w = mpd->album[ii].tex.getSize().x;
            h = mpd->album[ii].tex.getSize().y - 32;
            x = (WIN_WIDTH_FLOAT - 32) / getWidth();
            y = (WIN_HEIGHT_FLOAT - 32) / getHeight();

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
}

void Draw::drawStatus() {
    /* draw bar */
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(0, 0, 0));
    rs.setPosition(0, WIN_HEIGHT - (WIN_HEIGHT - (sys->app.getSize().y / 18) - 540));
    rs.setSize(sf::Vector2f(3000, 3000));
    sys->app.draw(rs);

    /* set status string */
    std::string st = mpd->album[sel].artist;
    st += " - "; st += mpd->album[sel].name;
    st += " | cmd: "; st += sys->cmd;

    /* set as window title */
    sys->app.setTitle(st);

    /* initialize text object */
    sf::Text text(st, font);
    text.setColor(sf::Color(255, 0, 0));
    text.setPosition(10, WIN_HEIGHT - (WIN_HEIGHT - (sys->app.getSize().y / 18) - 540));
    text.setScale(WIN_WIDTH / 100, WIN_HEIGHT / 100);
    text.scale(1000 / (sys->app.getSize().x / 2), 1000 / (sys->app.getSize().y / 2));
    text.scale(.1, .1);
    text.scale(.5, .5);
    text.scale(.5, .5);

    /* draw*/
    sys->app.draw(text);
}
