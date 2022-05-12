#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "functions.h"

#define BOARD_X 10
#define BOARD_Y 21

#define INGAME 0
#define END 1

#define BASE 0
#define STATE 1
#define PLAYER 2

#define NONE -1
#define A 0
#define B 1

#define FLEET_TYPES 6
#define SHIP_TYPE_BOUND 10

#define CAR 5
#define BAT 4
#define CRU 3
#define DES 2

#define NOT_DEFINED -1
#define SHOT 0
#define PLACED 1

#define ON 1
#define OFF 0

using namespace std;

int main()
{

    // --- PARAMETERS AND VARIABLES --- //
    int gameStatus = INGAME;
    int view = BASE;
    int currPlayer = NONE;
    int players[2] = { A, B };
    int nextPlayer = A;
    int playerCmdCountA = 0;
    int playerCmdCountB = 0;
    
    int started = 0;
    int playersAction = NOT_DEFINED;
    initializedPositions PlA, PlB;
    PlA = { 0, BOARD_Y / 2-1, 0, BOARD_X - 1 };
    PlB = { BOARD_Y / 2+1, BOARD_Y - 1, 0, BOARD_X - 1 };
    int extendedShips = OFF;

    char command[100];

    int AI = OFF;
    char AIplayer = 'N';
    int AIplayerINT = A;
    int AIenemy;
    int randSEED = 0;
    int seedGiven = 0;

    int reefsCount = 0;
    int reefsLimit = BOARD_Y * BOARD_X;
    point reefsArr[BOARD_X * BOARD_Y];
    int reefsIndex = 0;

    char board[BOARD_Y][BOARD_X];
    char board_extended[BOARD_Y][BOARD_X];
    char board_player[BOARD_Y][BOARD_X];
    int board_players_view_tmp[BOARD_Y][BOARD_X];
   
    int boardIdA[BOARD_Y][BOARD_X]; // arrays to store player's ships by their ids
    int boardIdB[BOARD_Y][BOARD_X]; // 

    int spyBoard[BOARD_Y][BOARD_X];
    reset_spy(spyBoard);

    FILL_BOARD(board);
    FILL_BOARD_ID(boardIdA);
    FILL_BOARD_ID(boardIdB);

    ships shipsA[41];
    ships shipsB[41];
    int indexA = 0;
    int indexB = 0;

    shipsA[40].id = 99;
    shipsB[40].id = 99;

    int fleet_set_A[FLEET_TYPES] = { 0, 0, 4, 3, 2, 1 }; //  {NOTHING, NOTHING, [DES]TROYER, [CRU]ISER, [BAT]TLESHIP, [CAR]RIER
    int fleet_set_B[FLEET_TYPES] = { 0, 0, 4, 3, 2, 1 }; //
    int fleetA[FLEET_TYPES] = { 0, 0, 0, 0, 0, 0 }; //
    int fleetB[FLEET_TYPES] = { 0, 0, 0, 0, 0, 0 }; //
    int fleetIdsA[6][SHIP_TYPE_BOUND];  // starting from 6, as to make Y axis as a ship size
    int fleetIdsB[6][SHIP_TYPE_BOUND];  // same here
    int fleetEnemyAI_Ids[6][SHIP_TYPE_BOUND];  // same here
    int fleetEnemyAI[FLEET_TYPES] = { 0, 0, 0, 0, 0, 0 };

    int cord_x, cord_y;
    char ship_direction;
    char ship_class_given[4];
    int ship_class;
    int number;

    int movesA[6][SHIP_TYPE_BOUND];
    int movesB[6][SHIP_TYPE_BOUND];
    int movesLimit[6][SHIP_TYPE_BOUND];
    fill_moves_limit_array(movesLimit);

    int shotsA[6][SHIP_TYPE_BOUND];
    int shotsB[6][SHIP_TYPE_BOUND];
    int shotsLimit[6][SHIP_TYPE_BOUND];
    fill_shots_limit_array(shotsLimit);

    int ships_placed = 0;
    int spyPlanesSent = 0;

    int* healthyParts = NULL;
    int healthyPartsA = 0, healthyPartsB = 0;

    // preparing place for player's fleet and moves checking
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < SHIP_TYPE_BOUND; j++) {
            fleetIdsA[i][j] = 0;
            fleetIdsB[i][j] = 0;
            movesA[i][j] = 0;
            movesB[i][j] = 0;
        }
    }

    // --- GAME --- //
    while (gameStatus == INGAME && cin >> command) {

        // --- Toggling [XXX] modes --- //
        if (strcmp(command, "[state]") == 0) {
            view = STATE;
        }
        if (strcmp(command, "[playerA]") == 0) {
            if (nextPlayer == B) {
                cout << "INVALID OPERATION \"[playerA] \": THE OTHER PLAYER EXPECTED";
                exit(0);
            }
            view = PLAYER;
            if (currPlayer == B) {
                playersAction = NOT_DEFINED;
                reset_moves(movesA);
                reset_shots(shotsA);
                reset_spy(spyBoard);
                spyPlanesSent = 0;
            }
            currPlayer = A;
            healthyParts = &healthyPartsA;
        }
        if (strcmp(command, "[playerB]") == 0) {
            if (nextPlayer == A) {
                cout << "INVALID OPERATION \"[playerB] \": THE OTHER PLAYER EXPECTED";
                exit(0);
            }
            view = PLAYER;
            if (currPlayer == A) {
                playersAction = NOT_DEFINED;
                reset_moves(movesB);
                reset_shots(shotsB);
                reset_spy(spyBoard);
                spyPlanesSent = 0;
            }
            currPlayer = B;
            healthyParts = &healthyPartsB;
        }


        // --- [state] mode --- //
        while (view == STATE) {
            cin >> command;

            // --- Printing board --- //
            if (strcmp(command, "PRINT") == 0) {
                int parameter;
                cin >> parameter;
                if (parameter == 0) {
                    PRINT_BOARD(board);
                    if ((healthyPartsA > 0 || healthyPartsB > 0)) {
                        cout << "PARTS REMAINING:: A : " << healthyPartsA << " B : " << healthyPartsB << endl;
                    }
                }
                else if (parameter == 1) {
                    render_board_extended(board_extended, board, shipsA, shipsB, indexA, indexB);
                    EXTENDED_PRINT_BOARD(board_extended);
                    cout << "PARTS REMAINING:: A : " << healthyPartsA << " B : " << healthyPartsB << endl;
                }
            }

            // --- Setting fleet --- //
            if (strcmp(command, "SET_FLEET") == 0) {

                char player;
                cin >> player;

                if (player == 'A') {
                    for (int i = FLEET_TYPES - 1; i >= 2; i--) {
                        cin >> fleet_set_A[i];
                    }
                }
                else {
                    for (int i = FLEET_TYPES - 1; i >= 2; i--) {
                        cin >> fleet_set_B[i];
                    }
                }
            }

            // --- setting reefs --- //
            if (strcmp(command, "REEF") == 0) {
                cin >> cord_y >> cord_x;

                if (cord_y >= 0 && cord_y < BOARD_Y && cord_x >= 0 && cord_x < BOARD_X) {

                    if (reefsCount < reefsLimit) {
                        board[cord_y][cord_x] = '#';
                        reefsCount++;
                        reefsArr[reefsIndex].y = cord_y;
                        reefsArr[reefsIndex].x = cord_x; 
                        reefsIndex++;
                    }
                    else {
                        cout << "INAVALID OPERATION \"REEF " << cord_y << " " << cord_x << "\" EXCEEDED REEFS LIMIT" << endl;
                        exit(0);
                    }
                }
                else {
                    cout << "INAVALID OPERATION \"REEF " << cord_y << " " << cord_x << "\" REEF IS NOT PLACED ON BOARD" << endl;
                    exit(0);
                }
            }

            if (strcmp(command, "INIT_POSITION") == 0) {
                char player;
                cin >> player;
                if (player == 'A') {
                    cin >> PlA.y1 >> PlA.x1 >> PlA.y2 >> PlA.x2;
                }
                else if (player == 'B') {
                    cin >> PlB.y1 >> PlB.x1 >> PlB.y2 >> PlB.x2;
                }

            }

            // --- preassigning ships --- // 
            if (strcmp(command, "SHIP") == 0) {

                char player;
                cin >> player;
                cin >> cord_y >> cord_x;
                cin >> ship_direction >> number >> ship_class_given;
                char parts[6];
                cin >> parts;
                ship_class = get_ship_class(ship_class_given); // assigning ship's size according to its class

                int ready_to_place = 0;
                int isAlreadyPresent = 0;
                int isIdValid = 0;

                int isOnReef = check_reef(board, cord_y, cord_x, ship_direction, ship_class);
                int isAnyShipNearby = checkShipsNearby(board, cord_y, cord_x, ship_direction, ship_class);
                if (isOnReef == 0) {

                    if (player == 'A') {
                        ready_to_place = check_position_A(cord_y, cord_x, ship_direction, ship_class, PlA);
                        isAlreadyPresent = check_presence(fleetIdsA, ship_class, number);
                        isIdValid = check_fleet(fleetA, fleet_set_A, ship_class);
                        add_info(&shipsA[indexA], cord_y, cord_x, number, ship_class, ship_direction); // updating array containing ships info

                        int partsindex = 0;
                        while (parts[partsindex] != '\0') {
                            shipsA[indexA].shipparts[partsindex] = (int)(parts[partsindex] - '0');
                            partsindex++;
                        }
                        indexA++;

                    }
                    else if (player == 'B') {
                        ready_to_place = check_position_B(cord_y, cord_x, ship_direction, ship_class, PlB);
                        isAlreadyPresent = check_presence(fleetIdsB, ship_class, number);
                        isIdValid = check_fleet(fleetB, fleet_set_B, ship_class);
                        add_info(&shipsB[indexB], cord_y, cord_x, number, ship_class, ship_direction); // updating array containing ships info

                        int partsindex = 0;
                        while (parts[partsindex] != '\0') {
                            shipsA[indexB].shipparts[partsindex] = (int)(parts[partsindex] - '0'); // adding information about destroyed and 
                            partsindex++;                                                          // undestroyed parts of the ship
                        }
                        indexB++;
                    }

                    if (ready_to_place == 1 && isAlreadyPresent == 0 && isIdValid == 1 && isAnyShipNearby == 0) {
                        place_ship_parts(board, cord_y, cord_x, ship_direction, ship_class, parts);

                        if (player == 'A') {
                            fleetIdsA[ship_class][number]++;
                            fleetA[ship_class]++;
                            place_ship_id(boardIdA, cord_y, cord_x, ship_direction, number, ship_class); // placing ship's id on dedicated board
                            healthyPartsA += ship_class;
                            for (int i = 0; parts[i] != '\0'; i++) {
                                if (parts[i] == '0')
                                    healthyPartsA--;
                            }

                        }
                        else if (player == 'B') {
                            fleetIdsB[ship_class][number]++;
                            fleetB[ship_class]++;
                            place_ship_id(boardIdB, cord_y, cord_x, ship_direction, number, ship_class); // placing ship's id on dedicated board
                            healthyPartsB += ship_class;
                            for (int i = 0; parts[i] != '\0'; i++) {
                                if (parts[i] == '0')
                                    healthyPartsB--;
                            }
                        }

                    }
                    else if (ready_to_place == 0) {
                        cout << "INVALID OPERATION \"SHIP " << player << " " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << " " << parts << "\": NOT IN STARTING POSITION" << endl;
                        exit(0);
                    }
                    else if (isAlreadyPresent == 1) {
                        cout << "INVALID OPERATION \"SHIP " << player << " " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << " " << parts << "\": SHIP ALREADY PRESENT" << endl;
                        exit(0);
                    }
                    else if (isIdValid == 0) {
                        cout << "INVALID OPERATION \"SHIP " << player << " " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << " " << parts << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
                        exit(0);
                    }
                    else if (isAnyShipNearby == 1) {
                        cout << "INVALID OPERATION \"SHIP " << player << " " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << " " << parts << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                        exit(0);
                    }
                }
                else {
                    cout << "INVALID OPERATION \"SHIP " << player << " " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << " " << parts << "\": PLACING SHIP ON REEF" << endl;
                    exit(0);
                }
            }

            // --- turning on extended ships --- //
            if (strcmp(command, "EXTENDED_SHIPS") == 0) {
                extendedShips = ON;
            }

            // --- choosing next player --- //
            if (strcmp(command, "NEXT_PLAYER") == 0) {
                char player;
                cin >> player;
                if (player == currPlayer) {
                    continue;
                }
                else if (player == A) {
                    playersAction = NOT_DEFINED;
                    reset_moves(movesA);
                    reset_shots(shotsA);
                    currPlayer = A;
                    healthyParts = &healthyPartsA;
                }
                else if (player == B) {
                    playersAction = NOT_DEFINED;
                    reset_moves(movesB);
                    reset_shots(shotsB);
                    currPlayer = B;
                    healthyParts = &healthyPartsB;
                }
            }

            // --- setting SRAND for AI --- //
            if (strcmp(command, "SRAND") == 0) {
                cin >> randSEED;
                seedGiven = 1;
                srand(randSEED);
            }
            
            // --- saving current game --- ///
            if (strcmp(command, "SAVE") == 0) {
                cout << "[state]" << endl;
                //board size
                cout << "BOARD_SIZE " << BOARD_Y << " " << BOARD_X << endl;
                //next player
                char nextPlayerSAVE = 'A';
                if (currPlayer == A)
                    nextPlayerSAVE = 'B';
                if (currPlayer == B)
                    nextPlayerSAVE = 'A';
                cout << "NEXT_PLAYER " << nextPlayerSAVE << endl;
                //assigning players A ships
                cout << "INIT_POSITION A " << PlA.y1 << " " << PlA.x1 << " " << PlA.y2 << " " << PlA.x2 << endl;
                //setting players A fleet
                cout << "SET_FLEET A " << fleet_set_A[5] << " " << fleet_set_A[4] << " " << fleet_set_A[3] << " " << fleet_set_A[2] << endl;
                //setting players A ships
                for (int c = CAR; c >= DES; c--) {
                    for (int i = 0; i < indexA; i++) {
                        if (shipsA[i].shipclass == c) {
                            cout << "SHIP A " << shipsA[i].head_y << " " << shipsA[i].head_x << " " << shipsA[i].direction << " " << shipsA[i].id << " ";
                            print_ship_class(shipsA[i]);
                            cout << " ";
                            int partsindex = 0;
                            while (partsindex < shipsA[i].shipclass && shipsA[i].shipparts[partsindex] >= 0) {
                                cout << shipsA[i].shipparts[partsindex];
                                partsindex++;
                            }
                            cout << endl;
                        }
                    }
                }
                //assigning players B ships
                cout << "INIT_POSITION B " << PlB.y1 << " " << PlB.x1 << " " << PlB.y2 << " " << PlB.x2 << endl;
                //setting players A fleet
                cout << "SET_FLEET B " << fleet_set_B[5] << " " << fleet_set_B[4] << " " << fleet_set_B[3] << " " << fleet_set_B[2  ] << endl;
                //setting players B ships
                for (int c = CAR; c >= DES; c--) {
                    for (int i = 0; i < indexB; i++) {
                        if (shipsB[i].shipclass == c) {
                            cout << "SHIP B " << shipsB[i].head_y << " " << shipsB[i].head_x << " " << shipsB[i].direction << " " << shipsB[i].id << " ";
                            print_ship_class(shipsB[i]);
                            cout << " ";
                            int partsindex = 0;
                            while (partsindex < shipsB[i].shipclass && shipsB[i].shipparts[partsindex] >= 0) {
                                cout << shipsB[i].shipparts[partsindex];
                                partsindex++;
                            }
                            cout << endl;
                        }
                    }
                }
                //putting reefs
                for (int i = 0; i < reefsIndex; i++) {
                    cout << "REEF " << reefsArr[i].y << " " << reefsArr[i].x << endl;
                }
                //checking if extended_ships mode is turned on
                if (extendedShips == ON) {
                    cout << "EXTENDED_SHIPS" << endl;
                }
                //checking if AI was set
                if (AI == ON) {
                    cout << "SET_AI_PLAYER " << AIplayer << endl;
                }
                //checking if SRAND was set
                if (seedGiven == 1) {
                    cout << "SRAND " << randSEED + 1 << endl;
                }
                cout << "[state]";
                exit(0);
            }

            // --- setting AI --- //
            if (strcmp(command, "SET_AI_PLAYER") == 0) {
                cin >> AIplayer;
                AIplayerINT = AIplayer - 'A';
                if (AIplayerINT == A) {
                    AIenemy = B;
                    memcpy(fleetEnemyAI_Ids, fleetIdsB, 10 * 6 * sizeof(int));
                    memcpy(fleetEnemyAI, fleetB, 6 * sizeof(int));
                }
                else {
                    AIenemy = A;
                    memcpy(fleetEnemyAI_Ids, fleetIdsA, 10 * 6 * sizeof(int));
                    memcpy(fleetEnemyAI, fleetA, 6 * sizeof(int));
                }
                AI = ON;
            }

            // --- exiting from the [state] mode --- //
            if (strcmp(command, "[state]") == 0) {
                view = BASE;
            }
        }


        // --- [playerX] mode --- //
        while (view == PLAYER) {
            cin >> command;

            // --- placing ship --- //
            if (strcmp(command, "PLACE_SHIP") == 0) {

                cin >> cord_y >> cord_x;
                cin >> ship_direction >> number >> ship_class_given;
                ship_class = get_ship_class(ship_class_given); // getting ship's size according to its class

                if (playersAction == NOT_DEFINED) {
                    playersAction = PLACED;
                }

                if (playersAction == SHOT) { // checking player's previuos action
                    cout << "INVALID OPERATION \"PLACE_SHIP " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << "\": ALREADY SHOT IN THIS ROUND";
                    exit(0);
                }

                int ready_to_place = 0;
                int isAlreadyPresent = 0;
                int isIdValid = 0;

                int isOnReef = check_reef(board, cord_y, cord_x, ship_direction, ship_class);

                int isAnyShipNearby = checkShipsNearby(board, cord_y, cord_x, ship_direction, ship_class);


                if (isOnReef == 0) {

                    if (currPlayer == A) {
                        ready_to_place = check_position_A(cord_y, cord_x, ship_direction, ship_class, PlA);
                        isAlreadyPresent = check_presence(fleetIdsA, ship_class, number);
                        isIdValid = check_fleet(fleetA, fleet_set_A, ship_class);
                        add_info(&shipsA[indexA], cord_y, cord_x, number, ship_class, ship_direction); // adding info to ships info array
                        for (int i = 0; i < shipsA[indexA].shipclass; i++) {
                            shipsA[indexA].shipparts[i] = 1;
                        }
                        indexA++;

                    }
                    else if (currPlayer == B) {
                        ready_to_place = check_position_B(cord_y, cord_x, ship_direction, ship_class, PlB);
                        isAlreadyPresent = check_presence(fleetIdsB, ship_class, number);
                        isIdValid = check_fleet(fleetB, fleet_set_B, ship_class);
                        add_info(&shipsB[indexB], cord_y, cord_x, number, ship_class, ship_direction); // adding info to ships info array
                        for (int i = 0; i < shipsB[indexB].shipclass; i++) {
                            shipsB[indexB].shipparts[i] = 1;
                        }
                        indexB++;
                    }

                    if (ready_to_place == 1 && isAlreadyPresent == 0 && isIdValid == 1 && isAnyShipNearby == 0) {
                        place_ship(board, cord_y, cord_x, ship_direction, ship_class);
                        *healthyParts += ship_class;

                        if (currPlayer == A) {
                            fleetIdsA[ship_class][number]++;
                            fleetA[ship_class]++;
                            place_ship_id(boardIdA, cord_y, cord_x, ship_direction, number, ship_class); // placing ships on dedicated id board
                        }
                        else if (currPlayer == B) {
                            fleetIdsB[ship_class][number]++;
                            fleetB[ship_class]++;
                            place_ship_id(boardIdB, cord_y, cord_x, ship_direction, number, ship_class); // placing ships on dedicated id board
                        }

                        if (started == 0) {
                            if (ships_placed == ships_set_count(fleet_set_A, fleet_set_B)) {
                                started = 1; // flag "all ships are set, it's time to shoot"
                            }
                        }
                    }
                    else if (ready_to_place == 0) {
                        cout << "INVALID OPERATION \"PLACE_SHIP " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << "\": NOT IN STARTING POSITION" << endl;
                        exit(0);
                    }
                    else if (isAlreadyPresent == 1) {
                        cout << "INVALID OPERATION \"PLACE_SHIP " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << "\": SHIP ALREADY PRESENT" << endl;
                        exit(0);
                    }
                    else if (isIdValid == 0) {
                        cout << "INVALID OPERATION \"PLACE_SHIP " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << "\": ALL SHIPS OF THE CLASS ALREADY SET" << endl;
                        exit(0);
                    }
                    else if (isAnyShipNearby == 1) {
                        cout << "INVALID OPERATION \"PLACE_SHIP " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl;
                        exit(0);
                    }
                }
                else {
                    cout << "INVALID OPERATION \"PLACE_SHIP " << cord_y << " " << cord_x << " " << ship_direction << " " << number << " " << ship_class_given << "\": PLACING SHIP ON REEF" << endl;
                    exit(0);
                }
            }

           // --- shooting --- //
            if (strcmp(command, "SHOOT") == 0) {
                if (extendedShips == OFF) {
                    cin >> cord_y >> cord_x;

                    int ships_set = ships_set_count(fleet_set_A, fleet_set_B);
                    ships_placed = count_ships(fleetIdsA, fleetIdsB);

                    if (playersAction == NOT_DEFINED) {
                        playersAction = SHOT;
                    }

                    if (playersAction == PLACED) {
                        cout << "INVALID OPERATION \"SHOOT " << cord_y << " " << cord_x << "\": ALREADY PLACED SHIPS IN THIS TURN";
                        exit(0);
                    }

                    if (ships_set != ships_placed) { // checking if all ships are set on board
                        cout << "INVALID OPERATION \"SHOOT " << cord_y << " " << cord_x << "\": NOT ALL SHIPS PLACED";
                        exit(0);
                    }

                    else if (cord_y >= 0 && cord_y < BOARD_Y && cord_x >= 0 && cord_x < BOARD_X) {
                        if (board[cord_y][cord_x] == '+') {

                            if (boardIdA[cord_y][cord_x] >= 0 && board[cord_y][cord_x] == '+') {
                                healthyPartsA--;
                                ships* hit_ship = &(search_hit_ship(cord_y, cord_x, shipsA)); // getting info of hit ship
                                update_parts(hit_ship, cord_y, cord_x);
                            }
                            else if (boardIdB[cord_y][cord_x] >= 0 && board[cord_y][cord_x] == '+') {
                                healthyPartsB--;
                                ships* hit_ship = &(search_hit_ship(cord_y, cord_x, shipsB)); // getting info of hit ship
                                update_parts(hit_ship, cord_y, cord_x);
                            }

                            board[cord_y][cord_x] = 'x';

                            // --- checking victory --- //
                            if (healthyPartsA == 0) {
                                cout << "B won";
                                exit(0);
                            }
                            else if (healthyPartsB == 0) {
                                cout << "A won";
                                exit(0);
                            }
                        }


                    }
                    else {
                        cout << "INVALID OPERATION \"SHOOT " << cord_y << " " << cord_x << "\": FIELD DOES NOT EXIST";
                        exit(0);
                    }
                }
                else if (extendedShips == ON) {
                    int shipid;
                    cin >> shipid >> ship_class_given;
                    cin >> cord_y >> cord_x;
                    ship_class = get_ship_class(ship_class_given);

                    ships* shooting_ship = &shipsA[40];

                    if (currPlayer == A) {
                        shooting_ship = &(search_ship(shipsA, ship_class, shipid));
                    }
                    else if (currPlayer == B) {
                        shooting_ship = &(search_ship(shipsB, ship_class, shipid));
                    }

                    double distance = points_distance(shooting_ship, cord_y, cord_x); // distance for shooting 

                    if (board[shooting_ship->cannon.y][shooting_ship->cannon.x] == '+') {

                        int ships_set = ships_set_count(fleet_set_A, fleet_set_B);
                        ships_placed = count_ships(fleetIdsA, fleetIdsB);

                        if (playersAction == NOT_DEFINED) {
                            playersAction = SHOT;
                        }

                        if (playersAction == PLACED) {
                            cout << "INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": ALREADY PLACED SHIPS IN THIS TURN";
                            exit(0);
                        }

                        if (ships_set != ships_placed) { // checking if all ships are set
                            cout << "INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": NOT ALL SHIPS PLACED";
                            exit(0);
                        }
                        else if (cord_y >= 0 && cord_y < BOARD_Y && cord_x >= 0 && cord_x < BOARD_X) {
                            if (distance <= ship_class || ship_class == CAR) { // condition for shooting
                                if (currPlayer == A) {
                                    if (shotsA[ship_class][shipid] < shotsLimit[ship_class][shipid]) // checking limit of shoots
                                        shotsA[ship_class][shipid]++;
                                    else {
                                        cout << "INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": TOO MANY SHOOTS";
                                        exit(0);
                                    }
                                }
                                else if (currPlayer == B) {
                                    if (shotsB[ship_class][shipid] < shotsLimit[ship_class][shipid]) // checking limit of shoots
                                        shotsB[ship_class][shipid]++;
                                    else {
                                        cout << "INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": TOO MANY SHOOTS";
                                        exit(0);
                                    }
                                }
                                if (board[cord_y][cord_x] == '+') {

                                    if (boardIdA[cord_y][cord_x] >= 0 && board[cord_y][cord_x] == '+') {
                                        healthyPartsA--;
                                        ships* hit_ship = &(search_hit_ship(cord_y, cord_x, shipsA)); // getting info of hit ship
                                        update_parts(hit_ship, cord_y, cord_x);
                                    }
                                    else if (boardIdB[cord_y][cord_x] >= 0 && board[cord_y][cord_x] == '+') {
                                        healthyPartsB--;
                                        ships* hit_ship = &(search_hit_ship(cord_y, cord_x, shipsB)); // getting info of hit ship
                                        update_parts(hit_ship, cord_y, cord_x);
                                    }

                                    board[cord_y][cord_x] = 'x';

                                    // --- checking victiry --- //
                                    if (healthyPartsA == 0) {
                                        cout << "B won";
                                        exit(0);
                                    }
                                    else if (healthyPartsB == 0) {
                                        cout << "A won";
                                        exit(0);
                                    }
                                }
                            }
                            else {
                                cout << "INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": SHOOTING TOO FAR";
                                exit(0);
                            }


                        }
                        else {
                            cout << "INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": FIELD DOES NOT EXIST";
                            exit(0);
                        }
                    }
                    else {
                        cout << " INVALID OPERATION \"SHOOT " << shipid << " " << ship_class_given << " " << cord_y << " " << cord_x << "\": SHIP CANNOT SHOOT";
                        exit(0);
                    }
                }
            }

            if (strcmp(command, "MOVE") == 0) {
                cin >> number >> ship_class_given;
                char dir;
                cin >> dir;
                ship_class = get_ship_class(ship_class_given);
                ships* moving_ship = &shipsA[40];


                if (currPlayer == A) {
                    moving_ship = &(search_ship(shipsA, ship_class, number));

                }
                else if (currPlayer == B) {
                    moving_ship = &(search_ship(shipsB, ship_class, number));
                }

                if (board[moving_ship->engine.y][moving_ship->engine.x] == '+' || extendedShips == OFF) { // checking engine for extended ships

                    ships tmp_ship_val = *moving_ship;
                    ships* tmp_ship = &tmp_ship_val;

                    // --- checking possibility to move ship on tmp board --- //
                    char tmp_board_check[BOARD_Y][BOARD_X];
                    memcpy(tmp_board_check, board, BOARD_Y * BOARD_X * sizeof(char));


                    if (dir == 'F') {
                        remove_ship(tmp_board_check, tmp_ship);
                        change_pos_F(tmp_ship);
                    }
                    if (dir == 'L') {
                        remove_ship(tmp_board_check, tmp_ship);
                        change_pos_L(tmp_ship);
                    }
                    if (dir == 'R') {
                        remove_ship(tmp_board_check, tmp_ship);
                        change_pos_R(tmp_ship);
                    }

                    int isOnReef = check_reef(tmp_board_check, tmp_ship->head_y, tmp_ship->head_x, tmp_ship->direction, tmp_ship->shipclass);
                    int isAnyShipNearby = checkShipsNearby(tmp_board_check, tmp_ship->head_y, tmp_ship->head_x, tmp_ship->direction, tmp_ship->shipclass);
                    int isOnBoard = check_is_on_board(tmp_ship);

                    if (isOnReef == 0 && isAnyShipNearby == 0 && isOnBoard == 1) {
                        if (currPlayer == A) {
                            if (movesA[moving_ship->shipclass][moving_ship->id] < movesLimit[moving_ship->shipclass][moving_ship->id])
                                movesA[moving_ship->shipclass][moving_ship->id]++;
                            else {
                                cout << "INVALID OPERATION \"MOVE" << " " << number << " " << ship_class_given << " " << dir << "\": SHIP MOVED ALREADY";
                                exit(0);
                            }
                        }
                        else if (currPlayer == B) {
                            remove_shipId(boardIdA, moving_ship);
                            if (movesB[moving_ship->shipclass][moving_ship->id] < movesLimit[moving_ship->shipclass][moving_ship->id])
                                movesB[moving_ship->shipclass][moving_ship->id]++;
                            else {
                                cout << "INVALID OPERATION \"MOVE" << " " << number << " " << ship_class_given << " " << dir << "\": SHIP MOVED ALREADY";
                                exit(0);
                            }
                        }

                        // moving ship on playable board
                        if (dir == 'F') {
                            if (currPlayer == A) {
                                remove_shipId(boardIdA, moving_ship);
                            }
                            else if (currPlayer == B) {
                                remove_shipId(boardIdA, moving_ship);
                            }

                            remove_ship(board, moving_ship);
                            move_forward(board, boardIdA, boardIdB, moving_ship, currPlayer);

                            if (currPlayer == A) {
                                place_ship_id(boardIdA, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
                            }
                            else if (currPlayer == B) {
                                place_ship_id(boardIdB, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
                            }
                        }

                        else if (dir == 'L') {
                            if (currPlayer == A) {
                                remove_shipId(boardIdA, moving_ship);
                            }
                            else if (currPlayer == B) {
                                remove_shipId(boardIdA, moving_ship);
                            }

                            remove_ship(board, moving_ship);
                            move_left(board, boardIdA, boardIdB, moving_ship, currPlayer);

                            if (currPlayer == A) {
                                place_ship_id(boardIdA, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
                            }
                            else if (currPlayer == B) {
                                place_ship_id(boardIdB, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
                            }
                        }

                        else if (dir == 'R') {
                            if (currPlayer == A) {
                                remove_shipId(boardIdA, moving_ship);

                            }
                            else if (currPlayer == B) {
                                remove_shipId(boardIdA, moving_ship);
                            }

                            remove_ship(board, moving_ship);
                            move_right(board, boardIdA, boardIdB, moving_ship, currPlayer);

                            if (currPlayer == A) {
                                place_ship_id(boardIdA, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
                            }
                            else if (currPlayer == B) {
                                place_ship_id(boardIdB, moving_ship->head_y, moving_ship->head_x, moving_ship->direction, moving_ship->id, moving_ship->shipclass);
                            }
                        }
                    }
                    else if (isOnBoard == 0) {
                        cout << "INVALID OPERATION \"MOVE" << " " << number << " " << ship_class_given << " " << dir << "\": SHIP WENT FROM BOARD";
                        exit(0);
                    }
                    else if (isOnReef == 1) {
                        cout << "INVALID OPERATION \"MOVE" << " " << number << " " << ship_class_given << " " << dir << "\": PLACING SHIP ON REEF";
                        exit(0);
                    }
                    else if (isAnyShipNearby == 1) {
                        cout << "INVALID OPERATION \"MOVE" << " " << number << " " << ship_class_given << " " << dir << "\": PLACING SHIP TOO CLOSE TO OTHER SHIP";
                        exit(0);
                    }
                }
                else {
                    cout << "INVALID OPERATION \"MOVE" << " " << number << " " << ship_class_given << " " << dir << "\": SHIP CANNOT MOVE";
                    exit(0);
                }

            }

            // --- printing player's view --- //
            if (strcmp(command, "PRINT") == 0) {
                int parameter;
                cin >> parameter;
                if (parameter == 0) {
                    if (currPlayer == A) {
                        render_board_player(board_player, board, board_players_view_tmp, shipsA, indexA, spyBoard);
                        PRINT_BOARD(board_player);
                    }
                    else if (currPlayer == B) {
                        render_board_player(board_player, board, board_players_view_tmp, shipsB, indexB, spyBoard);
                        PRINT_BOARD(board_player);
                    }
                }
            }

            // --- sending a spy plane --- //
            if (strcmp(command, "SPY") == 0) {
                cin >> number;
                cin >> cord_y >> cord_x;

                int isCARonBoard = 0;
                ships* spying_ship = NULL;

                if (currPlayer == A) {
                    spying_ship = &(search_ship(shipsA, CAR, number));
                    for (int i = 0; i < 10; i++) {
                        if (fleetIdsA[CAR][i] == 1) {
                            isCARonBoard = 1;
                        }
                    }
                }
                else if (currPlayer == B) {
                    spying_ship = &(search_ship(shipsA, CAR, number));
                    for (int i = 0; i < 10; i++) {
                        if (fleetIdsB[CAR][i] == 1) {
                            isCARonBoard = 1;
                        }
                    }
                }

                int isAbleToShoot = 0;
                if (board[spying_ship->cannon.y][spying_ship->cannon.x] == '+') {
                    isAbleToShoot = 1;
                }

                if (isCARonBoard == 1) {
                    if (isAbleToShoot == 1) {
                        if (spyPlanesSent<CAR) {
                            spyPlanesSent++;
                            for (int i = cord_y - 1; i <= cord_y + 1; i++) {
                                for (int j = cord_x - 1; j <= cord_x + 1; j++) {
                                    if (i >= 0 && i < BOARD_Y && j >= 0 && j < BOARD_X) {
                                        spyBoard[i][j] = 1;
                                    }
                                }
                            }
                        }
                        else {
                            cout << "INVALID OPERATION \"SPY " << number << " " << cord_y << " " << cord_x << "\": ALL PLANES SENT";
                            exit(0);
                        }
                    }
                    else {
                        cout << "INVALID OPERATION \"SPY " << number << " " << cord_y << " " << cord_x << "\": CANNOT SEND PLANE";
                        exit(0);
                    }
                }
                else {
                    cout << "INVALID OPERATION \"SPY " << number << " " << cord_y << " " << cord_x << "\": CARRIER NOT PRESENT";
                    exit(0);
                }
            }


            // --- exiting [playerX] mode --- //
            if (strcmp(command, "[playerA]") == 0) {
                nextPlayer = B;
                view = BASE;
                playerCmdCountA++;
                if (started == 1 and healthyPartsB == 0) {
                    cout << "A won";
                    exit(0);
                }

            }
            if (strcmp(command, "[playerB]") == 0) {
                nextPlayer = A;
                view = BASE;
                playerCmdCountB++;
                if (started == 1 and healthyPartsB == 0) {
                    cout << "B won";
                    exit(0);
                }
            }
        }
    }
    return 0;
}