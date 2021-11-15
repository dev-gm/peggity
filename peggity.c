#include <peggity.h>
#include <peggity-interface.h>

const struct Board * new_board(int size[2], int player_len, const struct Player **players) {
    int i, j;
    struct Board *out = malloc(sizeof(struct Board));
    for (i=0; i<2; ++i)
        out->size[i] = size[i];
    out->cells = calloc(size[0], sizeof(struct Player **));
    for (i=0; i<size[0]; ++i)
        out->cells[i] = calloc(size[1], sizeof(struct Player *));
    out->players = players;
    out->player_len = player_len;
    out->filled_in = malloc(sizeof(int));
    return (const struct Board *) out;
}

const struct Player * new_player(int id, char *name) {
    struct Player *out = malloc(sizeof(struct Player));
    out->id = id;
    out->name = name;
    return (const struct Player *) out;
}

struct Player * get(const struct Board *board, int pos[2]) {
    if ((pos[0] < board->size[0] || pos[0] >= board->size[0]) ||
        (pos[1] < board->size[1] || pos[1] >= board->size[1]))
        return NULL;
    return board->cells[pos[0]][pos[1]];
}

int * move(int pos[2], int x, int y) {
    int out[2] = { pos[0] + x, pos[1] + y };
    return out;
}

int inner_check_cell_won(const struct Board *board, int pos[2]) {
    int out = 0, sum;
    int adjacent[2][2][2] = {
        {
            move(pos, 1, 0),
            move(pos, -1, 0),
        },
        {
            move(pos, 0, 1),
            move(pos, 0, -1),
        }
    };
    for (int i=0; i<2; ++i) {
        sum = 0; 
        for (int j=0; j<2; ++j)
            sum += inner_check_cell_won(board, adjacent[i][j]);
        if (sum > out)
            out = sum;
    }
    return out;
} 

int check_cell_won(const struct Board *board, int pos[2], int needed) {
    if (get(board, pos) != NULL && (needed == 0 || inner_check_cell_won(board, pos) >= needed))
        return 0;
    return 1;
}

int is_full(const struct Board *board) {
    if (*board->filled_in == board->size[0]*board->size[1])
        return 0;
    return 1;
}

enum PlaceCellResult place(const struct Board *board, const struct Player *player, int pos[2]) {
    if (pos[0] < 0 || pos[0] >= board->size[0] ||
        pos[1] < 0 || pos[1] >= board->size[1] ||
        board->cells[pos[0]][pos[1]] != NULL)
        return Fail;
    board->cells[pos[0]][pos[1]] = player;
    *board->filled_in += 1;
    if (check_cell_won(board, pos, 5) == 0)
        return Won;
    if (is_full(board) == 0)
        return RanOut;
    return Success;
}

const struct Board * setup_game(int argc, char *argv[]) {
    if (argc < 3)
        return NULL;
    int size[2] = { atoi(argv[1]), atoi(argv[2]) };
    struct Player *players[argc-3];
    for (int i=0; i<argc-3; ++i)
        players[i] = new_player(i, argv[i+3]);
    return new_board(size, argc-3, (const struct Player **) players);
}

const struct Player * run_game(const struct Board *board) {
    int i, *pos;
    while (1) {
        for (i=0; i<board->player_len; ++i) {
            pos = request_cell(i, board);
            switch (place(board, board->players[i], pos)) {
                case Success:
                    send_board(board->players[i], board);
                    break;
                case Fail:
                    --i;
                    break;
                case Won:
                    return board->players[i];
                case RanOut:
                    return NULL;
            }
        }
    }
}
