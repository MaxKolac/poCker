#include "CuTest.h"
#include "../src/ai.h"
#include "../src/gamerules.h"
#include "../src/gamestate.h"
#include "../src/player.h"

static void test_getAvailableDecisions_CallAndRaiseSituation(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .big_blind = 10,
        .small_blind = 5,
        .limit_fixed = true,
        .funds_per_player = 1000
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    GameState* state = gsCreateNew(&rules);
    CuAssert(ct, "", state->raises_performed != MAX_BETS_PER_ROUND);
    CuAssert(ct, "", state->bet < players[0]->funds);

    unsigned int decisions = ai_getAvailableDecisions(&rules, state, players[0]);

    CuAssert(ct, "", decisions & FOLD);
    CuAssert(ct, "", decisions & CALL);
    CuAssert(ct, "", !(decisions & TAPOUT));
    CuAssert(ct, "", decisions & RAISE);
}

static void test_getAvailableDecisions_OnlyCallSituation_MaxRaiseLimitReached(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .big_blind = 10,
        .small_blind = 5,
        .limit_fixed = true,
        .funds_per_player = 1000
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    GameState* state = gsCreateNew(&rules);
    state->raises_performed = MAX_BETS_PER_ROUND;
    CuAssert(ct, "", state->bet < players[0]->funds);

    unsigned int decisions = ai_getAvailableDecisions(&rules, state, players[0]);

    CuAssert(ct, "", decisions & FOLD);
    CuAssert(ct, "", decisions & CALL);
    CuAssert(ct, "", !(decisions & TAPOUT));
    CuAssert(ct, "", !(decisions & RAISE));
}

static void test_getAvailableDecisions_OnlyCallSituation_PlayerCantAffordRaises_NoLimit(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .big_blind = 10,
        .small_blind = 5,
        .limit_fixed = false,
        .funds_per_player = 1000
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    GameState* state = gsCreateNew(&rules);
    state->bet = players[0]->funds;

    unsigned int decisions = ai_getAvailableDecisions(&rules, state, players[0]);

    CuAssert(ct, "", decisions & FOLD);
    CuAssert(ct, "", decisions & CALL);
    CuAssert(ct, "", !(decisions & TAPOUT));
    CuAssert(ct, "", !(decisions & RAISE));
}

static void test_getAvailableDecisions_OnlyCallSituation_PlayerCantAffordRaises_FixedLimitFlop(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .big_blind = 10,
        .small_blind = 5,
        .limit_fixed = true,
        .funds_per_player = 1000
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    GameState* state = gsCreateNew(&rules);
    state->betting_round = 1;
    state->bet = rules.small_blind * 2;
    state->raises_performed = 2;
    players[0]->funds = state->bet + 2;

    unsigned int decisions = ai_getAvailableDecisions(&rules, state, players[0]);

    CuAssert(ct, "", decisions & FOLD);
    CuAssert(ct, "", decisions & CALL);
    CuAssert(ct, "", !(decisions & TAPOUT));
    CuAssert(ct, "", !(decisions & RAISE));
}

static void test_getAvailableDecisions_OnlyCallSituation_PlayerCantAffordRaises_FixedLimitRiver(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .big_blind = 10,
        .small_blind = 5,
        .limit_fixed = true,
        .funds_per_player = 1000
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    GameState* state = gsCreateNew(&rules);
    state->betting_round = 3;
    state->bet = rules.big_blind * 2;
    state->raises_performed = 2;
    players[0]->funds = state->bet + 9;

    unsigned int decisions = ai_getAvailableDecisions(&rules, state, players[0]);

    CuAssert(ct, "", decisions & FOLD);
    CuAssert(ct, "", decisions & CALL);
    CuAssert(ct, "", !(decisions & TAPOUT));
    CuAssert(ct, "", !(decisions & RAISE));
}

static void test_getAvailableDecisions_OnlyTappoutSituation(CuTest* ct){
    const GameRuleSet rules = {
        .player_count = 4,
        .big_blind = 10,
        .small_blind = 5,
        .limit_fixed = true,
        .funds_per_player = 1000
    };
    Player* players[rules.player_count];
    for (int i = 0; i < rules.player_count; ++i){
        players[i] = playerCreateNewWithFunds(rules.funds_per_player);
    }
    GameState* state = gsCreateNew(&rules);
    state->bet = players[0]->funds + 1;

    unsigned int decisions = ai_getAvailableDecisions(&rules, state, players[0]);

    CuAssert(ct, "", decisions & FOLD);
    CuAssert(ct, "", !(decisions & CALL));
    CuAssert(ct, "", decisions & TAPOUT);
    CuAssert(ct, "", !(decisions & RAISE));
}

CuSuite* AiGetSuite(){
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_getAvailableDecisions_CallAndRaiseSituation);
    SUITE_ADD_TEST(suite, test_getAvailableDecisions_OnlyCallSituation_MaxRaiseLimitReached);
    SUITE_ADD_TEST(suite, test_getAvailableDecisions_OnlyCallSituation_PlayerCantAffordRaises_NoLimit);
    SUITE_ADD_TEST(suite, test_getAvailableDecisions_OnlyCallSituation_PlayerCantAffordRaises_FixedLimitFlop);
    SUITE_ADD_TEST(suite, test_getAvailableDecisions_OnlyCallSituation_PlayerCantAffordRaises_FixedLimitRiver);
    SUITE_ADD_TEST(suite, test_getAvailableDecisions_OnlyTappoutSituation);
    return suite;
}
