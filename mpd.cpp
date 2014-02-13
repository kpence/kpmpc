/* global function definitions */
bool initMpd() {
    mpd = &_mpd;
    mpd->update(MPD_TAG_UNKNOWN, "");
    return true;
}

bool loopMpd()
    { return true; }

void termMpd() {
}

/* main functions */
Mpd::Mpd(): mode(MODE_NORMAL), limit(false) {
    conn = mpd_connection_new(NULL, 0, 30000);
}

Mpd::~Mpd() {
    /* */
}

void Mpd::err()
    { /* */ }

bool Mpd::isErr()
    { return errStatus; }

/* music functions */
/* album functions */

/* filter functions */
/* grid functions */
/* mpd functions */
bool Mpd::update(mpd_tag_type type, const char *value) {
    mpd_pair *pair;
    album.clear();

    if (!mpd_search_db_tags(conn, MPD_TAG_ALBUM)) { err(); return false; }

    if (strcmp(value, "") != 0)
        if (!mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT, type, value)) { err(); return false; }

    if (!mpd_search_commit(conn)) { err(); return false; }

    while ((pair = mpd_recv_pair_tag(conn, MPD_TAG_ALBUM)) != NULL) {
        album.push_back(Album(pair->value));
        mpd_return_pair(conn, pair);
    }

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) { err(); return false; }

    if (!mpd_response_finish(conn)) { err(); return false; }

    cout << "Successful Update" << endl;
    for (std::vector<Album>::iterator i = album.begin(); i != album.end(); ++i)
        i->buildTags();
    sys->sortAlbums(MPD_TAG_UNKNOWN);
    for (std::vector<Album>::iterator i = album.begin(); i != album.end(); ++i)
        i->build();
    return true;
}

char *Mpd::getTag(const char *_album, mpd_tag_type type) {
    char *tagRet = new char[255];
    mpd_song *song;
    if (!mpd_search_db_songs(conn, false)) { err(); return NULL; }

    if (!mpd_search_add_tag_constraint(conn, MPD_OPERATOR_DEFAULT, MPD_TAG_ALBUM, _album)) { err(); return NULL; }

    if (!mpd_search_commit(conn)) { err(); return NULL; }

    if (type == MPD_TAG_UNKNOWN) {
        if ((song = mpd_recv_song(conn)) != NULL) {
            if (mpd_song_get_uri(song) != NULL)
                strcpy(tagRet, mpd_song_get_uri(song));
            else
                tagRet = NULL;
        }
    } else if ((song = mpd_recv_song(conn)) != NULL) {
        if (mpd_song_get_tag(song, type, 0) != NULL)
            strcpy(tagRet, mpd_song_get_tag(song, type, 0));
        else
            tagRet = NULL;
    }

    mpd_song_free(song);

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) { err(); return NULL; }

    if (!mpd_response_finish(conn)) { err(); return NULL; }

    return tagRet;
}

const char *Mpd::getDir(std::string _album)
    { return getTag(_album.c_str(), MPD_TAG_UNKNOWN); }
