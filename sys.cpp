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
Sys::Sys(): app(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "kpmpc-1.0"), cmd(""), tileSize(0), searchFlags(0) {
    search = "";
}

void Sys::build() {
    buildConfig();
}

void Sys::buildConfig() {
    std::string s = getenv("HOME");
    s += "/.kpmpcrc";
    std::ifstream config; config.open(s.c_str());
    std::string line[3];
    int i;

    while (!config.eof()) {
        Map m("", "", false);
        i = 0; line[0] = ""; line[1] = ""; line[2] = "";

        if (!config.eof()) { config >> line[0]; }

        /* comment */
        if (line[0][0] == '#') while (config.get() != '\n');

        /* commands automatically entered on startup */
        if (line[0].compare("do") == 0) {
            if (!config.eof()) { ++i; config >> line[0]; }
            cmd += line[0];
        }

        /* cover art tile size */
        if (line[0].compare("tile-size") == 0 || line[0].compare("art-size") == 0 || line[0].compare("art-tile-size") == 0)
            if (!config.eof()) { ++i; config >> line[0]; tileSize = atoi(line[0].c_str()); }

        /* cover art file paths */
        if (line[0].compare("art-path") == 0 || line[0].compare("art") == 0 || line[0].compare("art-file-path") == 0) {
            if (!config.eof()) { ++i; config >> line[0]; }
            artPath.push_back(line[0]);
        }

        /* input mappings */
        if (line[0].compare("map") == 0) {
            if (!config.eof()) { ++i; config >> line[0]; }
            if (!config.eof()) { ++i; config >> line[1]; }
            if (!config.eof()) { ++i; config >> line[2]; }
            if (i != 3) { while (config.get() != '\n' && !config.eof()); break; }

            m = Map(line[0], line[1], true);
            std::cout << "mapping added: " << line[0] << ", " << line[1] << ", " << line[2] << std::endl;
            if (line[2].compare("p") == 0 || line[2].compare("part") == 0) m.whole = false;
            map.push_back(m);
        }
    }

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

    if (ke != NULL) {
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
            case sf::Keyboard::Return: str[0] = "<return>"; str[1] = "<s-return>"; str[2] = "<c-return>"; str[3] = "<c-s-return>"; break;
            case sf::Keyboard::BackSpace: str[0] = "<backspace>"; str[1] = "<s-backspace>"; str[2] = "<c-backspace>"; str[3] = "<c-s-backspace>"; break;
            case sf::Keyboard::Space: str[0] = "<spacebar>"; str[1] = "<s-spacebar>"; str[2] = "<c-spacebar>"; str[3] = "<c-s-spacebar>"; break;
            case sf::Keyboard::Escape: str[0] = "<esc>"; str[1] = "<s-esc>"; str[2] = "<c-esc>"; str[3] = "<c-s-esc>"; break;
            case sf::Keyboard::Dash: str[0] = "<->"; str[1] = "_"; str[2] = "<c-->"; str[3] = "<c-_>"; break;
            case sf::Keyboard::Equal: str[0] = "="; str[1] = "+"; str[2] = "<c-=>"; str[3] = "<c-+>"; break;
            case sf::Keyboard::Slash: str[0] = "/"; str[1] = "?"; str[2] = "<c-/>"; str[3] = "<c-?>"; break;
            case sf::Keyboard::Unknown: break;
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
    }

    /* finds number of characters of vim-style number prefixes (and stops if that's all that has been typed) */
    unsigned int f = 0; for (; cmd[f] == '0' || cmd[f] == '1' || cmd[f] == '2' || cmd[f] == '3' || cmd[f] == '4' || cmd[f] == '5' || cmd[f] == '6' || cmd[f] == '7' || cmd[f] == '8' || cmd[f] == '9'; f++);
    if (cmd.size() == f && ke != NULL) { std::cout << cmd; return true; }

    /* applies all command remappings */
    remapCmd(cmd);

    /* insert space in text field (such as when searching for artists) */
    if (cmd.find("<space>") != std::string::npos)
        if (cmd.find(":search<") == f)
            cmd.replace(cmd.find("<space>"), 7, " ");

    /* delete word back */
    if (cmd.find("<del-word-back>") != std::string::npos) {
        cmd.erase(cmd.end() - 15, cmd.end());
        do {
            if (cmd.empty()) break;
            cmd.erase(cmd.end() - 1, cmd.end());
        } while (cmd[cmd.size() - 1] != '<' || cmd[cmd.size() - 1] != '>' || cmd[cmd.size() - 1] != '.' || cmd[cmd.size() - 1] != ' ');
        return true;
    }

    /* delete character back */
    if (cmd.find("<del-back>") != std::string::npos) {
        cmd.erase(cmd.end() - 10, cmd.end());
        if (cmd.find(":search<") == f && cmd.find(">") == cmd.size() - 1) cmd = "";
        else {
            if (cmd.size() < 1) cmd = "";
            else cmd.erase(cmd.end() - 1, cmd.end());
        }
        std::cout << cmd << std::endl;
        return true;
    }

    /* clear */
    if (cmd.find("<clear>") != std::string::npos) {
        cmd = "";
        std::cout << "<clear> (Clearing cmd string)" << std::endl;
        return true;
    }

    /* does : commands */
    testCmd(cmd);

    /* handles <cr> delimiter, calls this entire function again for each time a command delimited by <cr> happens */
    if (cmd.find("<cr>") != std::string::npos) { cmd.erase(0, cmd.find("<cr>") + 4); onKeyPress(NULL); } if (cmd.empty()) cmd = "";

    /* checks if key mapping is possible */
    bool find = false;
    if (cmd.find(":search<") == f) find = true; else
    for (std::vector<Map>::iterator ii = map.begin(); ii != map.end(); ++ii) if (ii->str.find(cmd) == 0) find = true;
    if (!find) cmd = "";

    /* done */
    return true;
}

bool Sys::isMap() {
    unsigned int f; for (f = 0; cmd[f] == '0' || cmd[f] == '1' || cmd[f] == '2' || cmd[f] == '3' || cmd[f] == '4' || cmd[f] == '5' || cmd[f] == '6' || cmd[f] == '7' || cmd[f] == '8' || cmd[f] == '9'; f++);
    if (cmd.size() == f) return false;

    for (std::vector<Map>::iterator ii = map.begin(); ii != map.end(); ++ii)
        if ((!ii->whole && cmd.find(ii->str) != std::string::npos) || (ii->whole && cmd.find(ii->str) == f))
            return true;
    return false;
}

bool Sys::remapCmd(std::string &_cmd) {
    unsigned int f; for (f = 0; _cmd[f] == '0' || _cmd[f] == '1' || _cmd[f] == '2' || _cmd[f] == '3' || _cmd[f] == '4' || _cmd[f] == '5' || _cmd[f] == '6' || _cmd[f] == '7' || _cmd[f] == '8' || _cmd[f] == '9'; f++);
    if (_cmd.size() == f) return true;

    do {
        for (std::vector<Map>::iterator ii = map.begin(); ii != map.end(); ++ii) {
            if ((!ii->whole && _cmd.find(ii->str) != std::string::npos) || (ii->whole && _cmd.find(ii->str) == f)) {
                std::cout << ii->str << " --> ";
                _cmd.replace(_cmd.find(ii->str), ii->str.length(), ii->remap);
            }
        }
    } while (isMap());
    return true;
}

bool Sys::testCmd(std::string &_cmd) {
    /* stops if nothing in cmd string, and declare argument variables */
    if (_cmd != "" && !_cmd.empty()) std::cout << _cmd << std::endl;
    std::string arg[5];
    int argNum = 1;
    int num = 0;

    /* finds the prefix number argument */
    int f; arg[0] = ""; for (f = 0; _cmd[f] == '0' || _cmd[f] == '1' || _cmd[f] == '2' || _cmd[f] == '3' || _cmd[f] == '4' || _cmd[f] == '5' || _cmd[f] == '6' || _cmd[f] == '7' || _cmd[f] == '8' || _cmd[f] == '9'; f++) arg[0] += _cmd[f];
    unsigned int nSize = (unsigned)f;
    if (arg[0] == "") nSize = 0;
    if (nSize > 0) num = atoi(_cmd.substr(0, f).c_str());

    /* makes sure the commands are valid */
    if (_cmd.find(":") != nSize || _cmd.find("<cr>") == std::string::npos) return true;

    /* finds the arguments */
    f = 1;
    if (_cmd.find("<") != _cmd.find("<cr>"))
    for (int i = 1; i < 10; i++) {
        if (_cmd[_cmd.find("<") + i] == ',') { f++; continue; }
        if (_cmd[_cmd.find("<") + i] == '>') { break; }
        arg[f] += _cmd[_cmd.find("<") + i];
        argNum = f;
    }

    /* select album above */
    if (_cmd.find(":mvcur<") == nSize) {
        for (int foo = 0; foo < std::max(1, num); foo++) {
            if (arg[2].compare("-1") == 0) {

                if (floor((float)draw->sel / draw->getWidth()) > 0) draw->sel -= draw->getWidth();
                draw->sel = std::max(0, draw->sel);
                if (floor((float)draw->sel / draw->getWidth()) < draw->viewY) draw->viewY--;
                if (draw->sel <= 0) break;

            } else if (arg[2].compare("+1") == 0) {

                draw->sel += draw->getWidth();
                draw->sel = std::min((int)mpd->album.size() - 1, draw->sel);
                if (floor((float)draw->sel / draw->getWidth()) >= (draw->getHeight() + draw->viewY)) draw->viewY++;
                if (draw->sel >= (int)mpd->album.size() - 1) break;

            }

            if (arg[1].compare("-1") == 0) { selPrev(); if (draw->sel <= 0) break; }
            if (arg[1].compare("+1") == 0) { selNext(); if (draw->sel >= (int)mpd->album.size() - 1) break; }
        }

    /* quit game */
    } else if (_cmd.find(":quit") == nSize) {
        std::cout << "Quiting program" << std::endl;
        isRunning = false;

    /* zoom in/out */
    } else if (_cmd.find(":art-size") == nSize) {
        if (arg[1].compare("-1"))
            tileSize = std::min(320, (std::max(8, tileSize - 1)));
        else if (arg[1].compare("+1"))
            tileSize = std::min(320, (std::max(8, tileSize + 1)));

    /* search next */
    } else if (_cmd.find(":search-next<cr>") == nSize) {
        searchNext(search, searchFlags, false);

    /* search next */
    } else if (_cmd.find(":search-prev<cr>") == nSize) {
        searchNext(search, searchFlags | S_REVERSE, false);

    /* search */
    } else if (_cmd.find(":search<") == nSize) {

        searchFlags = 0;
        for (int ff = 0; ff < argNum; ff++) {
            if (arg[ff].compare("album") == 0) searchFlags = searchFlags | S_ALBUM;
            else if (arg[ff].compare("artist") == 0) searchFlags = searchFlags | S_ARTIST;
            else if (arg[ff].compare("genre") == 0) searchFlags = searchFlags | S_GENRE;
        }
        search = _cmd; search.erase(0, _cmd.find(">") + 1); search.erase(search.find("<cr>"), search.size());
        searchNext(search, searchFlags, false);

    /* add selected album */
    } else if (_cmd.find(":add<cr>") == nSize) {
        addAlbum(mpd->album[draw->sel].name.c_str());
        for (std::vector<std::string>::iterator ii = sl.begin(); ii != sl.end(); ++ii) mpd_run_add(mpd->conn, ii->c_str());
        mpd_run_play(mpd->conn);

    /* play selected album */
    } else if (_cmd.find(":play<cr>") == nSize) {
        mpd_run_clear(mpd->conn);
        addAlbum(mpd->album[draw->sel].name.c_str());
        for (std::vector<std::string>::iterator ii = sl.begin(); ii != sl.end(); ++ii) mpd_run_add(mpd->conn, ii->c_str());
        mpd_run_play(mpd->conn);

    }
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

/* search/sel functions */
void Sys::searchNext(std::string _search, int _flags, bool realTime, bool wrap) {
    std::cout << "Searching for: " << _search << std::endl;
    search = _search;
    searchFlags = _flags;
    bool suc = true;
    if (!HAS_FLAG(_flags, S_REVERSE)) {
        int _sel = draw->sel;
        if (!realTime) _sel++;
        do {
            if (_sel == draw->sel) { suc = false; break; }
            if (mpd->album[_sel].name.find(_search) != std::string::npos || mpd->album[_sel].artist.find(_search) != std::string::npos) { break; }
            _sel += 1;
            while (_sel >= (signed)mpd->album.size()) _sel -= mpd->album.size();
            while (_sel < 0) _sel += mpd->album.size();
            if (floor((float)draw->sel / draw->getWidth()) >= (draw->getHeight() + draw->viewY)) draw->viewY++;
        } while (true);

        if (suc) draw->sel = _sel;

    } else {
        int _sel = draw->sel;
        if (!realTime) _sel--;
        do {
            if (_sel == draw->sel) { suc = false; break; }
            if (mpd->album[_sel].name.find(_search) != std::string::npos || mpd->album[_sel].artist.find(_search) != std::string::npos) { break; }
            _sel -= 1;
            while (_sel >= (signed)mpd->album.size()) _sel -= mpd->album.size();
            while (_sel < 0) _sel += mpd->album.size();
        } while (true);

        if (suc) draw->sel = _sel;

    }
}

void Sys::selPrev(bool wrap) {
    draw->sel--;
    if (wrap) {
        while (draw->sel >= (signed)mpd->album.size()) draw->sel -= mpd->album.size();
        while (draw->sel < 0) draw->sel += mpd->album.size();
    } else
    draw->sel = std::max(0, draw->sel);
}

void Sys::selNext(bool wrap) {
    draw->sel++;
    if (wrap) {
        while (draw->sel >= (signed)mpd->album.size()) draw->sel -= mpd->album.size();
        while (draw->sel < 0) draw->sel += mpd->album.size();
    } else
    draw->sel = std::min((int)mpd->album.size() - 1, draw->sel);
    if (floor((float)draw->sel / draw->getWidth()) >= (draw->getHeight() + draw->viewY)) draw->viewY++;
}
