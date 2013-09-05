//$game = file('games/1.txt');

//$board = $game[count($game)-1];

/*$board = 
'0808'.
'----------------'.
'------wn--------'.
'----wr----bp----'.
'----------bk----'.
'--------bp------'.
'----------------'.
'----wp--wpwk--bq'.
'--------------br'.'0,900,250';
*/

typedef struct
	{
	int x;
	int y;
	int to_x;
	int to_y;
	int piece;
	int captured_piece;
	} MOVE;
void move_copy(MOVE *to, MOVE *from)
	{
	to->x = from->x;
	to->y = from->y;
	to->to_x = from->to_x;
	to->to_y = from->to_y;
	to->piece = from->piece;
	to->captured_piece = from->captured_piece;
	}

typedef struct
	{
	int width;
	int height;
	int cells[8][8]; // malloc in the future
	int side;
	int resource[2];
	} BOARD;

void generate_moves(int, int);
void make_move(MOVE*);
void unmake_move(MOVE*);
void print_algebraic(MOVE);
int in_check(int);

int absolute_material_value[13];
int material_value[13] = {0, 100, 300, 300, 500, 900, 9999, -100, -300, -300, -500, -900, -9999};
int absolute_material_value[13] = {0, 100, 300, 300, 500, 900, 9999, 100, 300, 300, 500, 900, 9999};
int positional_value[8][8] = {
0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 1, 2, 2, 1, 1, 0,
0, 1, 2, 4, 4, 2, 1, 0,
0, 2, 5, 10, 10, 5, 2, 0,
0, 2, 5, 10, 10, 5, 2, 0,
0, 1, 2, 4, 4, 2, 1, 0,
0, 1, 1, 2, 2, 1, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0};
// 99 - maximum depth
int move_counter[99];
MOVE pv[99][99];
MOVE moves[99][999];
BOARD board;
int attacked_squares[99][8][8];
int attacked_squares_only = 0; // used for check detection - update attacked squares only, no moves
int node_counter;

void precalculate()
	{
	}
