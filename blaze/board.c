#include "board.h"
#include "move.h"
#include "movegen.h"
#include "hash.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int pieceValue[13] = {0,   100, 320, 330, 500, 900,  20000,
                      100, 320, 330, 500, 900, 20000};

/* parseFEN: parses `fen` (Forsyth–Edwards Notation) string into `board` structure */
void parseFEN(char *fen, struct board_t *board)
{
        int rank = RANK_8;
        int file = FILE_A;
        int piece = 0;
        int count = 0;
        int i = 0;
        int sq64 = 0;
        int sq120 = 0;

        assert(fen != NULL);
        assert(board != NULL);

        clearBoard(board);

        while ((rank >= RANK_1) && *fen) {
                count = 1;
                switch (*fen) {
                case 'p':
                        piece = bP;
                        break;
                case 'r':
                        piece = bR;
                        break;
                case 'n':
                        piece = bN;
                        break;
                case 'b':
                        piece = bB;
                        break;
                case 'k':
                        piece = bK;
                        break;
                case 'q':
                        piece = bQ;
                        break;
                case 'P':
                        piece = wP;
                        break;
                case 'R':
                        piece = wR;
                        break;
                case 'N':
                        piece = wN;
                        break;
                case 'B':
                        piece = wB;
                        break;
                case 'K':
                        piece = wK;
                        break;
                case 'Q':
                        piece = wQ;
                        break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                        piece = EMPTY;
                        count = *fen - '0';
                        break;

                case '/':
                case ' ':
                        rank--;
                        file = FILE_A;
                        fen++;
                        continue;

                default:
                        printf("Invalid FEN string!\n");
                }

                for (i = 0; i < count; i++) {
                        sq64 = rank * 8 + file;
                        sq120 = SQ64TO120(sq64);
                        if (piece != EMPTY)
                                board->pieces[sq120] = piece;
                        file++;
                }
                fen++;
        }

        assert(*fen == 'w' || *fen == 'b');

        board->turn = (*fen == 'w') ? WHITE : BLACK;
        fen += 2;

        for (i = 0; i < 4; i++) {
                if (*fen == ' ')
                        break;
                switch (*fen) {
                case 'K':
                        board->castling |= WKCA;
                        break;
                case 'Q':
                        board->castling |= WQCA;
                        break;
                case 'k':
                        board->castling |= BKCA;
                        break;
                case 'q':
                        board->castling |= BQCA;
                        break;
                }
                fen++;
        }
        fen++;

        assert(board->castling >= 0 && board->castling <= 15);

        if (*fen != '-') {
                file = fen[0] - 'a';
                rank = fen[1] - '1';

                assert(file >= FILE_A && file <= FILE_H);
                assert(rank >= RANK_1 && rank <= RANK_8);

                board->enPassant = FR2SQ120(file, rank);
        }
        updatePieceList(board);
        board->zobristHash = generateHash(board);
}

/* clearBoard: clears `board` */
void clearBoard(struct board_t *board)
{
        int i;
        for (i = 0; i < SQNUM; i++)
                board->pieces[i] = OFFBOARD;

        for (i = 0; i < 64; i++) {
                board->pieces[SQ64TO120(i)] = EMPTY;
        }
        for (i = 0; i < 13; i++)
                board->pieceCount[i] = 0;

        board->turn = 0;
        board->ply = 0;
        board->hisPly = 0;
        board->castling = 0;
        board->enPassant = NO_SQ;
        board->kingSquare[WHITE] = board->kingSquare[BLACK] = NO_SQ;
        board->fiftyMove = 0;
        board->material[WHITE] = board->material[BLACK] = 0;
}

/* updatePieceList: update `pieceList` on board */
void updatePieceList(struct board_t *board)
{
        int piece, color, i;

        for (i = 0; i < SQNUM; i++) {
                piece = board->pieces[i];
                if (piece != OFFBOARD && piece != EMPTY) {
                        color = pieceColor(piece);
                        board->pieceList[piece][board->pieceCount[piece]] = i;
                        board->pieceCount[piece]++;
                        board->material[color] += pieceValue[piece];
                        if (piece == wK)
                                board->kingSquare[WHITE] = i;
                        if (piece == bK)
                                board->kingSquare[BLACK] = i;
                }
        }
}

/* printBoard: print `board` in ascii representation */
void printBoard(struct board_t *board)
{
        int file, rank, sq, piece;
        char pieceChar[] = ".PNBRQKpnbrqk";

        printf("\n\n");
        for (rank = RANK_8; rank >= RANK_1; rank--) {
                printf("%d ", rank + 1);
                for (file = FILE_A; file <= FILE_H; file++) {
                        sq = FR2SQ120(file, rank);
                        piece = board->pieces[sq];
                        printf("%3c", pieceChar[piece]);
                }
                printf("\n");
        }
        printf("\n  ");
        for (file = FILE_A; file <= FILE_H; file++)
                printf("%3c", 'a' + file);
        printf("\n");
        printf("Turn: %d\n", board->turn);
        printf("Castle: %d\n", board->castling);
        printf("Zobrist Hash: %0llx\n", board->zobristHash);
}


/* printPV: print the principle variation `pv` */
void printPV(PV *pv)
{
        int i;

        printf("pv ");

        for (i = 0; i < pv->count; i++) {
                printMove(pv->moves[i]);
                printf(" ");
        }
}


int parseMove(struct board_t *board, char *move, struct move_t *m)
{
        int from, to;
        int moveNum = 0, promoted = EMPTY;
        MoveList list[1];

        if (move[1] > '8' || move[1] < '1')
                return 0;
        if (move[3] > '8' || move[3] < '1')
                return 0;
        if (move[0] > 'h' || move[0] < 'a')
                return 0;
        if (move[2] > 'h' || move[2] < 'a')
                return 0;

        from = FR2SQ120(move[0] - 'a', move[1] - '1');
        to = FR2SQ120(move[2] - 'a', move[3] - '1');

        generateMoves(board, list);
        for (moveNum = 0; moveNum < list->count; moveNum++) {
                m->from = list->moves[moveNum].from;
                m->to = list->moves[moveNum].to;
                m->captured = list->moves[moveNum].captured;
                m->promoted = list->moves[moveNum].promoted;
                m->enPassant = list->moves[moveNum].enPassant;
                m->pawnStart = list->moves[moveNum].pawnStart;
                m->castle = list->moves[moveNum].castle;
                if (m->from == from && m->to == to) {
                        promoted = m->promoted;
                        if (promoted != EMPTY) {
                                if (isRook(promoted) && move[4] == 'r') {
                                        return 1;
                                } else if (isBishop(promoted) &&
                                           move[4] == 'b') {
                                        return 1;
                                } else if (isQueen(promoted) &&
                                           move[4] == 'q') {
                                        return 1;
                                } else if (isKnight(promoted) &&
                                           move[4] == 'n') {
                                        return 1;
                                }
                                continue;
                        }
                        return 1;
                }
        }
        return 0;
}

int scores[13] = {0,   100, 200, 300, 400, 500, 600,
                  100, 200, 300, 400, 500, 600};

int mvvlva(int victim, int attacker)
{
        return scores[victim] + 6 - (scores[attacker] / 100);
}

int compareMoves(const void *m1, const void *m2)
{
        struct move_t *mv1 = (struct move_t *)m1;
        struct move_t *mv2 = (struct move_t *)m2;

        if (mv1->score > mv2->score)
                return -1;
        if (mv1->score < mv2->score)
                return +1;
        return 0;
}

int moveScore(struct board_t *board, struct move_t move)
{
        int score = mvvlva(move.captured, board->pieces[move.from]);

        if (move.captured) {
                score += 1000000;
        } else {

                if (move.from == board->kill[0][board->ply].from &&
                    move.to == board->kill[0][board->ply].to) {
                        score = 900000;
                } else if (move.from == board->kill[1][board->ply].from &&
                           move.to == board->kill[1][board->ply].to) {
                        score = 800000;
                } else {
                        score += board->his[board->pieces[move.from]][move.to];
                }
        }
        return score;
}

const int knightDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int rookDir[4] = {-1, -10, 1, 10};
const int bishopDir[4] = {-9, -11, 11, 9};
const int kingDir[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

int isAttacked(struct board_t *board, int square, int turn)
{
        int i, piece, t_square, dir;
        assert(onBoard(square));
        assert(turn == WHITE || turn == BLACK);

        // Checking pawn attacks
        if (turn == WHITE) {
                if (board->pieces[square - 11] == wP ||
                    board->pieces[square - 9] == wP)
                        return 1;
        } else {
                if (board->pieces[square + 11] == bP ||
                    board->pieces[square + 9] == bP)
                        return 1;
        }
        // Checking knight attacks

        for (i = 0; i < 8; i++) {
                piece = board->pieces[square + knightDir[i]];
                if (piece != OFFBOARD && isKnight(piece) &&
                    pieceColor(piece) == turn)
                        return 1;
        }

        // Checking Rooks and Queens attacks
        for (i = 0; i < 4; i++) {
                dir = rookDir[i];                // dir = -1
                t_square = square + dir;         // t_square = 55 - 1 = 54
                piece = board->pieces[t_square]; // piece = board->piece[54]
                while (piece != OFFBOARD) {
                        if (piece != EMPTY) {
                                if ((isRook(piece) || isQueen(piece)) &&
                                    pieceColor(piece) == turn) {
                                        return 1;
                                }
                                break;
                        }
                        t_square += dir;
                        piece = board->pieces[t_square];
                }
        }

        // Checking Rooks and Queens attacks
        for (i = 0; i < 4; i++) {
                dir = bishopDir[i];
                t_square = square + dir;
                piece = board->pieces[t_square];
                while (piece != OFFBOARD) {
                        if (piece != EMPTY) {
                                if ((isBishop(piece) || isQueen(piece)) &&
                                    pieceColor(piece) == turn) {
                                        return 1;
                                }
                                break;
                        }
                        t_square += dir;
                        piece = board->pieces[t_square];
                }
        }

        // Checking king attacks
        for (i = 0; i < 8; i++) {
                piece = board->pieces[square + kingDir[i]];
                if (piece != OFFBOARD && isKing(piece) &&
                    pieceColor(piece) == turn) {
                        return 1;
                }
        }

        return 0;
}

// direction of pieces
int pieceDirs[13][8] = {{0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0},
                        {-8, -19, -21, -12, 8, 19, 21, 12},
                        {-9, -11, 11, 9, 0, 0, 0, 0},
                        {-1, -10, 1, 10, 0, 0, 0, 0},
                        {-1, -10, 1, 10, -9, -11, 11, 9},
                        {-1, -10, 1, 10, -9, -11, 11, 9},
                        {0, 0, 0, 0, 0, 0, 0},
                        {-8, -19, -21, -12, 8, 19, 21, 12},
                        {-9, -11, 11, 9, 0, 0, 0, 0},
                        {-1, -10, 1, 10, 0, 0, 0, 0},
                        {-1, -10, 1, 10, -9, -11, 11, 9},
                        {-1, -10, 1, 10, -9, -11, 11, 9}};
// number of directions of each pieces (used with pieceDirs)
int numDirs[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

// slider pieces and non-slider pieces used in the loop to generate moves
int sliderPieces[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int nonSliderPieces[6] = {wN, wK, 0, bN, bK, 0};

// add move to list of moves (if pawn then addPawnMove())
void addMove(struct board_t *board, MoveList *list, unsigned char from, unsigned char to,
             unsigned char captured, unsigned char promoted,
             unsigned char enPassant, unsigned char pawnStart,
             unsigned char castle)
{
        assert(onBoard(from));
        assert(onBoard(to));

        struct move_t move;
        move.from = from;
        move.to = to;
        move.captured = captured;
        move.promoted = promoted;
        move.enPassant = enPassant;
        move.pawnStart = pawnStart;
        move.castle = castle;
        move.score = mvvlva(move.captured, board->pieces[move.from]);
        list->moves[list->count] = move;
        list->count++;
}
// adding a pawn move
void addPawnMove(struct board_t *board, MoveList *list, unsigned char from,
                 unsigned char to, unsigned char captured,
                 unsigned char enPassant, unsigned char pawnStart,
                 unsigned char castle)
{
        assert(onBoard(from));
        assert(onBoard(to));

        // checking for promotion
        if (board->pieces[from] == wP && RANK120(from) == RANK_7) {
                addMove(board, list, from, to, captured, wQ, enPassant,
                        pawnStart, castle);
                addMove(board, list, from, to, captured, wR, enPassant,
                        pawnStart, castle);
                addMove(board, list, from, to, captured, wB, enPassant,
                        pawnStart, castle);
                addMove(board, list, from, to, captured, wN, enPassant,
                        pawnStart, castle);
        } else if (board->pieces[from] == bP && RANK120(from) == RANK_2) {
                addMove(board, list, from, to, captured, bQ, enPassant,
                        pawnStart, castle);
                addMove(board, list, from, to, captured, bR, enPassant,
                        pawnStart, castle);
                addMove(board, list, from, to, captured, bB, enPassant,
                        pawnStart, castle);
                addMove(board, list, from, to, captured, bN, enPassant,
                        pawnStart, castle);
        } else {
                addMove(board, list, from, to, captured, EMPTY, enPassant,
                        pawnStart, castle);
        }
}
// TODO later, give different scoring to quiet and capture moves

void generateMoves(struct board_t *board, MoveList *list)
{

        // setting some variables
        list->count = 0;
        int piece = EMPTY;
        int turn = board->turn;
        int pieceCount = 0;
        int square = 0;
        int pieceIndex;
        int index;
        int dir;
        int targetSquare;

        // generating pawn moves
        if (turn == WHITE) {
                // Adding white pawn moves
                for (pieceCount = 0; pieceCount < board->pieceCount[wP];
                     ++pieceCount) {
                        square = board->pieceList[wP][pieceCount];
                        if (board->pieces[square + 10] == EMPTY) {
                                addPawnMove(board, list, square, square + 10,
                                            EMPTY, 0, 0, 0);
                                if (RANK120(square) == RANK_2 &&
                                    board->pieces[square + 20] == EMPTY) {
                                        addMove(board, list, square,
                                                square + 20, EMPTY, EMPTY, 0, 1,
                                                0);
                                }
                        }
                        // Adding white pawn captures
                        if (board->pieces[square + 9] != OFFBOARD &&
                            board->pieces[square + 9] != EMPTY &&
                            pieceColor(board->pieces[square + 9]) == BLACK) {
                                addPawnMove(board, list, square, square + 9,
                                            board->pieces[square + 9], 0, 0, 0);
                        }
                        if (board->pieces[square + 11] != OFFBOARD &&
                            board->pieces[square + 11] != EMPTY &&
                            pieceColor(board->pieces[square + 11]) == BLACK) {
                                addPawnMove(board, list, square, square + 11,
                                            board->pieces[square + 11], 0, 0,
                                            0);
                        }
                        // Adding white pawn enpassant captures
                        if (board->enPassant != NO_SQ) {
                                if (square + 9 == board->enPassant) {
                                        addMove(board, list, square, square + 9,
                                                EMPTY, EMPTY, 1, 0, 0);
                                }
                                if (square + 11 == board->enPassant) {
                                        addMove(board, list, square,
                                                square + 11, EMPTY, EMPTY, 1, 0,
                                                0);
                                }
                        }
                }
                // adding white's kingside and queenside castling moves
                if (board->castling & WKCA) {
                        if (board->pieces[F1] == EMPTY &&
                            board->pieces[G1] == EMPTY) {
                                if (!isAttacked(board, E1, BLACK) &&
                                    !isAttacked(board, F1, BLACK)) {
                                        addMove(board, list, E1, G1, EMPTY,
                                                EMPTY, 0, 0, 1);
                                }
                        }
                }

                if (board->castling & WQCA) {
                        if (board->pieces[D1] == EMPTY &&
                            board->pieces[C1] == EMPTY &&
                            board->pieces[B1] == EMPTY) {
                                if (!isAttacked(board, E1, BLACK) &&
                                    !isAttacked(board, D1, BLACK)) {
                                        addMove(board, list, E1, C1, EMPTY,
                                                EMPTY, 0, 0, 1);
                                }
                        }
                }
        } else {
                // adding black pawn moves
                for (pieceCount = 0; pieceCount < board->pieceCount[bP];
                     ++pieceCount) {
                        square = board->pieceList[bP][pieceCount];
                        if (board->pieces[square - 10] == EMPTY) {
                                addPawnMove(board, list, square, square - 10,
                                            EMPTY, 0, 0, 0);
                                if (RANK120(square) == RANK_7 &&
                                    board->pieces[square - 20] == EMPTY) {
                                        addMove(board, list, square,
                                                square - 20, EMPTY, EMPTY, 0, 1,
                                                0);
                                }
                        }
                        // adding black pawn captures
                        if (board->pieces[square - 9] != OFFBOARD &&
                            board->pieces[square - 9] != EMPTY &&
                            pieceColor(board->pieces[square - 9]) == WHITE) {
                                addPawnMove(board, list, square, square - 9,
                                            board->pieces[square - 9], 0, 0, 0);
                        }
                        if (board->pieces[square - 11] != OFFBOARD &&
                            board->pieces[square - 11] != EMPTY &&
                            pieceColor(board->pieces[square - 11]) == WHITE) {
                                addPawnMove(board, list, square, square - 11,
                                            board->pieces[square - 11], 0, 0,
                                            0);
                        }
                        // adding black pawn enpassant captures
                        if (board->enPassant != NO_SQ) {
                                if (square - 9 == board->enPassant) {
                                        addMove(board, list, square, square - 9,
                                                EMPTY, EMPTY, 1, 0, 0);
                                }
                                if (square - 11 == board->enPassant) {
                                        addMove(board, list, square,
                                                square - 11, EMPTY, EMPTY, 1, 0,
                                                0);
                                }
                        }
                }
                // adding black's kingside and queenside castling moves
                if (board->castling & BKCA) {
                        if (board->pieces[F8] == EMPTY &&
                            board->pieces[G8] == EMPTY) {
                                if (!isAttacked(board, E8, WHITE) &&
                                    !isAttacked(board, F8, WHITE)) {
                                        addMove(board, list, E8, G8, EMPTY,
                                                EMPTY, 0, 0, 1);
                                }
                        }
                }

                if (board->castling & BQCA) {
                        if (board->pieces[D8] == EMPTY &&
                            board->pieces[C8] == EMPTY &&
                            board->pieces[B8] == EMPTY) {
                                if (!isAttacked(board, E8, WHITE) &&
                                    !isAttacked(board, D8, WHITE)) {
                                        addMove(board, list, E8, C8, EMPTY,
                                                EMPTY, 0, 0, 1);
                                }
                        }
                }
        }
        // generating slider pieces moves
        pieceIndex = turn * 4;
        piece = sliderPieces[pieceIndex++];
        while (piece != 0) {
                for (pieceCount = 0; pieceCount < board->pieceCount[piece];
                     pieceCount++) {
                        square = board->pieceList[piece][pieceCount];
                        for (index = 0; index < numDirs[piece]; index++) {
                                dir = pieceDirs[piece][index];
                                targetSquare = square + dir;

                                while (board->pieces[targetSquare] !=
                                       OFFBOARD) {
                                        if (board->pieces[targetSquare] !=
                                            EMPTY) {
                                                if (pieceColor(
                                                        board->pieces
                                                            [targetSquare]) ==
                                                    (turn ^ 1)) {
                                                        addMove(
                                                            board, list, square,
                                                            targetSquare,
                                                            board->pieces
                                                                [targetSquare],
                                                            EMPTY, 0, 0, 0);
                                                }
                                                break;
                                        }
                                        addMove(board, list, square,
                                                targetSquare, EMPTY, EMPTY, 0,
                                                0, 0);
                                        targetSquare += dir;
                                }
                        }
                }
                piece = sliderPieces[pieceIndex++];
        }
        // generating non-slider pieces moves
        pieceIndex = turn * 3;
        piece = nonSliderPieces[pieceIndex++];
        while (piece != 0) {
                for (pieceCount = 0; pieceCount < board->pieceCount[piece];
                     pieceCount++) {
                        square = board->pieceList[piece][pieceCount];
                        for (index = 0; index < numDirs[piece]; index++) {
                                dir = pieceDirs[piece][index];
                                targetSquare = square + dir;
                                if (board->pieces[targetSquare] == OFFBOARD) {
                                        continue;
                                }
                                if (board->pieces[targetSquare] != EMPTY) {
                                        if (pieceColor(
                                                board->pieces[targetSquare]) ==
                                            (turn ^ 1)) {
                                                addMove(
                                                    board, list, square,
                                                    targetSquare,
                                                    board->pieces[targetSquare],
                                                    EMPTY, 0, 0, 0);
                                        }
                                        continue;
                                }
                                addMove(board, list, square, targetSquare,
                                        EMPTY, EMPTY, 0, 0, 0);
                        }
                }
                piece = nonSliderPieces[pieceIndex++];
        }
}

void generateCaptures(struct board_t *board, MoveList *list)
{

        // setting some variables
        list->count = 0;
        int piece = EMPTY;
        int turn = board->turn;
        int pieceCount = 0;
        int square = 0;
        int pieceIndex;
        int index;
        int dir;
        int targetSquare;

        // generating pawn moves
        if (turn == WHITE) {
                // Adding white pawn moves
                for (pieceCount = 0; pieceCount < board->pieceCount[wP];
                     ++pieceCount) {
                        square = board->pieceList[wP][pieceCount];
                        // Adding white pawn captures
                        if (board->pieces[square + 9] != OFFBOARD &&
                            board->pieces[square + 9] != EMPTY &&
                            pieceColor(board->pieces[square + 9]) == BLACK) {
                                addPawnMove(board, list, square, square + 9,
                                            board->pieces[square + 9], 0, 0, 0);
                        }
                        if (board->pieces[square + 11] != OFFBOARD &&
                            board->pieces[square + 11] != EMPTY &&
                            pieceColor(board->pieces[square + 11]) == BLACK) {
                                addPawnMove(board, list, square, square + 11,
                                            board->pieces[square + 11], 0, 0,
                                            0);
                        }
                        // Adding white pawn enpassant captures
                        if (board->enPassant != NO_SQ) {
                                if (square + 9 == board->enPassant) {
                                        addMove(board, list, square, square + 9,
                                                EMPTY, EMPTY, 1, 0, 0);
                                }
                                if (square + 11 == board->enPassant) {
                                        addMove(board, list, square,
                                                square + 11, EMPTY, EMPTY, 1, 0,
                                                0);
                                }
                        }
                }
        } else {
                // adding black pawn moves
                for (pieceCount = 0; pieceCount < board->pieceCount[bP];
                     ++pieceCount) {
                        square = board->pieceList[bP][pieceCount];
                        // adding black pawn captures
                        if (board->pieces[square - 9] != OFFBOARD &&
                            board->pieces[square - 9] != EMPTY &&
                            pieceColor(board->pieces[square - 9]) == WHITE) {
                                addPawnMove(board, list, square, square - 9,
                                            board->pieces[square - 9], 0, 0, 0);
                        }
                        if (board->pieces[square - 11] != OFFBOARD &&
                            board->pieces[square - 11] != EMPTY &&
                            pieceColor(board->pieces[square - 11]) == WHITE) {
                                addPawnMove(board, list, square, square - 11,
                                            board->pieces[square - 11], 0, 0,
                                            0);
                        }
                        // adding black pawn enpassant captures
                        if (board->enPassant != NO_SQ) {
                                if (square - 9 == board->enPassant) {
                                        addMove(board, list, square, square - 9,
                                                EMPTY, EMPTY, 1, 0, 0);
                                }
                                if (square - 11 == board->enPassant) {
                                        addMove(board, list, square,
                                                square - 11, EMPTY, EMPTY, 1, 0,
                                                0);
                                }
                        }
                }
        }
        // generating slider pieces moves
        pieceIndex = turn * 4;
        piece = sliderPieces[pieceIndex++];
        while (piece != 0) {
                for (pieceCount = 0; pieceCount < board->pieceCount[piece];
                     pieceCount++) {
                        square = board->pieceList[piece][pieceCount];
                        for (index = 0; index < numDirs[piece]; index++) {
                                dir = pieceDirs[piece][index];
                                targetSquare = square + dir;

                                while (board->pieces[targetSquare] !=
                                       OFFBOARD) {
                                        if (board->pieces[targetSquare] !=
                                            EMPTY) {
                                                if (pieceColor(
                                                        board->pieces
                                                            [targetSquare]) ==
                                                    (turn ^ 1)) {
                                                        addMove(
                                                            board, list, square,
                                                            targetSquare,
                                                            board->pieces
                                                                [targetSquare],
                                                            EMPTY, 0, 0, 0);
                                                }
                                                break;
                                        }
                                        targetSquare += dir;
                                }
                        }
                }
                piece = sliderPieces[pieceIndex++];
        }
        // generating non-slider pieces moves
        pieceIndex = turn * 3;
        piece = nonSliderPieces[pieceIndex++];
        while (piece != 0) {
                for (pieceCount = 0; pieceCount < board->pieceCount[piece];
                     pieceCount++) {
                        square = board->pieceList[piece][pieceCount];
                        for (index = 0; index < numDirs[piece]; index++) {
                                dir = pieceDirs[piece][index];
                                targetSquare = square + dir;
                                if (board->pieces[targetSquare] == OFFBOARD) {
                                        continue;
                                }
                                if (board->pieces[targetSquare] != EMPTY) {
                                        if (pieceColor(
                                                board->pieces[targetSquare]) ==
                                            (turn ^ 1)) {
                                                addMove(
                                                    board, list, square,
                                                    targetSquare,
                                                    board->pieces[targetSquare],
                                                    EMPTY, 0, 0, 0);
                                        }
                                        continue;
                                }
                        }
                }
                piece = nonSliderPieces[pieceIndex++];
        }
}
