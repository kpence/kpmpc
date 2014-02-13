/* global function definitions */
bool initSys() {
    sys = &_sys;
    sys->build();
    return true;
}

bool loopSys() {
    sys->loop();
    return true;
}

void termSys() {
    sys = NULL;
}

/* main functions */
Sys::Sys(): app(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "kpmpc-1.0"), cmd("") {
}

void Sys::build() {
    buildMap();
}

void Sys::buildMap() {
    Map m("<c-j>", "<cr>", false); map.push_back(m);
    m = Map("<enter>", "<cr>", false); map.push_back(m);
    m = Map("h", ":mvcur -1,0<cr>", true); map.push_back(m);
    m = Map("l", ":mvcur +1,0<cr>", true); map.push_back(m);
    m = Map("j", ":mvcur 0,+1<cr>", true); map.push_back(m);
    m = Map("k", ":mvcur 0,-1<cr>", true); map.push_back(m);
    m = Map("qq", ":quit<cr>", true); map.push_back(m);
    m = Map("ZZ", ":quit<cr>", true); map.push_back(m);
    m = Map("<c-c>", "<clear>", false); map.push_back(m);
    m = Map("<esc>", "<clear>", false); map.push_back(m);
    m = Map("<space>", ":play<cr>", true); map.push_back(m);
    m = Map("<cr>", ":play<cr>", true); map.push_back(m);
}

Sys::~Sys() {
}

void Sys::clean() {
}

void Sys::loop() {
    while (app.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: isRunning = false; app.close(); break;
            case sf::Event::KeyPressed: onKeyPress(&event.key); break;
            default: break;
        }
    }
}

bool Sys::onKeyPress(sf::Event::KeyEvent *ke) {
    std::string str[4] = {"", "", "", ""};

    switch (ke->code) {
        case sf::Keyboard::A: str[0] = "a"; str[1] = "A"; str[2] = "<c-a>"; str[3] = "<c-A>"; break;
        case sf::Keyboard::B: str[0] = "b"; str[1] = "B"; str[2] = "<c-b>"; str[3] = "<c-B>"; break;
        case sf::Keyboard::C: str[0] = "c"; str[1] = "C"; str[2] = "<c-c>"; str[3] = "<c-C>"; break;
        case sf::Keyboard::D: str[0] = "d"; str[1] = "D"; str[2] = "<c-d>"; str[3] = "<c-D>"; break;
        case sf::Keyboard::E: str[0] = "e"; str[1] = "E"; str[2] = "<c-e>"; str[3] = "<c-E>"; break;
        case sf::Keyboard::F: str[0] = "f"; str[1] = "F"; str[2] = "<c-f>"; str[3] = "<c-F>"; break;
        case sf::Keyboard::G: str[0] = "g"; str[1] = "G"; str[2] = "<c-g>"; str[3] = "<c-G>"; break;
        case sf::Keyboard::H: str[0] = "h"; str[1] = "H"; str[2] = "<c-h>"; str[3] = "<c-H>"; break;
        case sf::Keyboard::I: str[0] = "i"; str[1] = "I"; str[2] = "<c-i>"; str[3] = "<c-I>"; break;
        case sf::Keyboard::J: str[0] = "j"; str[1] = "J"; str[2] = "<c-j>"; str[3] = "<c-J>"; break;
        case sf::Keyboard::K: str[0] = "k"; str[1] = "K"; str[2] = "<c-k>"; str[3] = "<c-K>"; break;
        case sf::Keyboard::L: str[0] = "l"; str[1] = "L"; str[2] = "<c-l>"; str[3] = "<c-L>"; break;
        case sf::Keyboard::M: str[0] = "m"; str[1] = "M"; str[2] = "<c-m>"; str[3] = "<c-M>"; break;
        case sf::Keyboard::N: str[0] = "n"; str[1] = "N"; str[2] = "<c-n>"; str[3] = "<c-N>"; break;
        case sf::Keyboard::O: str[0] = "o"; str[1] = "O"; str[2] = "<c-o>"; str[3] = "<c-O>"; break;
        case sf::Keyboard::P: str[0] = "p"; str[1] = "P"; str[2] = "<c-p>"; str[3] = "<c-P>"; break;
        case sf::Keyboard::Q: str[0] = "q"; str[1] = "Q"; str[2] = "<c-q>"; str[3] = "<c-Q>"; break;
        case sf::Keyboard::R: str[0] = "r"; str[1] = "R"; str[2] = "<c-r>"; str[3] = "<c-R>"; break;
        case sf::Keyboard::S: str[0] = "s"; str[1] = "S"; str[2] = "<c-s>"; str[3] = "<c-S>"; break;
        case sf::Keyboard::T: str[0] = "t"; str[1] = "T"; str[2] = "<c-t>"; str[3] = "<c-T>"; break;
        case sf::Keyboard::U: str[0] = "u"; str[1] = "U"; str[2] = "<c-u>"; str[3] = "<c-U>"; break;
        case sf::Keyboard::V: str[0] = "v"; str[1] = "V"; str[2] = "<c-v>"; str[3] = "<c-V>"; break;
        case sf::Keyboard::W: str[0] = "w"; str[1] = "W"; str[2] = "<c-w>"; str[3] = "<c-W>"; break;
        case sf::Keyboard::X: str[0] = "x"; str[1] = "X"; str[2] = "<c-x>"; str[3] = "<c-X>"; break;
        case sf::Keyboard::Y: str[0] = "y"; str[1] = "Y"; str[2] = "<c-y>"; str[3] = "<c-Y>"; break;
        case sf::Keyboard::Z: str[0] = "z"; str[1] = "Z"; str[2] = "<c-z>"; str[3] = "<c-Z>"; break;
        case sf::Keyboard::Num1: str[0] = "1"; str[1] = "!"; str[2] = "<c-1>"; str[3] = "<c-!>"; break;
        case sf::Keyboard::Num2: str[0] = "2"; str[1] = "@"; str[2] = "<c-2>"; str[3] = "<c-@>"; break;
        case sf::Keyboard::Num3: str[0] = "3"; str[1] = "#"; str[2] = "<c-3>"; str[3] = "<c-#>"; break;
        case sf::Keyboard::Num4: str[0] = "4"; str[1] = "$"; str[2] = "<c-4>"; str[3] = "<c-$>"; break;
        case sf::Keyboard::Num5: str[0] = "5"; str[1] = "%"; str[2] = "<c-5>"; str[3] = "<c-%>"; break;
        case sf::Keyboard::Num6: str[0] = "6"; str[1] = "^"; str[2] = "<c-6>"; str[3] = "<c-^>"; break;
        case sf::Keyboard::Num7: str[0] = "7"; str[1] = "&"; str[2] = "<c-7>"; str[3] = "<c-&>"; break;
        case sf::Keyboard::Num8: str[0] = "8"; str[1] = "&"; str[2] = "<c-8>"; str[3] = "<c-*>"; break;
        case sf::Keyboard::Num9: str[0] = "9"; str[1] = "("; str[2] = "<c-9>"; str[3] = "<c-(>"; break;
        case sf::Keyboard::Num0: str[0] = "0"; str[1] = ")"; str[2] = "<c-0>"; str[3] = "<c-)>"; break;
        case sf::Keyboard::Space: str[0] = "<space>"; str[1] = "<s-space>"; str[2] = "<c-space>"; str[3] = "<c-s-space>"; break;
        default: return true; break;
    }
    if (!ke->shift && !ke->control)
        cmd += str[0];
    else if (ke->shift && !ke->control)
        cmd += str[1];
    else if (!ke->shift && ke->control)
        cmd += str[2];
    else if (ke->shift && ke->control)
        cmd += str[3];

    remapCmd(cmd);
    if (cmd.find("<clear>") != std::string::npos) {
        cmd = "";
        return true;
    }
    testCmd(cmd);
    std::cout << cmd << std::endl;
    if (cmd.find("<cr>") != std::string::npos)
        cmd = "";
    return true;
}

bool Sys::isMap() {
    for (std::vector<Map>::iterator ii = map.begin(); ii != map.end(); ++ii) {
        if ((!ii->whole && cmd.find(ii->str) != std::string::npos) || (ii->whole && cmd.compare(ii->str) == 0)) {
            isRunning = false;
            return true;
        }
    }
    return false;
}

bool Sys::remapCmd(std::string &_cmd) {
    do {
        for (std::vector<Map>::iterator ii = map.begin(); ii != map.end(); ++ii) {
            if ((!ii->whole && _cmd.find(ii->str) != std::string::npos) || (ii->whole && _cmd.compare(ii->str) == 0)) {
                std::cout << ii->str << " --> ";
                _cmd.replace(_cmd.find(ii->str), ii->str.length(), ii->remap);
            }
        }
    } while (isMap());
    return true;
}

bool Sys::testCmd(std::string &_cmd) {
    if (_cmd.compare(":mvcur 0,-1<cr>") == 0) {
        draw->sel -= draw->getWidth();
        draw->sel = std::max(0, draw->sel);
        if (floor((float)draw->sel / draw->getWidth()) < draw->viewY)
            draw->viewY--;
    } else if (_cmd.compare(":mvcur 0,+1<cr>") == 0) {
        draw->sel += draw->getWidth();
        draw->sel = std::min((int)mpd->album.size() - 1, draw->sel);
        if (floor((float)draw->sel / draw->getWidth()) >= (draw->getHeight() - 1 + draw->viewY))
            draw->viewY++;
    } else if (_cmd.compare(":mvcur +1,0<cr>") == 0) {
        draw->sel++;
        draw->sel = std::min((int)mpd->album.size() - 1, draw->sel);
    } else if (_cmd.compare(":mvcur -1,0<cr>") == 0) {
        draw->sel -= 1;
        draw->sel = std::max(0, draw->sel);
    } else if (_cmd.compare(":quit<cr>") == 0) {
        isRunning = false;
    } else if (_cmd.compare(":play<cr>") == 0) {
        mpd_run_clear(mpd->conn);
        addAlbum(mpd->album[draw->sel].name.c_str());
        for (std::vector<std::string>::iterator ii = sl.begin(); ii != sl.end(); ++ii)
            mpd_run_add(mpd->conn, ii->c_str());
        mpd_run_play(mpd->conn);
    }
    std::cout << "sel: " << draw->sel << ", " << draw->getWidth() << std::endl;
    return true;
}

bool Sys::addAlbum(const char *a) {
    sl.clear();
    mpd_song *song;
    if (!mpd_search_db_songs(mpd->conn, false))
        return false;
    std::cout << "Searched db songs" << std::endl;
    if (!mpd_search_add_tag_constraint(mpd->conn, MPD_OPERATOR_DEFAULT, MPD_TAG_ALBUM, a))
        return false;
    std::cout << "Adding tag constraint" << std::endl;
    if (!mpd_search_commit(mpd->conn))
        return false;
    std::cout << "Searching commit" << std::endl;
    while ((song = mpd_recv_song(mpd->conn)) != NULL) {
        sl.push_back(mpd_song_get_uri(song));
        mpd_song_free(song);
    }
    if (mpd_connection_get_error(mpd->conn) != MPD_ERROR_SUCCESS)
        return false;
    if (!mpd_response_finish(mpd->conn))
        return false;
    return true;
}

void Sys::sortAlbums(mpd_tag_type type) {
    std::cout << "" << std::endl;
    std::sort(mpd->album.begin(), mpd->album.end(), helper_sort);
}
