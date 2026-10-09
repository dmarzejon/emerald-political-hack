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

// Chapter 2: the Orchard Road (Route 102 slot), Thornfield and Isolde's Trial.
#define TRAINER_ORCHARD_ROAD_PICKER     TRAINER_DAISY
#define TRAINER_ORCHARD_ROAD_BAILIFF    TRAINER_RHETT
#define TRAINER_ORCHARD_ROAD_POET       TRAINER_MARCOS
#define TRAINER_THORNFIELD_BAILIFF      TRAINER_BERKE
#define TRAINER_PALACE_GARDENER_1       TRAINER_RANDALL
#define TRAINER_PALACE_GARDENER_2       TRAINER_PARKER
#define TRAINER_PALACE_COURTIER         TRAINER_GEORGE
#define TRAINER_ISOLDE                  TRAINER_NORMAN_1

// Chapter 3: Chain Road (Route 104 slot), Gallows Wood (Petalburg Woods), Cragholt and Brannoc's Trial.
#define TRAINER_CHAIN_ROAD_FISHER           TRAINER_DARIAN
#define TRAINER_CHAIN_ROAD_COLLECTOR        TRAINER_IVAN
#define TRAINER_GALLOWS_WOOD_BUG_CATCHER    TRAINER_LYLE
#define TRAINER_DEBT_WARDEN                 TRAINER_GRUNT_WEATHER_INST_1
#define TRAINER_ORE_OFFICE_CLERK            TRAINER_GRUNT_WEATHER_INST_2
#define TRAINER_PITHEAD_FOREMAN             TRAINER_MIKE_2
#define TRAINER_PITHEAD_MINER_1             TRAINER_BRICE
#define TRAINER_PITHEAD_MINER_2             TRAINER_CLARK
#define TRAINER_BRANNOC                     TRAINER_BRAWLY_1

// The king's gift dog. Give it explicit moves (see docs/balance.md) so Raikou
// doesn't start with Extreme Speed.
#define KINGS_GIFT_LEVEL                5

#endif // GUARD_CONSTANTS_SLICE_BALANCE_H
