#include <stdlib.h>

struct Player {
    int id;
    char *name;
};

struct Board {
    const struct Player **players;
    int player_len;
	int size[2];
    const struct Player ***cells; // Array (table) of array (row) of pointers
    int *filled_in;
};

enum PlaceCellResult {
    Success,
    Fail,
    Won,
    RanOut,
};

const struct Board * new_board(int size[2], int player_len, const struct Player **players);
const struct Player * new_player(int id, char *name);

struct Player * get(const struct Board *board, int pos[2]);
int * move(int pos[2], int x, int y);
enum PlaceCellResult place(const struct Board *board, const struct Player *player, int pos[2]);

const struct Board * setup_game(int argc, char *argv[]);
const struct Player * run_game(const struct Board *board);