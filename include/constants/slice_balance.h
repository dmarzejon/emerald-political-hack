#ifndef GUARD_CONSTANTS_SLICE_BALANCE_H
#define GUARD_CONSTANTS_SLICE_BALANCE_H

// Story names for the vertical slice's trainers. There is only room for 9 new
// trainer IDs, so the slice reuses vanilla ones; the teams are in
// src/data/trainers.party and the full table is in docs/balance.md.

// Tamsin, picked by VAR_STARTER_MON (0 Treecko, 1 Torchic, 2 Mudkip). The vanilla
// suffix names the player's starter; Tamsin has the one that beats it.
#define TRAINER_TAMSIN_LOWMERE_0        TRAINER_MAY_ROUTE_103_TREECKO
#define TRAINER_TAMSIN_LOWMERE_1        TRAINER_MAY_ROUTE_103_TORCHIC
#define TRAINER_TAMSIN_LOWMERE_2        TRAINER_MAY_ROUTE_103_MUDKIP

#define TRAINER_MIRE_ROAD_CLERK         TRAINER_RICK
#define TRAINER_MIRE_ROAD_POACHER       TRAINER_TIANA
#define TRAINER_MIRE_ROAD_YOUNGSTER     TRAINER_ALLEN
#define TRAINER_MIRE_ROAD_CARTER        TRAINER_ANDREW
#define TRAINER_MIRE_ROAD_WAGON_CLERK   TRAINER_GRUNT_PETALBURG_WOODS

#define TRAINER_HAYMARKET_FOREMAN       TRAINER_GRUNT_RUSTURF_TUNNEL
#define TRAINER_HAYMARKET_CLERK_1       TRAINER_GRUNT_MUSEUM_1
#define TRAINER_HAYMARKET_CLERK_2       TRAINER_GRUNT_MUSEUM_2

#define TRAINER_PAVILION_BROKER_1       TRAINER_TOMMY
#define TRAINER_PAVILION_BIDDER         TRAINER_JOSH
#define TRAINER_PAVILION_BROKER_2       TRAINER_MARC

#define TRAINER_CORWIN                  TRAINER_ROXANNE_1

// The king's gift dog. Give it explicit moves (see docs/balance.md) so Raikou
// doesn't start with Extreme Speed.
#define KINGS_GIFT_LEVEL                5

#endif // GUARD_CONSTANTS_SLICE_BALANCE_H
