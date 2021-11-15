#include <peggity.h>
#include <peggity-interface.h>

const struct Board *board;
const struct Player *winner;

int * request_cell(int id, const struct Board *board) {
    int *out = calloc(2, sizeof(int));
    printf("Player %d, please choose the cell to play: \n", id);
    scanf("%d %d", &out[0], &out[1]);
    return out;
}

void send_board(const struct Player *player, const struct Board *board) {
    int i, j;
    for (i=0; i<board->size[0]; ++i) {
        for (j=0; j<board->size[1]; ++j)
            printf("| %d ", board->cells[i][j] != NULL ? board->cells[i][j]->id : -1);
        printf("|\n");
        for (j=0; j<board->size[0]; ++j)
            printf("-----");
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    board = setup_game(argc, argv);
    if (board == NULL)
        return 1;
    winner = run_game(board);
    if (winner == NULL) {
        printf("\nDraw!");
        return 1;
    }
    printf("\n%s won the game!\n", winner->name);
    return 0;
}
