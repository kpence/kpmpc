/* forward declaration */
void initDraw(int &argc, char **argv) {
    if (IS_GUI) {
        //draw = new Draw(argc, argv);
        draw = new Draw();
        setImgs();
        cout << "Gonna do loop" << endl;
        sortAlbums();
        draw->initLoop();
    }
}
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
    if (control->getMode() == MODE_TYPING_SEARCH)
        strcat(msg, " - /");
    if (control->getSearch() != NULL) {
        if (strcmp(control->getSearch(), "") != 0) {
            if (control->getMode() != MODE_TYPING_SEARCH)
                strcat(msg, " (/");
            strcat(msg, control->getSearch());
            if (control->getMode() != MODE_TYPING_SEARCH)
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

void Draw::scrollDown() {
    viewY++;
    if (control->getSelY() + 1 <= getViewY()) {
        for (unsigned int i = 0; i < control->getColNum(); i++)
            control->selNext();
    }
}
void Draw::scrollUp() {
    viewY--;
    if (control->getSelY() >= getViewY() + control->getRowNum()) {
        for (unsigned int i = 0; i < control->getColNum(); i++)
            control->selPrev();
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
            if (event->Type == sf::Event::TextEntered && control->getMode() == MODE_TYPING_SEARCH) {
                updateBar();
                char txt = (char)event->Text.Unicode;
                printf("Text: %i, %c\n", txt, txt);
                if (txt == 3) {
                    control->rmSearch();
                    control->setMode(MODE_NORMAL);
                    updateBar();
                    control->setSel(markSel);
                    markSel = NULL;
                } else if (txt == 13 || txt == 10) {
                    control->setMode(MODE_NORMAL);
                    updateBar();
                } else if ((txt != '/' || ignoreFirstChar <= 0) && txt != 31)
                    control->charCatSearch(txt);
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
                            control->setMode(MODE_TYPING_SEARCH);
                            control->rmSearch();
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
                if (event->Key.Control)
                    scrollUp();
            }
            if (testEvent(keyCmd["ScrollDown"], event) && control->getSelY() >= 0 && viewY < control->getBottomViewY()) {
                if (event->Key.Control)
                    scrollDown();
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
