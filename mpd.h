/* forward declarations */
bool initMpd();
bool loopMpd();
void termMpd();

/* class mpd */
class Mpd {
private:
    /* variables */
    bool errStatus;

    /* functions */
    void err();
protected:
public:
    /* variables */
    mpd_connection *conn;
    int mode;
    std::vector<Album> album;

    /* main functions */
    Mpd();
    ~Mpd();
    bool isErr();
    bool limit;
    Album *sel;
    std::string search;

    /* music functions */
    int getMode();
    void setMode(int _mode);
    void play();

    /* album functions */
    bool addAlbum(Album *_sel);
    bool addAlbum(char *_album);
    void addAlbum();
    void addSongList();
    bool isSelNext();
    void selPrev(bool wrap = false);
    void selNext(bool wrap = false);
    void setSel(Album *_sel);
    void sortAlbums(mpd_tag_type type = MPD_TAG_ARTIST);

    /* filter functions */
    bool isLimit();
    void toggleLimit();
    void searchPrev();
    void searchNext();
    Album *getSearchPrev(Album *_search);
    Album *getSearchNext(Album *_search);
    const char *getSearch();
    void rmSearch();
    void charCatSearch(const char _search);

    /* grid functions */
    unsigned int getColNum();
    unsigned int getRowNum();
    float getViewWidth();
    float getViewHeight();
    unsigned int getSelY();
    unsigned int getSelX();
    unsigned int getBottomY();
    unsigned int getBottomViewY();

    /* mpd functions */
    bool update(mpd_tag_type type, const char *value);
    std::string getTag(const char *_album, mpd_tag_type type);
    std::string getDir(std::string _album);
    void printDirs();

} _mpd;
Mpd *mpd;
